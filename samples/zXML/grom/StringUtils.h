// Free open-source Grom library

#ifndef __Grom_System_StringUtils_h__
#define __Grom_System_StringUtils_h__

#include "Config.h"

#include <vector>
#include <string>
#include <stdarg.h>

namespace Sys
{

void ReplaceChar(char* str, char ch, char by);
void ReplaceChar(std::string& str, char ch, char by);
std::string LowerCase(const char* str);
std::string LowerCase(const std::string& str);
std::string UpperCase(const char* str);
std::string UpperCase(const std::string& str);
std::string SentenceCase(const char* str);
std::string SentenceCase(const std::string& str);
int CompareStr(const char* str1, const char* str2);
int CompareText(const char* str1, const char* str2);
bool SameStr(const char* str1, const char* str2);
bool SameText(const char* str1, const char* str2);
bool IsEmpty(const char* str);
bool ContainsChar(const char* str, char ch);
char* CopyStr(const char* source, char* target, int targetSize);
std::string TrimSpaces(const char* str);
const char* SkipChars(const char* str, const char* chars);
const char* FindChars(const char* str, const char* chars);
std::string GetNextToken(const char** str, const char* delimiters, const char* spaces);
std::string StrFromInt(int value);
int IntFromStr(const char* value);
int IntFromStrDef(const char* value, int defaultResult);
#ifdef CFG_WIN32
std::string MultiByteStringFromWideString(const wchar_t* str);
std::basic_string<wchar_t> WideStringFromMultiByteString(const char* str);
#endif

// FormattedString

class FormattedString : public std::string
{
public:
  FormattedString();
  FormattedString(const char* format, ...);

private:
  void DoFormat(const char* format, ...);
  void DoFormatArgs(const char* format, va_list argList);
};

// MultiString

class MultiString : public std::string
{
public:
  MultiString();
  MultiString(const MultiString& a);
  MultiString(const char* str, const char* delimiters, const char* spaces);

  int word_count() const;
  const char* word(int index) const;

private:
  std::vector<size_t> m_words;
};

// Inline implementations

inline
bool IsEmpty(const char* str)
{
  return (str == NULL) || str[0] == '\0';
}

} // namespace Sys

#endif //__Grom_System_StringUtils_h__
