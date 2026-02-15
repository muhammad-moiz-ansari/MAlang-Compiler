package src;
import java.io.*;
import src.TokenType;
import src.Token;

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
    public int getTokenCount() {
        return tokenCount;
    }
    private Token returnToken(TokenType ttype) {
        return new Token(ttype, yytext(), yyline + 1, yycolumn + 1);
    }
    private Token returnToken(TokenType ttype, String text) {
        return new Token(ttype, text, yyline + 1, yycolumn + 1);
    }
%}

/* ------------ Macros ------------ */
Keyword = "start"|"finish"|"loop"|"condition"|"declare"|"output"|"input"|"function"|"return"|"break"|"continue"|"else"
Identifier = [A-Z][a-z0-9_]{0,30}
Float_literal = [+-]?[0-9]+\.[0-9]{1,6}([eE][+-]?[0-9]+)?
Int_literal = [+-]?[0-9]+
String_literal = \"([^\"\\\n]|\\[\"\\ntr])*\"
Char_literal = \'([^\'\\\n]|\\[\'\\ntr])\'
Bool_literal = "true"|"false"
Operator = "++" | "--" | "**" | "==" | "+=" | "-=" | "*=" | "!=" | "=" | "<=" | ">=" | "<" | ">" | "&&" | "||" | "!" | "+" | "-" | "*" | "/" | "%"
Punctuator = [(){}[\\][,;:]]
Single_line_comment = ##[^\n]*
Multi_line_comment = #\*([^*]|\*+[^*#])*\*+#
Comment = {Single_line_comment} | {Multi_line_comment}
White_space = [ \t\r\n]+

%%

/* Ignore Whitespace and comments */
{White_space}   { /* ignore */ }
{Comment}       { /* ignore */ }

/* Return tokens */
{Keyword}   { tokenCount++; return returnToken(TokenType.KEYWORD); }
{Bool_literal}   { tokenCount++; return returnToken(TokenType.BOOL_LITERAL); }
{Identifier}   { tokenCount++; return returnToken(TokenType.IDENTIFIER); }
{Int_literal}   { tokenCount++; return returnToken(TokenType.INT_LITERAL); }
{Float_literal}   { tokenCount++; return returnToken(TokenType.FLOAT_LITERAL); }
{String_literal}    { tokenCount++; return returnToken(TokenType.STRING_LITERAL); }
{Char_literal}   { tokenCount++; return returnToken(TokenType.CHAR_LITERAL); }
{Operator}   { tokenCount++; return returnToken(TokenType.OPERATOR); }
{Punctuator}   { tokenCount++; return returnToken(TokenType.PUNCTUATOR); }

/* End Of File */
<<EOF>>         { return returnToken(TokenType.EOF, null); }

/* Error Handling */
.      { System.out.println("Lexical error at Line: " + (yyline + 1) + ", Col: " + (yycolumn + 1) + " : " + yytext());
         return returnToken(TokenType.ERROR); }
[^]    { System.out.println("WARNING! Unrecognized character at Line: " + (yyline + 1) + ", Col: " + (yycolumn + 1) + " : " + yytext());
         return returnToken(TokenType.EOF); }