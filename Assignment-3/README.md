# CS4031 — Compiler Construction
## Assignment 03: Bottom-Up Parser (SLR(1) and LR(1))
### Spring 2026

---

## Team Members

| Name | Roll Number |
|------|-------------|
| Moiz Ansari | 23i-0523 |
| Abdullah Siddiqui | 23i-0617 |

**Section:** F  
**Language:** C++17  
**IDE:** Visual Studio (Windows)

---

## Project Structure

```
23i0523-23i0617-F/
│
├── src/
│   ├── main.cpp                  # Entry point — orchestrates all steps, grammar selection
│   ├── grammar.h / grammar.cpp   # Grammar structs, file loading, helpers (isTerminal, isNonTerminal)
│   ├── grammar_augment.h / .cpp  # Augments grammar by adding S' -> S
│   ├── first_follow.h / .cpp     # FIRST and FOLLOW set computation
│   ├── items.h / items.cpp       # LR(0) and LR(1) items, CLOSURE, GOTO, canonical collections
│   ├── parsing_table.h / .cpp    # ACTION/GOTO table construction, conflict detection, save/print
│   ├── slr_parser.h / .cpp       # Calls dfa_generate + buildParsingTable for SLR(1) and LR(1)
│   ├── parser.h / parser.cpp     # Shift-reduce engine, step trace, parse tree integration
│   ├── tree.h / tree.cpp         # ParseTreeNode struct, ASCII + colored tree printing
│   └── stack.h                   # Generic stack (header-only, reused from Assignment 2)
│
├── input/
│   ├── grammar1.txt              # Simple expression grammar (Expr, Term, Factor)
│   ├── grammar2.txt              # Expression with multiplication and parentheses
│   ├── grammar3.txt              # Classic SLR(1) conflict example (LR(1) resolves it)
│   ├── grammar_with_conflict.txt # Grammar demonstrating shift/reduce conflict
│   ├── input_valid1.txt          # Valid input strings for grammar1
│   ├── input_valid2.txt          # Valid input strings for grammar2
│   ├── input_valid3.txt          # Valid input strings for grammar3
│   ├── input_valid4.txt          # Valid input strings for grammar_with_conflict
│   ├── input_invalid1.txt        # Invalid input strings for grammar1
│   ├── input_invalid2.txt        # Invalid input strings for grammar2
│   ├── input_invalid3.txt        # Invalid input strings for grammar3
│   └── input_invalid4.txt        # Invalid input strings for grammar_with_conflict
│
├── output/                       # Auto-generated on every run
│   ├── augmented_grammar.txt     # Grammar after adding S' -> S
│   ├── first_follow.txt          # FIRST and FOLLOW sets
│   ├── slr_items.txt             # All LR(0) item sets (I0, I1, ...)
│   ├── slr_parsing_table.txt     # SLR(1) ACTION and GOTO tables
│   ├── slr_trace.txt             # Step-by-step SLR(1) parse traces
│   ├── lr1_items.txt             # All LR(1) item sets with lookaheads
│   ├── lr1_parsing_table.txt     # LR(1) ACTION and GOTO tables
│   ├── lr1_trace.txt             # Step-by-step LR(1) parse traces
│   ├── parse_trees.txt           # ASCII parse trees for all accepted strings
│   └── comparison.txt            # SLR(1) vs LR(1): states, time, memory
│
├── docs/
│   └── report.pdf
│
├── Makefile                      # For MinGW make
├── build.bat                     # Windows batch build script
└── README.md
```

---

## Compilation Instructions

### Option A — Visual Studio (Recommended on Windows)

1. Open `CC Asgn-3.sln` in Visual Studio
2. Ensure all `.cpp` files under `src/` are included in the project
   *(Right-click project → Add → Existing Item if any are missing)*
3. Set C++ Language Standard to **ISO C++17**:
   Right-click project → Properties → C/C++ → Language → C++ Language Standard → **ISO C++17**
4. Set build config to **Debug** or **Release** (x64)
5. Press **Ctrl+Shift+B** to build
6. Press **Ctrl+F5** to run without debugging

### Option B — build.bat via Command Line (MinGW)

Install MinGW from https://www.mingw-w64.org if not already installed, and ensure `g++` is on your PATH.

```bat
:: Compile only
build.bat

:: Compile and immediately run
build.bat run

:: Clean compiled output
build.bat clean
```

### Option C — Manual g++ compile

```bat
g++ -std=c++17 -Wall -g -o parser.exe src/main.cpp src/grammar.cpp src/grammar_augment.cpp src/first_follow.cpp src/items.cpp src/parsing_table.cpp src/slr_parser.cpp src/parser.cpp src/tree.cpp
parser.exe
```

---

## Execution Instructions

### Step 1 — Run the executable

```bat
parser.exe
```

Or press **Ctrl+F5** in Visual Studio.

### Step 2 — Select a grammar interactively

On startup the program prompts:

```
Choose one of the following grammars (Corresponding input files will be selected automatically):
1. grammar1.txt
2. grammar2.txt
3. grammar3.txt
4. grammar_with_conflict.txt
Enter choice (1-4):
```

Enter a number. The matching grammar and input files are loaded automatically — no manual file editing needed.

### Step 3 — View output

All output files are written to `output/` automatically. The console shows live output with colored parse trees (green = non-terminal, blue = terminal, yellow = epsilon).

