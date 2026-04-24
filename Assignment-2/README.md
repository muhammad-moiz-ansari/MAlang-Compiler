# CS4031 — Compiler Construction
## Assignment 02: LL(1) Parser Design & Implementation

---

## Team Members

| Name | Roll Number |
|------|-------------|
| Moiz Ansari | 23i-0523 |
| Abdullah Siddiqui | 23i-0617 |

**Section:** F  
**Language:** C++  
**IDE:** Visual Studio (Windows)

---

## Project Structure

```
CC Asgn-2/
├── src/
│   ├── main.cpp                  # Entry point — orchestrates all steps
│   ├── grammar.h / grammar.cpp   # Grammar structs + file loading + helpers
│   ├── left_factoring.h / .cpp   # Left factoring transformation
│   ├── left_recursion.h / .cpp   # Left recursion removal (direct + indirect)
│   ├── first_follow.h / .cpp     # FIRST and FOLLOW set computation
│   ├── parser.h / parser.cpp     # LL(1) parsing table + stack-based parser
│   ├── tree.h / tree.cpp         # Parse tree construction + ASCII printing
│   ├── stack.h                   # Generic stack implementation (header-only)
│   └── error_handler.h / .cpp    # Error detection and recovery
├── input/
│   ├── grammar1_left_recursion1.txt
│   ├── grammar1_left_recursion2.txt
│   ├── grammar1_left_recursion3.txt
│   ├── grammar1_left_recursion4.txt
│   ├── grammar1_left_recursion5.txt
│   ├── grammar2_left_factor.txt
│   ├── grammar3_first_follow1.txt   ... grammar3_first_follow5.txt
│   ├── grammar4_LR_LF_1.txt
│   ├── grammar5_all_accept1.txt
│   ├── grammar5_all_accept2.txt
│   ├── grammar5_error_handle1.txt
│   ├── input5_all_accept1.txt
│   ├── input5_all_accept2.txt
│   └── input5_error_handle1.txt
├── output/
│   ├── grammar_transformed.txt   # Grammar after LR removal + left factoring
│   ├── parsing_table.txt         # LL(1) parsing table
│   ├── parsing_trace1.txt        # Parsing trace for input string 1
│   ├── parsing_trace2.txt        # ... and so on per input string
│   └── parse_trees.txt           # All parse trees (appended)
├── docs/
│   └── report.pdf
├── build.bat                     # Windows build script (g++)
└── README.md
```

---

## Compilation Instructions

### Option A — Visual Studio (Recommended on Windows)

1. Open `CC Asgn-2.sln` in Visual Studio
2. Make sure all `.cpp` files are included in the project  
   *(Right-click project → Add → Existing Item if any are missing)*
3. Set build config to **Debug** or **Release** (x64)
4. Press **Ctrl+Shift+B** to build
5. Press **Ctrl+F5** to run without debugging

### Option B — g++ via Command Line (MinGW)

Install MinGW from https://www.mingw-w64.org if not already installed.

```bat
:: Compile
build.bat

:: Compile and immediately run
build.bat run

:: Clean compiled output
build.bat clean
```

Or manually:
```bat
g++ -std=c++17 -Wall -Isrc src/main.cpp src/grammar.cpp src/left_factoring.cpp src/left_recursion.cpp src/first_follow.cpp src/parser.cpp src/tree.cpp -o ll1parser.exe
ll1parser.exe
```

---

## Execution Instructions

### Step 1 — Prepare input files

Place your grammar file in the `input/` folder.  
Place your input strings file in the `input/` folder.

### Step 2 — Set the filename suffix in `main.cpp`

Open `src/main.cpp` and change this line to match your input files:

```cpp
string filename_suffix = "5_all_accept1";
//                        ^^^^^^^^^^^^^^
// This loads:  input/grammar5_all_accept1.txt
//              input/input5_all_accept1.txt
```

### Step 3 — Run

Build and run via Visual Studio or `build.bat run`.

### Step 4 — Check outputs

