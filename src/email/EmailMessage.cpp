#include <string>
#include <vector>
#include <chrono>
#include <optional>

#include "EmailMessage.h"
#include "EmailAccount.h"
#include "EmailAddress.h"


EmailMessage constructMessage(
    int mId, 
    EmailAccount &mSender, 
    EmailMessage::State mState = EmailMessage::State::draft,
    std::optional<std::vector<EmailAddress>> mRecipients,
    std::optional<std::vector<EmailAddress>> mCc,
    std::optional<std::vector<EmailAddress>> mBcc,
    std::optional<std::string> Msubject,
    std::optional<std::string> Mcontents) {
    
    EmailMessage temp;

    temp.id = mId;
    temp.sender = mSender;
    temp.state = mState;
    
    if (mRecipients) {
        for(auto& val : mRecipients.value()) {
            
        }
    }
}