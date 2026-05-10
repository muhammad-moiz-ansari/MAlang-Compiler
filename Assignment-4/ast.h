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

  // Every node in our tree MUST know how to print itself as XML.
  // The "tag" parameter is used because JSON keys become XML tags!
  virtual void printXML(const std::string &tag = "") = 0;
};

// ---------------------------------------------------------
// 2. Scalar Values (Strings, Numbers, Booleans)
// ---------------------------------------------------------
class StringNode : public ASTNode {
  std::string value;

public:
  StringNode(std::string v) : value(v) {}
  void printXML(const std::string &tag) override;
};

class NumberNode : public ASTNode {
  double value;

public:
  NumberNode(double v) : value(v) {}
  void printXML(const std::string &tag) override;
};

class BoolNode : public ASTNode {
  bool value;

public:
  BoolNode(bool v) : value(v) {}
  void printXML(const std::string &tag) override;
};

// ---------------------------------------------------------
// 3. Null Value
// ---------------------------------------------------------
class NullNode : public ASTNode {
public:
  NullNode() {}
  void printXML(const std::string &tag) override;
};

// ---------------------------------------------------------
// 4. Complex Structures (Arrays and Objects)
// ---------------------------------------------------------
class ArrayNode : public ASTNode {
  std::vector<ASTNode *> elements;

public:
  void addElement(ASTNode *element) { elements.push_back(element); }
  void printXML(const std::string &tag) override;
};

class ObjectNode : public ASTNode {
  // A vector of Key-Value pairs preserves the exact order of the JSON!
  std::vector<std::pair<std::string, ASTNode *>> members;

public:
  void addMember(const std::string &key, ASTNode *value) {
    members.push_back(std::make_pair(key, value));
  }
  void printXML(const std::string &tag) override;
};

#endif