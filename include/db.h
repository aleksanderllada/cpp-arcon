#ifndef DB_H
#define DB_H

#include "xml_model.h"
#include "Poco/Data/Session.h"
using Poco::Data::Session;
using Poco::Data::Statement;

#include <string>
using std::string;

enum DBType { SQLite, ODBC, MySQL };

/**
 * The DB class abstracts the database connection. It allows you to write the model, instead of executing SQL statements.
 */
class DB {
private:
    DBType type;
    string name;

    string getConnectorName();
    int getLastInsertId(Session &st);

public:
    /**
     * Constructs a new DB object.
     * @param type The type of the database. Currently the only supported type is DBType::SQLite.
     * @param name The name of the database. 
     */
    DB(DBType type, string name);

    /**
     * Opens a connection with the database and ensures the database schema.
     */
    void open();

    /**
     * Writes a Feedback structure to the database.
     */
    void write(Feedback& feedback);

    /**
     * Writes a raw XML file into the RawXML table.
     * @param data The XML file to be written.
     */
    void writeRawXML(string data);
};

#endif