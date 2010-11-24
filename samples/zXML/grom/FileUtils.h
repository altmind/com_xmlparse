// Free open-source Grom library

#ifndef __Grom_System_FileUtils_h__
#define __Grom_System_FileUtils_h__

#include "Config.h"
#include <time.h>
#include <string>

namespace Sys
{

template <class T> class TValueArray;

extern const char* FILE_PATH_DELIMITERS;
extern const char* URL_PATH_DELIMITERS;

std::string CurrentDir();
void SetCurrentDir(const char* dir);
bool DirExists(const char* dir);
void CreateDir(const char* dir);
void ForceDir(const char* dir);
bool TryDeleteDir(const char* dir, bool recursively);
void DeleteDir(const char* dir, bool recursively);
bool IsDir(const char* path);
bool IsPathDelimiter(char ch); // FILE_PATH_DELIMITERS
bool IsAbsolutePath(const char* path);
bool IsNetworkPath(const char* path);
std::string JoinPath(const char* head, const char* tail);
std::string NormalizePath(const char* path);
std::string RealPath(const char* path);
void ExtractDir(const char* path, std::string* result);
void ExtractFileName(const char* path, std::string* result);
bool FileExists(const char* path);
time_t FileAge(const char* path);
void DeleteFile(const char* path);
void RenameFile(const char* oldPath, const char* newPath);
void GetFiles(const char* dir, bool recursively, TValueArray<std::string>* result);
void GetDirs(const char* dir, bool recursively, TValueArray<std::string>* result);
void GetFilesAndDirs(const char* dir, bool recursively, TValueArray<std::string>* result);

// FileEnumerator

class FileEnumerator
{
public:
  FileEnumerator();
  FileEnumerator(const char* startupDir, bool recursive); // calls Open
  ~FileEnumerator();

  const char* StartupDir();
  void SetStartupDir(const char* value);
  bool Recursive();
  void SetRecursive(bool value);

  void Reset();
  bool MoveNext();
  const char* CurrentPath();
  const char* CurrentDir();
  const char* CurrentFileName();

private:
  std::string m_StartupDir;
  class NonRecursiveFileEnumerator* m_CurrentNonRecursiveEnumerator;
  int m_Flags;
  std::string m_Current;
};

inline
const char* FileEnumerator::StartupDir()
{
  return m_StartupDir.c_str();
}

// TempCurrentDir

class TempCurrentDir
{
public:
  explicit TempCurrentDir(const char* dir);
  ~TempCurrentDir();

private:
  std::string m_SavedDir;
};

// IsPathDelimiter

inline
bool IsPathDelimiter(char ch)
{
  return ch != '\0' && strchr(FILE_PATH_DELIMITERS, ch) != NULL;
}

} // namespace Sys

#endif //__Grom_System_FileUtils_h__
