#pragma once
#include <string>
#include <memory>    // unique_ptr
#include <vector>

// ─────────────────────────────────────────────────────────────
//  WHY unique_ptr?
//
//  Tree nodes point to child nodes. We use unique_ptr<ASTNode>
//  instead of raw pointers (ASTNode*) because:
//  1. No memory leaks — memory is freed automatically
//  2. Ownership is clear — each node OWNS its children
//  3. No manual delete needed
// ─────────────────────────────────────────────────────────────


// ─────────────────────────────────────────────────────────────
//  BASE CLASS — every node in the tree IS an ASTNode
//
//  This is pure virtual (abstract). You can never create a raw
//  ASTNode — only its subtypes (NumberNode, LetNode, etc.)
//
//  virtual ~ASTNode() = default  →  ensures child destructors
//  are called properly when deleting through a base pointer.
// ─────────────────────────────────────────────────────────────
struct ASTNode {
    virtual ~ASTNode() = default;
};

// Shorthand alias — saves typing everywhere
using NodePtr = std::unique_ptr<ASTNode>;


// ─────────────────────────────────────────────────────────────
//  NumberNode — a literal integer value
//
//  Example:  10, 42, 7
//
//  Tree position:
//       NumberNode(10)     ← leaf node, no children
// ─────────────────────────────────────────────────────────────
struct NumberNode : ASTNode {
    int value;

    explicit NumberNode(int v) : value(v) {}
};


// ─────────────────────────────────────────────────────────────
//  VarNode — a variable being READ (not assigned)
//
//  Example:  x, myVar, result
//
//  Tree position:
//       VarNode("x")       ← leaf node, no children
// ─────────────────────────────────────────────────────────────
struct VarNode : ASTNode {
    std::string name;

    explicit VarNode(std::string n) : name(std::move(n)) {}
};


// ─────────────────────────────────────────────────────────────
//  BinaryOpNode — a math operation between two expressions
//
//  Example:  x + 5,   10 - 3,   a * b
//
//  Tree for "x + 5":
//         BinaryOpNode('+')
//         /              \
//     VarNode("x")   NumberNode(5)
//
//  'op' stores the operator character: '+' '-' '*' '/'
//  'left' and 'right' are the two sides (any expression)
// ─────────────────────────────────────────────────────────────
struct BinaryOpNode : ASTNode {
    char    op;
    NodePtr left;
    NodePtr right;

    BinaryOpNode(char o, NodePtr l, NodePtr r)
        : op(o), left(std::move(l)), right(std::move(r)) {}
};


// ─────────────────────────────────────────────────────────────
//  LetNode — a variable declaration + assignment
//
//  Example:  let x = 10 + 5;
//
//  Tree:
//         LetNode("x")
//              |
//         BinaryOpNode('+')
//         /              \
//    NumberNode(10)   NumberNode(5)
//
//  'name' is the variable being declared
//  'value' is whatever expression is on the right side
// ─────────────────────────────────────────────────────────────
struct LetNode : ASTNode {
    std::string name;
    NodePtr     value;

    LetNode(std::string n, NodePtr v)
        : name(std::move(n)), value(std::move(v)) {}
};


// ─────────────────────────────────────────────────────────────
//  PrintNode — a print statement
//
//  Example:  print(y);   print(10 + 5);
//
//  Tree:
//         PrintNode
//              |
//         VarNode("y")     ← the thing being printed
// ─────────────────────────────────────────────────────────────
struct PrintNode : ASTNode {
    NodePtr expr;   // what to print (any expression)

    explicit PrintNode(NodePtr e) : expr(std::move(e)) {}
};


// ─────────────────────────────────────────────────────────────
//  ProgramNode — the ROOT of the entire tree
//
//  A NeoLang program is just a list of statements.
//  This node holds ALL of them together.
//
//  Tree for a full program:
//         ProgramNode
//         /    |     \
//    LetNode  LetNode  PrintNode
//       ...    ...       ...
// ─────────────────────────────────────────────────────────────
struct ProgramNode : ASTNode {
    std::vector<NodePtr> statements;
};