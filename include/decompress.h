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
     * Unzips a compressed gzip file into a directory.
     * @param stream The open stream for reading the file.
     * @param filename The name of the gzip file.
     */
    static void gz(istream& stream, string filename);

    /**
     * Unzips a compressed zip file into a directory.
     * @param stream The open stream for reading the file.
     * @param filename The name of the gzip file.
     */
    static void zip(istream& stream, string filename);
};

#endif
