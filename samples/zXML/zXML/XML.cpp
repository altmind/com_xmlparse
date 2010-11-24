#include "stdafx.h"
#include "XML.h"
#include "expat.h"
#include <locale.h>
#include "zXML.h"
#include <exception>
#include <stdio.h>

#import <msxml6.dll> raw_interfaces_only
using namespace MSXML2;


#pragma comment(lib, "libexpat.lib")
#pragma comment(lib, "System.lib")


// Macro that calls a COM method returning HRESULT value.
#define CHK_HR(stmt)        do { hr=(stmt); if (FAILED(hr)) goto CleanUp; } while(0)
// Macro to verify memory allcation.
#define CHK_ALLOC(p)        do { if (!(p)) { hr = E_OUTOFMEMORY; goto CleanUp; } } while(0)
// Macro that releases a COM object if not NULL.
#define SAFE_RELEASE(p)     do { if ((p)) { (p)->Release(); (p) = NULL; } } while(0)

static void XMLCALL startproc(void *userData, const char *name, const char **atts)
{

	//((XMLTools*)userData)->
		nestLevel++;
	//printf("start %s\n",name);
}
static void XMLCALL endproc(void *userData, const char *name)
{
	//((XMLTools*)userData)->
		nestLevel--;
	//printf("end %s\n",name);
}


XMLTools::XMLTools() throw(...)
{
	nestLevel=0;
	ep=XML_ParserCreate(NULL);
	XML_SetUserData(ep, this);
	XML_SetElementHandler(ep, startproc, endproc);
	//XML_SetStartElementHandler(ep,&startproc);
	//XML_SetEndElementHandler(ep,&endproc);
}
void XMLTools::addText(char* src, long sz) throw(...)
{
	if (XML_Parse(ep, src, sz, 0) == XML_STATUS_ERROR) {
		char* buff =(char*) malloc(1024);
		sprintf(buff, "Parse error at line %d:\n%s\n",
			XML_GetCurrentLineNumber(ep),
			XML_ErrorString(XML_GetErrorCode(ep)));
		throw std::exception(buff);

	}
}
int XMLTools::finishAdding() throw(...)
{
	XML_ParserFree(ep);
	if (nestLevel!=0)
	{
		throw std::exception("Nesting error");
	}
	return 0;
}

// Helper function to create a VT_BSTR variant from a null terminated string. 
HRESULT MSXMLTools::VariantFromString(PCWSTR wszValue, VARIANT &Variant)
{
	HRESULT hr = S_OK;
	BSTR bstr = SysAllocString(wszValue);
	CHK_ALLOC(bstr);

	V_VT(&Variant)   = VT_BSTR;
	V_BSTR(&Variant) = bstr;

CleanUp:
	return hr;
}
HRESULT MSXMLTools::VariantFromString(char* wszValue, VARIANT &Variant)
{
	HRESULT hr = S_OK;
	int sz=strlen(wszValue);

	LPWSTR dest=(LPWSTR)calloc(sz,sizeof(WCHAR));
	mbstowcs(dest,wszValue,sz+1);

	BSTR bstr = SysAllocString(dest);
	CHK_ALLOC(bstr);

	V_VT(&Variant)   = VT_BSTR;
	V_BSTR(&Variant) = bstr;

CleanUp:
	return hr;
}

