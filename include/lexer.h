#pragma once
#include <string>
#include <vector>

//I HATE PYTHON😭😭
// ─────────────────────────────────────────
//  All possible token types in NeoLang
// ─────────────────────────────────────────
enum class TokenType {
    // Literals
    NUMBER,        // 10, 42, 3
    IDENTIFIER,    // x, myVar, result

    // Keywords
    LET,           // let
    PRINT,         // print

    // Operators
    PLUS,          // +
    MINUS,         // -
    STAR,          // *
    SLASH,         // /
    EQUALS,        // =

    // Punctuation
    LPAREN,        // (
    RPAREN,        // )
    SEMICOLON,     // ;

    // Special
    END_OF_FILE,   // marks end of input
    UNKNOWN        // anything we don't recognize
};

// ─────────────────────────────────────────
//  A Token = type + its actual text value
//  e.g.  { NUMBER, "10" }
//        { IDENTIFIER, "x" }
//        { PLUS, "+" }
// ─────────────────────────────────────────
struct Token {
    TokenType   type;
    std::string value;
    int         line;   // which line it appeared on (for error messages)

    Token(TokenType t, std::string v, int l)
        : type(t), value(std::move(v)), line(l) {}
};

// ─────────────────────────────────────────
//  Lexer: turns raw source text → tokens
// ─────────────────────────────────────────
class Lexer {
public:

    explicit Lexer(const std::string& source);

    // Main function — call this to get all tokens at once
    std::vector<Token> tokenize();

private:
    std::string m_source;   // the raw source code
    int         m_pos;      // current character position
    int         m_line;     // current line number

    // ── helpers ──────────────────────────
    char current() const;           // peek at current char
    char peek(int offset = 1) const;// peek ahead without consuming
    char advance();                 // consume current char, move forward
    void skipWhitespace();          // skip spaces, tabs, newlines

    // ── token builders ───────────────────
    Token readNumber();             // reads multi-digit numbers like "123"
    Token readIdentifierOrKeyword();// reads "let", "print", "myVariable"

    bool isAtEnd() const;
};