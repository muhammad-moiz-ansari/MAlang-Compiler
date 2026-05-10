#include "ast.h"

// Helper: Escape XML
std::string escapeXML(const std::string &data) {
  std::string buffer;
  for (char c : data) {
    switch (c) {
    case '&':
      buffer.append("&amp;");
      break;
    case '<':
      buffer.append("&lt;");
      break;
    case '>':
      buffer.append("&gt;");
      break;
    case '\"':
      buffer.append("&quot;");
      break;
    default:
      buffer += c;
      break;
    }
  }
  return buffer;
}

// Helper: Generate spaces based on depth (4 spaces per level)
std::string getIndent(int depth) { return std::string(depth * 4, ' '); }

// ---------------------------------------------------------
// Scalar Values
// ---------------------------------------------------------
void StringNode::printXML(const std::string &tag, int depth) {
  if (!tag.empty())
    std::cout << getIndent(depth) << "<" << tag << ">";
  std::cout << escapeXML(value);
  if (!tag.empty())
    std::cout << "</" << tag << ">\n";
}

void NumberNode::printXML(const std::string &tag, int depth) {
  if (!tag.empty())
    std::cout << getIndent(depth) << "<" << tag << ">";
  std::cout << value;
  if (!tag.empty())
    std::cout << "</" << tag << ">\n";
}

void BoolNode::printXML(const std::string &tag, int depth) {
  if (!tag.empty())
    std::cout << getIndent(depth) << "<" << tag << ">";
  std::cout << (value ? "true" : "false");
  if (!tag.empty())
    std::cout << "</" << tag << ">\n";
}

// ---------------------------------------------------------
// Null Value
// ---------------------------------------------------------
void NullNode::printXML(const std::string &tag, int depth) {
  if (!tag.empty()) {
    std::cout << getIndent(depth) << "<" << tag << "/>\n";
  }
}

// ---------------------------------------------------------
// Complex Structures
// ---------------------------------------------------------
void ArrayNode::printXML(const std::string &tag, int depth) {
  int childDepth = depth;
  if (!tag.empty()) {
    std::cout << getIndent(depth) << "<" << tag << ">\n";
    childDepth++; // Increase depth for the children inside
  }

  for (auto &element : elements) {
    element->printXML("item", childDepth);
  }

  if (!tag.empty())
    std::cout << getIndent(depth) << "</" << tag << ">\n";
}

void ObjectNode::printXML(const std::string &tag, int depth) {
  int childDepth = depth;
  if (!tag.empty()) {
    std::cout << getIndent(depth) << "<" << tag << ">\n";
    childDepth++; // Increase depth for the members inside
  }

  for (auto &member : members) {
    member.second->printXML(member.first, childDepth);
  }

  if (!tag.empty())
    std::cout << getIndent(depth) << "</" << tag << ">\n";
}

// ---------------------------------------------------------
// AST Printing (Bonus Feature)
// ---------------------------------------------------------
void StringNode::printTree(int depth) {
    std::cout << getIndent(depth) << "- StringNode: \"" << value << "\"\n";
}

void NumberNode::printTree(int depth) {
    std::cout << getIndent(depth) << "- NumberNode: " << value << "\n";
}

void BoolNode::printTree(int depth) {
    std::cout << getIndent(depth) << "- BoolNode: " << (value ? "true" : "false") << "\n";
}

void NullNode::printTree(int depth) {
    std::cout << getIndent(depth) << "- NullNode\n";
}

void ArrayNode::printTree(int depth) {
    std::cout << getIndent(depth) << "- ArrayNode\n";
    for (auto& element : elements) {
        element->printTree(depth + 1); // Indent children further
    }
}

void ObjectNode::printTree(int depth) {
    std::cout << getIndent(depth) << "- ObjectNode\n";
    for (auto& member : members) {
        // Print the key first, then print the value node deeper
        std::cout << getIndent(depth + 1) << "[Key: " << member.first << "]\n";
        member.second->printTree(depth + 2); 
    }
}