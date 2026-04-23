# CS4031 - Compiler Construction
## Assignment 03 — Bottom-Up Parser (SLR(1) and LR(1))
### Spring 2026

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
23i0523-23i0617-F/
│
├── src/
│   ├── main.cpp               # Orchestrates all steps
│   ├── grammar.h/.cpp         # Grammar loading, printing, helpers
│   ├── grammar_augment.h/.cpp # Augments grammar with S' -> S
│   ├── first_follow.h/.cpp    # FIRST and FOLLOW set computation
│   ├── items.h/.cpp           # LR(0) and LR(1) item sets, CLOSURE, GOTO
│   ├── parsing_table.h/.cpp   # ACTION/GOTO table construction
│   ├── slr_parser.h/.cpp      # Builds SLR(1) and LR(1) tables
│   ├── parser.h/.cpp          # Shift-reduce engine + parse tree
│   ├── tree.h/.cpp            # Parse tree node, print functions
│   └── stack.h                # Generic stack (header-only)
│
├── input/
│   ├── grammar1.txt           # Simple expression grammar
│   ├── grammar2.txt           # Expression with multiplication
│   ├── grammar3.txt           # Classic SLR(1) conflict example
│   ├── grammar_with_conflict.txt
│   ├── input_valid.txt        # Valid input strings
│   └── input_invalid.txt      # Invalid input strings
│
├── output/                    # Auto-generated on run
│   ├── augmented_grammar.txt
│   ├── first_follow.txt
│   ├── slr_items.txt
│   ├── slr_parsing_table.txt
│   ├── slr_trace.txt
│   ├── lr1_items.txt
│   ├── lr1_parsing_table.txt
│   ├── lr1_trace.txt
│   ├── parse_trees.txt
│   └── comparison.txt
│
├── docs/
│   └── report.pdf
│
├── Makefile
├── build.bat
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

## Input File Format

### Grammar File (`input/grammar1.txt`)

- One rule per line
- Format: `NonTerminal -> production1 | production2`
- Use `->` as the arrow
- Use `|` to separate alternatives
- **Non-terminals:** Multi-character names starting with uppercase (e.g. `Expr`, `Term`, `Factor`)
- **Single-character non-terminals (E, T, F) are NOT allowed**
- **Terminals:** lowercase letters, operators, keywords (e.g. `id`, `+`, `*`, `(`, `)`)
- **Epsilon:** use `epsilon` or `@`

**Example:**
```
Expr -> Expr + Term | Term
Term -> Term * Factor | Factor
Factor -> ( Expr ) | id
```

### Input Strings File (`input/input_valid.txt`)

- One input string per line
- Symbols separated by spaces
- Do **not** include `$` — it is appended automatically

**Example:**
```
id + id * id
id
( id + id )
```

---

## Sample Commands

### Run SLR(1) parser on grammar1:
```bat
parser.exe
```
*(grammar file is set in `main.cpp` — change `"input/grammar1.txt"` to switch grammars)*

### To test a different grammar, edit line in `main.cpp`:
```cpp
g = loadGrammar("input/grammar2.txt");
```
Then recompile with `build.bat`.

---

## Pipeline Overview

```
grammar.txt
    │
    ▼
loadGrammar()          ← grammar.cpp
    │
    ▼
augmentGrammar()       ← grammar_augment.cpp   (adds S' -> S)
    │
    ▼
computeFirst/Follow()  ← first_follow.cpp
    │
    ├──► buildSLR()    ← slr_parser.cpp
    │         │
    │         ▼
    │    dfa_generate(g)          LR(0) canonical collection
    │    buildParsingTable(...,0) SLR ACTION/GOTO via FOLLOW sets
    │
    └──► buildLR1()   ← slr_parser.cpp
              │
              ▼
         dfa_generate(g, FIRST)   LR(1) canonical collection
         buildParsingTable(...,1) LR(1) ACTION/GOTO via lookaheads
              │
              ▼
         parse()      ← parser.cpp
              │
              ▼
         printTreeColored() ← tree.cpp
```

---

## Output Files

| File | Contents |
|------|----------|
| `augmented_grammar.txt` | Grammar after adding `S' -> S` |
| `first_follow.txt` | FIRST and FOLLOW sets for all non-terminals |
| `slr_items.txt` | All LR(0) item sets (states I0, I1, ...) |
| `slr_parsing_table.txt` | SLR(1) ACTION and GOTO tables |
| `slr_trace.txt` | Step-by-step SLR(1) parse trace for all inputs |
| `lr1_items.txt` | All LR(1) item sets with lookaheads |
| `lr1_parsing_table.txt` | LR(1) ACTION and GOTO tables |
| `lr1_trace.txt` | Step-by-step LR(1) parse trace for all inputs |
| `parse_trees.txt` | ASCII parse trees for all accepted strings |
| `comparison.txt` | SLR(1) vs LR(1) state count comparison |

---

## Known Limitations

- Grammar non-terminals must be multi-character starting with uppercase (`Expr`, not `E`)
- Input symbols must be space-separated
- Grammar file must use `->` (not `→` or `:`)
- Conflicts are detected and reported but parsing halts on first conflict entry (last-write-wins in table)
- Parse tree is only generated for accepted strings

---