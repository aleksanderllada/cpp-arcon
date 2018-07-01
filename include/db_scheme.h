#ifndef DB_SCHEME_H
#define DB_SCHEME_H

#include <string>

/**
 * Defines the Database scheme.
 */
std::string db_scheme = R"(
PRAGMA foreign_keys = ON;

CREATE TABLE IF NOT EXISTS Feedback (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    org_name VARCHAR(200),
    email VARCHAR(250),
    extra_contact_info VARCHAR(200),
    report_id VARCHAR(200),
    data_begin TIMESTAMP,
    data_end TIMESTAMP
);

CREATE TABLE IF NOT EXISTS RawXML (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    document TEXT
);

CREATE TABLE IF NOT EXISTS PolicyPublished (
    feedback_id INTEGER,
    domain VARCHAR(200),
    adkim VARCHAR(200),
    aspf VARCHAR(200),
    p VARCHAR(200),
    sp VARCHAR(200),
    pct VARCHAR(200),
    FOREIGN KEY(feedback_id) REFERENCES Feedback(id)
);

CREATE TABLE IF NOT EXISTS Record (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    feedback_id INTEGER,
    header_from VARCHAR(200),
    FOREIGN KEY(feedback_id) REFERENCES Feedback(id)
);

CREATE TABLE IF NOT EXISTS Row (
    record_id INTEGER,
    source_ip VARCHAR(45),
    disposition VARCHAR(200),
    dkim VARCHAR(200),
    spf VARCHAR(200),
    FOREIGN KEY(record_id) REFERENCES Record(id)
);

CREATE TABLE IF NOT EXISTS AuthResult (
    record_id INTEGER,
    domain VARCHAR(45),
    result VARCHAR(200),
    type VARCHAR(10),
    FOREIGN KEY(record_id) REFERENCES Record(id)
);
)";

#endif