# MAlang Compiler — Lexical Analyzer

> **CS4031 — Compiler Construction | Assignment 01 | Spring 2026**

---

## 👥 Team Members

| Name | Roll Number |
|------|-------------|
| Moiz Ansari | 23i-0523 |
| Abdullah Siddiqui | 23i-0617 |

---

## 📌 Language Overview

| Property | Value |
|----------|-------|
| Language Name | **MAlang** |
| File Extension | `.malang` |
| Case Sensitivity | Yes — keywords are lowercase, identifiers start with uppercase |
| Paradigm | Custom structured / procedural |

---

## 🔑 Keywords

| Keyword | Meaning |
|---------|---------|
| `start` | Marks the beginning of the program / block |
| `finish` | Marks the end of the program / block |
| `loop` | Defines a loop construct |
| `condition` | Defines a conditional (if) construct |
| `declare` | Variable declaration statement |
| `output` | Prints output to the console |
| `input` | Reads input from the user |
| `function` | Defines a function |
| `return` | Returns a value from a function |
| `break` | Breaks out of a loop |
| `continue` | Skips the current loop iteration |
| `else` | Alternative branch of a conditional |

---

## 🏷️ Identifier Rules

- **Must start** with an uppercase letter `[A-Z]`
- **Followed by** zero to thirty lowercase letters, digits, or underscores `[a-z0-9_]{0,30}`
- **Maximum length:** 31 characters total (1 uppercase + up to 30 characters)
- **Regex:** `[A-Z][a-z0-9_]{0,30}`

### ✅ Valid Identifiers

```
Count
Variable_name
X
Total_sum_2024
Maximum_length_identifier_1234    ← exactly 31 characters (valid)
```

### ❌ Invalid Identifiers

```
count           ← starts with lowercase
myVariable      ← starts with lowercase
2Count          ← starts with a digit
Variable_Name   ← contains uppercase after first character
```

---

## 🔢 Literal Formats

### Integer Literals
- **Regex:** `[+-]?[0-9]+`
- Optional leading `+` or `-` sign
- One or more digits

| Valid | Invalid |
|-------|---------|
| `42` | `12.34` |
| `+100` | `1,000` |
| `-567` | |
| `0` | |

---

### Floating-Point Literals
- **Regex:** `[+-]?[0-9]+\.[0-9]{1,6}([eE][+-]?[0-9]+)?`
- Optional sign, digits, a mandatory decimal point, 1–6 decimal digits, and an optional scientific notation exponent

| Valid | Invalid |
|-------|---------|
| `3.14` | `3.` (no decimal digits) |
| `+2.5` | `.14` (no leading digit) |
| `-0.123456` | `1.2345678` (7 decimal digits — too many) |
| `1.5e10` | |
| `2.0E-3` | |

---

### String Literals
- **Regex:** `"([^"\\\n]|\\["\\ntr])*"`
- Delimited by double quotes
- Supports escape sequences: `\"`, `\\`, `\n`, `\t`, `\r`

```
"Hello, World!"
"Line1\nLine2"
"He said \"hello\""
```

---

### Character Literals
- **Regex:** `'([^'\\\n]|\\['\\ntr])'`
- Exactly one character enclosed in single quotes
- Supports same escape sequences as strings

```
'A'
'\n'
'\\'
```

---

### Boolean Literals

| Literal | Meaning |
|---------|---------|
| `true` | Boolean true |
| `false` | Boolean false |

> Both are **case-sensitive** and **lowercase only**.

---

## ⚙️ Operators

Listed from **highest to lowest precedence:**

| Precedence | Operator(s) | Type |
|-----------|-------------|------|
| 1 (Highest) | `++`, `--` | Increment / Decrement |
| 2 | `**` | Exponentiation |
| 3 | `!` | Logical NOT |
| 4 | `*`, `/`, `%` | Multiplicative |
| 5 | `+`, `-` | Additive |
| 6 | `<`, `>`, `<=`, `>=` | Relational |
| 7 | `==`, `!=` | Equality |
| 8 | `&&` | Logical AND |
| 9 | `\|\|` | Logical OR |
| 10 (Lowest) | `=`, `+=`, `-=`, `*=` | Assignment |

---

## 💬 Comment Syntax

### Single-Line Comments
Begin with `##` and extend to the end of the line.

```
## This is a single-line comment
```

### Multi-Line Comments
Enclosed between `#*` and `*#`.

```
#*
  This is a
  multi-line comment
*#
```

> **Note:** Multi-line comments do **not** nest.

---

## 📄 Sample Programs

### Sample 1 — Valid Tokens (`test1.malang`)

```malang
## test1.malang - All valid implemented tokens

## Identifiers and Booleans
Count true false X
Variable_name
Total_sum_2024

## Integer Literals
42 +100 -567 0

## Floating-Point Literals
3.14 +2.5 -0.123456
1.5e10 2.0E-3

## Punctuators
( ) { } [ ] , ; :
```

---

### Sample 2 — Complex Nested Structures (`test2.malang`)

```malang
## test2.malang - Complex nested structures

Data_array [ { ( 42 , -3.14 ) } , { ( true , false ) } ] ;

Nested_call ( Call_one ( 1.5e10 ) , Call_two ( -567 ) ) :
{
    Value_one : +100 ;
    Value_two : 2.0E-3 ;
}
```

