#include "application.h"
#include "db.h"
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
    cout << "Starting DB..." << endl;
    
    DB db(DBType::SQLite, "test.db");
    db.open();
    Feedback feedback;
    feedback.orgname = "org_name";
    feedback.email = "dmarc_reports@emailsrvr.com";
    feedback.extra_contact_info = "http://emailsrvr.com";
    feedback.report_id = "8098bbb4-0ce7-4203-88c0-072acb2f0765";
    feedback.data_begin = 1526860800;
    feedback.data_end = 1526947200;

    PolicyPublished policy;
    policy.domain = "Test.com";
    policy.adkim = "r";
    policy.aspf = "r";
    policy.p = "none";
    policy.sp = "none";
    policy.pct = 100;
    feedback.policies_published.push_back(policy);

    Record record;
    record.header_from = "Test.com";

    Row row;
    row.source_ip = "100.137.237.121";
    row.disposition = "none";
    row.dkim = "fail";
    row.spf = "fail";
    record.rows.push_back(row);

    AuthResult ar1;
    ar1.domain = "Test.com";
    ar1.result = "neutral";
    ar1.type = "spf";
    record.auth_results.push_back(ar1);

    AuthResult ar2;
    ar2.domain = "rediffmailpro.com";
    ar2.result = "pass";
    ar2.type = "dkim";
    record.auth_results.push_back(ar2);

    feedback.records.push_back(record);

    db.write(feedback);
}