#include "application.h"
#include "db.h"
#include "xml_parser.h"
#include "xml_model.h"
#include "decompress.h"

#include <iostream>

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
    // TODO: fetch the zip file from the e-mail
    
    // Unzip the XML file
    Decompress::gz("gz/yahoo.com!anbglobal.com!1530230400!1530316799.xml.gz", "xml/TestXML.xml");

    // Parse it into the Feedback model
    XMLParser parser("xml/TestXML.xml");
    Feedback f = parser.parse();

    // Write it into the database
    cout << "Starting DB..." << endl;
    DB db(DBType::SQLite, "test.db");
    db.open();
    db.write(f);
    db.writeRawXML(parser.getRawXML());
}