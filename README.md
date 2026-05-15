# 🛠️ CS4031 — Compiler Construction
### Spring 2026 | FAST-NUCES

> A complete compiler front-end implementation across four assignments — a hand-built lexical analyzer, a full LL(1) predictive parser, a bottom-up SLR(1)/LR(1) parser, and a JSON-to-XML translator using Flex & Bison with AST construction, all built from scratch in Java and C++.

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
├── 📂 Assignment-1/          ← Lexical Analyzer (Java)
│   ├── src/
│   ├── tests/
│   ├── docs/
│   └── README.md
│
├── 📂 Assignment-2/          ← LL(1) Parser (C++)
│   ├── src/
│   ├── input/
│   ├── output/
│   ├── docs/
│   └── README.md
│
├── 📂 Assignment-3/          ← Bottom-Up Parser: SLR(1) + LR(1) (C++)
│   ├── src/
│   ├── input/
│   ├── output/
│   ├── docs/
│   └── README.md
│
├── 📂 Assignment-4/          ← JSON to XML Translator (C++ / Flex / Bison)
│   ├── scanner.l
│   ├── parser.y
│   ├── ast.h
│   ├── ast.cpp
│   ├── main.cpp
│   ├── Makefile
│   ├── tests/
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

→ **[Full Assignment 01 README](./Assignment-1/README.md)**

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

→ **[Full Assignment 02 README](./Assignment-2/README.md)**

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

→ **[Full Assignment 03 README](./Assignment-3/README.md)**

---

## 📒 Assignment 04 — JSON to XML Translator

**Language:** C++ &nbsp;|&nbsp; **Tools:** Flex + Bison

A full compiler-style translator pipeline that reads valid JSON from stdin, tokenizes it with Flex, parses it with Bison, constructs an AST, and outputs well-formed, pretty-printed XML to stdout.

### Pipeline Overview

```
JSON Input (stdin)
       │
       ▼
  [ Flex Lexer ]         ── Tokenizes the input
       │
       ▼
  [ Bison Parser ]       ── Validates grammar, builds AST
       │
       ▼
  [ AST Traversal ]      ── Walks the tree
       │
       ▼
  XML Output (stdout)
```

### What It Does

| Step | Description |
|------|-------------|
| 1 | Reads JSON from standard input |
| 2 | **Flex** lexer tokenizes all JSON constructs — `{ } [ ] : ,`, strings, numbers, `true`/`false`/`null` |
| 3 | **Bison** parser validates the grammar and builds a compact AST during parsing |
| 4 | AST is traversed to generate **well-formed, indented XML** |
| 5 | Lexical and syntax errors reported with **line + column** numbers |

### AST Node Types

| Class | JSON Type | Stores |
|---|---|---|
| `StringNode` | String | `std::string value` |
| `NumberNode` | Number (int/float/scientific) | `double value` |
| `BoolNode` | `true` / `false` | `bool value` |
| `NullNode` | `null` | *(nothing)* |
| `ArrayNode` | Array `[...]` | `vector<ASTNode*> elements` |
| `ObjectNode` | Object `{...}` | `vector<pair<string, ASTNode*>> members` |

All nodes inherit from abstract base `ASTNode` with `printXML()` and `printTree()` virtual methods.

### Conversion Rules

| JSON | XML |
|---|---|
| Entire input | Wrapped in `<root>...</root>` |
| Object `{ "key": val }` | `<key>val</key>` |
| Array `[a, b, c]` | `<item>a</item><item>b</item><item>c</item>` |
| String / Number / Bool | Text content inside tag |
| `null` | Self-closing tag `<tagName/>` |
| Special chars (`& < > "`) | XML-escaped in output |

### Example

**Input:**
```json
{ "author": { "uid": "u1", "name": "Sara" }, "published": true }
```

**Output:**
```
========== AST STRUCTURE ==========
- ObjectNode
    [Key: author]
        - ObjectNode
            [Key: uid]
                - StringNode: "u1"
            [Key: name]
                - StringNode: "Sara"
    [Key: published]
        - BoolNode: true

========== XML OUTPUT =============
<root>
    <author>
        <uid>u1</uid>
        <name>Sara</name>
    </author>
    <published>true</published>
</root>
```

### Quick Start

```bash
# Prerequisites
sudo apt-get install flex bison g++

# Build
make

# Run
./json2xml < input.json

# Redirect output to a file
./json2xml < input.json > output.xml

# Run all tests at once
make run-all

# Clean generated files
make clean
```

### Bonus Features Implemented

| Feature | Detail |
|---|---|
| ✅ Pretty-printed XML | 4-space indentation per nesting level |
| ✅ AST printing | Visual tree displayed before XML output |
| ✅ Column-based error detail | Errors report exact line **and** column number |
| ✅ Unicode escape support | `\uXXXX` decoded to UTF-8 (full BMP range) |
| ✅ Scientific notation | Numbers like `1.5e10`, `2.3E-4`, `-6e+2` supported |

→ **[Full Assignment 04 README](./Assignment-4/README.md)**

---

## 🔧 Technologies Used

| | Assignment 01 | Assignment 02 | Assignment 03 | Assignment 04 |
|--|---------------|---------------|---------------|---------------|
| **Language** | Java | C++ (C++17) | C++ (C++17) | C++ (C++11) |
| **Build** | `javac` / JFlex | Visual Studio / g++ | Visual Studio / g++ | `make` / g++ |
| **Key Concepts** | DFA, NFA, Regex, Symbol Tables | CFG, LL(1), FIRST/FOLLOW, Parse Trees | LR(0)/LR(1) Items, SLR(1), LR(1), Shift-Reduce | Flex, Bison, AST Construction, XML Generation |
| **External Tools** | JFlex 1.9.1 | None | None | Flex, Bison |

---

## 📚 Course Information

| | |
|--|--|
| **Course** | CS4031 — Compiler Construction |
| **Institute** | FAST-NUCES |
| **Semester** | Spring 2026 |