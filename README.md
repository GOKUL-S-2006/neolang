# 🚀 NeoLang — A Mini Programming Language built in C++

NeoLang is a custom programming language built from scratch in C++. It implements a complete compiler pipeline and an interactive shell, designed to demonstrate how real languages work internally.

Built as a learning project — every component written by hand, no libraries, no shortcuts.

---

## ✨ What NeoLang Can Do

```neolang
let x = 10;
let y = x + 5;
print(y);         → 15

let result = 2 + 3 * 4;
print(result);    → 14  (correct precedence)

let a = (2 + 3) * 4;
print(a);         → 20  (parentheses work)
```

You can also use plain English:

```
print sum of 10 and 20         → 30
store 5 times 4 in result
print result                   → 20
print difference of 100 and 45 → 55
```

---

## 🧩 Architecture

```
Source Code / Natural Language
              ↓
       NLP Converter          (English → NeoLang syntax)
              ↓
           Lexer              (text → tokens)
              ↓
           Parser             (tokens → AST)
              ↓
              AST             (Abstract Syntax Tree)
              ↓
        Interpreter           (walks AST, executes it)
              ↓
           Output
```

---

## ✅ Features

### Compiler Pipeline
- **Lexer** — tokenizes source code character by character
- **Recursive Descent Parser** — validates syntax, builds AST
- **AST** — tree representation of program structure
- **Interpreter** — tree-walk executor with symbol table
- **Symbol Table** — stores and looks up variable values

### Language Features
- Variable declaration with `let`
- Arithmetic: `+ - * /`
- Correct operator precedence (`*` before `+`)
- Parentheses to override precedence
- `print()` statements
- Meaningful runtime error messages

### NLP Converter
Translate plain English into NeoLang code:

| Input | Generated Code |
|---|---|
| `print sum of 10 and 20` | `print(10 + 20);` |
| `print difference of 100 and 45` | `print(100 - 45);` |
| `print product of 4 and 5` | `print(4 * 5);` |
| `print quotient of 20 and 4` | `print(20 / 4);` |
| `store 5 times 4 in result` | `let result = 5 * 4;` |
| `store sum of 3 and 7 in total` | `let total = 3 + 7;` |
| `store 10 in x` | `let x = 10;` |
| `print x` | `print(x);` |

Case-insensitive — `Print Sum Of 10 And 20` works too.

### REPL (Interactive Shell)
- Type and run NeoLang line by line
- Variables persist across lines
- Errors are caught and reported — shell keeps running
- Raw NeoLang and English input both work

---

## 🛠️ Tech Stack

- **Language:** C++
- **Compiler:** g++ / clang++
- **Libraries:** STL only (`vector`, `map`, `string`, `sstream`)

---

## 📁 Project Structure

```
neolang/
├── include/
│   ├── lexer.h
│   ├── parser.h
│   ├── ast.h
│   ├── interpreter.h
│   ├── nlp.h
│   └── repl.h
├── src/
│   ├── lexer.cpp
│   ├── parser.cpp
│   ├── interpreter.cpp
│   ├── nlp.cpp
│   ├── repl.cpp
│   └── main.cpp
├── examples/
│   └── test1.neo
└── README.md
```

---

## 🚀 Getting Started

### 1. Clone the repository

```bash
git clone https://github.com/your-username/neolang.git
cd neolang
```

### 2. Build

```bash
g++ src/main.cpp src/lexer.cpp src/parser.cpp src/interpreter.cpp src/repl.cpp src/nlp.cpp -I include -o neolang
```

### 3. Run — Interactive REPL

```bash
./neolang          # Linux/Mac
neolang.exe        # Windows
```

### 4. Run — Execute a file

```bash
./neolang examples/test1.neo
```

---

## 🧠 How the Pipeline Works

| Stage | Input | Output |
|---|---|---|
| NLP Converter | `print sum of 10 and 20` | `print(10 + 20);` |
| Lexer | `print(10 + 20);` | `[PRINT][LPAREN][NUMBER:10][PLUS][NUMBER:20][RPAREN][SEMICOLON]` |
| Parser | token list | AST tree |
| Interpreter | AST tree | `30` printed to terminal |

---

## 🔤 Language Syntax

```neolang
let x = 10;
let y = x + 5;
print(y);
```

```neolang
let result = (2 + 3) * 4;
print(result);
```

Exit the REPL:
```
exit   or   quit   or   :q
```

---

## 🔹 Code Style

- `PascalCase` for classes (`Lexer`, `Parser`, `ASTNode`)
- `camelCase` for variables and functions
- `m_` prefix for class member variables (`m_symbols`, `m_pos`)
- `const` wherever possible
- `unique_ptr` for all owned pointers — no raw `new`/`delete`

---

## 🔹 Commit Message Format

- `Add:` New feature
- `Fix:` Bug fix
- `Refactor:` Code improvement
- `Docs:` Documentation updates

---

## 🌟 Roadmap

- ✅ Lexer
- ✅ Parser + AST
- ✅ Interpreter + Symbol Table
- ✅ REPL
- ✅ NLP Converter
- 🔄 if / else statements
- 🔄 while loops
- 🔄 Functions
- 🔄 String support

---

⭐ If you found this useful, give it a star!