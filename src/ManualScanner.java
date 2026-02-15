import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;

public class ManualScanner {
    private int[][] transitionTable = new int[35][];
    private Map<String, Integer> symbolToIndex = new HashMap<>();
    private char[] alphabets = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789#_,:;()[]{}.+-\n\t\r ".toCharArray();
    ArrayList<Token> tok = new ArrayList<>();
    int lineNum = 1, col = 1;
    int prevl=0,prevc=0;
    boolean fc = true;
    SymbolTable st = new SymbolTable();
    ErrorHandler ea = new ErrorHandler();

    public String returnTokenName(int state){
        if(state == 3)
            return "Single-Line-Comment";
        if(state == 5)
            return "Integer-Literal";
        if(state == 11 || state == 8)
            return "Float-Literal";
        if(state == 12)
            return "Identifier";
        if(state == 13)
            return "Newline";
        if(state == 14)
            return "Carriage";
        if(state == 15)
            return "Tab";
        if(state == 16)
            return "Space";
        if(state == 20)
            return "True-Bool";
        if(state == 25)
            return "False-Bool";
        if(state == 26)
            return "Left-Round-Bracket";
        if(state == 27)
            return "Right-Round-Bracket";
        if(state == 28)
            return "Left-Curly-Bracket";
        if(state == 29)
            return "Right-Curly-Bracket";
        if(state == 30)
            return "Left-Square-Bracket";
        if(state == 31)
            return "Right-Square-Bracket";
        if(state == 32)
            return "Comma";
        if(state == 33)
            return "SemiColon";
        if(state == 34)
            return "Colon";
        return null;
    }

