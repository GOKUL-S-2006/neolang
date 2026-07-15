#include "../include/nlp.h"
#include <sstream>
#include <algorithm>
#include <cctype>

// ─────────────────────────────────────────────────────────────
//  translate() — main entry point
//
//  Tries each pattern matcher in order.
//  First match wins and returns the translated code.
//  If nothing matches, returns the original input unchanged.
//
//  m_wasTranslated lets the REPL show what was generated,
//  so the user can learn NeoLang syntax from the translations.
// ─────────────────────────────────────────────────────────────
std::string NLPConverter::translate(const std::string& input) {
    m_wasTranslated = false;

    std::string lowered = toLower(input);
    std::string result  = "";

    // Try each pattern — order matters, more specific first
    if (result.empty()) result = tryPrintOperation(lowered);
    if (result.empty()) result = tryPrintValue(lowered);
    if (result.empty()) result = tryStoreOperation(lowered);
    if (result.empty()) result = tryStoreInlineOp(lowered);
    if (result.empty()) result = tryStoreValue(lowered);

    if (!result.empty()) {
        m_wasTranslated   = true;
        m_lastTranslation = result;
        return result;
    }

    // No pattern matched — return original (raw NeoLang passthrough)
    return input;
}


// ─────────────────────────────────────────────────────────────
//  tryPrintOperation()
//
//  Matches patterns like:
//    "print sum of 10 and 20"        → print(10 + 20);
//    "print difference of x and 5"   → print(x - 5);
//    "print product of a and b"       → print(a * b);
//    "print quotient of 20 and 4"    → print(20 / 4);
//
//  Word structure:
//    words[0] = "print"
//    words[1] = "sum" / "difference" / "product" / "quotient"
//    words[2] = "of"
//    words[3] = left operand
//    words[4] = "and"
//    words[5] = right operand
// ─────────────────────────────────────────────────────────────
std::string NLPConverter::tryPrintOperation(const std::string& input) {
    std::vector<std::string> w = split(input);

    // Need exactly 6 words
    if (w.size() != 6) return "";

    // Must start with "print"
    if (w[0] != "print") return "";

    // Second word must be a math keyword
    std::string op = operatorSymbol(w[1]);
    if (op.empty()) return "";

    // Third word must be "of"
    if (w[2] != "of") return "";

    // Fourth and sixth must be numbers or identifiers
    if (!isNumberOrIdent(w[3])) return "";

    // Fifth word must be "and"
    if (w[4] != "and") return "";

    if (!isNumberOrIdent(w[5])) return "";

    // All checks passed — build the NeoLang code
    return "print(" + w[3] + " " + op + " " + w[5] + ");";
}


// ─────────────────────────────────────────────────────────────
//  tryPrintValue()
//
//  Matches patterns like:
//    "print x"             → print(x);
//    "print 42"            → print(42);
//    "print value of x"    → print(x);
//
//  Word structure for "print x":
//    words[0] = "print"
//    words[1] = value/identifier
//
//  Word structure for "print value of x":
//    words[0] = "print"
//    words[1] = "value"
//    words[2] = "of"
//    words[3] = identifier
// ─────────────────────────────────────────────────────────────
std::string NLPConverter::tryPrintValue(const std::string& input) {
    std::vector<std::string> w = split(input);

    if (w.empty() || w[0] != "print") return "";

    // "print x" or "print 42"
    if (w.size() == 2 && isNumberOrIdent(w[1])) {
        return "print(" + w[1] + ");";
    }

    // "print value of x"
    if (w.size() == 4 &&
        w[1] == "value" && w[2] == "of" && isNumberOrIdent(w[3])) {
        return "print(" + w[3] + ");";
    }

    return "";
}


// ─────────────────────────────────────────────────────────────
//  tryStoreOperation()
//
//  Matches patterns like:
//    "store sum of 3 and 4 in result"       → let result = 3 + 4;
//    "store product of a and b in total"    → let total = a * b;
//
//  Word structure:
//    words[0] = "store"
//    words[1] = "sum" / "difference" / "product" / "quotient"
//    words[2] = "of"
//    words[3] = left operand
//    words[4] = "and"
//    words[5] = right operand
//    words[6] = "in"
//    words[7] = variable name
// ─────────────────────────────────────────────────────────────
std::string NLPConverter::tryStoreOperation(const std::string& input) {
    std::vector<std::string> w = split(input);

    if (w.size() != 8)       return "";
    if (w[0] != "store")     return "";

    std::string op = operatorSymbol(w[1]);
    if (op.empty())          return "";

    if (w[2] != "of")        return "";
    if (!isNumberOrIdent(w[3])) return "";
    if (w[4] != "and")       return "";
    if (!isNumberOrIdent(w[5])) return "";
    if (w[6] != "in")        return "";
    if (!isNumberOrIdent(w[7])) return "";

    return "let " + w[7] + " = " + w[3] + " " + op + " " + w[5] + ";";
}


