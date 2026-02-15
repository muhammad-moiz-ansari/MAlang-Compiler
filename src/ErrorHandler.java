import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class ErrorHandler {

    // Row of the table
    class ErrorEntry {
        String name;      // ERROR NAME
        int line;      // "KEYWORD", "IDENTIFIER", etc
        int col;    // First occurrence
        String reason;

        ErrorEntry(String name, int l, int c, String r) {
            this.name = name;
            this.line = l;
            this.col = c;
            this.reason = r;
        }
    }

    // The table
    // Key = identifier's name (String)
    // Value = row data (SymbolEntry)
    private ArrayList<ErrorEntry> table;

    // Constructor
    public ErrorHandler() {
        table = new ArrayList<>();
    }

    // Add or update an identifier
    public void insert(String name, int l, int c,String r) {
            ErrorHandler.ErrorEntry entry = new ErrorEntry(name, l, c, r);
            table.add(entry);
    }

    // Print table
    public void printTable() {
            System.out.println("\n--- ERROR TABLE ---");
        System.out.printf("%-20s %-15s %-10s %-10s\n", "Name", "Line", "Column", "Reason");
        System.out.println("---------------------------------------------------------------");

        for(int i=0; i< table.size(); i++) {
            System.out.printf("%-20s %-15s %-10s %-10s\n", table.get(i).name, table.get(i).line, table.get(i).col, table.get(i).reason);
        }
    }
}
