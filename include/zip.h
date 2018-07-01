#ifndef ZIP_H
#define ZIP_H

#include <string>
using std::string;

/**
 * Static class for unzipping files.
 */
class Zip {
private:
    // This is a static class, it shouldn't be instantiated.
    Zip() {};

public:
    /**
     * Unzips a compressed file into a directory.
     * @param filename The name of the zip file.
     * @param outputDir {The output directory to which the files will be decompressed.
     * Attempts to create the directory if it doesn't exist.}
     */
    static void unzip(string filename, string outputDir = "./output");
};

#endif