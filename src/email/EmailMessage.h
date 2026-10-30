#pragma once

#include <string>
#include <vector>
#include <optional>

#include "EmailAccount.h"
#include "EmailAddress.h"

/*
Defines an email message on the email client
This message is of any type, received, draft, or already sent

Plaintext message to be shown to the user
    Mime parsing and compilation will happen before assignment to message struct
*/
struct EmailMessage {
    int id;
    std::string messageId;
    std::string referenceId;
    std::string replyId;

    EmailAccount sender;

    std::vector<EmailAddress> recipients;
    std::vector<EmailAddress> carbonCopy;
    std::vector<EmailAddress> blindCarbonCopy;

    std::string subject;
    std::string contents;

    enum class State { received, draft, sent };
    State state;

    std::string date;
};


/*
Formats Date for RFC 5322
*/
std::string getDate();

/*
Generate UUID v4 for message id with 122 bits of security
*/
std::string getUuid();


/*
Creates a unique message id according to RFC 5322 rules
*/
std::string getMessageId(EmailAccount& mSender);

/*
Given basic information, likely pulled from local database, will construct an email message struct
Required: id, sender
Default value: draft for new messages, saved messages value passed in
Optional: Recipients, cc, bcc, subject, and contents
*/
EmailMessage constructMessage(
    int mId, 
    EmailAccount mSender, 
    EmailMessage::State mState = EmailMessage::State::draft,
    std::optional<std::vector<EmailAddress>> mRecipients,
    std::optional<std::vector<EmailAddress>> mCc,
    std::optional<std::vector<EmailAddress>> mBcc,
    std::optional<std::string> Msubject,
    std::optional<std::string> Mcontents
    );