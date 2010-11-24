#include "PropertyTreeManaged.h"

using namespace System;

namespace Grom
{
	public ref class ManagedPropertyTree
	{
	private:
		Sys::PropertyTree* propertyTree;

	public:
		ManagedPropertyTree()
		{	
			propertyTree = Sys::Create<Sys::PropertyTree>(Sys::GlobalOwner());
		}

		ManagedPropertyTree(Sys::PropertyTree* tree)
		{
			propertyTree = tree;
		} 
		~ManagedPropertyTree()
		{
			propertyTree->Destroy();
		}

		property Sys::PropertyTree* UnmanagedTree
		{
			Sys::PropertyTree* get()
			{
				return propertyTree;
			}
		}
	};

	static public ref class ManagedPropertyTreeXmlSerializer
	{
	public:
		static ManagedPropertyTree^ Deserialize(String^ filename)
		{
			Sys::PropertyTree* prop = Sys::Create<Sys::PropertyTree>(Sys::GlobalOwner());
			Utl::LoadPropertyTreeFromXmlFile(prop, (const char *)(void *)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(filename));
			return gcnew ManagedPropertyTree(prop);
		}

		static void Serialize(String^ filename, ManagedPropertyTree^ tree)
		{
			if (tree == nullptr)
				throw gcnew ArgumentNullException();
			Utl::SavePropertyTreeToXmlFile(tree->UnmanagedTree, (const char *)(void *)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(filename));
		}
	};
}