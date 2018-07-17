#ifndef POP3_CLIENT_H
#define POP3_CLIENT_H

#include "Poco/Net/POP3ClientSession.h"
#include <string>
#include <vector>

using std::vector;
using std::string;
using Poco::Net::POP3ClientSession;

class POP3Client {
private:
    string host;
    string login;
    string password;

    POP3ClientSession *session;
    
public:
    /**
     * Opens a connection with the POP3 server and logs in.
     */
    POP3Client(string host, string login, string password);
    ~POP3Client();

    /**
     * Fetches all messages from the server. Reads the mime part containing the
     * "application/gzip" content-type and extracts its contents.
     * @return a list of filenames with the extracted XML files.
     */
    vector<string> fetchMessages();
};

#endif