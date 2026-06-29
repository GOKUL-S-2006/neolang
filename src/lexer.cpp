#include "../include/lexer.h"
#include <stdexcept>
#include <cctype>    // isdigit(), isalpha(), isalnum()

// ─────────────────────────────────────────
//  Constructor
// ─────────────────────────────────────────
Lexer::Lexer(const std::string& source)
    : m_source(source), m_pos(0), m_line(1) {}


// ─────────────────────────────────────────
//  MAIN FUNCTION
//  Loops through every character and builds
//  a list of tokens.
// ─────────────────────────────────────────
std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (!isAtEnd()) {
        skipWhitespace();

        if (isAtEnd()) break;

        char c = current();

        // ── Numbers: 0-9 ─────────────────
        if (std::isdigit(c)) {
            tokens.push_back(readNumber());
        }

        // ── Identifiers & Keywords ────────
        // identifiers start with a letter or underscore
        else if (std::isalpha(c) || c == '_') {
            tokens.push_back(readIdentifierOrKeyword());
        }

        // ── Single-character tokens ───────
        else {
            switch (c) {
                case '+': tokens.emplace_back(TokenType::PLUS,      "+", m_line); advance(); break;
                case '-': tokens.emplace_back(TokenType::MINUS,     "-", m_line); advance(); break;
                case '*': tokens.emplace_back(TokenType::STAR,      "*", m_line); advance(); break;
                case '/': tokens.emplace_back(TokenType::SLASH,     "/", m_line); advance(); break;
                case '=': tokens.emplace_back(TokenType::EQUALS,    "=", m_line); advance(); break;
                case '(': tokens.emplace_back(TokenType::LPAREN,    "(", m_line); advance(); break;
                case ')': tokens.emplace_back(TokenType::RPAREN,    ")", m_line); advance(); break;
                case ';': tokens.emplace_back(TokenType::SEMICOLON, ";", m_line); advance(); break;

                default:
                    // Unknown character — store it but warn
                    tokens.emplace_back(TokenType::UNKNOWN, std::string(1, c), m_line);
                    advance();
                    break;
            }
        }
    }

    // Always end with EOF token so the parser knows when to stop
    tokens.emplace_back(TokenType::END_OF_FILE, "", m_line);
    return tokens;
}


// ─────────────────────────────────────────
//  Read a multi-digit number
//  e.g.  "123" → Token{ NUMBER, "123" }
// ─────────────────────────────────────────
Token Lexer::readNumber() {
    int startLine = m_line;
    std::string num;

    while (!isAtEnd() && std::isdigit(current())) {
        num += advance();
    }

    return Token(TokenType::NUMBER, num, startLine);
}


// ─────────────────────────────────────────
//  Read an identifier or keyword
//  First checks if it's a keyword (let, print)
//  Otherwise it's a user-defined identifier
// ─────────────────────────────────────────
Token Lexer::readIdentifierOrKeyword() {
    int startLine = m_line;
    std::string word;

    // Identifiers can contain letters, digits, underscores
    while (!isAtEnd() && (std::isalnum(current()) || current() == '_')) {
        word += advance();
    }

    // Check if it's a reserved keyword
    if (word == "let")   return Token(TokenType::LET,   word, startLine);
    if (word == "print") return Token(TokenType::PRINT, word, startLine);

    // Otherwise it's a variable name
    return Token(TokenType::IDENTIFIER, word, startLine);
}


// ─────────────────────────────────────────
//  Skip spaces, tabs, newlines
//  Newlines increment the line counter
//  so error messages show the right line
// ─────────────────────────────────────────
void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = current();
        if (c == '\n') {
            m_line++;
            m_pos++;
        } else if (c == ' ' || c == '\t' || c == '\r') {
            m_pos++;
        } else {
            break;
        }
    }
}


// ─────────────────────────────────────────
//  Helper: are we past the end?
// ─────────────────────────────────────────
bool Lexer::isAtEnd() const {
    return m_pos >= static_cast<int>(m_source.size());
}


// ─────────────────────────────────────────
//  Helper: current character (no consume)
// ─────────────────────────────────────────
char Lexer::current() const {
    if (isAtEnd()) return '\0';
    return m_source[m_pos];
}


// ─────────────────────────────────────────
//  Helper: look ahead without consuming
// ─────────────────────────────────────────
char Lexer::peek(int offset) const {
    int idx = m_pos + offset;
    if (idx >= static_cast<int>(m_source.size())) return '\0';
    return m_source[idx];
}


// ─────────────────────────────────────────
//  Helper: consume current char, move pos
// ─────────────────────────────────────────
char Lexer::advance() {
    return m_source[m_pos++];
}