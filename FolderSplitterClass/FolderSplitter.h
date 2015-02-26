// CLASS NAME: FolderSplitter - using DeepBurner
// CLASS DESC: Not sure yet

#include <iostream>
using namespace std;

#include <windows.h>
#include <Shlwapi.h>
#include <string.h>

class FolderSplitter
{
private:
	// VARIABLES
	HANDLE pathHandle;
	WIN32_FIND_DATAW pathData;

	// FUNCTIONS
	
	// returns total number of paths within specified parent path
	//inline int pathTotal(wchar_t* parentPath)
	//{
	//	int i = 0;
	//	HANDLE hPath;
	//	
	//	FindFirstFileW(parentPath, NULL);
	//	i++;

	//	while ( FindNextFileW(parentPath, NULL) )
	//		i++;

	//	return i;
	//}

	void iterator(wchar_t* parentPath)
	{
		// set up the path so that it ends with '*' for FindFile functions
		PathAppendW(parentPath, L"*");

		/*
		1. go to next path
		2. if path is file:
		  a. record the file
	    3. if path is dir:
		  b. record the director
		  c. recursively go to 1
		*/

		

	}

public:
}