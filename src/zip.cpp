#include "zip.h"

#include "Poco/Zip/Decompress.h"
#include "Poco/File.h"

#include <fstream>
#include <exception>
#include <iostream>

using Poco::Zip::Decompress;
using Poco::File;
using namespace std;

void Zip::unzip(string filename, string outputDir) {
    ifstream in(filename);

    // Create the directory, if it doesn't exist.
    File dir(outputDir);
    try {
        dir.createDirectory();
    } catch (exception& e) {
        cout << "Exception: Failed to create the './output_zip' directory. Reason: " << e.what() << endl;
        return;
    }

    // Decompress into the directory.
    Decompress(in, outputDir).decompressAllFiles();
}