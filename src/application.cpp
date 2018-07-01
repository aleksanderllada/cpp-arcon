#include "application.h"
#include "db.h"
#include "xml_parser.h"
#include "xml_model.h"

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
    XMLParser parser("xml/TestXML.xml");
    Feedback f = parser.parse();
    cout << "Starting DB..." << endl;
    
    DB db(DBType::SQLite, "test.db");
    db.open();
    db.write(f);
    db.writeRawXML(parser.getRawXML());
}