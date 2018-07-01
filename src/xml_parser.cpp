#include "xml_parser.h"
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/Document.h"
#include "Poco/SAX/InputSource.h"

#include <fstream>
#include <streambuf>
#include <iostream>

using namespace std;
using Poco::XML::InputSource;
using Poco::XML::DOMParser;
using Poco::XML::Document;
using Poco::XML::Node;
using Poco::XML::NodeList;

Feedback XMLParser::parseFeedback(Node* pNode) {
    if (pNode->nodeName() != "feedback")
        throw "parseFeedback(Node*): Expected node name to be 'feedback'.";
    
    Feedback feedback;

    pNode = pNode->firstChild();
    while(pNode) {
        string name = pNode->nodeName();
        if (name == "report_metadata") {
            ReportMetadata report_metadata = parseReportMetadata(pNode);
            feedback.report_metadata = report_metadata;
        } else if (name == "policy_published") {
            PolicyPublished policy_published = parsePolicyPublished(pNode);
            feedback.policies_published.push_back(policy_published);
        } else if (name == "record") {
            Record record = parseRecord(pNode);
            feedback.records.push_back(record);
        }

        pNode = pNode->nextSibling();
    }

    return feedback;
}

ReportMetadata XMLParser::parseReportMetadata(Node* pNode) {
    if (pNode->nodeName() != "report_metadata")
        throw "parseReportMetadata(Node*): Expected node name to be 'report_metadata'.";

    ReportMetadata report_metadata;

    pNode = pNode->firstChild();
    while(pNode) {
        string name = pNode->nodeName();
        string value = pNode->innerText();
        if (name == "org_name") {
            report_metadata.org_name = value;
        } else if (name == "email") {
            report_metadata.email = value;
        } else if (name == "extra_contact_info") {
            report_metadata.extra_contact_info = value;
        } else if (name == "report_id") {
            report_metadata.report_id = value;
        } else if (name == "data_range") {
            DataRange data_range = parseDataRange(pNode);
            report_metadata.data_range = data_range;
        }

        pNode = pNode->nextSibling();
    }

    return report_metadata;
}

DataRange XMLParser::parseDataRange(Node* pNode) {
    if (pNode->nodeName() != "data_range")
        throw "parseDataRange(Node*): Expected node name to be 'data_range'.";

    DataRange data_range;

    pNode = pNode->firstChild();
    while(pNode) {
        string name = pNode->nodeName();
        string value = pNode->innerText();
        if (name == "begin") {
            data_range.begin = stoi(value);
        } else if (name == "end") {
            data_range.end = stoi(value);
        }

        pNode = pNode->nextSibling();
    }

    return data_range;
}

PolicyPublished XMLParser::parsePolicyPublished(Node* pNode) {
    if (pNode->nodeName() != "policy_published")
        throw "parsePolicyPublished(Node*): Expected node name to be 'policy_published'.";

    PolicyPublished policy_published;

    pNode = pNode->firstChild();
    while(pNode) {
        string name = pNode->nodeName();
        string value = pNode->innerText();
        if (name == "domain") {
            policy_published.domain = value;
        } else if (name == "adkim") {
            policy_published.adkim = value;
        } else if (name == "aspf") {
            policy_published.aspf = value;
        } else if (name == "p") {
            policy_published.p = value;
        } else if (name == "sp") {
            policy_published.sp = value;
        } else if (name == "pct" && !value.empty()) {
            policy_published.pct = stod(value);
        }

        pNode = pNode->nextSibling();
    }

    return policy_published;
}

Record XMLParser::parseRecord(Node* pNode) {
    if (pNode->nodeName() != "record")
        throw "parseRecord(Node*): Expected node name to be 'record'.";

    Record record;

    pNode = pNode->firstChild();
    while(pNode) {
        string name = pNode->nodeName();
        if (name == "row") {
            Row row = parseRow(pNode);
            record.rows.push_back(row);
        } else if (name == "identifiers") {
            Identifier identifier = parseIdentifier(pNode);
            record.identifier = identifier;
        } else if (name == "auth_results") {
            record.auth_results = parseAuthResults(pNode);
        }

        pNode = pNode->nextSibling();
    }

    return record;
}

Row XMLParser::parseRow(Node* pNode) {
    if (pNode->nodeName() != "row")
        throw "parseRow(Node*): Expected node name to be 'row'.";

    Row row;

    pNode = pNode->firstChild();
    while(pNode) {
        string name = pNode->nodeName();
        string value = pNode->innerText();
        if (name == "source_ip") {
            row.source_ip = value;
        } else if (name == "count" && !value.empty()) {
            row.count = stoi(value);
        } else if (name == "policy_evaluated") {
            row.policy_evaluated = parsePolicyEvaluated(pNode);
        }

        pNode = pNode->nextSibling();
    }

    return row;
}

Identifier XMLParser::parseIdentifier(Node* pNode) {
    if (pNode->nodeName() != "identifiers")
        throw "parseIdentifier(Node*): Expected node name to be 'identifiers'.";

    Identifier identifier;

    pNode = pNode->firstChild();
    while(pNode) {
        string name = pNode->nodeName();
        string value = pNode->innerText();
        if (name == "header_from") {
            identifier.header_from = value;
        }

        pNode = pNode->nextSibling();
    }

    return identifier;
}

PolicyEvaluated XMLParser::parsePolicyEvaluated(Node* pNode) {
    if (pNode->nodeName() != "policy_evaluated")
        throw "parsePolicyEvaluated(Node*): Expected node name to be 'policy_evaluated'.";

    PolicyEvaluated policy_evaluated;

    pNode = pNode->firstChild();
    while(pNode) {
        string name = pNode->nodeName();
        string value = pNode->innerText();
        if (name == "disposition") {
            policy_evaluated.disposition = value;
        } else if (name == "dkim") {
            policy_evaluated.dkim = value;
        } else if (name == "spf") {
            policy_evaluated.spf = value;
        }
        
        pNode = pNode->nextSibling();
    }

    return policy_evaluated;
}

std::vector<AuthResult> XMLParser::parseAuthResults(Node* pNode) {
    if (pNode->nodeName() != "auth_results")
        throw "parseAuthResult(Node*): Expected node name to be 'auth_results'.";

    std::vector<AuthResult> auth_results;

    pNode = pNode->firstChild();
    while(pNode) {
        string name = pNode->nodeName();
        if (name != "spf" && name != "dkim") {
            pNode = pNode->nextSibling();
            continue;
        }
        
        AuthResult auth_result;
        auth_result.type = name;
        
        Node* childNode = pNode->firstChild();
        while(childNode) {
            string name = childNode->nodeName();
            string value = childNode->innerText();
            if (name == "domain") {
                auth_result.domain = value;
            } else if (name == "result") {
                auth_result.result = value;
            }

            childNode = childNode->nextSibling();
        }

        auth_results.push_back(auth_result);

        pNode = pNode->nextSibling();
    }

    return auth_results;
}

XMLParser::XMLParser(string filename) : filename(filename) {
}

Feedback XMLParser::parse() {
    ifstream in(filename);
    InputSource src(in);
    DOMParser parser;

    Document* pDoc = parser.parse(&src);
    Feedback feedback;
    try {
        feedback = parseFeedback(pDoc->firstChild());
    } catch (const char* e) {
        cout << "Exception caught: " << e << endl;
    }

    pDoc->release();
    in.close();

    return feedback;
}

string XMLParser::getRawXML() {
    ifstream in(filename);
    string data((istreambuf_iterator<char>(in)),
                istreambuf_iterator<char>());

    return data;
}