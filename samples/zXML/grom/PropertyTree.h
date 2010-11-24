// Free open-source Grom library

#ifndef __Grom_System_PropertyTree_h__
#define __Grom_System_PropertyTree_h__

#include "Config.h"

#include <string>
#include "Object.h"
#include "ValueArray.h"

namespace Sys
{

// The PropertyTree class provides hierarchical list of name/value strings.
// It is useful to work with XML in memory.

// PropertyTree

class PropertyTree : public Object
{
public:
  void CopyFrom(PropertyTree* from);
  const char* Name();
  void SetName(const char* value);
  const char* Text();
  void SetText(const char* value);
  void SetText(const char* value, int valueLength);
  // Items
  int ItemCount();
  const char* ItemName(int index);
  const char* ItemValue(int index);
  const char* ItemValueByName(const char* name, const char* defaultValue);
  int FindItem(const char* name);
  int SetItem(const char* name, const char* value);
  void RemoveItem(const char* name);
  void RemoveItemAt(int index);
  void Clear();
  // Hierarchy
  PropertyTree* Parent();
  void SetParent(PropertyTree* value);
  int ChildCount();
  PropertyTree* Child(int index);
  PropertyTree* FindChild(const char* path, const char* itemName, const char* itemValue); // names in path are delimited by '/'
  const char* ChildItemValueByName(const char* path, const char* itemName, const char* defaultValue);
  PropertyTree* CreateChild(const char* name);

protected:
  virtual void Initialize(Object* owner); // override
  virtual void Finalize(); // override

private:
  // Types
  struct NameValue
  {
    std::string Name;
    std::string Value;
  };

  // Fields
  std::string m_Name;
  std::string m_Text;
  TValueArray<NameValue> m_Items;
  Collection* m_Children;
};

class PropertyTreeEnumerator
{
public:
  PropertyTreeEnumerator();
  PropertyTreeEnumerator(PropertyTree* root);
  ~PropertyTreeEnumerator();

  PropertyTree* Root();
  void SetRoot(PropertyTree* value);

  void Reset();
  bool MoveNext();
  PropertyTree* Current();

private:
  void DeleteNonRecursiveEnumerator();

  PropertyTree* m_Root;
  class NonRecursivePropertyTreeEnumerator* m_ChildrenEnumerator;
};

inline
PropertyTree* PropertyTreeEnumerator::Root()
{
  return m_Root;
}

} // namespace Sys

#endif // __Grom_System_PropertyTree_h__
