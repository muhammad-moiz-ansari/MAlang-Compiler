package src;

import java.util.HashMap;
import java.util.Map;

public class SymbolTable {

    // Row of the table
    class SymbolEntry {
        String name;
        String type;      // "KEYWORD", "IDENTIFIER", etc
        int firstLine;    // First occurrence
        int frequency;

        SymbolEntry(String name, String type, int firstLine, int frequency) {
            this.name = name;
            this.type = type;
            this.firstLine = firstLine;
            this.frequency = frequency;
        }
    }

    // The table
    // Key = identifier's name (String)
    // Value = row data (SymbolEntry)
    private Map<String, SymbolEntry> table;

    // Constructor
    public SymbolTable() {
        table = new HashMap<>();
    }

    // Add or update an identifier
    public void insert(String name, String type, int line) {
        if (table.containsKey(name)) {
            SymbolEntry entry = table.get(name);
            entry.firstLine = line;
        }
        else {
            SymbolEntry entry = new SymbolEntry(name, type, line, 1);
        }
    }

    // Print table
    public void printTable() {
        System.out.println("\n--- SYMBOL TABLE ---");
        System.out.printf("%-20s %-15s %-10s %-10s\n", "Name", "Type", "First Line", "Frequency");
        System.out.println("---------------------------------------------------------------");

        for (int i=0; i<table.size(); i++) {
            SymbolEntry entry = table.get(i);
            System.out.printf("%-20s %-15s %-10s %-10s\n", entry.name, entry.type, entry.firstLine, entry.frequency);
        }
    }
}