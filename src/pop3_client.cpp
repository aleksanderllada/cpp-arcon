#include "pop3_client.h"
#include "decompress.h"
#include "Poco/Net/POP3ClientSession.h"
#include "Poco/Net/MailMessage.h"
#include <iostream>
#include <algorithm>
#include <iterator>

using Poco::Net::MailMessage;
using namespace std;

POP3Client::POP3Client(string host, string login, string password) 
: host(host), login(login), password(password) {
    session = new POP3ClientSession(host);
    session->login(login, password);
}

POP3Client::~POP3Client() {
    session->close();
    delete session;
}

vector<string> POP3Client::fetchMessages() {
    vector<string> messages;

    POP3ClientSession::MessageInfoVec msgInfo;
    session->listMessages(msgInfo);

    // Iterate through all messages
    for(auto msgIt = msgInfo.begin(); msgIt != msgInfo.end(); ++msgIt) {
        MailMessage msg;

        // Retrieve the message
        cout << "======================================" << endl;
        cout << "Fetching message " << msgIt->id << endl;
        session->retrieveMessage(msgIt->id, msg);
        cout << "Message subject: " << msg.getSubject() << endl;

        // Find which part of the message contains the application/gzip content.
        for(auto partIt = msg.parts().begin(); partIt != msg.parts().end(); ++partIt) {
            string applicationType;
            if (partIt->pSource->mediaType().find("application/gzip") != string::npos)
                applicationType = "gzip";
            else if (partIt->pSource->mediaType().find("application/gzip") != string::npos)
                applicationType = "zip";

            if (!applicationType.empty()) {
                // Format the filename
                string filename = partIt->pSource->filename();
                filename.erase(remove(filename.begin(), filename.end(), '"'), filename.end());
                filename = filename.substr(0, filename.size()-3);
                filename = "xml/" + filename;

                // Decompress it and push the data into a file
                if (applicationType == "gzip")
                    Decompress::gz(partIt->pSource->stream(), filename);
                else
                    Decompress::zip(partIt->pSource->stream(), filename);

                messages.push_back(filename);
            }
        }
    }

    return messages;
}
