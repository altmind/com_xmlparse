// zXML.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include <commctrl.h>
#include <commdlg.h>
#include <stdio.h>
#include <iostream>
#include <cassert>
#include <fcntl.h>
#include <Psapi.h>
#include <io.h>
#include "Scintilla.h"
#include "SciLexer.h"
#include "XML.h"
#include "zXML.h"
#pragma comment(lib, "Psapi.lib")
LRESULT handleFindDialogMessage(LPMSG msg);
wchar_t* printErrorA(DWORD errorCode, LPSTR file, DWORD line)
{
	LPWSTR allocd = (LPWSTR)calloc(255,sizeof(wchar_t));

	assert(allocd!=NULL);
	MultiByteToWideChar( CP_ACP, 0, file, strlen(file)+1, allocd, 255 );
	return printErrorW(errorCode, (LPWSTR)allocd,line);
}

wchar_t* printErrorW(DWORD errorCode, LPWSTR file, DWORD line)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,GetLastError(),MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf,0, NULL );
	wchar_t* allocd = (wchar_t*) calloc(255,sizeof(wchar_t));
	
	LPWSTR strBuf = (LPWSTR) lpMsgBuf;
	int len = wcslen(strBuf);
	strBuf[len-2]=0;
	wsprintf(allocd,L"Error: %d(%ls). %ls:%hu\n",errorCode,lpMsgBuf, file, line);
	return allocd;
}


#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна
HMODULE hmod;
HWND hwndScintilla;
HWND hWnd;
HWND fdHWND;
UINT uFindReplaceMsg;  // message identifier for FINDMSGSTRING 
static WCHAR szFindText [512] ;
const COLORREF black = RGB(0,0,0);
const COLORREF white = RGB(0xff,0xff,0xff);

LRESULT SendEditor(UINT Msg, WPARAM wParam=0, LPARAM lParam=0) {
	return SendMessage(hwndScintilla, Msg, wParam, lParam);
}
void SetAStyle(int style, COLORREF fore, COLORREF back=white, int size=-1, const char *face=0);
void SetAStyle(int style, COLORREF fore, COLORREF back, int size, const char *face) {
	SendEditor(SCI_STYLESETFORE, style, fore);
	SendEditor(SCI_STYLESETBACK, style, back);
	if (size >= 1)
		SendEditor(SCI_STYLESETSIZE, style, size);
	if (face) 
		SendEditor(SCI_STYLESETFONT, style, reinterpret_cast<LPARAM>(face));
}

int dialogSaveXML(LPWSTR str)
{
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = str;

	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = L"XML file(*.xml)\0*.xml";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrDefExt=L"xml";
	ofn.lpstrInitialDir = NULL;
	//ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	return (int)GetSaveFileNameW(&ofn);
}

BOOL ValidateXML()
{
	return true;
}
void saveXML(LPWSTR str)
{
	if (ValidateXML())
	{
		FILE* f;

		catchError(f=_wfopen(str,L"w"));


		long sz = SendMessageW(hwndScintilla,SCI_GETTEXTLENGTH, 0, 0);

		catchError(LPWSTR lpwstr = (LPWSTR)malloc(sz));
		assert(lpwstr!=NULL);

		SendMessage(hwndScintilla,SCI_GETTEXT, sz, (LPARAM)lpwstr);

		fwrite(lpwstr,1,sz,f);
		catchError(fclose(f));
	}
}

HWND  ShowFindDialog()
{
	static FINDREPLACE fr ; // must be static for modeless dialog!!!

	fr.lStructSize = sizeof (FINDREPLACE) ;
	fr.hwndOwner = hWnd ;
	fr.hInstance = hInst ;
	//fr.Flags = FR_HIDEUPDOWN | FR_HIDEMATCHCASE | FR_HIDEWHOLEWORD;
	fr.Flags = FR_DOWN | FR_HIDEWHOLEWORD| FR_HIDEUPDOWN|FR_HIDEMATCHCASE;
	fr.lpstrFindWhat = szFindText ;
	fr.lpstrReplaceWith = NULL ;
	fr.wFindWhatLen = 512 ;
	fr.wReplaceWithLen = 0 ;
	fr.lCustData = 0 ;
	fr.lpfnHook = NULL ;
	fr.lpTemplateName = NULL ;
	catchError(HWND ret = FindText(&fr));
	DWORD errCode = CommDlgExtendedError();

	return ret;

}

