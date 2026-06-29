#include "../include/parser.h"
#include <stdexcept>
#include <iostream>

// ─────────────────────────────────────────────────────────────
//  Constructor
// ─────────────────────────────────────────────────────────────
Parser::Parser(std::vector<Token> tokens)
    : m_tokens(std::move(tokens)), m_pos(0) {}


// ─────────────────────────────────────────────────────────────
//  MAIN ENTRY POINT
//
//  Builds a ProgramNode which holds all statements.
//  Keeps calling parseStatement() until we hit EOF.
// ─────────────────────────────────────────────────────────────
std::unique_ptr<ProgramNode> Parser::parse() {
    auto program = std::make_unique<ProgramNode>();

    while (!isAtEnd()) {
        program->statements.push_back(parseStatement());
    }

    return program;
}


// ─────────────────────────────────────────────────────────────
//  parseStatement()
//
//  Decides WHICH kind of statement we're looking at
//  by checking what the current token is.
//
//  NeoLang Phase 1 has 2 statement types:
//    let x = ...;
//    print(...);
// ─────────────────────────────────────────────────────────────
NodePtr Parser::parseStatement() {
    if (check(TokenType::LET)) {
        return parseLetStatement();
    }
    if (check(TokenType::PRINT)) {
        return parsePrintStatement();
    }

    // Unknown statement — give a helpful error
    throw std::runtime_error(
        "Line " + std::to_string(current().line) +
        ": Unexpected token '" + current().value + "'. " +
        "Expected 'let' or 'print'."
    );
}


// ─────────────────────────────────────────────────────────────
//  parseLetStatement()
//
//  Grammar:  "let" IDENTIFIER "=" expression ";"
//
//  Example:  let x = 10 + 5;
//
//  What we build:
//         LetNode("x")
//              |
//         BinaryOpNode('+')
//         /              \
//    NumberNode(10)   NumberNode(5)
// ─────────────────────────────────────────────────────────────
NodePtr Parser::parseLetStatement() {
    expect(TokenType::LET, "Expected 'let'");

    // The next token MUST be the variable name
    Token nameToken = expect(TokenType::IDENTIFIER,
        "Expected variable name after 'let'");

    expect(TokenType::EQUALS,
        "Expected '=' after variable name '" + nameToken.value + "'");

    // Parse the right-hand side — could be any expression
    NodePtr value = parseExpression();

    expect(TokenType::SEMICOLON,
        "Expected ';' at end of let statement");

    return std::make_unique<LetNode>(nameToken.value, std::move(value));
}


// ─────────────────────────────────────────────────────────────
//  parsePrintStatement()
//
//  Grammar:  "print" "(" expression ")" ";"
//
//  Example:  print(x + 5);
//
//  What we build:
//         PrintNode
//              |
//         BinaryOpNode('+')
//         /              \
//     VarNode("x")   NumberNode(5)
// ─────────────────────────────────────────────────────────────
NodePtr Parser::parsePrintStatement() {
    expect(TokenType::PRINT, "Expected 'print'");

    expect(TokenType::LPAREN,
        "Expected '(' after 'print'");

    // Parse whatever expression is inside the parens
    NodePtr expr = parseExpression();

    expect(TokenType::RPAREN,
        "Expected ')' to close print statement");

    expect(TokenType::SEMICOLON,
        "Expected ';' after print statement");

    return std::make_unique<PrintNode>(std::move(expr));
}


// ─────────────────────────────────────────────────────────────
//  parseExpression()  — handles + and -
//
//  Grammar:  term ( ("+" | "-") term )*
//
//  Why is this SEPARATE from parseTerm()?
//  → Operator precedence!
//
//  2 + 3 * 4  should be  2 + (3*4) = 14, NOT (2+3)*4 = 20
//
//  By calling parseTerm() from inside parseExpression(),
//  terms (with */) get fully resolved FIRST before +/- sees them.
//  This naturally gives * and / higher precedence.
//
//  Trace for "2 + 3 * 4":
//    parseExpression() calls parseTerm() → gets "2"
//    sees '+', calls parseTerm() again
//      parseTerm() calls parsePrimary() → gets "3"
//      sees '*', calls parsePrimary() → gets "4"
//      returns BinaryOp('*', 3, 4)
//    returns BinaryOp('+', 2, BinaryOp('*',3,4))  ✅
// ─────────────────────────────────────────────────────────────
NodePtr Parser::parseExpression() {
    NodePtr left = parseTerm();   // always start with a term

    // Keep consuming + or - as long as we see them
    while (!isAtEnd() &&
           (check(TokenType::PLUS) || check(TokenType::MINUS))) {

        char op = current().value[0];  // '+' or '-'
        consume();                     // move past the operator

        NodePtr right = parseTerm();   // get the right side

        // Wrap both sides in a BinaryOpNode
        // Note: left becomes the NEW left (left-associative)
        // So "1 + 2 + 3" → BinaryOp('+', BinaryOp('+',1,2), 3)
        left = std::make_unique<BinaryOpNode>(op,
                    std::move(left), std::move(right));
    }

    return left;
}


