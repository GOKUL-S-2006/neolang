#pragma once
#include <map>
#include <string>
#include "ast.h"

// ─────────────────────────────────────────────────────────────
//  Interpreter: walks the AST and executes it
//
//  One central function: evaluate(node) → int
//
//  Every node returns an integer value after evaluation.
//  (NeoLang Phase 1 only supports integers)
//
//  The Symbol Table (m_symbols) is a map that stores
//  variable name → value.
//
//  Example after running "let x = 10; let y = x + 5;":
//    m_symbols = { "x": 10, "y": 15 }
// ─────────────────────────────────────────────────────────────
class Interpreter {
public:
    // Main entry point — run a full program
    void run(const ProgramNode* program);

private:
    // ── Symbol Table ─────────────────────────────────────────
    // Stores all variables and their current values
    std::map<std::string, int> m_symbols;

    // ── Core evaluator ───────────────────────────────────────
    // Takes ANY node, figures out its type, returns its value.
    // This function is recursive — it calls itself on children.
    int evaluate(const ASTNode* node);

    // ── Per-node evaluators ──────────────────────────────────
    // Each one handles exactly one node type.
    // evaluate() dispatches to these.
    int evalNumber  (const NumberNode*    node);
    int evalVar     (const VarNode*       node);
    int evalBinaryOp(const BinaryOpNode*  node);
    int evalLet     (const LetNode*       node);
    int evalPrint   (const PrintNode*     node);
};