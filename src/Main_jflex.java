
import java.io.FileReader;

public class Main_jflex {
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
            System.out.println("------------- JFLEX SCANNER OUTPUT -------------");

            for (int i = 0; i < 5; ++i ) {
                System.out.println(" _____________________________   ");
                System.out.println("/        _____________        \\  ");
                System.out.println("| == .  |             |       |  ");
                System.out.println("|   _   |             |   Y   |  ");
                System.out.println("|  / \\  |             | X   B |  ");
                System.out.println("| | O | |    TEST " + String.valueOf(i+1) + "   |   A   |  ");
                System.out.println("|  \\_/  |             |       |  ");
                System.out.println("|       |             | . . . |  ");
                System.out.println("|  :::  |             | . . . |  ");
                System.out.println("|  :::  |_____________| . . . |  ");
                System.out.println("|       MALANG COMPILER       |  ");
                System.out.println("\\_____________________________/  \n");

                // Symbol Table
                SymbolTable symbolTable = new SymbolTable();
                ErrorHandler errorHandler = new ErrorHandler();

                // Read file
                String filename = "tests\\test" + String.valueOf(i+1) + ".malang";
                Yylex jflex_scanner = new Yylex(new FileReader(filename));

                // First token
                Token token = jflex_scanner.yylex();

                // Keep reading all tokens
                while (token != null && token.type != TokenType.EOF) {
                    if (token.type != TokenType.ERROR) {
                        System.out.println(token.toString());
                        symbolTable.insert(token.lexeme, token.type.toString(), token.line);
                    }
                    else {
                        //System.out.println(token.toString());     // Print error right in between the tokens
                        errorHandler.insert(token.lexeme, token.line, token.column, "Reason");
                    }
                    token = jflex_scanner.yylex();
                }

                System.out.println("Total tokens found: " + jflex_scanner.getTokenCount());

                symbolTable.printTable();
                symbolTable.printTotalFrequency();
                jflex_scanner.getErrorHandler().printTable();

                System.out.println("================================================================================================");
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}