// ─────────────────────────────────────────────────────────────
//  parseTerm()  — handles * and /
//
//  Grammar:  primary ( ("*" | "/") primary )*
//
//  Same structure as parseExpression() but one level deeper.
//  This is how recursive descent naturally encodes precedence.
// ─────────────────────────────────────────────────────────────
NodePtr Parser::parseTerm() {
    NodePtr left = parsePrimary();   // start with a primary

    while (!isAtEnd() &&
           (check(TokenType::STAR) || check(TokenType::SLASH))) {

        char op = current().value[0];  // '*' or '/'
        consume();

        NodePtr right = parsePrimary();

        left = std::make_unique<BinaryOpNode>(op,
                    std::move(left), std::move(right));
    }

    return left;
}


// ─────────────────────────────────────────────────────────────
//  parsePrimary()  — the LEAF level
//
//  Grammar:  NUMBER | IDENTIFIER | "(" expression ")"
//
//  This handles the most basic units — things that can't
//  be broken down further:
//    42          → NumberNode(42)
//    x           → VarNode("x")
//    (x + 5)     → calls parseExpression() recursively!
//
//  The parenthesis case is interesting — "(" resets precedence,
//  so (2+3)*4 works correctly because the inner expression
//  is fully resolved before * sees it.
// ─────────────────────────────────────────────────────────────
NodePtr Parser::parsePrimary() {
    // ── Number literal ────────────────────────────────────────
    if (check(TokenType::NUMBER)) {
        Token t = consume();
        return std::make_unique<NumberNode>(std::stoi(t.value));
    }

    // ── Variable reference ────────────────────────────────────
    if (check(TokenType::IDENTIFIER)) {
        Token t = consume();
        return std::make_unique<VarNode>(t.value);
    }

    // ── Parenthesised expression ──────────────────────────────
    if (check(TokenType::LPAREN)) {
        consume();                          // eat the '('
        NodePtr expr = parseExpression();   // parse inside
        expect(TokenType::RPAREN, "Expected ')' to close expression");
        return expr;
    }

    // ── Nothing matched — syntax error ────────────────────────
    throw std::runtime_error(
        "Line " + std::to_string(current().line) +
        ": Unexpected token '" + current().value + "' in expression. " +
        "Expected a number, variable, or '('."
    );
}


// ═════════════════════════════════════════════════════════════
//  HELPER FUNCTIONS
// ═════════════════════════════════════════════════════════════

// Current token (no move)
Token& Parser::current() {
    return m_tokens[m_pos];
}

// Look ahead without consuming
Token& Parser::peek(int offset) {
    int idx = m_pos + offset;
    if (idx >= static_cast<int>(m_tokens.size()))
        return m_tokens.back();   // return EOF if out of range
    return m_tokens[idx];
}

// Consume current token and advance
Token Parser::consume() {
    Token t = m_tokens[m_pos];
    m_pos++;
    return t;
}

// Consume and ASSERT the type matches — otherwise crash with error
Token Parser::expect(TokenType type, const std::string& errorMsg) {
    if (!check(type)) {
        throw std::runtime_error(
            "Line " + std::to_string(current().line) +
            ": " + errorMsg +
            " (got '" + current().value + "' instead)"
        );
    }
    return consume();
}

// Is current token this type?
bool Parser::check(TokenType type) const {
    return m_tokens[m_pos].type == type;
}

// Are we past the last real token?
bool Parser::isAtEnd() const {
    return m_tokens[m_pos].type == TokenType::END_OF_FILE;
}