%{
#include <iostream>
#include <string>
#include <cstring>
#include "parser.tab.h"
using namespace std;

// Track line and column numbers for better error reporting
int line_num = 1;
int col_num = 1;

// Flex runs this macro right before executing the code for ANY matched rule.
// yyleng is the length of the text it just matched.
#define YY_USER_ACTION col_num += yyleng;

// --- Helper function to process strings and decode \uXXXX ---
string processString(const string& input) {
    string result = "";
    // Loop through the string, skipping the opening and closing quotes
    for (size_t i = 1; i < input.length() - 1; ++i) { 
        if (input[i] == '\\' && i + 1 < input.length() - 1) {
            char next = input[i+1];
            
            // If we find a Unicode escape like \u1234
            if (next == 'u' && i + 5 < input.length() - 1) {
                string hex_str = input.substr(i + 2, 4);
                int cp = stoi(hex_str, nullptr, 16); // Convert Hex to Integer
                
                // Convert Integer to UTF-8 Bytes
                if (cp <= 0x7F) {
                    result += (char)cp;
                } else if (cp <= 0x7FF) {
                    result += (char)(0xC0 | ((cp >> 6) & 0x1F));
                    result += (char)(0x80 | (cp & 0x3F));
                } else if (cp <= 0xFFFF) {
                    result += (char)(0xE0 | ((cp >> 12) & 0x0F));
                    result += (char)(0x80 | ((cp >> 6) & 0x3F));
                    result += (char)(0x80 | (cp & 0x3F));
                }
                i += 5; // Skip the 'u' and the 4 hex digits
            } 
            // Handle standard escapes
            else if (next == 'n') { result += '\n'; i++; }
            else if (next == 't') { result += '\t'; i++; }
            else if (next == 'r') { result += '\r'; i++; }
            else if (next == '"') { result += '"'; i++; }
            else if (next == '\\') { result += '\\'; i++; }
            else { result += next; i++; }
        } else {
            result += input[i];
        }
    }
    return result;
}

%}

%option noyywrap
%option nounput
%option noinput

%%

"{"         { return LBRACE; }
"}"         { return RBRACE; }
"["         { return LBRACKET; }
"]"         { return RBRACKET; }
":"         { return COLON; }
","         { return COMMA; }

"true"      { return TRUE_VAL; }
"false"     { return FALSE_VAL; }
"null"      { return NULL_VAL; }

\"([^\"\\]|\\.)*\" { 
    // --- UPDATED: Pass the raw text through our new decoder! ---
    string decodedText = processString(yytext);
    yylval.str_val = strdup(decodedText.c_str());
    return STRING; 
}

-?[0-9]+(\.[0-9]+)?([eE][+-]?[0-9]+)? { 
    yylval.num_val = atof(yytext);
    return NUMBER; 
}

[ \t\r]+    { /* Ignore spaces, tabs, and carriage returns */ }

\n          { 
    /* When we hit a newline, increment the line count and reset column to 1 */
    line_num++; 
    col_num = 1; 
}

.           { 
    cerr << "Lexical Error at Line " << line_num << ", Col " << col_num 
              << ": unexpected character '" << yytext << "'\n"; 
    exit(1); 
}

%%