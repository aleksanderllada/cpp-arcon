#include "db.h"
#include "db_scheme.h"

#include "Poco/Exception.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/Session.h"
using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Exception;

using namespace std;

DB::DB(DBType type, string name) : type(type), name(name) {
    switch (type) {
    case SQLite:
        Poco::Data::SQLite::Connector::registerConnector();
        break;
    }
}

string DB::getConnectorName() {
    switch (type) {
    case SQLite:
        return "SQLite";
        break;
    case ODBC:
        return "ODBC";
        break;
    case MySQL:
        return "MySQL";
        break;
    default:
        return "SQLite";
    }
}

void DB::open() {
    Session session(getConnectorName(), name);
    session << db_scheme, now;
}

int DB::getLastInsertId(Session& session) {
    Statement st(session);
    int lastInsertId = 0;
    st << "SELECT last_insert_rowid();",
        into(lastInsertId),
        now;

    return lastInsertId;
}

void DB::write(Feedback& feedback) {
    Session session(getConnectorName(), name);
    Statement insert(session);
    string statement;

    // Insert Feedback object
    statement = "INSERT INTO Feedback(org_name, email, extra_contact_info, report_id, data_begin, data_end) VALUES(?, ?, ?, ?, ?, ?);";
    insert << statement,
        use(feedback.report_metadata.org_name),
        use(feedback.report_metadata.email),
        use(feedback.report_metadata.extra_contact_info),
        use(feedback.report_metadata.report_id),
        use(feedback.report_metadata.data_range.begin),
        use(feedback.report_metadata.data_range.end);
    try {
        insert.execute();
    } catch (Exception& e) {
        cout << "Failed to execute " << statement << ": " << e.displayText() << endl;
        return;
    }
    feedback.id = getLastInsertId(session);

    // Insert PolicyPublished objects
    for (auto pp = feedback.policies_published.begin(); pp != feedback.policies_published.end(); ++pp) {
        insert.reset(session);

        pp->feedback_id = feedback.id;
        statement = "INSERT INTO PolicyPublished(feedback_id, domain, adkim, aspf, p, sp, pct) VALUES(?, ?, ?, ?, ?, ?, ?);";
        insert << statement,
            use(pp->feedback_id),
            use(pp->domain),
            use(pp->adkim),
            use(pp->aspf),
            use(pp->p),
            use(pp->sp),
            use(pp->pct);
        try {
            insert.execute();
        } catch (Exception& e) {
            cout << "Failed to execute " << statement << ": " << e.displayText() << endl;
            return;
        }
    }

    // Insert Record objects
    for (auto record = feedback.records.begin(); record != feedback.records.end(); ++record) {
        insert.reset(session);

        record->feedback_id = feedback.id;
        statement = "INSERT INTO Record(feedback_id, header_from) VALUES(?, ?);";
        insert << statement,
            use(record->feedback_id),
            use(record->identifier.header_from);
        try {
            insert.execute();
        } catch (Exception& e) {
            cout << "Failed to execute " << statement << ": " << e.displayText() << endl;
            return;
        }
        record->id = getLastInsertId(session);

        // Insert Row objects
        for (auto row = record->rows.begin(); row != record->rows.end(); ++row) {
            insert.reset(session);

            row->record_id = record->id;
            statement = "INSERT INTO Row(record_id, source_ip, disposition, dkim, spf) VALUES(?, ?, ?, ?, ?);";
            insert << statement,
                use(row->record_id),
                use(row->source_ip),
                use(row->policy_evaluated.disposition),
                use(row->policy_evaluated.dkim),
                use(row->policy_evaluated.spf);
            try {
                insert.execute();
            } catch (Exception& e) {
                cout << "Failed to execute " << statement << ": " << e.displayText() << endl;
                return;
            }
        }

        // Insert AuthResult objects
        for (auto ar = record->auth_results.begin(); ar != record->auth_results.end(); ++ar) {
            insert.reset(session);

            ar->record_id = record->id;
            statement = "INSERT INTO AuthResult(record_id, domain, result, type) VALUES(?, ?, ?, ?);";
            insert << statement,
                use(ar->record_id),
                use(ar->domain),
                use(ar->result),
                use(ar->type);
            try {
                insert.execute();
            } catch (Exception& e) {
                cout << "Failed to execute " << statement << ": " << e.displayText() << endl;
                return;
            }
        }
    }
}


void DB::writeRawXML(string data) {
    Session session(getConnectorName(), name);
    Statement insert(session);
    string statement;

    // Insert Feedback object
    statement = "INSERT INTO RawXML(document) VALUES(?);";
    insert << statement,
        use(data);
    try {
        insert.execute();
    } catch (Exception& e) {
        cout << "Failed to execute " << statement << ": " << e.displayText() << endl;
        return;
    }
}