// ─────────────────────────────────────────────────────────────
//  tryStoreValue()
//
//  Matches patterns like:
//    "store 10 in x"     → let x = 10;
//    "store 42 in total" → let total = 42;
//
//  Word structure:
//    words[0] = "store"
//    words[1] = value (number or identifier)
//    words[2] = "in"
//    words[3] = variable name
// ─────────────────────────────────────────────────────────────
std::string NLPConverter::tryStoreValue(const std::string& input) {
    std::vector<std::string> w = split(input);

    if (w.size() != 4)          return "";
    if (w[0] != "store")        return "";
    if (!isNumberOrIdent(w[1])) return "";
    if (w[2] != "in")           return "";
    if (!isNumberOrIdent(w[3])) return "";

    return "let " + w[3] + " = " + w[1] + ";";
}


// ═════════════════════════════════════════════════════════════
//  HELPERS
// ═════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────
//  split() — breaks a sentence into words by spaces
//
//  "print sum of 10 and 20"
//  → ["print", "sum", "of", "10", "and", "20"]
//
//  Uses stringstream as a word-by-word reader.
//  >> on a stringstream reads one whitespace-delimited token.
// ─────────────────────────────────────────────────────────────
std::vector<std::string> NLPConverter::split(const std::string& s) {
    std::vector<std::string> words;
    std::stringstream ss(s);
    std::string word;

    while (ss >> word) {
        words.push_back(word);
    }

    return words;
}


// ─────────────────────────────────────────────────────────────
//  operatorSymbol() — maps English word → operator character
//
//  "sum"        → "+"
//  "difference" → "-"
//  "product"    → "*"
//  "quotient"   → "/"
//  anything else → "" (means "not a math keyword")
// ─────────────────────────────────────────────────────────────
std::string NLPConverter::operatorSymbol(const std::string& word) {
    if (word == "sum")        return "+";
    if (word == "difference") return "-";
    if (word == "product")    return "*";
    if (word == "quotient")   return "/";
    if (word == "plus")       return "+";
    if (word == "minus")      return "-";
    if (word == "times")      return "*";
    if (word == "divided")    return "/";   // "divided by" — partial match
    return "";
}


// ─────────────────────────────────────────────────────────────
//  isNumberOrIdent() — is this a valid operand?
//
//  Valid operands are either:
//  - A number:     all digits          "10", "42"
//  - An identifier: letters/digits/_   "x", "myVar", "result"
//
//  This prevents garbage like "print sum of ??? and ###"
//  from being translated.
// ─────────────────────────────────────────────────────────────
bool NLPConverter::isNumberOrIdent(const std::string& s) {
    if (s.empty()) return false;

    // Check if it's all digits (number)
    bool allDigits = true;
    for (char c : s) {
        if (!std::isdigit(c)) { allDigits = false; break; }
    }
    if (allDigits) return true;

    // Check if it's a valid identifier (starts with letter or _)
    if (!std::isalpha(s[0]) && s[0] != '_') return false;
    for (char c : s) {
        if (!std::isalnum(c) && c != '_') return false;
    }
    return true;
}


// ─────────────────────────────────────────────────────────────
//  toLower() — convert string to all lowercase
//
//  So "Print Sum Of 10 And 20" works same as
//     "print sum of 10 and 20"
//
//  std::tolower() works on single chars — we apply it to each.
// ─────────────────────────────────────────────────────────────
std::string NLPConverter::toLower(const std::string& s) {
    std::string result = s;
    for (char& c : result) {
        c = static_cast<char>(std::tolower(c));
    }
    return result;
}

// ─────────────────────────────────────────────────────────────
//  tryStoreInlineOp() — handles "store X op Y in varname"
//
//  "store 5 times 4 in result"   → let result = 5 * 4;
//  "store 10 plus 5 in total"    → let total = 10 + 5;
//
//  Word structure:
//    words[0] = "store"
//    words[1] = left operand
//    words[2] = "times" / "plus" / "minus" / "divided"
//    words[3] = right operand
//    words[4] = "in"
//    words[5] = variable name
// ─────────────────────────────────────────────────────────────
std::string NLPConverter::tryStoreInlineOp(const std::string& input) {
    std::vector<std::string> w = split(input);

    if (w.size() != 6)          return "";
    if (w[0] != "store")        return "";
    if (!isNumberOrIdent(w[1])) return "";

    std::string op = operatorSymbol(w[2]);
    if (op.empty())             return "";

    if (!isNumberOrIdent(w[3])) return "";
    if (w[4] != "in")           return "";
    if (!isNumberOrIdent(w[5])) return "";

    return "let " + w[5] + " = " + w[1] + " " + op + " " + w[3] + ";";
}