    public TokenType returnCompatibleTokenName(int state){
            if(state == 3)
                return TokenType.SINGLE_LINE_COMMENT;
            if(state == 5)
                return TokenType.INT_LITERAL;
            if(state == 11 || state == 8)
                return TokenType.FLOAT_LITERAL;
            if(state == 12)
                return TokenType.IDENTIFIER;
            if(state == 13)
                return TokenType.NEWLINE;
            if(state == 14)
                return TokenType.CARRIAGE_RETURN;
            if(state == 15)
                return TokenType.TAB;
            if(state == 16)
                return TokenType.WHITE_SPACE;     // Space
            if (state == 20)
                return TokenType.TRUE_BOOL;          // "True-Bool"
            if (state == 25)
                return TokenType.FALSE_BOOL;         // "False-Bool"
            if (state == 26)
                return TokenType.L_ROUND_BRACKET;    // "Left-Round-Bracket"
            if (state == 27)
                return TokenType.R_ROUND_BRACKET;    // "Right-Round-Bracket"
            if (state == 28)
                return TokenType.L_CURLY_BRACKET;    // "Left-Curly-Bracket"
            if (state == 29)
                return TokenType.R_CURLY_BRACKET;    // "Right-Curly-Bracket"
            if (state == 30)
                return TokenType.L_SQUARE_BRACKET;   // "Left-Square-Bracket"
            if (state == 31)
                return TokenType.R_SQUARE_BRACKET;   // "Right-Square-Bracket"
            if (state == 32)
                return TokenType.COMMA;              // "Comma"
            if (state == 33)
                return TokenType.SEMICOLON;          // "SemiColon"
            if (state == 34)
                return TokenType.COLON;              // "Colon"
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

    public boolean isValidAlphabet(char c){
        for(int i=0; i< alphabets.length;i++){
            if(c==alphabets[i])
                return true;
        }
        return false;
    }

    public void updateLineAndCols(char c){
        prevl = lineNum;
        prevc = col;
        if(c == '\n'){
            lineNum=lineNum+1;
            col=1;
            fc= true;
        }
        else {
            if (!fc){
                col++;
            }
            else {
                fc = false;
            }
        }
    }

    public void printToken(String input, int s, int e, int currentState, int sl, int sc){
        //System.out.print("Token Generated: "+returnTokenName(currentState)+ " -> ");
        String lex="";
        for(int i=s; i<e; i++){
            //System.out.print(input.charAt(i));
            lex = lex.concat(input.charAt(i)+"");
        }
        //System.out.println(" ");
        Token tt = new Token(returnCompatibleTokenName(currentState),lex,sl,sc);
        st.insert(lex,returnCompatibleTokenName(currentState).name(),sl);
        tok.add(tt);
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

    public void printInvalidCharError(int currentState,int s, int e, String input, char c, int sline, int scol){
        //System.out.println("Line : "+lineNum+", Column : "+col+" Lexical Error: Character "+c+" does not belong to the language");
        ea.insert("Lexical Error", lineNum, col,"Character "+c+" does not belong to the language");
        if(returnTokenName(currentState)!=null){
            printToken(input,s,e,currentState,sline,scol);
        }
    }

    public int getNextState(char c, int currentState){
        String cat = getCategory(c, currentState);
        int col=-1,row=-1;
        if(!cat.isEmpty())
            col = symbolToIndex.get(cat);
        if(currentState>0)
            row = currentState;
        else
            row=1;

        int nextState=0;
        if(col>=0 && row>=0)
            nextState = transitionTable[row][col];
        return nextState;
    }

    public void printAllTokens(){
        for(int i=0; i< tok.size(); i++){
            System.out.println(tok.get(i).toString());
        }
    }

    public void run(String input) {
        int currentState = 1, s=0, e=0, idcons = 0, floatcons = 0, sline = lineNum, scol = col;
        char c='@';

        while (true){
            // Check if input is complete
            if(e >= input.length()) {
                if(returnTokenName(currentState)!=null){
                    printToken(input,s,e,currentState,sline,scol);
                }
                break;
            }

            // Check Identifier Character Limit
            idcons = check_ID_Constraint(currentState,idcons);
            if(idcons >= 32){
                //System.out.println("Line : "+sline+", Column : "+scol+" Lexical Error: ");
                ea.insert("Lexical Error", sline, scol,"Maximum 31 characters allowed in Identifier Name");
                s=e;
                idcons = 0;
                currentState = 1;
                continue;
            }

            // Check Float Decimal Point Limit
            floatcons = check_Float_Constraint(currentState,floatcons);
            if(floatcons >= 7){
                //System.out.println("Line : "+sline+", Column : "+scol+ " Lexical Error: ");
                ea.insert("Lexical Error", sline, scol,"Maximum 6 characters allowed after Decimal Point in Float");
                s=e;
                floatcons = 0;
                currentState = 1;
                continue;
            }

            // Get input character
            c = input.charAt(e);
            updateLineAndCols(c);
            if(currentState == 1) {
                sline=lineNum;
                scol= col;
            }

            // Check Character Validity if it belongs to alphabets or not
            if(!isValidAlphabet(c)){
                printInvalidCharError(currentState,s,e,input,c,sline,scol);
                e++;
                s=e;
                currentState=1;
                continue;
            }

            // Get Next State based on input character
            int nextState = getNextState(c,currentState);

            // If input character violates the transition
            if(nextState == -1) {
                //System.out.println("Line : "+sline+", Column : "+scol+" Lexical Error : Violation of Rule, no token either start or contain "+c);
                ea.insert("Lexical Error", sline, scol,"Violation of Rule, no token either start or contain "+c);
                currentState = 1;
                s=e+1;
            }
            // If a transition completes, Go back to initial state for other patterns match
            else if(nextState == 1){
                printToken(input,s,e,currentState,sline,scol);
                currentState = 1;
                s=e;
                e--;
                lineNum = prevl;
                col = prevc;
            }
            // Otherwise Go to next state
            else{
                currentState=nextState;
            }
            e++;
        }
        printAllTokens();
        st.printTable();
        ea.printTable();
    }

    public static void main(String[] args) {
        ManualScanner dfa = new ManualScanner();
        dfa.loadCSV("src/dfa.csv");

        String testInput = "Abc**s90_,:()+1.5E-1##Abc  )**(   +1.5654329E+15  A";
        dfa.run(testInput);
    }
}