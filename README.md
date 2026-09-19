# Basic C++ SMTP Implementation
### Features
- libcurl usage to send email
- light wrapper class around libcurl to allow instances
- No terminal UI
- No recieving emails
### Research
---
To begin this big step in the project I first began to do research, I found [this guide](https://hackernoon.com/how-to-implement-smtp-client-on-c) on one method to accomplishing it. Unfortunately as I began to attempt to build my own SMTP implementation in c++ I understood exactly none of it.
For example I read through the whole article and there were many sections like this that while I could understand the basics(functions and loops) the more complicated and frankly more important stuff went right over my head.
```c++
void SMTPClient::connect(const std::string& hostname, unsigned short port) {
   boost::system::error_code error = boost::asio::error::host_not_found;
   boost::asio::ip::tcp::resolver resolver(service_);
   boost::asio::ip::tcp::resolver::query query(hostname, boost::lexical_cast<std::string>(port));
   boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
   boost::asio::ip::tcp::resolver::iterator end;

   while (error && endpoint_iterator != end) {
      socket_.close();
      socket_.connect(*endpoint_iterator++, error);
   }

   if (error) {
      std::cerr << "error: " << error.message());
   } else {
      handshake();
   }
}
```
*This uses the Boost library which I have decided to switch away from to have more modern functionality and high level programming without having to manage everything myself*

__Library Selection:__ 
After understanding the guide a little bit further with a greater understanding of C++ I realized that the guide uses boost, an amalgamation of many C++ utils, mostly low level and hard to understand, as a result of this I looked for libraries that have similar functionality and looked at these ones:
- Libcurl(What I have decided to use)
	- Higher level abstraction
	- Built for production apps needing full SMTP, DNS, and TLS protocols
	- Hides low level SMTP calls, so no writing raw SMTP commands
- mbedTLS
	- More modern application utils
	- Low level functionality and must build everything yourself
	- Must issue raw SMTP commands
- Openssl
	- Raw socket connections
	- extremely low level
- Boost.Asio
	- Heavy boilerplate
	- Complex syntax

__Learning libcurl__
In order to be able to to use the library I need to be able to understand the library, for which the libcurl [docs](https://curl.se/libcurl/c/), and specifically the API [tutorial](https://curl.se/libcurl/c/libcurl-tutorial.html), such as their example of how to execute basic SMTP protocol using the API [here](https://curl.se/libcurl/c/smtp-mail.html), I ended up using this example to base my code off of more than the example above as it allowed me to know what specific functions I need to be able to send the email

### Code Write Up
---
__Layout__
- I used a wrapper class(SMTPClient) around libcurl to be able to use c++ OOP while still being inline with the libcurl c type library, ensureing using static functions or const variables whenever necessary , and performing conversions as well, especially from std::string to c strings using c_str()
- Using the wrapper class in main I am able to initialize libcurl, create an instance of a class in order to be able to pass data, and call sendMail function to send an email
__Libcurl formatting__
- Most of the libcurl is configured in the SMTPclient class
- To begin I format the email how libcurl wants it(To, From, CC, Subject, Message) by formating first the to and from headers for if name is given to class alongside email. 
- Also ensure that correct \r\n line endings are used
```c++
std::string fromHeader = senderName_.empty() ?
"<" + senderEmail_ + ">"
: senderName_ + " <" + senderEmail_ + ">";

std::string toHeader = receiverName.empty() ?
"<" + receiverEmail + ">"
: receiverName + " <" + receiverEmail + ">";


std::string rawMessage =
"To: " + toHeader + "\r\n" +
"From: " + fromHeader + "\r\n" +
// "CC: " + ccHeader + "\r\n" +
"Subject: " + subject + "\r\n" +
"\r\n" + // Empty line divides headers from body
body + "\r\n";
```
__Libcurl Options__
- During config I use curl_easy_setopt(handle, CURLoption, parameter) to set email options after initializing curl easy
```c++
  CURL *curl = curl_easy_init();
```
- I use options to set up username, password, server url, reciever email, pass byte read function, and set other options
- After options are set you can call curl_easy_perform(handle) to do what you have configured curl to do
__Byte read function__
- in class payloadCallBack is a required function by libcurl(name doesn't matter as long as passed into CURLOPT_READFUNCTION option in setopt) which uses  curl passed params to ensure entire message is sent without overloading the server
- The function give curl the bytes to copy from the message, limited either by message size or max bytes possible curl passes in, whichever is lower, also updates variable that ensures starting at where copying last left off