int dialogOpenXML(LPWSTR str)
{
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = str;

	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = L"XML file(*.xml)\0*.xml";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	return (int)GetOpenFileNameW(&ofn);
}
void openXML(LPWSTR str)
{
	/*DWORD dword;
	DWORD unused;
	catchError(HANDLE hFile = CreateFile(str, 
	GENERIC_READ,
	0, 
	NULL,
	OPEN_EXISTING, 
	FILE_ATTRIBUTE_NORMAL, 
	NULL));
	catchError(dword=GetFileSize(hFile,&unused));
	catchError(CloseHandle(hFile));*/
	FILE* f;

	catchError(f=_wfopen(str,L"r"));
	catchError(LPWSTR lpwstr = (LPWSTR)malloc(4096));
	assert(lpwstr!=NULL);
	RtlZeroMemory(lpwstr,4096);

	catchError(SendMessage(hwndScintilla,SCI_CLEARALL, 0, 0));

	int sz;
	while((sz=fread(lpwstr,1,4096,f))>0)
	{
		catchError(SendMessage(hwndScintilla,SCI_ADDTEXT, sz, (LPARAM)lpwstr));
		RtlZeroMemory(lpwstr,4096);
	}

	catchError(SendMessage(hwndScintilla,SCI_SETSTYLEBITS, 7, 0));
	catchError(SendMessage(hwndScintilla,SCI_STARTSTYLING, 0, 0));
	catchError(fclose(f));
}


// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


void reassignConsoleIO()
{
	int hConHandle;
	long lStdHandle;
	CONSOLE_SCREEN_BUFFER_INFO coninfo;

	FILE *fp;
	AllocConsole();
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.Y = 500;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "r" );
	*stdin = *fp;
	setvbuf( stdin, NULL, _IONBF, 0 );
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stderr = *fp;
	setvbuf( stderr, NULL, _IONBF, 0 );
	//	ios::sync_with_stdio();
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	reassignConsoleIO();
	CoInitialize(NULL);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: разместите код здесь.
	MSG msg;
	HACCEL hAccelTable;

	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ZXML, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	uFindReplaceMsg = RegisterWindowMessage(FINDMSGSTRING);


	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ZXML));

	// Цикл основного сообщения:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			/*if (IsDialogMessage(fdHWND,&msg))
			{
				handleFindDialogMessage(&msg);
			}*/
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	CoUninitialize();
	return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
//  КОММЕНТАРИИ:
//
//    Эта функция и ее использование необходимы только в случае, если нужно, чтобы данный код
//    был совместим с системами Win32, не имеющими функции RegisterClassEx'
//    которая была добавлена в Windows 95. Вызов этой функции важен для того,
//    чтобы приложение получило "качественные" мелкие значки и установило связь
//    с ними.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ZXML));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_ZXML);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

	hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
//
//
const char htmlKeyWords[] = 
"a abbr acronym address applet area b base basefont "
"bdo big blockquote body br button caption center "
"cite code col colgroup dd del dfn dir div dl dt em encoding"
"fieldset font form frame frameset h1 h2 h3 h4 h5 h6 "
"head hr html i iframe img input ins isindex kbd label "
"legend li link map menu meta noframes noscript "
"object ol optgroup option p param pre q s samp "
"script select small span strike strong style sub sup "
"table tbody td textarea tfoot th thead title tr tt u ul "
"var xmlns "
"abbr accept-charset accept accesskey action align alink "
"alt archive axis background bgcolor border "
"cellpadding cellspacing char charoff charset checked cite "
"class classid clear codebase codetype color cols colspan "
"compact content coords "
"data datafld dataformatas datapagesize datasrc datetime "
"declare defer dir disabled enctype "
"face for frame frameborder "
"headers height href hreflang hspace http-equiv "
"id ismap label lang language link longdesc "
"marginwidth marginheight maxlength media method multiple "
"name nohref noresize noshade nowrap "
"object onblur onchange onclick ondblclick onfocus "
"onkeydown onkeypress onkeyup onload onmousedown "
"onmousemove onmouseover onmouseout onmouseup "
"onreset onselect onsubmit onunload "
"profile prompt readonly rel rev rows rowspan rules "
"scheme scope shape size span src standby start style "
"summary tabindex target text title type usemap "
"valign value valuetype version vlink vspace width "
"text password checkbox radio submit reset "
"file hidden image "
"public !doctype xml";

