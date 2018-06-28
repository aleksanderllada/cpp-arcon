#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <vector>

using namespace std;

class Application {
// This is a singleton class. The user must use the builder method.
private:
    Application();

public:
    static Application& get();

    int run(int argc, char** argv);
    int run(vector<string> args);
};

#endif