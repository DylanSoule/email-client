#include <string>
#include <vector>
#include <chrono>
#include <format>
#include <optional>
#include <stdexcept>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "EmailMessage.h"
#include "EmailAccount.h"
#include "EmailAddress.h"

std::string getDate()
{

    const auto now = std::chrono::floor<std::chrono::seconds>(
        std::chrono::system_clock::now()
    );
    const std::chrono::zoned_time localNow{std::chrono::current_zone(), now};

    return std::format("{:%a, %d %b %Y %H:%M:%S %z}", localNow);
}

std::string getUuid() {
    // Generate uuid using boost uuids with 122 bits of security
    using namespace boost::uuids;

    random_generator gen;
    uuid id = gen();
    return to_string(id);
}

std::string getMessageId(const std::string& addr) {
    // Split the email string based on "@" symbol to get domain
    std::size_t pos = addr.find('@');

    // Ensure split is valid and there is actual "@" symbol
    if (pos == std::string::npos) {
        throw std::runtime_error("Not a valid address");
    }
    
    // Append domain and uuid together to get message id
    std::string domain = addr.substr(pos + 1);
    std::string uuid = getUuid();

    return uuid + "@" + domain;
}


EmailMessage constructMessage(
    int mId, 
    const EmailAccount& mSender,
    EmailMessage::State mState,
    std::optional<std::string> mReference,
    std::optional<std::vector<EmailAddress>> mRecipients,
    std::optional<std::vector<EmailAddress>> mCc,
    std::optional<std::vector<EmailAddress>> mBcc,
    std::optional<std::string> mSubject,
    std::optional<std::string> mContents) {
    
    // Declare temporary struct
    EmailMessage temp;

    // Assign basic values to temp passed into function
    temp.id = mId;
    temp.sender = mSender;
    temp.state = mState;

    // Assign what message references if value passed into function
    if (mReference){
        temp.referenceId = mReference.value();
    }
    
    // Loop through recipients for recipients, cc, and bcc
    // in order to append them vector in message struct
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

    // Apply subject and contents to smtp data if they exist
    if (mSubject) {
        temp.subject = mSubject.value();
    }
    if (mContents) {
        temp.contents = mContents.value();
    }

    // Get date that message is created
    temp.date = getDate();

    // Get unique message id with format uuid@example.com
    temp.messageId = getMessageId(mSender.emailAddress);

    return temp;
}