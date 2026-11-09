#pragma once

#include <curl/curl.h>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

/*
RAII class to initialize and cleanup curl multi interface
- Also adds and removes easy handles for parallel performing
*/
class CurlMulti {
    public:
        struct Payload {
            std::string message;
            size_t bytesRead = 0;
        };

        struct Transaction {
            std::uint64_t id = 0;
            CURL* easyHandle = nullptr;
            curl_slist* recipients = nullptr;
            Payload payload;

            Transaction() = default;

            ~Transaction();

            Transaction(const Transaction&) = delete;
            Transaction& operator=(const Transaction&) = delete;
            Transaction(Transaction&&) noexcept = default;
            Transaction& operator=(Transaction&&) noexcept = default;
        };

        struct TransferResult {
            bool success = false;
            std::string error;
        };

        CurlMulti();

        ~CurlMulti();

        CurlMulti(const CurlMulti&) = delete;
        CurlMulti& operator=(const CurlMulti&) = delete;

        void addHandle(CURL* easy_handle);

        void removeHandle(CURL* easy_handle);

        std::uint64_t addTransaction(std::unique_ptr<Transaction> transaction);

        CURLcode perform(int timeoutMs = 1000);

        std::optional<TransferResult> takeResult(std::uint64_t id);

        size_t activeCount() const;

    private:
        void collectCompletedTransfers();

        CURLM* multi_;
        std::vector<std::unique_ptr<Transaction>> activeTransactions_;
        std::unordered_map<std::uint64_t, TransferResult> completedResults_;
        std::uint64_t nextId_ = 1;
};