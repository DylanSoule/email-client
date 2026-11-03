#include <curl/curl.h>
#include <stdexcept>
#include <string>

#include "CurlMulti.h"

CurlMulti::Transaction::~Transaction() {
    if (recipients != nullptr) {
        curl_slist_free_all(recipients);
        recipients = nullptr;
    }

    if (easyHandle != nullptr) {
        curl_easy_cleanup(easyHandle);
        easyHandle = nullptr;
    }
}

CurlMulti::CurlMulti() {
    multi_ = curl_multi_init();
    
    if (!multi_) {
        throw std::runtime_error("Failed to init libcurl multi");
    }
}

CurlMulti::~CurlMulti() {
    for (auto& transaction : activeTransactions_) {
        if (transaction->easyHandle != nullptr) {
            curl_multi_remove_handle(multi_, transaction->easyHandle);
        }
    }
    activeTransactions_.clear();

    curl_multi_cleanup(multi_);
}

void CurlMulti::addHandle(CURL* easy_handle) {
    CURLMcode rc  = curl_multi_add_handle(multi_, easy_handle);
    if (rc != CURLM_OK) {
        throw std::runtime_error("Failed to add handle to multi interface");
    }
}

void CurlMulti::removeHandle(CURL* easy_handle) {
    curl_multi_remove_handle(multi_, easy_handle);
}

std::uint64_t CurlMulti::addTransaction(std::unique_ptr<Transaction> transaction) {
    if (!transaction || transaction->easyHandle == nullptr) {
        throw std::invalid_argument("Transaction must contain a valid easy handle");
    }

    transaction->id = nextId_++;

    CURLMcode rc = curl_multi_add_handle(multi_, transaction->easyHandle);
    if (rc != CURLM_OK) {
        throw std::runtime_error("Failed to add handle to multi interface");
    }

    const std::uint64_t id = transaction->id;
    activeTransactions_.push_back(std::move(transaction));
    return id;
}

CURLcode CurlMulti::perform(int timeoutMs) {
    int stillRunning = 0;

    CURLMcode rc = curl_multi_perform(multi_, &stillRunning);
    while (rc == CURLM_CALL_MULTI_PERFORM) {
        rc = curl_multi_perform(multi_, &stillRunning);
    }

    if (rc != CURLM_OK) {
        return CURLE_SEND_ERROR;
    }

    collectCompletedTransfers();

    while (stillRunning > 0) {
        int numFileDescriptors = 0;
        rc = curl_multi_wait(multi_, nullptr, 0, timeoutMs, &numFileDescriptors);
        if (rc != CURLM_OK) {
            return CURLE_SEND_ERROR;
        }

        rc = curl_multi_perform(multi_, &stillRunning);
        while (rc == CURLM_CALL_MULTI_PERFORM) {
            rc = curl_multi_perform(multi_, &stillRunning);
        }

        if (rc != CURLM_OK) {
            return CURLE_SEND_ERROR;
        }

        collectCompletedTransfers();
    }

    return CURLE_OK;
}

std::optional<CurlMulti::TransferResult> CurlMulti::takeResult(std::uint64_t id) {
    auto it = completedResults_.find(id);
    if (it == completedResults_.end()) {
        return std::nullopt;
    }

    TransferResult result = it->second;
    completedResults_.erase(it);
    return result;
}

size_t CurlMulti::activeCount() const {
    return activeTransactions_.size();
}

void CurlMulti::collectCompletedTransfers() {
    int messagesInQueue = 0;
    CURLMsg* message = nullptr;

    while ((message = curl_multi_info_read(multi_, &messagesInQueue)) != nullptr) {
        if (message->msg != CURLMSG_DONE) {
            continue;
        }

        for (auto it = activeTransactions_.begin(); it != activeTransactions_.end(); ++it) {
            Transaction& transaction = *(*it);
            if (transaction.easyHandle != message->easy_handle) {
                continue;
            }

            TransferResult transferResult;
            transferResult.success = (message->data.result == CURLE_OK);
            if (!transferResult.success) {
                transferResult.error = curl_easy_strerror(message->data.result);
            }

            completedResults_[transaction.id] = transferResult;
            curl_multi_remove_handle(multi_, transaction.easyHandle);
            activeTransactions_.erase(it);
            break;
        }
    }
}