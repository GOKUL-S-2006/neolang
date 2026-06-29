#include <iostream>
#include <string>
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/ast.h"



// ─────────────────────────────────────────
//  Helper: converts TokenType → readable string
//  (only needed for debug printing)
// ─────────────────────────────────────────
std::string tokenTypeName(TokenType type) {
    switch (type) {
        case TokenType::NUMBER:      return "NUMBER";
        case TokenType::IDENTIFIER:  return "IDENTIFIER";
        case TokenType::LET:         return "LET";
        case TokenType::PRINT:       return "PRINT";
        case TokenType::PLUS:        return "PLUS";
        case TokenType::MINUS:       return "MINUS";
        case TokenType::STAR:        return "STAR";
        case TokenType::SLASH:       return "SLASH";
        case TokenType::EQUALS:      return "EQUALS";
        case TokenType::LPAREN:      return "LPAREN";
        case TokenType::RPAREN:      return "RPAREN";
        case TokenType::SEMICOLON:   return "SEMICOLON";
        case TokenType::END_OF_FILE: return "EOF";
        case TokenType::UNKNOWN:     return "UNKNOWN";
        default:                     return "???";
    }
}



// ─────────────────────────────────────────────────────────────
//  AST Printer — walks the tree and prints it visually
//
//  'indent' grows by 2 spaces at each level so you can
//  see the tree structure in the terminal.
//
//  dynamic_cast figures out which node type we're looking at
//  since all nodes are stored as base class ASTNode*
// ─────────────────────────────────────────────────────────────
void printAST(const ASTNode* node, int indent = 0) {
    std::string pad(indent, ' ');
 
    if (!node) {
        std::cout << pad << "(null)\n";
        return;
    }
 
    if (auto* n = dynamic_cast<const ProgramNode*>(node)) {
        std::cout << pad << "ProgramNode\n";
        for (const auto& stmt : n->statements)
            printAST(stmt.get(), indent + 2);
    }
    else if (auto* n = dynamic_cast<const LetNode*>(node)) {
        std::cout << pad << "LetNode (name=\"" << n->name << "\")\n";
        printAST(n->value.get(), indent + 2);
    }
    else if (auto* n = dynamic_cast<const PrintNode*>(node)) {
        std::cout << pad << "PrintNode\n";
        printAST(n->expr.get(), indent + 2);
    }
    else if (auto* n = dynamic_cast<const BinaryOpNode*>(node)) {
        std::cout << pad << "BinaryOpNode (op='" << n->op << "')\n";
        std::cout << pad << "  [left]\n";
        printAST(n->left.get(),  indent + 4);
        std::cout << pad << "  [right]\n";
        printAST(n->right.get(), indent + 4);
    }
    else if (auto* n = dynamic_cast<const NumberNode*>(node)) {
        std::cout << pad << "NumberNode (value=" << n->value << ")\n";
    }
    else if (auto* n = dynamic_cast<const VarNode*>(node)) {
        std::cout << pad << "VarNode (name=\"" << n->name << "\")\n";
    }
    else {
        std::cout << pad << "(unknown node)\n";
    }
}


int main() {
    std::cout << "NeoLang v0.1 — Lexer Test\n";
    std::cout << "==========================\n\n";

    // ── Test source code ─────────────────
    std::string source = R"(
        let x = 10;
        let y = x + 5;
        print(y);
    )";

    std::cout << "Source:\n" << source << "\n";
    std::cout << "Tokens:\n";
    std::cout << "--------------------------\n";

    // ── Run the lexer ─────────────────────
    //Step-1:Tokenize the source code into tokens
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
     
    // ── Print each token ──────────────────
    for (const Token& tok : tokens) {
        std::cout << "Line " << tok.line
                  << "  |  "
                  << tokenTypeName(tok.type)
                  << "\t\"" << tok.value << "\"\n";
    }
    

    
 
    // Step 2: Parse
    Parser parser(std::move(tokens));
    auto ast = parser.parse();
 
    // Step 3: Print the AST
    std::cout << "AST:\n";
    std::cout << "----------------------------\n";
    printAST(ast.get());


     
    return 0;
}