All output files are written to the `output/` folder automatically:
- `grammar_transformed.txt` — transformed grammar
- `parsing_table.txt` — LL(1) parsing table
- `parsing_trace1.txt`, `parsing_trace2.txt`, ... — one file per input string
- `parse_trees.txt` — all parse trees appended together

---

## Input File Formats

### Grammar File Format (`input/grammarX.txt`)

```
NonTerminal -> production1 | production2 | ...
```

Rules:
- One non-terminal per line
- Use `->` as the arrow
- Use `|` to separate alternatives (with spaces around it)
- Non-terminals: multi-character names starting with **uppercase** (e.g. `Expr`, `Term`)
- Terminals: lowercase words, operators, keywords (e.g. `id`, `+`, `if`, `then`)
- Epsilon: use `epsilon` or `@`
- The **first** rule's non-terminal is automatically the start symbol

**Example (`grammar5_all_accept1.txt`):**
```
Expr -> Expr + Term | Term
Term -> Term * Factor | Factor
Factor -> ( Expr ) | id
```

### Input Strings File Format (`input/inputX.txt`)

```
token1 token2 token3 ...
```

Rules:
- One input string per line
- Tokens separated by spaces
- Only terminals from the grammar (no `$` needed — added automatically)

**Example (`input5_all_accept1.txt`):**
```
id + id * id
( id + id ) * id
id
```

---

## Sample Grammar and What Happens

**Input grammar:**
```
Expr -> Expr + Term | Term
Term -> Term * Factor | Factor
Factor -> ( Expr ) | id
```

**After left recursion removal:**
```
Expr -> Term NEWExpr
NEWExpr -> + Term NEWExpr | epsilon
Term -> Factor NEWTerm
NEWTerm -> * Factor NEWTerm | epsilon
Factor -> ( Expr ) | id
```

**After left factoring:** *(no common prefixes remain in this grammar)*

**FIRST sets:**
```
Expr:    { (, id }
NEWExpr: { +, epsilon }
Term:    { (, id }
NEWTerm: { *, epsilon }
Factor:  { (, id }
```

**FOLLOW sets:**
```
Expr:    { ), $ }
NEWExpr: { ), $ }
Term:    { +, ), $ }
NEWTerm: { +, ), $ }
Factor:  { *, +, ), $ }
```

**Parsing trace for `id + id`:**
```
Step | Stack                                             | Input               | Action
-----|---------------------------------------------------|---------------------|-------------------------
1    | $ Expr                                            | id + id $           | Gen Expr -> Term NEWExpr
2    | $ NEWExpr Term                                    | id + id $           | Gen Term -> Factor NEWTerm
3    | $ NEWExpr NEWTerm Factor                          | id + id $           | Gen Factor -> id
4    | $ NEWExpr NEWTerm id                              | id + id $           | Match id
5    | $ NEWExpr NEWTerm                                 | + id $              | Gen NEWTerm -> epsilon
6    | $ NEWExpr                                         | + id $              | Gen NEWExpr -> + Term NEWExpr
...
```

---

## Known Limitations

- Grammar non-terminals must start with an **uppercase letter** — single character non-terminals like `E`, `T`, `F` are not supported (use `Expr`, `Term`, `Factor`)
- Indirect left recursion handling depends on the **order** non-terminals are listed in the grammar file
- Error recovery uses **panic mode** — pops the stack or skips input tokens until a valid synchronizing symbol is found
- Parse tree is only generated for strings that are **fully accepted** without errors
- Very long production rules may be trimmed in the parsing table display (shown as `...`)

---

## How to Test with Different Grammars

Just change the `filename_suffix` in `main.cpp`:

```cpp
// Test grammar 1 (left recursion)
string filename_suffix = "1_left_recursion1";

// Test grammar 2 (left factoring)
string filename_suffix = "2_left_factor";

// Test grammar 5 with error handling
string filename_suffix = "5_error_handle1";
```

Each suffix loads `input/grammar<suffix>.txt` and `input/input<suffix>.txt`.
