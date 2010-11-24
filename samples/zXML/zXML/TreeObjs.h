#include "grom\ValueArray.h"
#include "grom\Object.h"

enum NodeType { XMLText, XMLAttribute, XMLElement, XMLComment, XMLPI };

class XMLElementNode;

class XMLNode
{
private:
	NodeType type;
public:
	XMLElementNode* parent;
};

class XMLTextNode: public XMLNode
{
private:
	std::string* value;
public:
	XMLTextNode(std::string* value);
	~XMLTextNode();
	std::string* getValue();
};

class XMLCommentNode: public XMLNode
{
private:
	std::string* value;
public:
	XMLCommentNode(std::string* value);
	~XMLCommentNode();
	std::string* getValue();
};
/*
class XMLPINode: public XMLNode
{
private:
	std::string* name;
	std::string* value;
public:
	XMLPINode(std::string* name,std::string* value);
	std::string* getValue();
	std::string* getName();
};*/



class XMLAttributeNode: public XMLNode
{
private:
	std::string* name;
	std::string* value;
public:
	std::string* getName();
	std::string* getValue();
	XMLAttributeNode(std::string* name,std::string* value);
	~XMLAttributeNode();
};

class XMLElementNode: public XMLNode
{
private:
	std::string* name;
public:
	std::string* getName();
	XMLElementNode(std::string* name);
	Sys::TValueArray<XMLNode>* children;
	void addChild(XMLNode* xn);
	~XMLElementNode();
};

class XMLCDataNode: public XMLNode
{
private:
	std::string* value;
public:
	std::string* getValue();
	XMLCDataNode(std::string* value);
	~XMLCDataNode();
};