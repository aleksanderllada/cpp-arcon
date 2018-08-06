#include "decompress.h"

#include "Poco/Path.h"
#include "Poco/InflatingStream.h"
#include "Poco/File.h"
#include "Poco/Zip/Decompress.h"

#include <fstream>
#include <exception>
#include <iostream>

using Poco::File;
using Poco::InflatingInputStream;
using Poco::InflatingStreamBuf;
using namespace std;

void Decompress::gz(istream& stream, string filename) {
    File f("xml/");
    f.createDirectory();
    
    // Decompress into the directory.
    cout << "Inflating " << filename << endl;

    string data, xml;
    InflatingInputStream inflater(stream, InflatingStreamBuf::STREAM_GZIP);
    do {
        data.clear();
        inflater >> data;
        xml += data + '\n';
    } while (data.length());

    ofstream out(filename);

    if (!out.is_open()) {
        cout << "Decompress::gz: Could not open the output file." << endl;
        return;
    }

    out << xml;
    out.close();
}

void Decompress::zip(istream& stream, string filename) {
    Poco::Zip::Decompress dec(stream, filename);
    dec.decompressAllFiles();
}
