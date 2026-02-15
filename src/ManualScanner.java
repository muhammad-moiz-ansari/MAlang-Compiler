package src;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;

public class ManualScanner {
    //private int tableSize = 40;   if add "start"
    private int tableSize = 35;
    private int[][] transitionTable = new int[tableSize][];
    private Map<String, Integer> symbolToIndex = new HashMap<>();
    private char[] alphabets = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789#_,:;()[]{}.+-\n\t\r ".toCharArray();
    private int[] finalstates = {3,6,8,11,12,13,14,15,16,20,25,26,27,28,29,30,31,32,33,34};

    public String returnTokenName(int state){
        if(state == 3)
            return TokenType.SINGLE_LINE_COMMENT.toString();
        if(state == 5)
            return TokenType.INT_LITERAL.name();
        if(state == 11 || state == 8)
            return TokenType.FLOAT_LITERAL.name();
        if(state == 12)
            return TokenType.IDENTIFIER.name();
        if(state == 13)
            return TokenType.NEWLINE.name();
        if(state == 14)
            return TokenType.CARRIAGE_RETURN.name();
        if(state == 15)
            return TokenType.TAB.name();
        if(state == 16)
            return TokenType.WHITE_SPACE.name();     // Space
        if(state == 20)
            return TokenType.BOOL_LITERAL.name();         // "True-Bool";
        if(state == 25)
            return TokenType.BOOL_LITERAL.name();         // "False-Bool";
        if(state == 26)
            return TokenType.PUNCTUATOR.name();         // "Left-Round-Bracket";
        if(state == 27)
            return TokenType.PUNCTUATOR.name();         // "Right-Round-Bracket";
        if(state == 28)
            return TokenType.PUNCTUATOR.name();         // "Left-Curly-Bracket";
        if(state == 29)
            return TokenType.PUNCTUATOR.name();         // "Right-Curly-Bracket";
        if(state == 30)
            return TokenType.PUNCTUATOR.name();         // "Left-Square-Bracket";
        if(state == 31)
            return TokenType.PUNCTUATOR.name();         // "Right-Square-Bracket";
        if(state == 32)
            return TokenType.PUNCTUATOR.name();         // "Comma";
        if(state == 33)
            return TokenType.PUNCTUATOR.name();         // "SemiColon";
        if(state == 34)
            return TokenType.PUNCTUATOR.name();         // "Colon";
        //if(state == 40)
        //    return TokenType.KEYWORD.name();            // "Start";

        /*   Maybe later
        if(state == 35)
            return TokenType.MULTI_LINE_COMMENT.name();         // "/*";
        if(state == 36)
            return TokenType.MULTI_LINE_COMMENT.name();         // "* /";   // Even java doesn't support nested multi-line comments -_-
        */
        return null;
    }

    public String getCategory(char ch, int current_s){
        if(ch>=65 && ch<=90 && current_s!=8){
            return "upper";
        }
        if(ch>=97 && ch<=122 && current_s!=8){
            return "lower";
        }
        if(ch>=48 && ch<=57){
            return "digit";
        }
        if(ch == 'e' || ch == 'E' && current_s == 8)
            return "exp";
        if(ch == '\n')
            return "\\n";
        if(ch == ' ')
            return " ";
        return ch+"";
    }

    public void loadCSV(String csvFile) {
        try (BufferedReader br = new BufferedReader(new FileReader(csvFile))) {
            String line = br.readLine();
            if (line == null) return;

            String[] headers = line.split(",");
            int x = 0;boolean b = false;
            for (int i = 1; i < headers.length; i++) {
                String sym = headers[i];

                // Clean up the quotes: "," becomes ,
                if (sym.contains("\"")){
                    if(!b) {
                        symbolToIndex.put(",", x);
                        x++;
                        b=true;
                    }
                    continue;
                }
                symbolToIndex.put(sym, x);
                x++;
            }

            int symbolCount = symbolToIndex.size();

            while ((line = br.readLine()) != null) {
                String[] values = line.split(",");
                if (values.length == 0) continue;

                int stateId = Integer.parseInt(values[0].trim());
                int[] transitions = new int[symbolCount];

                for (int i = 1; i < values.length; i++) {
                    String val = values[i].trim();
                    transitions[i - 1] = val.isEmpty() ? -1 : Integer.parseInt(val);
                }

                // Store in the table at the exact State ID index
                transitionTable[stateId] = transitions;
            }
        } catch (IOException e) {
            System.err.println("Error: " + e.getMessage());
        }
    }

