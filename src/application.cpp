#include "application.h"
#include "db.h"
#include "xml_parser.h"
#include "xml_model.h"
#include "decompress.h"
#include "pop3_client.h"

#include <iostream>

using Poco::Exception;

Application::Application() {
}

Application& Application::get() {
    static Application app;
    return app;
}

int Application::run(int argc, char** argv) {
    vector<string> args;
    for (unsigned i = 0; i < argc; i++) {
        args.push_back(string(argv[i]));
    }

    run(args);
}

int Application::run(vector<string> args) {
    if (args.size() < 5) {
        cout << "Invalid number of parameters." << endl;
        cout << "Usage: ./cpp-arcon <db_name> <pop3_server> <email> <password>" << endl;
        return -1;
    }

    string dbName = args[1];
    string pop3Server = args[2];
    string email = args[3];
    string password = args[4];

    // Open the database connection
    cout << "Opening the database " << dbName << endl;
    DB db(DBType::SQLite, dbName);
    try {
        db.open();
    } catch (Exception& e) {
        cout << "Failed to open the connection to the database: " << e.displayText() << endl;
        return -1;
    }

    // Fetch all messages
    vector<string> messages;
    try {
        cout << "Loggin into " << pop3Server << endl;
        POP3Client client(pop3Server, email, password);

        cout << "Fetching messages" << endl;
        messages = client.fetchMessages();
    } catch (Exception& e) {
        cout << "Failed to fetch the messages: " << e.displayText() << endl;
        return -1;
    }

    // Parse all messages and insert them into the database.
    for(auto msg = messages.begin(); msg != messages.end(); ++msg) {
        cout << "Processing " << (*msg) << endl;
        XMLParser parser(*msg);
        Feedback f = parser.parse();
        db.write(f);
        db.writeRawXML(parser.getRawXML());
    }
    
    return 0;
}