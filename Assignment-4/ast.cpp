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