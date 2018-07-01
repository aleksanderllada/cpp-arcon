#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <vector>

using namespace std;

/**
 * The Application class is a singleton class which is responsible for running the application's main loop.
 */
class Application {
// This is a singleton class. The user must use the builder method.
private:
    Application();

public:
    static Application& get();

    /**
     * Starts the application mainloop with the given arguments.
     */
    int run(vector<string> args);

    /**
     * Overloaded function for receiving C-styled parameters.
     */
    int run(int argc, char** argv);
};

#endif