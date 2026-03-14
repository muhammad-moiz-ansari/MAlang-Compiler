import java.io.*;

/* ------------ Jflex Options ------------ */
// JFLEX OPTIONS
%%
%class Yylex          // Tells JFlex to name the generated Java file "Yylex.java"
%unicode
%public
%line                 // Tells JFlex to count lines (accessible via yyline)
%column               // Tells JFlex to count columns (accessible via yycolumn)
%type Token           // Tells JFlex that our rules will return "Token" objects

%{
    private int tokenCount = 0;
    private ErrorHandler eh = new ErrorHandler();
    public int getTokenCount() {
        return tokenCount;
    }
    private Token returnToken(TokenType ttype) {
        return new Token(ttype, yytext().toString(), yyline + 1, yycolumn + 1);
    }
    private Token returnToken(TokenType ttype, String text) {
        return new Token(ttype, text, yyline + 1, yycolumn + 1);
    }

    public ErrorHandler getErrorHandler() {
        return eh;
    }
    private void reportError(String reason) {
        eh.insert("Lexical Error", yyline + 1, yycolumn + 1, reason);
    }
%}

/* ------------ Macros ------------ */
Keyword = "start"|"finish"|"loop"|"condition"|"declare"|"output"|"input"|"function"|"return"|"break"|"continue"|"else"
Identifier = [A-Z][a-z0-9_]{0,30}
Float_literal = [+-]?[0-9]+\.[0-9]{1,6}([eE][+-]?[0-9]+)?
Int_literal = [+-]?[0-9]+
String_literal = \"([^\"\\\n]|\\[\"\\ntr]|\\u[0-9a-fA-F]{4})*\"
Char_literal = \'([^\'\\\n]|\\[\'\\ntr])\'
Operator = "++"|"--"|"**"|"=="|"+="|"-="|"*="|"!="|"="|"<="|">="|"<"|">"|"&&"|"||"|"!"|"+"|"-"|"*"|"/"|"%"

L_Round_Bracket  = "("
R_Round_Bracket  = ")"
L_Curly_Bracket  = "{"
R_Curly_Bracket  = "}"
L_Square_Bracket = "["
R_Square_Bracket = "]"
Comma            = ","
Semicolon        = ";"
Colon            = ":"

True_bool = "true"
False_bool = "false"

Space = " "
Newline = \n
Carriage = \r
Tab = \t

Single_line_comment = ##[^\n]*
Multi_line_comment = #\*([^*]|\*+[^*#])*\*+#
Comment = {Single_line_comment} | {Multi_line_comment}
White_space = {Space} | {Newline} | {Carriage} | {Tab}

// Error Defining Macros //
Invalid_ID_Start = [a-z][a-z0-9_]*          // Invalid Identifier (starts with lowercase)
Long_Identifier = [A-Z][a-z0-9_]{31}        // Identifier Too Long (31 or more characters)
Long_Float = [+-]?[0-9]+\.[0-9]{7}([eE][+-]?[0-9]+)?      // Float Too Long (7 or more decimals)
// CHANGE THE FOLLOWING WHEN ADD OPERATORS
Invalid_Char = [^a-zA-Z0-9#_,:;()\[\]{}\.\+\-\n\t\r\u ]    // Characters other than language --> Invalid
%%

/* Ignore Whitespace and comments */
/* If ignore whitespace and comments
{White_space}               { /* ignore */ }
{Single_line_comment}       { /* ignore */ }
{Multi_line_comment}        { /* ignore */ }
*/

/* Whitespace Rules */
/*
*/
{Space}              { tokenCount++; return returnToken(TokenType.SPACE); }
{Newline}            { tokenCount++; return returnToken(TokenType.NEWLINE, "\\n"); }
{Carriage}           { tokenCount++; return returnToken(TokenType.CARRIAGE_RETURN, "\\r"); }
{Tab}                { tokenCount++; return returnToken(TokenType.TAB, "\\t"); }

