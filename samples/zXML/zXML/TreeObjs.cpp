#include "stdafx.h"
#include "TreeObjs.h"
#include "grom\ValueArray.h"

/*XMLPINode::XMLPINode(std::string* name,std::string* value)
{
	this->name=name;
	this->value=value;
}
std::string* XMLPINode::getValue()
{
	return this->value;
}
std::string* XMLPINode::getName()
{
	return this->name;
}*/

XMLTextNode::XMLTextNode(std::string*)
{
	this->value = value;
}
std::string* XMLTextNode::getValue()
{
	return this->value;
}
XMLTextNode::~XMLTextNode()
{
	delete this->value;
}
XMLAttributeNode::XMLAttributeNode(std::string* name,std::string* value)
{
	this->name=name;
	this->value=value;
}
std::string* XMLAttributeNode::getName()
{
	return this->name;
}
std::string* XMLAttributeNode::getValue()
{
	return this->value;
}
XMLAttributeNode::~XMLAttributeNode()
{
	delete this->name;
	delete this->value;
}
XMLElementNode::XMLElementNode(std::string* name)
{
	this->name=name;
	this->children = new Sys::TValueArray<XMLNode>();
}
void XMLElementNode::addChild(XMLNode* xn)
{
	this->children->Add((*xn));
}
std::string* XMLElementNode::getName()
{
	return this->name;
}
XMLElementNode::~XMLElementNode()
{
	delete this->name;
	int i=0;
	//WTF?! where is iterator?
	/*for (;i<this->children->Count();i++)
	{
		delete &(this->children->Item(i));
	}*/
	delete this->children;
}
XMLCommentNode::XMLCommentNode(std::string* value)
{
	this->value=value;
}
std::string* XMLCommentNode::getValue()
{
	return this->value;
}
XMLCommentNode::~XMLCommentNode()
{
	delete this->value;
}

XMLCDataNode::XMLCDataNode(std::string* value)
{
	this->value=value;
}
std::string* XMLCDataNode::getValue()
{
	return this->value;
}
XMLCDataNode::~XMLCDataNode()
{
	delete this->value;
}