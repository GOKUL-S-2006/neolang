#pragma once
#include "interpreter.h"

// ─────────────────────────────────────────────────────────────
//  REPL: Read → Evaluate → Print → Loop
//
//  The REPL owns ONE interpreter instance.
//  That interpreter's symbol table (m_symbols) persists
//  across every line the user types — this is what gives
//  the REPL its "memory".
//
//  Each iteration:
//    1. Print prompt
//    2. Read one line
//    3. Lex + Parse + Run it
//    4. Catch any errors (don't crash, just report)
//    5. Loop
// ─────────────────────────────────────────────────────────────
class Repl {
public:
    Repl();
    void start();   // enters the infinite loop

private:
    Interpreter m_interpreter;   // ONE instance, lives forever

    void printWelcome();
    void printPrompt();
    bool isExitCommand(const std::string& input);
    void runLine(const std::string& line);
};