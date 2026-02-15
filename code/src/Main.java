package src;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class Main {
    public static void main(String[] args) throws IOException {
        Yylex jflex_scanner = new Yylex(new FileReader("tests/test1.malang"));
        System.out.println("-------- JFLEX SCANNER OUTPUT --------");
        Token token = jflex_scanner.yylex();

        while (token.type != TokenType.EOF) {
            System.out.println(token.toString());
            token = jflex_scanner.yylex();
        }
        System.out.println("Total tokens found: " + jflex_scanner.getTokenCount());
    }
}