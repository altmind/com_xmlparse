#include <map>
#include "expat.h"
#include "TreeObjs.h"
#import <msxml6.dll> raw_interfaces_only
using namespace MSXML2;
static int nestLevel;
static XML_Parser ep;
static XML_Parser tep;
static XMLElementNode* tparent=0;
static std::map<std::string*,std::string*>* props;
static XML_Index cdatastart;
static char* globalref=0;
class XMLTools
{
private:
	//XML_Parser ep;
public:
	XMLTools() throw(...);
	void addText(char* src, long sz) throw(...);
	int finishAdding() throw(...); 

};
class MSXMLTools
{
private:
	HRESULT VariantFromString(char* wszValue, VARIANT &Variant);
	HRESULT VariantFromString(PCWSTR wszValue, VARIANT &Variant);
	HRESULT CreateAndInitDOM(MSXML2::IXMLDOMDocument **ppDoc);
public:
	MSXMLTools(char* src, long sz) throw(...);
};
class XMLTree
{
private:

public:
	XMLTree();
	XMLElementNode* fromText(char* in, int sz);
};