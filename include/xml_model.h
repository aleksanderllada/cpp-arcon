#ifndef XML_MODEL_H
#define XML_MODEL_H

#include <string>
#include <vector>
using std::string;

struct AuthResult {
    int record_id;
    string domain;
    string result;
    string type;
};

struct PolicyEvaluated {
    string disposition;
    string dkim;
    string spf;
};

struct Row {
    int record_id;
    string source_ip;
    int count;
    PolicyEvaluated policy_evaluated; // Might be std::vector
};

struct Identifier {
    string header_from;
};

struct Record {
    int id;
    int feedback_id;
    Identifier identifier; // Might be std::vector

    std::vector<Row> rows;
    std::vector<AuthResult> auth_results;
};

struct PolicyPublished {
    int feedback_id;
    string domain;
    string adkim;
    string aspf;
    string p;
    string sp;
    double pct;
};

struct DataRange {
    int begin;
    int end;
};

struct ReportMetadata {
    string org_name;
    string email;
    string extra_contact_info;
    string report_id;
    DataRange data_range;
};

struct Feedback {
    int id;
    ReportMetadata report_metadata; // Might be std::vector

    std::vector<PolicyPublished> policies_published;
    std::vector<Record> records;
};

#endif