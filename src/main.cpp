#include "shell/repl.h"
#include <iostream>

int main() {
    try {
        miniql::REPL repl;
        repl.run();
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
}