HRESULT MSXMLTools::CreateAndInitDOM(MSXML2::IXMLDOMDocument **ppDoc)
{
	HRESULT hr = CoCreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(ppDoc));
	if (SUCCEEDED(hr))
	{
		// these methods should not fail so don't inspect result
		(*ppDoc)->put_async(VARIANT_FALSE);  
		(*ppDoc)->put_validateOnParse(VARIANT_TRUE);
		(*ppDoc)->put_resolveExternals(VARIANT_FALSE);
	}
	return hr;
}
MSXMLTools::MSXMLTools(char* src, long sz) throw(...)
{

	HRESULT hr = S_OK;
	MSXML2::IXMLDOMDocument *pXMLDom=NULL;
	MSXML2::IXMLDOMParseError *pXMLErr = NULL;

	BSTR bstrXML = NULL;
	BSTR bstrErr = NULL;
	VARIANT_BOOL varStatus;

	LPWSTR dest=(LPWSTR)calloc(sz,sizeof(WCHAR));
	mbstowcs(dest,src,sz+1);

	BSTR xmlContent= SysAllocString(dest);
	//VariantInit(&xmlContent);

	CHK_HR(CreateAndInitDOM(&pXMLDom));    

	setlocale( LC_ALL, "English" );
	// XML file name to load
//	CHK_HR(VariantFromString(src, xmlContent));
	CHK_HR(pXMLDom->loadXML(xmlContent, &varStatus));
	HRESULT Error;
		// Failed to load xml, get last parsing error
	CHK_HR(Error=pXMLDom->get_parseError(&pXMLErr));
	if (pXMLErr==NULL || varStatus == VARIANT_TRUE)
	{
		CHK_HR(pXMLDom->get_xml(&bstrXML));
		//printf("XML DOM loaded:\n%S\n", bstrXML);
	}
	else
	{
		CHK_HR(pXMLErr->get_reason(&bstrErr));
		char* buff =(char*) malloc(1024);
		int reqsz=1023;
		catchError(WideCharToMultiByte(CP_ACP,0,(LPWSTR)bstrErr,-1,buff,reqsz,NULL,NULL));
    
		//wcstombs(buff,bstrErr,1024);
		throw std::exception(buff);
	}
CleanUp:
	SAFE_RELEASE(pXMLDom);
	SAFE_RELEASE(pXMLErr);
	SysFreeString(bstrXML);
	SysFreeString(bstrErr);
//	VariantClear(&xmlContent);

}
static void XMLCALL treecharproc(void *userData, const XML_Char *s, int len)
{
	XMLTextNode* xtn = new XMLTextNode(new std::string(s,len));
	xtn->parent=tparent;
	tparent->addChild(xtn);
	
};
static void XMLCALL treestartproc(void *userData, const char *name, const char **atts)
{
	XMLElementNode* xen = new XMLElementNode(new std::string(name));

	while (*atts) {
    
    
		const char *name = *atts;
		atts++;
		const char *value = *atts;

		XMLAttributeNode* xan = new XMLAttributeNode(new std::string(name),new std::string(value));
		xan->parent=xen;
		xen->addChild(xan);
		atts++;
	}


	xen->parent=tparent;
	tparent->addChild(xen);
	tparent=xen;

};
static void XMLCALL treeendproc(void *userData, const char *name)
{
	tparent=tparent->parent;
};
static void XMLCALL treepiproc(void *userData, const XML_Char *target, const XML_Char *data)
{
	printf("XML PI '%s' with value '%s'.",target,data);
	//std::string* index = new std::string(target);
	//props[index]=new std::string(data);
}
void XMLCALL treecommentproc(void *userData, const XML_Char *data)
{
	XMLCommentNode* xcn = new XMLCommentNode(new std::string(data));
	xcn->parent=tparent;
	tparent->addChild(xcn);
}
void XMLCALL cdataprocstart(void *userData)
{
	cdatastart=XML_GetCurrentByteIndex(tep);
}
void XMLCALL cdataprocend(void *userData)
{
	XML_Index delta=XML_GetCurrentByteIndex(tep)-cdatastart;
	XMLCDataNode* xcdn = new XMLCDataNode(new std::string(globalref+cdatastart,delta));
	xcdn->parent=tparent;
	tparent->addChild(xcdn);
	cdatastart=0;
}

XMLTree::XMLTree()
{
	//props=new std::map<std::string*, std::string*>();
	tep=XML_ParserCreate(NULL);
	XML_SetProcessingInstructionHandler(tep, treepiproc);
	XML_SetElementHandler(tep, treestartproc, treeendproc);
	XML_SetCharacterDataHandler(tep, treecharproc);
	XML_SetCommentHandler(tep,treecommentproc);
	XML_SetCdataSectionHandler(tep,cdataprocstart,cdataprocend);
};
XMLElementNode* XMLTree::fromText(char* in, int sz)
{
	globalref=in;
	XMLElementNode* xen = new XMLElementNode(new std::string("Root"));
	xen->children = new Sys::TValueArray<XMLNode>();
	tparent=xen;
	if (XML_Parse(tep, in, sz, 0) == XML_STATUS_ERROR) {
		char* buff =(char*) malloc(1024);
		sprintf(buff, "Parse error at line %d:\n%s\n",
			XML_GetCurrentLineNumber(tep),
			XML_ErrorString(XML_GetErrorCode(tep)));
		throw std::exception(buff);
	}
	return xen;
}