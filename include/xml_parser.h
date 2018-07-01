#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <vector>
#include <string>
using std::string;

#include "Poco/DOM/Document.h"
using Poco::XML::Node;

#include "xml_model.h"

#include "string.h"

class XMLParser {
private:
    string filename;

protected:
    Feedback parseFeedback(Node* pNode);
    ReportMetadata parseReportMetadata(Node* pNode);
    DataRange parseDataRange(Node* pNode);
    PolicyPublished parsePolicyPublished(Node* pNode);
    Record parseRecord(Node* pNode);
    Row parseRow(Node* pNode);
    PolicyEvaluated parsePolicyEvaluated(Node* pNode);
    Identifier parseIdentifier(Node* pNode);
    std::vector<AuthResult> parseAuthResults(Node* pNode);

public:
    XMLParser(string filename);
    Feedback parse();
    string getRawXML();
};

#endif