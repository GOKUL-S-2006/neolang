#include <iostream>
#include <string>
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/interpreter.h"

int main() {
    std::cout << "==============================\n";
    std::cout << "  NeoLang v0.3 - Interpreter  \n";
    std::cout << "==============================\n\n";

    // ─────────────────────────────────────
    //  TEST 1: Basic variable + print
    // ─────────────────────────────────────
    {
        std::cout << "--- Test 1: Basic variable ---\n";
        std::string src = R"(
            let x = 10;
            let y = x + 5;
            print(y);
        )";

        Lexer  lexer(src);
        Parser parser(lexer.tokenize());
        auto   ast = parser.parse();

        Interpreter interp;
        interp.run(ast.get());
        // Expected output: 15
    }

    // ─────────────────────────────────────
    //  TEST 2: Operator precedence
    //  2 + 3 * 4 should be 14, not 20
    // ─────────────────────────────────────
    {
        std::cout << "\n--- Test 2: Precedence (expect 14) ---\n";
        std::string src = R"(
            let result = 2 + 3 * 4;
            print(result);
        )";

        Lexer  lexer(src);
        Parser parser(lexer.tokenize());
        auto   ast = parser.parse();

        Interpreter interp;
        interp.run(ast.get());
        // Expected output: 14
    }

    // ─────────────────────────────────────
    //  TEST 3: Parentheses override
    //  (2 + 3) * 4 should be 20
    // ─────────────────────────────────────
    {
        std::cout << "\n--- Test 3: Parens (expect 20) ---\n";
        std::string src = R"(
            let result = (2 + 3) * 4;
            print(result);
        )";

        Lexer  lexer(src);
        Parser parser(lexer.tokenize());
        auto   ast = parser.parse();

        Interpreter interp;
        interp.run(ast.get());
        // Expected output: 20
    }

    // ─────────────────────────────────────
    //  TEST 4: Chained variables
    // ─────────────────────────────────────
    {
        std::cout << "\n--- Test 4: Chained vars (expect 100) ---\n";
        std::string src = R"(
            let a = 10;
            let b = a * 2;
            let c = b + a * 8;
            print(c);
        )";

        Lexer  lexer(src);
        Parser parser(lexer.tokenize());
        auto   ast = parser.parse();

        Interpreter interp;
        interp.run(ast.get());
        // Expected: a=10, b=20, c = 20 + 80 = 100
    }

    // ─────────────────────────────────────
    //  TEST 5: Runtime error — undefined var
    // ─────────────────────────────────────
    {
        std::cout << "\n--- Test 5: Undefined variable error ---\n";
        std::string src = R"(
            print(z);
        )";

        try {
            Lexer  lexer(src);
            Parser parser(lexer.tokenize());
            auto   ast = parser.parse();

            Interpreter interp;
            interp.run(ast.get());
        } catch (const std::exception& e) {
            std::cout << "Caught: " << e.what() << "\n";
            // Expected: error saying z is not declared
        }
    }

    std::cout << "\n==============================\n";
    std::cout << "  All tests done!\n";
    std::cout << "==============================\n";

    return 0;
}