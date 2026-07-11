#include "../include/repl.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include <iostream>
#include <string>

// ─────────────────────────────────────────────────────────────
//  Constructor
//  Nothing to do — m_interpreter default-constructs itself
//  (its symbol table starts empty, which is what we want)
// ─────────────────────────────────────────────────────────────
Repl::Repl() {}


// ─────────────────────────────────────────────────────────────
//  start() — the main loop
//
//  This runs forever until the user types "exit" or "quit",
//  or hits Ctrl+C / Ctrl+D.
//
//  std::getline(std::cin, line) reads one full line including
//  spaces. Unlike cin >> x, it doesn't stop at whitespace.
//
//  if (!std::getline(...)) handles Ctrl+D (EOF on Linux/Mac)
//  and Ctrl+Z (EOF on Windows) — both gracefully exit.
// ─────────────────────────────────────────────────────────────
void Repl::start() {
    printWelcome();

    std::string line;

    while (true) {
        printPrompt();

        // Read a full line — exit cleanly on EOF (Ctrl+D / Ctrl+Z)
        if (!std::getline(std::cin, line)) {
            std::cout << "\nBye!\n";
            break;
        }

        // Skip empty lines — just re-show the prompt
        if (line.empty()) continue;

        // Check for exit commands
        if (isExitCommand(line)) {
            std::cout << "Bye!\n";
            break;
        }

        // Run the line through the full pipeline
        runLine(line);
    }
}


// ─────────────────────────────────────────────────────────────
//  runLine() — the pipeline for ONE line of input
//
//  Runs: Lexer → Parser → Interpreter
//  Wraps everything in try/catch so a bad line doesn't
//  kill the REPL — it just prints the error and loops back.
//
//  The interpreter (m_interpreter) is NOT recreated here.
//  It's the same object every time, so m_symbols keeps
//  all variables from previous lines.
//
//  This is the key insight of the REPL:
//  "let x = 10;" on line 1 and "print(x);" on line 3
//  both use the SAME m_interpreter, so x is still there.
// ─────────────────────────────────────────────────────────────
void Repl::runLine(const std::string& line) {
    try {
        // Step 1: Lex
        Lexer lexer(line);
        std::vector<Token> tokens = lexer.tokenize();

        // Step 2: Parse
        Parser parser(std::move(tokens));
        auto ast = parser.parse();

        // Step 3: Interpret (using the persistent interpreter)
        m_interpreter.run(ast.get());

    } catch (const std::exception& e) {
        // Don't crash — just show the error in red and loop back
        std::cout << "\033[31m[Error] " << e.what() << "\033[0m\n";
        //          ^^^^^^^^            ^^^^^^^^^^^^
        //          red color           reset color
    }
}


// ─────────────────────────────────────────────────────────────
//  isExitCommand()
//  Accepts "exit", "quit", ":q" (vim users will feel at home)
// ─────────────────────────────────────────────────────────────
bool Repl::isExitCommand(const std::string& input) {
    return input == "exit" || input == "quit" || input == ":q";
}


// ─────────────────────────────────────────────────────────────
//  printWelcome() — shown once at startup
// ─────────────────────────────────────────────────────────────
void Repl::printWelcome() {
    std::cout << "\033[36m";   // cyan color
    std::cout << "╔══════════════════════════════╗\n";
    std::cout << "║     NeoLang v0.4 - REPL      ║\n";
    std::cout << "║  type 'exit' or 'quit' to    ║\n";
    std::cout << "║  leave. Have fun!            ║\n";
    std::cout << "╚══════════════════════════════╝\n";
    std::cout << "\033[0m\n";  // reset color
}


// ─────────────────────────────────────────────────────────────
//  printPrompt() — shown before each input
//  \033[32m = green color, \033[0m = reset
//  std::flush forces it to appear before getline reads input
// ─────────────────────────────────────────────────────────────
void Repl::printPrompt() {
    std::cout << "\033[32mneolang>\033[0m " << std::flush;
}