#include "../include/repl.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/nlp.h"
#include <iostream>
#include <string>

Repl::Repl() {}

void Repl::start() {
    printWelcome();

    NLPConverter nlp;   // one NLP converter instance
    std::string  line;

    while (true) {
        printPrompt();

        if (!std::getline(std::cin, line)) {
            std::cout << "\nBye!\n";
            break;
        }

        if (line.empty()) continue;

        if (isExitCommand(line)) {
            std::cout << "Bye!\n";
            break;
        }

        // ── NLP step: translate before running ────────────────
        // Every line goes through the NLP converter first.
        // If it matches an English pattern → converted to NeoLang.
        // If not → passed through unchanged (raw NeoLang works too).
        std::string code = nlp.translate(line);

        // Show the user what was generated (teaches NeoLang syntax)
        if (nlp.wasTranslated()) {
            std::cout << "\033[33m→ " << code << "\033[0m\n";
            //          ^^^^^^^^       yellow color shows the translation
        }

        runLine(code);
    }
}

void Repl::runLine(const std::string& line) {
    try {
        Lexer  lexer(line);
        Parser parser(lexer.tokenize());
        auto   ast = parser.parse();

        m_interpreter.run(ast.get());

    } catch (const std::exception& e) {
        std::cout << "\033[31m[Error] " << e.what() << "\033[0m\n";
    }
}

bool Repl::isExitCommand(const std::string& input) {
    return input == "exit" || input == "quit" || input == ":q";
}

void Repl::printWelcome() {
    std::cout << "\033[36m";
    std::cout << "╔══════════════════════════════════╗\n";
    std::cout << "║     NeoLang v0.5 - REPL + NLP    ║\n";
    std::cout << "║  type 'exit' or 'quit' to leave  ║\n";
    std::cout << "║  try: print sum of 10 and 20     ║\n";
    std::cout << "╚══════════════════════════════════╝\n";
    std::cout << "\033[0m\n";
}

void Repl::printPrompt() {
    std::cout << "\033[32mneolang>\033[0m " << std::flush;
}