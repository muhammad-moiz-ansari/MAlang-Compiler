package src;

public enum TokenType {
    KEYWORD,
    IDENTIFIER,
    INT_LITERAL,
    FLOAT_LITERAL,
    STRING_LITERAL,
    CHAR_LITERAL,
    BOOL_LITERAL,
    OPERATOR,
    PUNCTUATOR,
    COMMENT,
    WHITE_SPACE,
    NEWLINE,
    CARRIAGE_RETURN,
    TAB,
    ERROR,
    EOF // End of File
}