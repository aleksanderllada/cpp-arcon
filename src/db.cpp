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

void DB::writeFeedback(Session& session, Feedback& feedback) {
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
        pp->feedback_id = feedback.id;
        writePolicyPublished(session, (*pp));
    }

    // Insert Record objects
    for (auto record = feedback.records.begin(); record != feedback.records.end(); ++record) {
        record->feedback_id = feedback.id;
        writeRecord(session, (*record));
    }
}

void DB::writePolicyPublished(Session& session, PolicyPublished& policy_published) {
    Statement insert(session);
    string statement;

    statement = "INSERT INTO PolicyPublished(feedback_id, domain, adkim, aspf, p, sp, pct) VALUES(?, ?, ?, ?, ?, ?, ?);";
    insert << statement,
        use(policy_published.feedback_id),
        use(policy_published.domain),
        use(policy_published.adkim),
        use(policy_published.aspf),
        use(policy_published.p),
        use(policy_published.sp),
        use(policy_published.pct);
    try {
        insert.execute();
    } catch (Exception& e) {
        cout << "Failed to execute " << statement << ": " << e.displayText() << endl;
        return;
    }
}

void DB::writeRecord(Session& session, Record& record) {
    Statement insert(session);
    string statement;

    statement = "INSERT INTO Record(feedback_id, header_from) VALUES(?, ?);";
    insert << statement,
        use(record.feedback_id),
        use(record.identifier.header_from);
    try {
        insert.execute();
    } catch (Exception& e) {
        cout << "Failed to execute " << statement << ": " << e.displayText() << endl;
        return;
    }
    record.id = getLastInsertId(session);

    // Insert Row objects
    for (auto row = record.rows.begin(); row != record.rows.end(); ++row) {
        row->record_id = record.id;
        writeRow(session, (*row));
    }

    // Insert AuthResult objects
    for (auto ar = record.auth_results.begin(); ar != record.auth_results.end(); ++ar) {
        ar->record_id = record.id;
        writeAuthResult(session, (*ar));
    }
}

void DB::writeRow(Session& session, Row& row) {
    Statement insert(session);
    string statement;

    statement = "INSERT INTO Row(record_id, source_ip, disposition, dkim, spf) VALUES(?, ?, ?, ?, ?);";
    insert << statement,
        use(row.record_id),
        use(row.source_ip),
        use(row.policy_evaluated.disposition),
        use(row.policy_evaluated.dkim),
        use(row.policy_evaluated.spf);
    try {
        insert.execute();
    } catch (Exception& e) {
        cout << "Failed to execute " << statement << ": " << e.displayText() << endl;
        return;
    }
}

void DB::writeAuthResult(Session& session, AuthResult& auth_result) {
    Statement insert(session);
    string statement;

    statement = "INSERT INTO AuthResult(record_id, domain, result, type) VALUES(?, ?, ?, ?);";
    insert << statement,
        use(auth_result.record_id),
        use(auth_result.domain),
        use(auth_result.result),
        use(auth_result.type);
    try {
        insert.execute();
    } catch (Exception& e) {
        cout << "Failed to execute " << statement << ": " << e.displayText() << endl;
        return;
    }
}

void DB::write(Feedback& feedback) {
    Session session(getConnectorName(), name);
    writeFeedback(session, feedback);
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