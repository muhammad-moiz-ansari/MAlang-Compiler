package src;

public class Token {
    public TokenType type;
    public String lexeme;
    public int line;
    public int column;

    // Constructor
    public Token(TokenType type, CharSequence lexeme, int line, int column) {
        this.type = type;
        this.lexeme = (lexeme == null) ? "" : lexeme.toString();
        this.line = line;
        this.column = column;
    }

    @Override
    public String toString() {
        // Format: <KEYWORD, "start", Line: 1, Col: 1>
        return "<" + type + ", \"" + lexeme + "\", Line: " + line + ", Col: " + column + ">";
    }
}