package src;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;

public class Main {
    public static void main(String[] args) {
        try {
            // Checking if file is empty
            /*
            File testFile = new File("code\\tests\\test1.malang");
            System.out.println("File exists? " + testFile.exists());
            System.out.println("File size (bytes): " + testFile.length() + " bytes");
            System.out.println("--------------------------------------");

            if (testFile.length() == 0) {
                System.out.println("EMPTY FILE!!");
                return;
            }
            */

            // Symbol Table
            SymbolTable symbolTable = new SymbolTable();

            // Read file
            Yylex jflex_scanner = new Yylex(new FileReader("tests\\test1.malang"));
            System.out.println("-------- JFLEX SCANNER OUTPUT --------");

            // First token
            Token token = jflex_scanner.yylex();

            // Keep reading all tokens
            while (token != null && token.type != TokenType.EOF) {
                symbolTable.insert(token.lexeme, token.type.toString(), token.line);
                System.out.println(token.toString());
                token = jflex_scanner.yylex();
            }

            System.out.println("Total tokens found: " + jflex_scanner.getTokenCount());

            symbolTable.printTable();
            symbolTable.printTotalFrequency();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}