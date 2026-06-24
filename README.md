


<img width="1254" height="1254" alt="ChatGPT Image Jun 24, 2026, 11_54_41 PM" src="https://github.com/user-attachments/assets/f83d39d2-d461-446b-a284-c36b83e9c259" />



# 🚀 NeoLang — AI-Powered Mini Programming Language

NeoLang is a beginner-friendly yet powerful custom programming language built using C++. It is designed to demonstrate how compilers and interpreters work internally, while also introducing AI-inspired capabilities such as natural language to code conversion and intelligent error suggestions.

---

## 🧾 Overview

NeoLang provides a complete compiler pipeline:

**Lexer → Parser → AST → Interpreter**

Along with this, it introduces smart features that make programming more intuitive and beginner-friendly.

---

## ✨ Features

### ✅ Core Compiler Features

* 🔤 **Lexical Analysis (Tokenizer)**
  Converts source code into tokens.

* 🌳 **Syntax Parsing (Recursive Descent Parser)**
  Validates syntax and builds structured representations.

* 🧠 **Abstract Syntax Tree (AST)**
  Represents program structure in a tree format.

* ⚙️ **Interpreter**
  Executes the AST directly.

* 📦 **Symbol Table**
  Stores variables and manages scope.

---

### 🤖 AI Features (Key Differentiator)

* 🔥 **Natural Language → Code Conversion**
  Example:

  ```
  Input:  print sum of 10 and 20
  Output: print(10 + 20);
  ```

* 🔥 **Smart Error Suggestions**
  Example:

  ```
  Error: Unexpected token '+'
  Suggestion: Did you forget a number before '+'?
  ```

---

## 🚧 Upcoming Features

* 🖥️ REPL (Interactive Shell)
* 🌲 AST Visualization
* 📘 Code Explanation Engine
* 🔁 Control Flow (if-else, loops)
* 🔧 Functions & Modular Code

---

## 🛠️ Tech Stack

### 💻 Core

* **Language:** C++
* **Compiler:** g++ / clang++
* **Build Tools:** Makefile or CMake

### 📚 Libraries

* STL (vector, map, stack, string)

**Optional:**

* `regex` (for tokenizing)
* `fstream` (for file handling)

---

<img width="1536" height="1024" alt="ChatGPT Image Jun 25, 2026, 12_03_01 AM" src="https://github.com/user-attachments/assets/78e5e2c2-2575-4181-ad49-ab76242abd30" />


## 🧩 Architecture

```
                Input Layer
     (Source Code / Natural Language)
                         ↓
                 NLP Converter
                         ↓
                ─────────────────
                Compiler Pipeline
                ─────────────────
                         ↓
                      Lexer
                (Tokenization)
                         ↓
                      Parser
               (Syntax Analysis)
                         ↓
                        AST
        (Abstract Syntax Tree Representation)
                         ↓
                   Interpreter
                 (Execution Engine)
                         ↓
                     Output
                 (Execution Result)

--------------------------------------------------

                 Core Services
   (Used across all pipeline stages)

   • Symbol Table
   • Type System
   • Error Handler
   • Suggestion Engine
   • Logger

--------------------------------------------------

                Infrastructure Layer

   • File Manager
   • Config Manager
   • Module Manager
   • Utilities
   • Build System
```


## 📁 Project Structure

```
neolang/
│
├── src/
│   ├── lexer.cpp / lexer.h
│   ├── parser.cpp / parser.h
│   ├── ast.cpp / ast.h
│   ├── interpreter.cpp / interpreter.h
│   ├── nlp.cpp / nlp.h
│   ├── utils.cpp / utils.h
│   └── main.cpp
│
├── examples/
│   ├── test1.neo
│   ├── test2.neo
│
├── README.md
├── Makefile / CMakeLists.txt
```

---

## 🔤 Language Syntax (Phase 1)

```neolang
let x = 10;
let y = x + 5;

print(y);
```

### ✅ Supported Features

* Variables (`let`)
* Arithmetic (`+ - * /`)
* Print statements

---

## 🚀 Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/your-username/neolang.git
cd neolang
```

### 2. Build

```bash
g++ src/*.cpp -o neolang
```

### 3. Run

```bash
./neolang examples/test1.neo
```

---

## 🧠 How It Works

| Stage       | Description                 |
| ----------- | --------------------------- |
| Lexer       | Converts code → tokens      |
| Parser      | Tokens → AST                |
| AST         | Tree representation of code |
| Interpreter | Executes the AST            |

---

## 🤝 Contributing

We ❤️ contributions! Follow these steps:

### 🔹 How to Contribute

```bash
# Fork the repository
git checkout -b feature/your-feature

# Make your changes
git commit -m "Add: your feature"

# Push changes
git push origin feature/your-feature
```

Then open a Pull Request 🚀

---

### 🔹 Contribution Guidelines

* Write clean, readable C++ code
* Follow consistent naming conventions
* Add comments for complex logic
* Do not break existing functionality
* Add test cases in `/examples`
* Keep PRs small and focused

---

### 🔹 Code Style

* Use **camelCase** for variables
* Use **PascalCase** for classes
* Keep functions small & modular
* Prefer `const` wherever possible

---

### 🔹 Commit Message Format

* `Add:` New feature
* `Fix:` Bug fix
* `Refactor:` Code improvement
* `Docs:` Documentation updates

---

## 🐛 Issues & Bugs

Use GitHub Issues to report bugs.

Please include:

* Expected behavior
* Actual behavior
* Steps to reproduce

---

## 🌟 Roadmap

* ✅ Lexer
* ✅ Parser
* ✅ AST
* ✅ Interpreter
* 🔄 NLP Module
* 🔄 REPL
* 🔄 Error Suggestion System

---

## 💡 Vision

NeoLang aims to bridge the gap between learning and building by combining compiler fundamentals with AI-driven enhancements — making programming more intuitive, educational, and powerful.

---

⭐ If you like this project, consider giving it a star!