/* Comment Rules */
/*
*/
{Single_line_comment} { tokenCount++; return returnToken(TokenType.SINGLE_LINE_COMMENT, yytext().replace('\r',' ')); }
{Multi_line_comment}  { tokenCount++; return returnToken(TokenType.MULTI_LINE_COMMENT, "#*...*#"); }

/* Return tokens */
{Keyword}         { tokenCount++; return returnToken(TokenType.KEYWORD); }
{True_bool}       { tokenCount++; return returnToken(TokenType.TRUE_BOOL); }
{False_bool}      { tokenCount++; return returnToken(TokenType.FALSE_BOOL); }
{Identifier}      { tokenCount++; return returnToken(TokenType.IDENTIFIER); }
{Int_literal}     { tokenCount++; return returnToken(TokenType.INT_LITERAL); }
{Float_literal}   { tokenCount++; return returnToken(TokenType.FLOAT_LITERAL); }
{String_literal}  { tokenCount++;
                    String translated_string = yytext()
                            .replace("\\u0024", "\u0024")  // $
                            .replace("\\u03A3", "\u03A3")  // Σ
                            .replace("\\u263A", "\u263A");  // ☺
                    return returnToken(TokenType.STRING_LITERAL, translated_string); }
{Char_literal}    { tokenCount++; return returnToken(TokenType.CHAR_LITERAL); }
{Operator}        { tokenCount++; return returnToken(TokenType.OPERATOR); }

/* Punctuator Rules */
{L_Round_Bracket}  { tokenCount++; return returnToken(TokenType.L_ROUND_BRACKET); }
{R_Round_Bracket}  { tokenCount++; return returnToken(TokenType.R_ROUND_BRACKET); }
{L_Curly_Bracket}  { tokenCount++; return returnToken(TokenType.L_CURLY_BRACKET); }
{R_Curly_Bracket}  { tokenCount++; return returnToken(TokenType.R_CURLY_BRACKET); }
{L_Square_Bracket} { tokenCount++; return returnToken(TokenType.L_SQUARE_BRACKET); }
{R_Square_Bracket} { tokenCount++; return returnToken(TokenType.R_SQUARE_BRACKET); }
{Comma}            { tokenCount++; return returnToken(TokenType.COMMA); }
{Semicolon}        { tokenCount++; return returnToken(TokenType.SEMICOLON); }
{Colon}            { tokenCount++; return returnToken(TokenType.COLON); }


/* ---------------- Error Rules ---------------- */

/* 1. Floats with too many decimals */
{Long_Float} {
    tokenCount++;
    reportError("Maximum 6 characters allowed after Decimal Point in Float");
    return returnToken(TokenType.ERROR, yytext());
}

/* 2. Identifiers too long */
{Long_Identifier} {
    tokenCount++;
    reportError("Maximum 31 characters allowed in Identifier Name");
    return returnToken(TokenType.ERROR, yytext());
}

/* 3. Identifiers starting with lowercase */
/* Excludes keywords/booleans as they are already matched above */
{Invalid_ID_Start} {
    tokenCount++;
    reportError("Violation of Rule, no token either start or contain " + yytext().substring(0,1));
    return returnToken(TokenType.ERROR, yytext());
}

/* 4. Catch invalid characters */
{Invalid_Char} {
    tokenCount++;
    reportError("Character " + yytext() + " does not belong to the language");
    return returnToken(TokenType.ERROR, yytext());
}

/* End Of File */
<<EOF>>         { return returnToken(TokenType.EOF, null); }

/* Error Handling */
[^]    { //System.out.println("WARNING! Unrecognized character at Line: " + (yyline + 1) + ", Col: " + (yycolumn + 1) + " : " + yytext());
          reportError("Unrecognized character: " + yytext());
          return returnToken(TokenType.ERROR, yytext());
       }