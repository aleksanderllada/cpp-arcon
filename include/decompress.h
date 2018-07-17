#ifndef ZIP_H
#define ZIP_H

#include <string>
#include <istream>
using std::string;
using std::istream;

/**
 * Static class for unzipping files.
 */
class Decompress {
private:
    // This is a static class, it shouldn't be instantiated.
    Decompress() {};

public:
    /**
     * Unzips a compressed file into a directory.
     * @param filename The name of the zip file.
     * @param outputDir {The output directory to which the files will be decompressed.
     * Attempts to create the directory if it doesn't exist.}
     */
    static void gz(istream& stream, string filename);
};

#endif