#include <string>
#include <vector>
#include <chrono>
#include <format>
#include <optional>

#include "EmailMessage.h"
#include "EmailAccount.h"
#include "EmailAddress.h"

std::string getRfc5322Date(std::string format)
{

    const auto now = std::chrono::floor<std::chrono::seconds>(
        std::chrono::system_clock::now()
    );
    const std::chrono::zoned_time localNow{std::chrono::current_zone(), now};

    return std::format(
        format,
        localNow
    );
}

std::string getMessageId(std::string& addr) {
    
}


EmailMessage constructMessage(
    int mId, 
    EmailAccount &mSender, 
    EmailMessage::State mState = EmailMessage::State::draft,
    std::optional<std::vector<EmailAddress>> mRecipients,
    std::optional<std::vector<EmailAddress>> mCc,
    std::optional<std::vector<EmailAddress>> mBcc,
    std::optional<std::string> MSubject,
    std::optional<std::string> MContents) {
    
    EmailMessage temp;

    temp.id = mId;
    temp.sender = mSender;
    temp.state = mState;
    
    if (mRecipients) {
        for(auto& val : mRecipients.value()) {
            temp.recipients.push_back(val);
        }
    }
    if (mCc) {
        for(auto& val : mCc.value()) {
            temp.carbonCopy.push_back(val);
        }
    }
    if (mBcc) {
        for(auto& val : mBcc.value()) {
            temp.blindCarbonCopy.push_back(val);
        }
    }
    if (MSubject) {
        temp.subject = MSubject.value();
    }
    if (MContents) {
        temp.contents = MContents.value();
    }

    temp.date = getRfc5322Date("{:%a, %d %b %Y %H:%M:%S %z}");
}