const char jsKeyWords[] = 
"break case catch continue default "
"do else for function if return throw try var while";

const char vbsKeyWords[] = 
"and as byref byval case call const "
"continue dim do each else elseif end error exit false for function global "
"goto if in loop me new next not nothing on optional or private public "
"redim rem resume select set sub then to true type while with "
"boolean byte currency date double integer long object single string type "
"variant";
void initLexer()
{
	SendEditor(SCI_SETLEXER, SCLEX_HTML);
	SendEditor(SCI_SETSTYLEBITS, 7);

	SendEditor(SCI_SETKEYWORDS, 0, 
		reinterpret_cast<LPARAM>(htmlKeyWords));
	SendEditor(SCI_SETKEYWORDS, 1, 
		reinterpret_cast<LPARAM>(jsKeyWords));
	SendEditor(SCI_SETKEYWORDS, 2, 
		reinterpret_cast<LPARAM>(vbsKeyWords));

	// Set up the global default style. These attributes are used wherever no explicit choices are made.
	SetAStyle(STYLE_DEFAULT, black, white, 10, "Verdana");
	SendEditor(SCI_STYLECLEARALL);	// Copies global style to all others

	const COLORREF red = RGB(0xFF, 0, 0);
	const COLORREF offWhite = RGB(0xFF, 0xFB, 0xF0);
	const COLORREF darkGreen = RGB(0, 0x80, 0);
	const COLORREF darkBlue = RGB(0, 0, 0x80);
	const COLORREF cyan = RGB(00,0x8B,0x7B);
	const COLORREF cyan_inc = RGB(0xC5,0x8B,0x7B);


	// Hypertext default is used for all the document's text
	SetAStyle(SCE_H_DEFAULT, black, white, 10, "Verdana");//Times New Roman

	// Unknown tags and attributes are highlighed in red. 
	// If a tag is actually OK, it should be added in lower case to the htmlKeyWords string.
	SetAStyle(SCE_H_TAG, darkBlue);
	SetAStyle(SCE_H_TAGUNKNOWN, darkBlue);
	SetAStyle(SCE_H_ATTRIBUTE, cyan);
	SetAStyle(SCE_H_ATTRIBUTEUNKNOWN, cyan);
	SetAStyle(SCE_H_NUMBER, RGB(0x80,0,0x80));
	SetAStyle(SCE_H_DOUBLESTRING, RGB(0,0x80,0));
	SetAStyle(SCE_H_SINGLESTRING, RGB(0,0x80,0));
	SetAStyle(SCE_H_OTHER, RGB(0x80,0,0x80));
	SetAStyle(SCE_H_COMMENT, RGB(0x80,0x80,0));
	SetAStyle(SCE_H_ENTITY, RGB(0x80,0,0x80));

	SetAStyle(SCE_H_TAGEND, darkBlue);
	SetAStyle(SCE_H_XMLSTART, darkBlue);	// <?
	SetAStyle(SCE_H_XMLEND, darkBlue);		// ?>
	SetAStyle(SCE_H_SCRIPT, darkBlue);		// <script
	SetAStyle(SCE_H_ASP, RGB(0x4F, 0x4F, 0), RGB(0xFF, 0xFF, 0));	// <% ... %>
	SetAStyle(SCE_H_ASPAT, RGB(0x4F, 0x4F, 0), RGB(0xFF, 0xFF, 0));	// <%@ ... %>

	SetAStyle(SCE_HB_DEFAULT, black);
	SetAStyle(SCE_HB_COMMENTLINE, darkGreen);
	SetAStyle(SCE_HB_NUMBER, RGB(0,0x80,0x80));
	SetAStyle(SCE_HB_WORD, darkBlue);
	SendEditor(SCI_STYLESETBOLD, SCE_HB_WORD, 1);
	SetAStyle(SCE_HB_STRING, RGB(0x80,0,0x80));
	SetAStyle(SCE_HB_IDENTIFIER, black);

	// This light blue is found in the windows system palette so is safe to use even in 256 colour modes.
	const COLORREF lightBlue = RGB(0xA6, 0xCA, 0xF0);
	// Show the whole section of VBScript with light blue background
	for (int bstyle=SCE_HB_DEFAULT; bstyle<=SCE_HB_STRINGEOL; bstyle++) {
		SendEditor(SCI_STYLESETFONT, bstyle, 
			reinterpret_cast<LPARAM>("Georgia"));
		SendEditor(SCI_STYLESETBACK, bstyle, lightBlue);
		// This call extends the backround colour of the last style on the line to the edge of the window
		SendEditor(SCI_STYLESETEOLFILLED, bstyle, 1);
	}
	SendEditor(SCI_STYLESETBACK, SCE_HB_STRINGEOL, RGB(0x7F,0x7F,0xFF));
	SendEditor(SCI_STYLESETFONT, SCE_HB_COMMENTLINE, 
		reinterpret_cast<LPARAM>("Comic Sans MS"));

	SetAStyle(SCE_HBA_DEFAULT, black);
	SetAStyle(SCE_HBA_COMMENTLINE, darkGreen);
	SetAStyle(SCE_HBA_NUMBER, RGB(0,0x80,0x80));
	SetAStyle(SCE_HBA_WORD, darkBlue);
	SendEditor(SCI_STYLESETBOLD, SCE_HBA_WORD, 1);
	SetAStyle(SCE_HBA_STRING, RGB(0x80,0,0x80));
	SetAStyle(SCE_HBA_IDENTIFIER, black);

	// Show the whole section of ASP VBScript with bright yellow background
	for (int bastyle=SCE_HBA_DEFAULT; bastyle<=SCE_HBA_STRINGEOL; bastyle++) {
		SendEditor(SCI_STYLESETFONT, bastyle, 
			reinterpret_cast<LPARAM>("Georgia"));
		SendEditor(SCI_STYLESETBACK, bastyle, RGB(0xFF, 0xFF, 0));
		// This call extends the backround colour of the last style on the line to the edge of the window
		SendEditor(SCI_STYLESETEOLFILLED, bastyle, 1);
	}
	SendEditor(SCI_STYLESETBACK, SCE_HBA_STRINGEOL, RGB(0xCF,0xCF,0x7F));
	SendEditor(SCI_STYLESETFONT, SCE_HBA_COMMENTLINE, 
		reinterpret_cast<LPARAM>("Verdana"));

	// If there is no need to support embedded Javascript, the following code can be dropped.
	// Javascript will still be correctly processed but will be displayed in just the default style.

	SetAStyle(SCE_HJ_START, RGB(0x80,0x80,0));
	SetAStyle(SCE_HJ_DEFAULT, black);
	SetAStyle(SCE_HJ_COMMENT, darkGreen);
	SetAStyle(SCE_HJ_COMMENTLINE, darkGreen);
	SetAStyle(SCE_HJ_COMMENTDOC, darkGreen);
	SetAStyle(SCE_HJ_NUMBER, RGB(0,0x80,0x80));
	SetAStyle(SCE_HJ_WORD, black);
	SetAStyle(SCE_HJ_KEYWORD, darkBlue);
	SetAStyle(SCE_HJ_DOUBLESTRING, RGB(0x80,0,0x80));
	SetAStyle(SCE_HJ_SINGLESTRING, RGB(0x80,0,0x80));
	SetAStyle(SCE_HJ_SYMBOLS, black);

	SetAStyle(SCE_HJA_START, RGB(0x80,0x80,0));
	SetAStyle(SCE_HJA_DEFAULT, black);
	SetAStyle(SCE_HJA_COMMENT, darkGreen);
	SetAStyle(SCE_HJA_COMMENTLINE, darkGreen);
	SetAStyle(SCE_HJA_COMMENTDOC, darkGreen);
	SetAStyle(SCE_HJA_NUMBER, RGB(0,0x80,0x80));
	SetAStyle(SCE_HJA_WORD, black);
	SetAStyle(SCE_HJA_KEYWORD, darkBlue);
	SetAStyle(SCE_HJA_DOUBLESTRING, RGB(0x80,0,0x80));
	SetAStyle(SCE_HJA_SINGLESTRING, RGB(0x80,0,0x80));
	SetAStyle(SCE_HJA_SYMBOLS, black);

	// Show the whole section of Javascript with off white background
	for (int jstyle=SCE_HJ_DEFAULT; jstyle<=SCE_HJ_SYMBOLS; jstyle++) {
		SendEditor(SCI_STYLESETFONT, jstyle, 
			reinterpret_cast<LPARAM>("Verdana"));//Lucida Sans Unicode
		SendEditor(SCI_STYLESETBACK, jstyle, offWhite);
		SendEditor(SCI_STYLESETEOLFILLED, jstyle, 1);
	}
	SendEditor(SCI_STYLESETBACK, SCE_HJ_STRINGEOL, RGB(0xDF, 0xDF, 0x7F));
	SendEditor(SCI_STYLESETEOLFILLED, SCE_HJ_STRINGEOL, 1);

	// Show the whole section of Javascript with brown background
	for (int jastyle=SCE_HJA_DEFAULT; jastyle<=SCE_HJA_SYMBOLS; jastyle++) {
		SendEditor(SCI_STYLESETFONT, jastyle, 
			reinterpret_cast<LPARAM>("Verdana"));//Lucida Sans Unicode
		SendEditor(SCI_STYLESETBACK, jastyle, RGB(0xDF, 0xDF, 0x7F));
		SendEditor(SCI_STYLESETEOLFILLED, jastyle, 1);
	}
	SendEditor(SCI_STYLESETBACK, SCE_HJA_STRINGEOL, RGB(0x0,0xAF,0x5F));
	SendEditor(SCI_STYLESETEOLFILLED, SCE_HJA_STRINGEOL, 1);
}
char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace(*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}
void MSWFCheck()
{
	try{

		long sz = SendMessageW(hwndScintilla,SCI_GETTEXTLENGTH, 0, 0);
		catchError(char* mbcstr = (char*)malloc(sz));
		assert(mbcstr!=NULL);
		SendMessage(hwndScintilla,SCI_GETTEXT, sz+1, (LPARAM)mbcstr);
		MSXMLTools mxt = MSXMLTools(trimwhitespace(mbcstr),sz);
		MessageBoxA(hWnd,"Document seem to be well-formed","Well-Formatness check",0);
	}
	catch (std::exception& e)
	{
		MessageBoxA(hWnd,e.what(),"MSXML Well-formatness check failed",0);
	}
}

