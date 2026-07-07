#include "../include/interpreter.h"
#include <iostream>
#include <stdexcept>

// ─────────────────────────────────────────────────────────────
//  run() — entry point
//
//  Gets called with the root ProgramNode.
//  Simply loops through every statement and evaluates it.
//
//  We don't care about the return value of statements
//  (let and print don't produce values, they produce effects)
//  so we just call evaluate() and discard the result.
// ─────────────────────────────────────────────────────────────
void Interpreter::run(const ProgramNode* program) {
    for (const auto& statement : program->statements) {
        evaluate(statement.get());
    }
}


// ─────────────────────────────────────────────────────────────
//  evaluate() — the HEART of the interpreter
//
//  This is the central dispatch function.
//  It receives any ASTNode*, figures out the real type
//  using dynamic_cast, then calls the right evaluator.
//
//  This function is RECURSIVE.
//  When it evaluates a BinaryOpNode, it calls evaluate()
//  on the left and right children — which might call
//  evaluate() again on their children — and so on,
//  all the way down to leaf nodes (NumberNode, VarNode).
//
//  This is post-order tree traversal from your DSA:
//  process children first, then the parent.
// ─────────────────────────────────────────────────────────────
int Interpreter::evaluate(const ASTNode* node) {
    if (!node) {
        throw std::runtime_error("Interpreter error: null node encountered");
    }

    // Try each node type — whichever cast succeeds, that's our type
    if (auto* n = dynamic_cast<const NumberNode*>(node))
        return evalNumber(n);

    if (auto* n = dynamic_cast<const VarNode*>(node))
        return evalVar(n);

    if (auto* n = dynamic_cast<const BinaryOpNode*>(node))
        return evalBinaryOp(n);

    if (auto* n = dynamic_cast<const LetNode*>(node))
        return evalLet(n);

    if (auto* n = dynamic_cast<const PrintNode*>(node))
        return evalPrint(n);

    throw std::runtime_error("Interpreter error: unknown node type");
}


// ─────────────────────────────────────────────────────────────
//  evalNumber() — simplest case
//
//  A number literal just returns itself.
//  NumberNode(42) → returns 42. That's it.
//
//  This is the BASE CASE of the recursion.
//  When we hit a number, we stop going deeper.
// ─────────────────────────────────────────────────────────────
int Interpreter::evalNumber(const NumberNode* node) {
    return node->value;
}


// ─────────────────────────────────────────────────────────────
//  evalVar() — variable lookup
//
//  Looks up the variable name in m_symbols.
//  If found, returns the stored value.
//  If not found, throws a "variable not defined" error.
//
//  This is also a BASE CASE — we don't recurse deeper.
//
//  m_symbols.find() returns an iterator.
//  If it equals m_symbols.end(), the key doesn't exist.
// ─────────────────────────────────────────────────────────────
int Interpreter::evalVar(const VarNode* node) {
    auto it = m_symbols.find(node->name);

    if (it == m_symbols.end()) {
        throw std::runtime_error(
            "Runtime error: Variable '" + node->name +
            "' is used but was never declared with 'let'."
        );
    }

    return it->second;   // it->first = key ("x"), it->second = value (10)
}


// ─────────────────────────────────────────────────────────────
//  evalBinaryOp() — math operations
//
//  This is the RECURSIVE CASE.
//  It evaluates both children first, then applies the operator.
//
//  For BinaryOpNode('+', VarNode("x"), NumberNode(5)):
//    left  = evaluate(VarNode("x"))  → looks up x → 10
//    right = evaluate(NumberNode(5)) → returns 5
//    10 + 5 = 15  ✅
//
//  Notice: evaluate() is called on children, which may
//  themselves be BinaryOpNodes — full recursion.
//
//  Division by zero is caught here with a clear error.
// ─────────────────────────────────────────────────────────────
int Interpreter::evalBinaryOp(const BinaryOpNode* node) {
    int left  = evaluate(node->left.get());   // recurse left
    int right = evaluate(node->right.get());  // recurse right

    switch (node->op) {
        case '+': return left + right;
        case '-': return left - right;
        case '*': return left * right;
        case '/':
            if (right == 0) {
                throw std::runtime_error(
                    "Runtime error: Division by zero."
                );
            }
            return left / right;

        default:
            throw std::runtime_error(
                std::string("Runtime error: Unknown operator '") +
                node->op + "'"
            );
    }
}


// ─────────────────────────────────────────────────────────────
//  evalLet() — variable declaration
//
//  1. Evaluate the right-hand side expression
//  2. Store the result in m_symbols under the variable name
//
//  For "let x = 10 + 5;":
//    evaluate(BinaryOpNode('+', 10, 5)) → 15
//    m_symbols["x"] = 15
//
//  Returns 0 (let statements don't produce a value,
//  they produce a side effect — storing in the symbol table)
//
//  If x already exists, this OVERWRITES it.
//  That means "let x = 5; let x = 10;" → x is 10.
// ─────────────────────────────────────────────────────────────
int Interpreter::evalLet(const LetNode* node) {
    int value = evaluate(node->value.get());  // evaluate right side
    m_symbols[node->name] = value;            // store in symbol table
    return 0;
}


// ─────────────────────────────────────────────────────────────
//  evalPrint() — output
//
//  1. Evaluate the expression inside the parens
//  2. Print the result to stdout
//
//  For "print(y);" where y = 15:
//    evaluate(VarNode("y")) → looks up y → 15
//    prints "15"
//
//  Returns 0 (print is a side-effect statement, no value)
// ─────────────────────────────────────────────────────────────
int Interpreter::evalPrint(const PrintNode* node) {
    int result = evaluate(node->expr.get());
    std::cout << result << "\n";
    return 0;
}