#ifndef XML_MODEL_H
#define XML_MODEL_H

#include <string>
#include <list>
using std::string;

struct AuthResult {
    int record_id;
    string domain;
    string result;
    string type;
};

struct Row {
    int record_id;
    string source_ip;
    string disposition;
    string dkim;
    string spf;
};

struct Record {
    int id;
    int feedback_id;
    string header_from;

    std::list<Row> rows;
    std::list<AuthResult> auth_results;
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

struct Feedback {
    int id;
    string orgname;
    string email;
    string extra_contact_info;
    string report_id;
    int data_begin;
    int data_end;

    std::list<PolicyPublished> policies_published;
    std::list<Record> records;
};

#endif