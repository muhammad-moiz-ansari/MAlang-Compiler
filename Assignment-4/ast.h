#ifndef AST_H
#define AST_H

#include <iostream>
#include <string>
#include <vector>

// ---------------------------------------------------------
// 1. The Base Class
// ---------------------------------------------------------
class ASTNode {
public:
  virtual ~ASTNode() {}
  // Added 'depth' to track indentation
  virtual void printXML(const std::string &tag = "", int depth = 0) = 0;
  virtual void printTree(int depth = 0) = 0;
};

// ---------------------------------------------------------
// 2. Scalar Values (Strings, Numbers, Booleans)
// ---------------------------------------------------------
class StringNode : public ASTNode {
  std::string value;

public:
  StringNode(std::string v) : value(v) {}
  void printXML(const std::string &tag, int depth) override;
  void printTree(int depth) override;
};

class NumberNode : public ASTNode {
  double value;

public:
  NumberNode(double v) : value(v) {}
  void printXML(const std::string &tag, int depth) override;
    void printTree(int depth) override;
};

class BoolNode : public ASTNode {
  bool value;

public:
  BoolNode(bool v) : value(v) {}
  void printXML(const std::string &tag, int depth) override;
  void printTree(int depth) override;
};

// ---------------------------------------------------------
// 3. Null Value
// ---------------------------------------------------------
class NullNode : public ASTNode {
public:
  NullNode() {}
  void printXML(const std::string &tag, int depth) override;
    void printTree(int depth) override;
};

// ---------------------------------------------------------
// 4. Complex Structures (Arrays and Objects)
// ---------------------------------------------------------
class ArrayNode : public ASTNode {
  std::vector<ASTNode *> elements;

public:
  void addElement(ASTNode *element) { elements.push_back(element); }
  void printXML(const std::string &tag, int depth) override;
  void printTree(int depth) override;
};

class ObjectNode : public ASTNode {
  std::vector<std::pair<std::string, ASTNode *>> members;

public:
  void addMember(const std::string &key, ASTNode *value) {
    members.push_back(std::make_pair(key, value));
  }
  void printXML(const std::string &tag, int depth) override;
  void printTree(int depth) override;
};

#endif