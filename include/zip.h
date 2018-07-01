#ifndef ZIP_H
#define ZIP_H

#include <string>
using std::string;

class Zip {
private:
    // This is a static class, it shouldn't be instantiated.
    Zip() {};

public:
    static void unzip(string filename, string outputDir = "./output");
};

#endif