    public boolean isFinal(int state){
        for(int i=0; i< finalstates.length;i++){
            if(finalstates[i]==state)
                return true;
        }
        return false;
    }

    public boolean isValidAlphabet(char c){
        for(int i=0; i< alphabets.length;i++){
            if(c==alphabets[i])
                return true;
        }
        return false;
    }

    public void printToken(String input, int s, int e, int currentState){
        System.out.print("Token Generated: "+returnTokenName(currentState)+ " -> ");
        for(int i=s; i<e; i++){
            System.out.print(input.charAt(i));
        }
        System.out.println(" ");
    }

    public int check_ID_Constraint(int currentState, int idcons){
        if(currentState == 12)
            idcons++;
        else
            idcons=0;
        return idcons;
    }

    public int check_Float_Constraint(int currentState, int cons){
        if(currentState == 8)
            cons++;
        else
            cons=0;
        return cons;
    }

    public void printInvalidCharError(int currentState,int s, int e, String input, char c){
        System.out.println("Lexical Error: Character "+c+" does not belong to the language");
        if(returnTokenName(currentState)!=null){
            printToken(input,s,e,currentState);
        }
    }

    public int getNextState(char c, int currentState){
        String cat = getCategory(c, currentState);
        int col=-1,row=-1;
        if(!cat.isEmpty())
            col = symbolToIndex.get(cat);
        row = currentState;

        int nextState=0;
        if(col>=0 && row>=0)
            nextState = transitionTable[row][col];
        return nextState;
    }

    public void run(String input) {
        int currentState = 1, s=0, e=0, idcons = 0, floatcons = 0;

        while (true){
            // Check if input is complete
            if(e >= input.length()) {
                if(returnTokenName(currentState)!=null){
                    printToken(input,s,e,currentState);
                }
                break;
            }

            // Check Identifier Character Limit
            idcons = check_ID_Constraint(currentState,idcons);
            if(idcons >= 32){
                System.out.println("Lexical Error: Maximum 31 characters allowed in Identifier Name");
                s=e;
                idcons = 0;
                currentState = 1;
                continue;
            }

            // Check Float Decimal Point Limit
            floatcons = check_Float_Constraint(currentState,floatcons);
            if(floatcons >= 7){
                System.out.println("Lexical Error: Maximum 6 characters allowed after Decimal Point in Float");
                s=e;
                floatcons = 0;
                currentState = 1;
                continue;
            }

            // Get input character
            char c = input.charAt(e);
            if(e==25){
                c='\n';
            }

            // Check Character Validity if it belongs to alphabets or not
            if(!isValidAlphabet(c)){
                printInvalidCharError(currentState,s,e,input,c);
                e++;
                s=e;
                currentState=1;
                continue;
            }

            // Get Next State based on input character
            int nextState = getNextState(c,currentState);

            // If input character violates the transition
            if(nextState == -1) {
                System.out.println("Lexical Error : Violation of Rule, no token either start or contain "+getCategory(c, currentState));
                currentState = 1;
                s=e+1;
            }
            // If a transition completes, Go back to initial state for other patterns match
            else if(nextState == 1){
                printToken(input,s,e,currentState);
                currentState = 1;
                s=e;
                e--;
            }
            // Otherwise Go to next state
            else{
                currentState=nextState;
            }
            e++;
        }
    }

    public static void main(String[] args) {
        ManualScanner dfa = new ManualScanner();
        dfa.loadCSV("src/dfa.csv");

        String testInput = "Abc**s90_,:()+1.5E-1##Abc )**(   +1.5654329E+15  A";
        dfa.run(testInput);
    }
}