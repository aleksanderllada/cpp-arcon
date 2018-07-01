#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <vector>
#include <string>
using std::string;

#include "Poco/DOM/Document.h"
using Poco::XML::Node;

#include "xml_model.h"

#include "string.h"

/**
 * The XMLParser class is responsible for parsing a provided XML into the model.
 */
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
    /**
     * Creates a XMLParser object.
     * @param filename The XML file which this object references.
     */
    XMLParser(string filename);

    /**
     * Attempts to parse the provided XML file. Throws an exception if it isn't able to do so.
     */
    Feedback parse();

    /**
     * Returns the raw data of the XML file.
     */
    string getRawXML();
};

#endif