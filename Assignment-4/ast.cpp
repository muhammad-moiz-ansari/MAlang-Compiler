#include "ast.h"
using namespace std;

// Helper function to escape special XML characters as required by the
// assignment
string escapeXML(const string &data) {
  string buffer;
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

// ---------------------------------------------------------
// Scalar Values
// ---------------------------------------------------------
void StringNode::printXML(const string &tag) {
  if (!tag.empty())
    cout << "<" << tag << ">";
  cout << escapeXML(value);
  if (!tag.empty())
    cout << "</" << tag << ">\n";
}

void NumberNode::printXML(const string &tag) {
  if (!tag.empty())
    cout << "<" << tag << ">";
  cout << value;
  if (!tag.empty())
    cout << "</" << tag << ">\n";
}

void BoolNode::printXML(const string &tag) {
  if (!tag.empty())
    cout << "<" << tag << ">";
  cout << (value ? "true" : "false");
  if (!tag.empty())
    cout << "</" << tag << ">\n";
}

// ---------------------------------------------------------
// Null Value
// ---------------------------------------------------------
void NullNode::printXML(const string &tag) {
  // A JSON null becomes an empty XML element
  if (!tag.empty()) {
    cout << "<" << tag << "/>\n";
  }
}

// ---------------------------------------------------------
// Complex Structures
// ---------------------------------------------------------
void ArrayNode::printXML(const string &tag) {
  if (!tag.empty())
    cout << "<" << tag << ">\n";

  // A JSON array becomes a sequence of repeated child elements using the tag
  // <item>
  for (auto &element : elements) {
    element->printXML("item");
  }

  if (!tag.empty())
    cout << "</" << tag << ">\n";
}

void ObjectNode::printXML(const string &tag) {
  // If there is no tag (like the very top level before we add <root>), don't
  // print empty brackets
  if (!tag.empty())
    cout << "<" << tag << ">\n";

  for (auto &member : members) {
    // member.first is the key, member.second is the ASTNode pointer
    member.second->printXML(member.first);
  }

  if (!tag.empty())
    cout << "</" << tag << ">\n";
}