# 🛠️ CS4031 — Compiler Construction
### Spring 2026 | FAST-NUCES

> A complete compiler front-end implementation in two assignments — a hand-built lexical analyzer and a full LL(1) predictive parser, built from scratch in Java and C++ respectively.

---

## 👥 Team Members

| Name | Roll Number |
|------|-------------|
| Moiz Ansari | 23i-0523 |
| Abdullah Siddiqui | 23i-0617 |

**Section:** F

---

## 📁 Repository Structure

```
📦 CS4031-Compiler-Construction/
├── 📂 Assignment-01/          ← Lexical Analyzer (Java)
│   ├── src/
│   ├── tests/
│   ├── docs/
│   └── README.md
│
├── 📂 Assignment-02/          ← LL(1) Parser (C++)
│   ├── src/
│   ├── input/
│   ├── output/
│   ├── docs/
│   └── README.md
│
└── README.md                  ← You are here
```

---

## 📘 Assignment 01 — Lexical Analyzer

**Language:** Java &nbsp;|&nbsp; **Target Language:** MAlang (custom)

A hand-built lexical analyzer for a custom programming language called **MAlang**, implemented two ways — a manual DFA scanner and a JFlex-generated scanner.

### What It Does

- Scans `.malang` source files token by token
- Recognizes identifiers, integer/float/string/char/boolean literals, operators, keywords, punctuators, and comments
- Builds a **symbol table** tracking name, type, first occurrence, and frequency
- Reports all **lexical errors** with line and column numbers

### MAlang at a Glance

```
## Single line comment

#*
  Multi-line comment
*#

declare Count : 42 ;
declare Pi    : 3.14159 ;
output ( "Hello, World!" ) ;
```

| Feature | Detail |
|---------|--------|
| Identifiers | Must start with uppercase — `[A-Z][a-z0-9_]{0,30}` |
| Keywords | `start`, `finish`, `loop`, `condition`, `declare`, `output`, `input`, `function`, `return`, `break`, `continue`, `else` |
| Comments | Single-line `##` and multi-line `#* ... *#` |
| Literals | Integer, Float (up to 6 decimal places), String, Char, Boolean |

### Quick Start

```bash
# Compile
javac src/ManualScanner.java src/Token.java src/TokenType.java src/SymbolTable.java src/ErrorHandler.java

# Run
java -cp src ManualScanner
```

→ **[Full Assignment 01 README](./Assignment-01/README.md)**

---

## 📗 Assignment 02 — LL(1) Parser

**Language:** C++ &nbsp;|&nbsp; **Input:** Any Context-Free Grammar

A complete LL(1) predictive parser that takes any CFG as input, transforms it, and parses input strings step by step with a full trace.

### What It Does

| Step | Description |
|------|-------------|
| 1 | Reads a CFG from a text file |
| 2 | Removes **left recursion** (direct + indirect) |
| 3 | Applies **left factoring** |
| 4 | Computes **FIRST** and **FOLLOW** sets |
| 5 | Builds the **LL(1) parsing table** |
| 6 | Parses input strings with a **stack-based algorithm** |
| 7 | Detects and recovers from **syntax errors** |
| 8 | Generates **ASCII parse trees** for accepted strings |

### Example

**Input Grammar:**
```
Expr -> Expr + Term | Term
Term -> Term * Factor | Factor
Factor -> ( Expr ) | id
```

**After Transformation + Parsing `id + id * id`:**
```
Step | Stack                    | Input          | Action
-----|--------------------------|----------------|---------------------------
1    | $ Expr                   | id + id * id $ | Gen Expr -> Term NEWExpr
2    | $ NEWExpr Term           | id + id * id $ | Gen Term -> Factor NEWTerm
3    | $ NEWExpr NEWTerm Factor | id + id * id $ | Gen Factor -> id
4    | $ NEWExpr NEWTerm id     | id + id * id $ | Match id
...
     | $                        | $              | Accept ✓
```

**ASCII Parse Tree:**
```
\-- Expr
    |-- Term
    |   |-- Factor
    |   |   \-- id
    |   \-- NEWTerm
    |       \-- epsilon
    \-- NEWExpr
        |-- +
        |-- Term
        ...
```

### Quick Start

```bash
# With g++ (MinGW)
build.bat run

# Or manually
g++ -std=c++17 -Isrc src/main.cpp src/grammar.cpp src/left_factoring.cpp src/left_recursion.cpp src/first_follow.cpp src/parser.cpp src/tree.cpp -o ll1parser.exe
ll1parser.exe
```

→ **[Full Assignment 02 README](./Assignment-02/README.md)**

---

## 🔧 Technologies Used

| | Assignment 01 | Assignment 02 |
|--|---------------|---------------|
| **Language** | Java | C++ (C++17) |
| **Build** | `javac` / JFlex | Visual Studio / g++ |
| **Key Concepts** | DFA, NFA, Regular Expressions, Symbol Tables | CFG, LL(1) Parsing, FIRST/FOLLOW, Parse Trees |
| **External Tools** | JFlex 1.9.1 | None (no compiler libraries used) |

---

## 📚 Course Information

| | |
|--|--|
| **Course** | CS4031 — Compiler Construction |
| **Institute** | FAST-NUCES |
| **Semester** | Spring 2026 |