void WFCheck()
{
	XMLTools* xt  = new XMLTools();
	try
	{
        long sz = SendMessageW(hwndScintilla,SCI_GETTEXTLENGTH, 0, 0);
		catchError(char* mbcstr = (char*)malloc(sz));
		assert(mbcstr!=NULL);
		SendMessage(hwndScintilla,SCI_GETTEXT, sz+1, (LPARAM)mbcstr);

        xt->addText(trimwhitespace(mbcstr),sz);
		xt->finishAdding();
		MessageBoxA(hWnd,"Document seem to be well-formed","Well-Formatness check",0);
	}
    catch (std::exception& e)
	{
        MessageBoxA(hWnd,e.what(),"Well-formatness check failed",0);
	}
}
void BuildTree()
{
	XMLTree* xt  = new XMLTree();
	try
	{
		PROCESS_MEMORY_COUNTERS pmc;
		GetProcessMemoryInfo(GetCurrentProcess(),&pmc,sizeof(PROCESS_MEMORY_COUNTERS));
		long workingSet = pmc.WorkingSetSize;
        long sz = SendMessageW(hwndScintilla,SCI_GETTEXTLENGTH, 0, 0);
		catchError(char* mbcstr = (char*)malloc(sz));
		assert(mbcstr!=NULL);
		SendMessage(hwndScintilla,SCI_GETTEXT, sz+1, (LPARAM)mbcstr);

        XMLElementNode* x = xt->fromText(trimwhitespace(mbcstr),sz);

		GetProcessMemoryInfo(GetCurrentProcess(),&pmc,sizeof(PROCESS_MEMORY_COUNTERS));
		long workingSet2=pmc.WorkingSetSize;
		long deltam=(workingSet2-workingSet)/1000000;

		wchar_t buf[255];
		wsprintf(buf,L"Tree was successfully built. %dM of mem used.",deltam);
		MessageBox(hWnd,buf,L"Tree",0);
	}
    catch (std::exception& e)
	{
        MessageBoxA(hWnd,e.what(),"exception",0);
	}
}

