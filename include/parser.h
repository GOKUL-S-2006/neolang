#pragma once
#include <vector>
#include <memory>
#include "lexer.h"   // needs Token, TokenType
#include "ast.h"     // needs all Node types

// ─────────────────────────────────────────────────────────────
//  Parser: takes a flat list of tokens → builds an AST
//
//  Uses Recursive Descent parsing.
//  Each grammar rule in NeoLang = one private function here.
//
//  Grammar (what NeoLang allows in Phase 1):
//
//  program    → statement*
//  statement  → letStmt | printStmt
//  letStmt    → "let" IDENTIFIER "=" expression ";"
//  printStmt  → "print" "(" expression ")" ";"
//  expression → term ( ("+" | "-") term )*
//  term       → primary ( ("*" | "/") primary )*
//  primary    → NUMBER | IDENTIFIER | "(" expression ")"
// ─────────────────────────────────────────────────────────────
class Parser {
public:
    explicit Parser(std::vector<Token> tokens);

    // Main entry point — call this to get the full AST
    std::unique_ptr<ProgramNode> parse();

private:
    std::vector<Token> m_tokens;   // all tokens from the lexer
    int                m_pos;      // current position in token list

    // ── Token navigation ─────────────────────────────────────
    Token& current();              // peek at current token
    Token& peek(int offset = 1);   // look ahead
    Token  consume();              // take current token, move forward
    Token  expect(TokenType type, const std::string& errorMsg);
                                   // consume BUT crash if wrong type

    bool isAtEnd() const;
    bool check(TokenType type) const;  // is current token this type?

    // ── Grammar rules (one function per rule) ────────────────
    NodePtr parseStatement();
    NodePtr parseLetStatement();
    NodePtr parsePrintStatement();
    NodePtr parseExpression();     // handles + and -  (low precedence)
    NodePtr parseTerm();           // handles * and /  (high precedence)
    NodePtr parsePrimary();        // handles numbers, vars, parentheses
};