---

## Sample SLR(1) Parser Run

**Grammar (grammar1.txt):**
```
Expr -> Expr + Term | Term
Term -> Term * Factor | Factor
Factor -> ( Expr ) | id
```

**After augmentation:**
```
S' -> Expr
Expr -> Expr + Term | Term
Term -> Term * Factor | Factor
Factor -> ( Expr ) | id
```

**Sample LR(0) Items (State I0):**
```
I0:
S'     -> . Expr   , x
Expr   -> . Expr + Term   , x
Expr   -> . Term   , x
Term   -> . Term * Factor   , x
Term   -> . Factor   , x
Factor -> . ( Expr )   , x
Factor -> . id   , x
```

**Sample SLR(1) Parsing Trace for `id + id * id`:**
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

---

## Input File Format

### Grammar File

```
NonTerminal -> production1 | production2 | ...
```

- One rule per line
- Use `->` as arrow, `|` to separate alternatives (spaces around `|`)
- **Non-terminals:** Multi-character, starting with uppercase — `Expr`, `Term`, `Factor`
- **Single-character non-terminals (`E`, `T`, `F`) are NOT allowed**
- **Terminals:** lowercase words, operators, keywords — `id`, `+`, `*`, `(`, `)`
- **Epsilon:** use `epsilon` or `@`
- First rule's non-terminal is automatically the start symbol

**Example:**
```
Expr -> Expr + Term | Term
Term -> Term * Factor | Factor
Factor -> ( Expr ) | id
```

### Input Strings File

- One input string per line
- Symbols separated by spaces
- Do **not** include `$` — appended automatically

**Example:**
```
id + id * id
id
( id + id )
```

---

## Sample Commands

```bat
:: Run SLR(1) + LR(1) on grammar 1 (simple expressions)
parser.exe
> 1

:: Run SLR(1) + LR(1) on grammar 3 (demonstrates SLR conflict resolved by LR1)
parser.exe
> 3
```

Both SLR(1) and LR(1) always run together for the selected grammar. Results are saved to separate output files.

---

## Pipeline Overview

```
grammar.txt
    │
    ▼
loadGrammar()              ← grammar.cpp
    │
    ▼
augmentGrammar()           ← grammar_augment.cpp     adds S' -> S
    │
    ▼
computeFirst/Follow()      ← first_follow.cpp
    │
    ├──► buildSLR()        ← slr_parser.cpp
    │         │
    │         ├── dfa_generate(g)             LR(0) canonical collection (no lookaheads)
    │         └── buildParsingTable(..., 0)   ACTION/GOTO using FOLLOW sets
    │                   │
    │                   ▼
    │              parse()                    ← parser.cpp  (shift-reduce engine)
    │                   │
    │                   ▼
    │              printTreeColored()         ← tree.cpp
    │
    └──► buildLR1()        ← slr_parser.cpp
              │
              ├── dfa_generate(g, FIRST)      LR(1) canonical collection (with lookaheads)
              └── buildParsingTable(..., 1)   ACTION/GOTO using per-item lookaheads
                        │
                        ▼
                   parse()                    ← parser.cpp  (same engine, different table)
                        │
                        ▼
                   printTreeColored()         ← tree.cpp
```

---

## Output Files

| File | Contents |
|------|----------|
| `augmented_grammar.txt` | Grammar after adding `S' -> S` |
| `first_follow.txt` | FIRST and FOLLOW sets for all non-terminals |
| `slr_items.txt` | All LR(0) item sets (I0, I1, ...) |
| `slr_parsing_table.txt` | SLR(1) ACTION and GOTO tables |
| `slr_trace.txt` | Step-by-step SLR(1) traces for valid and invalid inputs |
| `lr1_items.txt` | All LR(1) item sets with lookaheads |
| `lr1_parsing_table.txt` | LR(1) ACTION and GOTO tables |
| `lr1_trace.txt` | Step-by-step LR(1) traces for valid and invalid inputs |
| `parse_trees.txt` | Numbered ASCII parse trees for all accepted strings |
| `comparison.txt` | SLR(1) vs LR(1): state count, construction time, memory usage |

---

## SLR(1) vs LR(1) — Key Difference

| Aspect | SLR(1) | LR(1) |
|--------|--------|-------|
| Reduce decision | Uses FOLLOW(A) for all reductions | Uses exact per-item lookahead |
| States generated | Fewer | More (splits ambiguous merged states) |
| Parsing power | Weaker | Stronger |
| Conflicts | May conflict on grammars LR(1) handles | Handles more grammars cleanly |
| Table build time | Faster | Slower |
| Memory usage | Less | More |

**Grammar 3** (`Start -> L = R | R`, `L -> * R | id`, `R -> L`) is the standard demonstration — SLR(1) has a conflict because `L` appears in FOLLOW(R), making it impossible to decide whether to reduce or shift on `=`. LR(1) resolves this using the lookahead stored in the item itself. Select grammar 3 to see this in action.

---

## Known Limitations

- Non-terminals must be multi-character starting with uppercase (`Expr`, not `E`)
- Input symbols must be space-separated in input files
- Grammar files must use `->` (not `→` or `:`)
- On conflict, last-write-wins in the table (standard for educational implementations)
- Parse tree is only generated for accepted strings, not rejected ones
- Memory figures are approximations based on table entry counts, not actual heap profiling
