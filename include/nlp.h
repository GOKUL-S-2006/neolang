#pragma once
#include <string>
#include <vector>

// ─────────────────────────────────────────────────────────────
//  NLPConverter: natural language → NeoLang code
//
//  One public function: translate(input) → neolang code
//
//  If the input matches a known English pattern, it returns
//  the equivalent NeoLang code string.
//
//  If nothing matches, it returns the input unchanged —
//  so raw NeoLang code still works fine in the REPL.
//
//  Examples:
//    "print sum of 10 and 20"     → "print(10 + 20);"
//    "store 5 in x"               → "let x = 5;"
//    "print x"                    → "print(x);"
//    "let x = 10;"                → "let x = 10;"  (unchanged)
// ─────────────────────────────────────────────────────────────
class NLPConverter {
public:
    // Main entry point
    // Returns NeoLang code, or the original input if no match
    std::string translate(const std::string& input);

    // Lets REPL check if a line was NLP or raw code (for display)
    bool wasTranslated() const { return m_wasTranslated; }
    std::string lastTranslation() const { return m_lastTranslation; }

private:
    bool        m_wasTranslated   = false;
    std::string m_lastTranslation = "";

    // ── Pattern matchers (one per grammar rule) ───────────────
    // Each returns "" if pattern doesn't match,
    // or the translated code string if it does.

    std::string tryPrintOperation (const std::string& input);
    std::string tryPrintValue     (const std::string& input);
    std::string tryStoreOperation (const std::string& input);
    std::string tryStoreInlineOp  (const std::string& input);
    std::string tryStoreValue     (const std::string& input);

    // ── Helpers ───────────────────────────────────────────────

    // Split "print sum of 10 and 20" → ["print","sum","of","10","and","20"]
    std::vector<std::string> split(const std::string& s);

    // Convert "sum"→"+", "difference"→"-", etc.
    std::string operatorSymbol(const std::string& word);

    // Check if a string is a number ("10") or identifier ("x")
    bool isNumberOrIdent(const std::string& s);

    // Lowercase the entire string for case-insensitive matching
    std::string toLower(const std::string& s);
};