LRESULT handleFindDialogMessage(LPMSG msg)
{
	//printf("%ld\n",msg->message);
	switch (msg->message)
	{
	default:
		;
	//	return DefWindowProc(fdHWND, msg->message, msg->wParam, msg->lParam);
	}
	return 0;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	int hw,lw;
	LPWSTR lpwstr;
	if (message==uFindReplaceMsg)
	{

		Sci_TextToFind ttf;
		int startpos = SendMessage(hwndScintilla,SCI_GETSELECTIONEND, 0, 0);
		ttf.chrg.cpMin=startpos;
		ttf.chrg.cpMax=99999;
		static char buff[1024];
		catchError(WideCharToMultiByte(CP_ACP,0,(LPWSTR)szFindText,-1,buff,1023,NULL,NULL));
		ttf.lpstrText=buff;
		SendMessage(hwndScintilla,SCI_FINDTEXT, 0, (LPARAM)&ttf);
		SendMessage(hwndScintilla,SCI_SETSELECTIONSTART, ttf.chrgText.cpMin, 0);
		SendMessage(hwndScintilla,SCI_SETSELECTIONEND, ttf.chrgText.cpMax, 0);

	}
	else 
	switch (message)
	{
	
	case WM_CREATE:
		hWnd=hWnd;

		//uFindReplaceMsg = RegisterWindowMessage(FINDMSGSTRING);
		hmod = LoadLibrary(L"SciLexer.DLL");
		if (hmod==NULL)
		{
			MessageBox(hWnd,
				L"The DLL could not be loaded.",
				L"Error",
				MB_OK | MB_ICONERROR);
		}
		else
		{

			hwndScintilla = CreateWindowEx(0,
				L"Scintilla",L"", WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN,
				1,1,100,100,hWnd,(HMENU)NULL, hInst,NULL);
			initLexer();
			catchError(SendMessage(hwndScintilla,SCI_SETLEXER, SCLEX_HTML , 0));
            //catchError(SendMessage(hwndScintilla,SCI_SETCODEPAGE, SC_CP_UTF8,0));
		}
		break;
	case WM_SIZE:
		hw=HIWORD(lParam);
		lw=LOWORD(lParam);
		MoveWindow(hwndScintilla,1,1,lw-1,hw-1,TRUE);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_CREATE:
			SendMessage(hwndScintilla,SCI_CLEARALL, 0, 0);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_OPEN:
			lpwstr=(LPWSTR)malloc(512);
			if (dialogOpenXML(lpwstr))
			{
				openXML(lpwstr);
			}
			free(lpwstr);
			break;
		case IDM_SAVE:
			lpwstr=(LPWSTR)malloc(512);
			if (dialogSaveXML(lpwstr))
			{
				saveXML(lpwstr);
			}
			free(lpwstr);
			break;
		case IDM_UNDO:
			SendEditor(WM_UNDO);
			break;
		case IDM_REDO:
			SendEditor(SCI_REDO);
			break;
		case IDM_CUT:
			SendEditor(WM_CUT);
			break;
		case IDM_COPY:
			SendEditor(WM_COPY);
			break;
		case IDM_PASTE:
			SendEditor(WM_PASTE);
			break;
		case IDM_MSXMLWFCHECK:
			MSWFCheck();
			break;
		case IDM_WFCHECK:
			WFCheck();
			break;
		case IDM_BUILDTREE:
			BuildTree();
			break;
		case IDM_FIND:
			fdHWND = ShowFindDialog();
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: добавьте любой код отрисовки...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}