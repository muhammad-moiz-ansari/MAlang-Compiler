# CS-4031 Compiler Construction - Assignment 04
## JSON to XML Translator using Flex & Bison

| | |
|---|---|
| **Course** | CS-4031 Compiler Construction |
| **Assignment** | 04 - JSON to XML Translator |
| **Section** | (your section here) |

### Group Members

| Name | Roll Number |
|---|---|
| Moiz Ansari | 23i-0523 |
| Abdullah Siddiqui | 23i-0617 |

---

## Table of Contents

1. [Project Overview](#project-overview)
2. [File Structure](#file-structure)
3. [Build Instructions](#build-instructions)
4. [Run Instructions](#run-instructions)
5. [AST Structure](#ast-structure)
6. [Conversion Rules](#conversion-rules)
7. [Bonus Features Implemented](#bonus-features-implemented)
8. [Error Handling](#error-handling)
9. [Test Files](#test-files)
10. [Assumptions and Limitations](#assumptions-and-limitations)

---

## Project Overview

This program is a **JSON to XML Translator** built using:
- **Flex** for lexical analysis (tokenizing the JSON input)
- **Bison (Yacc)** for parsing (validating the grammar and building the AST)
- **C++** for AST construction and XML generation

The pipeline works as follows:

```
JSON Input (stdin)
       |
       v
  [ Flex Lexer ]         -- Tokenizes the input
       |
       v
  [ Bison Parser ]       -- Validates grammar, builds AST
       |
       v
  [ AST Traversal ]      -- Walks the tree
       |
       v
  XML Output (stdout)
```

---

## File Structure

```
.
├── scanner.l          # Flex lexer: tokenizes JSON input
├── parser.y           # Bison parser: grammar rules + AST construction
├── ast.h              # AST node class declarations
├── ast.cpp            # AST node method implementations (printXML, printTree)
├── main.cpp           # Entry point: calls yyparse(), prints AST + XML
├── Makefile           # Build automation
├── .gitignore         # Ignores generated C files & headers and compiled executable
├── README.md          # This file
├── expected_outputs/  # Expected XML outputs for the test cases
│   ├── expected_output1.xml
│   ├── expected_output2.xml
│   ├── expected_output3.xml
│   ├── expected_output4.xml
│   └── expected_output5.xml
└── tests/
    ├── test1.json             # Simple object
    ├── test2.json             # Array of scalars
    ├── test3.json             # Nested object
    ├── test4.json             # Null values
    ├── test5.json             # Complex/mixed nested structure
    ├── test6_e.json           # Error: invalid character
    ├── test7_e.json           # Error: syntax error
    ├── test8_e.json           # Error: unterminated string
    └── test9_unicode.json     # Bonus: Unicode escape sequences

```

---

## Build Instructions

### Prerequisites

Make sure the following tools are installed on your system:

```bash
sudo apt-get install flex bison g++
```

### Building the Project

Run the following command in the project root directory:

```bash
make
```

This will:
1. Run **Bison** on `parser.y` to generate `parser.tab.c` and `parser.tab.h`
2. Run **Flex** on `scanner.l` to generate `lex.yy.c`
3. Compile everything with **g++** into the executable `json2xml`

To clean all generated files and the binary:

```bash
make clean
```

---

## Run Instructions

### Single File

The program reads JSON from **standard input** and writes XML to **standard output**:

```bash
./json2xml < input.json
```

### Redirect Output to a File

```bash
./json2xml < input.json > output.xml
```

### Run All Tests at Once

```bash
make run-all
```

This automatically runs all valid tests (`test1–test5`, `test9_unicode`) and all error tests (`test6_e`, `test7_e`, `test8_e`).

### Example

**Input (`tests/test1.json`):**
```json
{ "id": 1, "name": "Ali", "age": 19 }
```

**Output:**
```xml
========== AST STRUCTURE ==========
- ObjectNode
    [Key: id]
        - NumberNode: 1
    [Key: name]
        - StringNode: "Ali"
    [Key: age]
        - NumberNode: 19

========== XML OUTPUT =============
<root>
    <id>1</id>
    <name>Ali</name>
    <age>19</age>
</root>
```

---

## AST Structure

The AST (Abstract Syntax Tree) is built **during parsing** by Bison grammar actions. Each node type corresponds to a JSON value type.

### Node Types

| Class | Represents | Stores |
|---|---|---|
| `StringNode` | JSON string | `std::string value` |
| `NumberNode` | JSON number (int or float) | `double value` |
| `BoolNode` | JSON `true` / `false` | `bool value` |
| `NullNode` | JSON `null` | *(nothing)* |
| `ArrayNode` | JSON array `[...]` | `vector<ASTNode*> elements` |
| `ObjectNode` | JSON object `{...}` | `vector<pair<string, ASTNode*>> members` |

All node classes inherit from the abstract base class `ASTNode`, which declares two pure virtual methods:

```cpp
virtual void printXML(const std::string& tag, int depth) = 0;
virtual void printTree(int depth) = 0;
```

### How the AST is Built

- The **Bison parser** creates new node objects in its grammar action blocks (the `{ ... }` sections).
- For example, when a `key : value` pair is matched, an `ObjectNode` is created and `addMember()` is called.
- The final root of the tree is stored in the global `ASTNode* root` pointer.
- After `yyparse()` returns successfully, `main.cpp` calls `root->printTree()` then `root->printXML()` to produce output.

### Example AST for `{ "scores": [10, 20], "pass": true }`

```
- ObjectNode
    [Key: scores]
        - ArrayNode
            - NumberNode: 10
            - NumberNode: 20
    [Key: pass]
        - BoolNode: true
```

---

## Conversion Rules

| JSON | XML |
|---|---|
| Entire input | Wrapped in `<root>...</root>` |
| Object `{ "key": value }` | `<key>value</key>` |
| Array `[a, b, c]` | `<item>a</item><item>b</item><item>c</item>` |
| String / Number / Bool | Text content inside tag |
| `null` | Empty self-closing tag `<tagName/>` |
| Special characters in values | Escaped (`&` → `&amp;`, `<` → `&lt;`, `>` → `&gt;`, `"` → `&quot;`) |
| Nesting | Fully preserved at all depths |

---

## Bonus Features Implemented

### 1. Pretty-Printed XML (+2)
XML output uses **4-space indentation** per nesting level for human readability, controlled by the `depth` parameter passed through all `printXML()` calls. The helper `getIndent(depth)` generates the appropriate leading spaces.

### 2. AST Printing (+2)
After parsing, the program prints a **visual tree representation** of the AST before the XML output. Each node type is shown with its value and children indented beneath it. Example:
```
========== AST STRUCTURE ==========
- ObjectNode
    [Key: name]
        - StringNode: "Ali"
```

### 3. Column-Based Error Detail (+2)
The lexer tracks both **line number** (`line_num`) and **column number** (`col_num`) using the `YY_USER_ACTION` macro. All error messages include both, for example:
```
Lexical Error at Line 3, Col 12: unexpected character '@'
Syntax Error at Line 2, Col 5: syntax error
```

### 4. Unicode Escape Support (+2)
The `processString()` function in `scanner.l` handles `\uXXXX` escape sequences by converting the 4-digit hex code point to UTF-8 bytes. This supports the full Basic Multilingual Plane (code points U+0000 to U+FFFF).

### 5. Scientific Notation Support (+2)
The number regex pattern in `scanner.l` supports exponent notation:
```
-?[0-9]+(\.[0-9]+)?([eE][+-]?[0-9]+)?
```
This matches numbers like `1.5e10`, `2.3E-4`, `-6e+2`.

---

## Error Handling

The program stops at the **first error** encountered and prints a descriptive message to `stderr`.

### Lexical Errors (from Flex)
Triggered by invalid characters in the input:
```
Lexical Error at Line <L>, Col <C>: unexpected character '<char>'
```

### Syntax Errors (from Bison)
Triggered by structurally invalid JSON (e.g., missing comma, unclosed bracket):
```
Syntax Error at Line <L>, Col <C>: syntax error
```

The program exits with return code `1` on any error, and `0` on success.

---

## Test Files

| File | Description | Expected Behavior |
|---|---|---|
| `test1.json` | Simple flat object with string, number values | Valid - produces XML |
| `test2.json` | Object with an array of scalar strings | Valid - uses `<item>` tags |
| `test3.json` | Nested object inside an object | Valid - preserves nesting |
| `test4.json` | Object with a null value | Valid - produces self-closing tag |
| `test5.json` | Complex mixed structure (arrays, objects, booleans, nulls) | Valid - full feature test |
| `test6_e.json` | Contains an invalid character (e.g. `@`) | Lexical error |
| `test7_e.json` | Missing comma between key-value pairs | Syntax error |
| `test8_e.json` | Unterminated string literal | Lexical error |
| `test9_unicode.json` | String values with `\uXXXX` Unicode escapes | Valid - decoded correctly |

---

## Assumptions and Limitations

- **Input must be valid JSON** - the program does not attempt recovery after the first error; it exits immediately.
- **Keys are assumed to be valid XML tag names** - as per the assignment spec, test cases use simple alphanumeric keys. Keys containing spaces or XML-reserved characters are not sanitized.
- **Numbers are stored as `double`** - very large integers may lose precision due to floating-point representation.
- **Unicode support covers BMP only** - `\uXXXX` escapes are handled for code points U+0000 to U+FFFF. Surrogate pairs (for code points above U+FFFF) are not supported.
- **No memory cleanup** - AST nodes allocated during parsing are not explicitly freed on exit, as the OS reclaims memory when the process ends. For a production compiler this would need proper destructors.
- **Single root value only** - the grammar requires exactly one top-level JSON value. Multiple root values are a syntax error (consistent with the JSON specification).
