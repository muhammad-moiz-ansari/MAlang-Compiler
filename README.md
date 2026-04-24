# 🛠️ CS4031 — Compiler Construction
### Spring 2026 | FAST-NUCES

> A complete compiler front-end implementation across three assignments — a hand-built lexical analyzer, a full LL(1) predictive parser, and a bottom-up SLR(1)/LR(1) parser, all built from scratch in Java and C++.

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
├── 📂 Assignment-03/          ← Bottom-Up Parser: SLR(1) + LR(1) (C++)
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

### Quick Start

```bash
build.bat run
```

→ **[Full Assignment 02 README](./Assignment-02/README.md)**

---

## 📙 Assignment 03 — Bottom-Up Parser (SLR(1) + LR(1))

**Language:** C++ &nbsp;|&nbsp; **Input:** Any Context-Free Grammar

A complete bottom-up parser implementing both SLR(1) and LR(1) parsing from scratch — including canonical item set construction, parsing table generation, shift-reduce simulation, conflict detection, and parse tree output.

### What It Does

| Step | Description |
|------|-------------|
| 1 | Reads a CFG from a text file |
| 2 | **Augments** the grammar with `S' -> S` |
| 3 | Computes **FIRST** and **FOLLOW** sets |
| 4 | Builds **LR(0) canonical collection** (for SLR) |
| 5 | Builds **SLR(1) parsing table** using FOLLOW sets |
| 6 | Builds **LR(1) canonical collection** with lookaheads |
| 7 | Builds **LR(1) parsing table** using per-item lookaheads |
| 8 | Parses input strings with a **shift-reduce stack algorithm** |
| 9 | Detects **shift/reduce** and **reduce/reduce** conflicts |
| 10 | Generates **ASCII parse trees** for accepted strings |
| 11 | Outputs a **comparison** of SLR(1) vs LR(1): states, time, memory |

### Example

**Input Grammar:**
```
Expr -> Expr + Term | Term
Term -> Term * Factor | Factor
Factor -> ( Expr ) | id
```

**SLR(1) Parsing Trace for `id + id * id`:**
```
Step  | Stack                          | Input               | Action
----------------------------------------------------------------------
1     | $                              | id + id * id $      | Shift 5
2     | $ id                           | + id * id $         | Reduce Factor -> id
3     | $ Factor                       | + id * id $         | Reduce Term -> Factor
4     | $ Term                         | + id * id $         | Reduce Expr -> Term
5     | $ Expr                         | + id * id $         | Shift 6
...
Result: ACCEPTED
```

**SLR(1) vs LR(1) at a Glance:**

| Aspect | SLR(1) | LR(1) |
|--------|--------|-------|
| Reduce uses | FOLLOW(A) | Per-item lookahead |
| States | Fewer | More |
| Power | Weaker | Stronger |
| Conflicts | Possible | Fewer |

Grammar 3 (`Start -> L = R | R`) is a classic example where SLR(1) fails with a conflict but LR(1) parses it correctly — demonstrated automatically by selecting grammar 3 at runtime.

### Quick Start

```bash
# Build and run
build.bat run

# Select grammar at the prompt
> 1   (simple expressions)
> 3   (SLR conflict demo)
```

→ **[Full Assignment 03 README](./Assignment-03/README.md)**

---

## 🔧 Technologies Used

| | Assignment 01 | Assignment 02 | Assignment 03 |
|--|---------------|---------------|---------------|
| **Language** | Java | C++ (C++17) | C++ (C++17) |
| **Build** | `javac` / JFlex | Visual Studio / g++ | Visual Studio / g++ |
| **Key Concepts** | DFA, NFA, Regex, Symbol Tables | CFG, LL(1), FIRST/FOLLOW, Parse Trees | LR(0)/LR(1) Items, SLR(1), LR(1), Shift-Reduce |
| **External Tools** | JFlex 1.9.1 | None | None |

---

## 📚 Course Information

| | |
|--|--|
| **Course** | CS4031 — Compiler Construction |
| **Institute** | FAST-NUCES |
| **Semester** | Spring 2026 |
