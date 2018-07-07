#include "decompress.h"

#include "Poco/Path.h"
#include "Poco/InflatingStream.h"
#include "Poco/File.h"

#include <fstream>
#include <exception>
#include <iostream>

using Poco::InflatingInputStream;
using Poco::InflatingStreamBuf;
using namespace std;

void Decompress::gz(string filename, string output) {
    ifstream in(filename, ios::binary);

    // Decompress into the directory.
    string data, xml;
    InflatingInputStream inflater(in, InflatingStreamBuf::STREAM_GZIP);
    do {
        data.clear();
        inflater >> data;
        xml += data + '\n';
    } while (data.length());

    ofstream out(output);

    if (!out.is_open()) {
        cout << "Could not open the output file." << endl;
        return;
    }

    out << xml;
    out.close();
}