---

### Sample 3 — Literal Edge Cases (`test3.malang`)

```malang
## test3.malang - Literal Edge Cases

## Float extremes (Signs on exponents, max decimal length)
-1.0e-5 +5.5E+10
0.000001 9.999999

## Integer extremes (Signed zeros)
+0 -0

## Identifiers hitting exactly the 31-character limit rule
Maximum_length_identifier_12345
```

---

### Sample 4 — Comments and Whitespace (`test5.malang`)

```malang
## test5.malang - Comments and Whitespace

## This is a single line comment.
## The next line has just whitespace and an identifier.

    Valid_identifier   ## Comment after code

## Testing multiple empty lines above and tabs
	Next_identifier
```

---

## 🛠️ Compilation & Execution Instructions

### Prerequisites

- **Java JDK 8+** installed and available on `PATH`
- **JFlex** (for Part 2 only) — download from [jflex.de](https://jflex.de)

---

### Part 1 — Manual Scanner (`ManualScanner.java`)

#### Step 1: Compile all Java source files

From the `src/` directory:

```bash
javac ManualScanner.java Token.java TokenType.java SymbolTable.java ErrorHandler.java
```

#### Step 2: Ensure the DFA table is in place

Make sure `dfa.csv` is accessible at `src/dfa.csv` (the scanner loads this at runtime).

#### Step 3: Ensure test files are in place

Test files should be located at:

```
tests/test1.malang
tests/test2.malang
tests/test3.malang
tests/test4.malang
tests/test5.malang
```

#### Step 4: Run the scanner

```bash
java ManualScanner
```

The scanner will automatically run all 5 test files and print:
- All recognized tokens in format `<TYPE, "lexeme", Line: N, Col: N>`
- Symbol table with name, type, first occurrence, and frequency
- Error table with any lexical errors found

---

### Part 2 — JFlex Scanner (`Scanner.flex`)

#### Step 1: Generate the lexer from the `.flex` specification

```bash
java -jar jflex-full-1.9.1.jar src/Scanner.flex
```

This generates `Yylex.java` in the same directory.

#### Step 2: Compile all Java source files (including generated Yylex.java)

```bash
javac src/Yylex.java src/Main_jflex.java src/Token.java src/TokenType.java src/SymbolTable.java src/ErrorHandler.java
```

#### Step 3: Run the JFlex scanner

```bash
java -cp src Main_jflex
```

The JFlex scanner runs all 5 test files in the same format as the manual scanner and additionally reports the total token count per file.

---

## 📊 Token Output Format

Every recognized token is printed in the following format:

```
<TOKEN_TYPE, "lexeme", Line: N, Col: N>
```

**Examples:**

```
<IDENTIFIER, "Count", Line: 1, Col: 1>
<INT_LITERAL, "42", Line: 5, Col: 1>
<FLOAT_LITERAL, "3.14", Line: 9, Col: 1>
<TRUE_BOOL, "true", Line: 3, Col: 7>
<SINGLE_LINE_COMMENT, "## This is a comment", Line: 1, Col: 1>
<L_ROUND_BRACKET, "(", Line: 12, Col: 1>
```

---

## ❗ Error Handling

The lexical analyzer detects and reports the following error types:

| Error Type | Example | Reason Reported |
|------------|---------|-----------------|
| Invalid character | `@`, `$`, `~` | Character does not belong to the language |
| Invalid identifier start | `count`, `myVar` | Violation of rule — identifier must start with uppercase |
| Identifier too long | 32+ character name | Maximum 31 characters allowed in identifier name |
| Float too many decimals | `1.2345678` | Maximum 6 characters allowed after decimal point |
| Unknown/unrecognized token | various | Violation of rule, no token either starts or contains `<char>` |

All errors are collected and displayed in the **Error Table** at the end of each file's scan:

```
--- ERROR TABLE ---
Name                 Line            Column     Reason
---------------------------------------------------------------------------
Lexical Error        4               1          Violation of Rule, no token either start or contain c
```

---

## 📁 Project Structure

```
23i-0523-23i-0617-Section/
├── src/
│   ├── ManualScanner.java      ← Manual DFA-based scanner
│   ├── Main_jflex.java         ← Entry point for JFlex scanner
│   ├── Scanner.flex            ← JFlex specification file
│   ├── Yylex.java              ← Generated by JFlex (auto-generated)
│   ├── Token.java              ← Token data class
│   ├── TokenType.java          ← Token type enum
│   ├── SymbolTable.java        ← Symbol table implementation
│   ├── ErrorHandler.java       ← Error logging and reporting
│   └── dfa.csv                 ← DFA transition table (used by ManualScanner)
├── docs/
│   ├── Automata_Design.pdf     ← NFA/DFA diagrams and transition tables
│   ├── Comparison.pdf          ← Manual vs JFlex output comparison
│   └── LanguageGrammar.txt     ← Formal grammar specification
├── tests/
│   ├── test1.malang            ← All valid tokens
│   ├── test2.malang            ← Complex nested structures
│   ├── test3.malang            ← Literal edge cases
│   ├── test4.malang            ← Lexical error cases
│   └── test5.malang            ← Comments and whitespace
└── README.md
```

---

*MAlang Lexical Analyzer — CS4031 Compiler Construction, Spring 2026*
