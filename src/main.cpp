#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/interpreter.h"
#include "../include/repl.h"

// ─────────────────────────────────────────────────────────────
//  Run a .neo file directly
//  Reads the whole file into a string, then runs it.
// ─────────────────────────────────────────────────────────────
void runFile(const std::string& path) {
    // Open the file
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << path << "'\n";
        return;
    }

    // Read entire file into a string
    // stringstream acts like a string buffer
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    // Run through the pipeline
    try {
        Lexer  lexer(source);
        Parser parser(lexer.tokenize());
        auto   ast = parser.parse();

        Interpreter interp;
        interp.run(ast.get());

    } catch (const std::exception& e) {
        std::cerr << "[Error] " << e.what() << "\n";
    }
}


// ─────────────────────────────────────────────────────────────
//  main()
//
//  Two modes:
//    neolang.exe            → launches the REPL
//    neolang.exe file.neo   → runs a file directly
//
//  argc = argument count (always at least 1, the program name)
//  argv = argument values  argv[0] = "neolang", argv[1] = filename
// ─────────────────────────────────────────────────────────────
int main(int argc, char* argv[]) {
    if (argc == 2) {
        // File mode — run the given .neo file
        runFile(argv[1]);
    } else {
        // Interactive mode — launch REPL
        Repl repl;
        repl.start();
    }

    return 0;
}