//// file path:  http://msdn.microsoft.com/en-us/library/bb773584(VS.85).aspx
//// file properties: http://msdn.microsoft.com/en-us/library/bb762179(VS.85).aspx
//// find file handle help: http://msdn.microsoft.com/en-us/library/aa364418(VS.85).aspx
//// look up the get functions under file management functions http://msdn.microsoft.com/en-us/library/aa364232(VS.85).aspx
//// search path http://msdn.microsoft.com/en-us/library/aa365527(VS.85).aspx
//// listing files in dir http://msdn.microsoft.com/en-us/library/aa365200(VS.85).aspx
//
#include <iostream>
#include <fstream>
using namespace std;

#include <string.h>

#include <windows.h>
#include <Shlwapi.h>

void pathIterator(LPWSTR);
void writeHeader(LPWSTR);
void writeFooter();
void writeFile(LPWSTR);
void writeDir(LPWSTR);

LPWSTR saveFileName = L"C:\\Users\\Karl\\Desktop\\SplitterTest.dbr";
wofstream outFile(saveFileName);

int main()
{
	//HANDLE hPath = INVALID_HANDLE_VALUE;
	//WIN32_FIND_DATAW pathData;

	//LPWSTR pathParent;
	//LPWSTR pathChild;

	//WCHAR buffer[MAX_PATH] = L"";
	//LPWSTR pathFull = buffer;


	//pathParent = L"D:\\Desktop\\Test Folder";	// choose directory
	//PathCombineW(pathFull, pathParent, L"*");	// prepare the directory for FindFile functions
	//hPath = FindFirstFileW(pathFull, &pathData);	// find the first path and put it in hPath

	//// do/while prints out each path is parentPath until there aren't any more
	//do
	//{
	//	// check to make sure it's not an invalid handle
	//	if ( hPath != INVALID_HANDLE_VALUE )	
	//	{
	//		// check to make sure it isn't just a bunch of dots
	//		if ( pathData.cFileName[0] != '.' )
	//		{
	//			wcout << pathData.cFileName << " = ";
	//			wcout << pathData.nFileSizeLow + pathData.nFileSizeHigh;

	//			if ( pathData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
	//			{
	//				PathCombineW(pathFull, pathParent, pathData.cFileName);
	//				if (PathIsDirectoryEmptyW(pathFull))
	//					wcout << " EMPTY ";
	//				wcout << " is DIR" << endl;
	//			}
	//			else
	//				wcout << " is FILE" << endl;
	//		}
	//		else
	//			wcout << "NOT A REAL DIR" << endl;
	//	}
	//	else
	//		wcout << "INVALID HANDLE" << endl;
	//}
	//while ( FindNextFile(hPath, &pathData) );

	

	LPWSTR root = L"C:\\Users\\Karl\\Music";

	if ( outFile.fail() )
	{
		cerr << "Could not create save file" << endl;
		outFile.close();
		return 1;
	}

	writeHeader(root);
	pathIterator(root);
	writeFooter();

	outFile.close();
	wcout << L"Done!";
	cin.get();
	return 0;
}

void writeHeader(LPWSTR root)
{
	outFile << "\
<DeepBurner_record ver=\"1.9.0.228\" type=\"data\">\n\
<data_cd ver=\"1\" device=\"\" session2import=\"0\" finalize_disc=\"1\" finalize_track=\"1\" bootable=\"0\" boot_image_path=\"\">\n\
<dir name=\"" << PathFindFileName(root) << " Backup" << "\" imp=\"0\">\n\
";
}

void writeFooter()
{
	outFile << "\
</dir>\n\
</data_cd>\n\
<cd_label ver=\"1\" zoom=\"0\" view=\"label\" layout=\"standart\">\n\
<label />\n\
<front />\n\
<back />\n\
</cd_label>\n\
<autorun ver=\"1\" use=\"0\">\n\
<main name=\"MainForm\" image_path=\"\" hint=\"\" />\n\
<title name=\"Title\" text=\"Title\" hint=\"Title box\" left=\"144\" top=\"48\" width=\"57\" height=\"33\" fontname=\"Times New Roman\" fontsize=\"20\" fontcolor=\"255\" visible=\"1\" fontstyle=\"0\" />\n\
<comment name=\"Comments\" text=\"Comment\" hint=\"Comment box\" left=\"40\" top=\"76\" width=\"89\" height=\"29\" fontname=\"Times New Roman\" fontsize=\"15\" fontcolor=\"255\" visible=\"1\" fontstyle=\"0\" />\n\
<exitbutton name=\"ButtonExit\" image_path=\"\" image_down_path=\"\" text=\"Exit\" hint=\"Exit this program\" left=\"120\" top=\"96\" width=\"75\" height=\"25\" fontname=\"MS Sans Serif\" fontsize=\"8\" fontcolor=\"255\" visible=\"1\" fontstyle=\"0\" />\n\
</autorun>\n\
</DeepBurner_record>\
";
}

void writeFile(LPWSTR filePath)
{
	//int numOfChars = 12 + sizeof(PathFindFileNameW(filePath)) / sizeof(PathFindFileName(filePath)[0]) + 8 + sizeof(filePath) / sizeof(filePath[0]) + 13;
	LPWSTR temp = "<file name=\"" + PathFindFileNameW(filePath) + "\" path=\"" + filePath + "\" imp=\"0\" />\n";
	outFile.write( temp, sizeof(temp)/sizeof(temp[0]) );
	//<< "\
<file name=\"" << PathFindFileNameW(filePath) << "\" path=\"" << filePath << "\" imp=\"0\" />\n\
";
}

void writeDir(LPWSTR dirPath)
{
	/*outFile << "\
<dir name=\"" << PathFindFileNameW(dirPath) << "\" imp=\"0\">\n\
";*/

	LPWSTR temp = "<dir name=\"" + PathFindFileNameW(dirPath) + "\" imp=\"0\">\n";
	outFile.write( temp, sizeof(temp)/sizeof(temp[0]) );
}

void pathIterator(LPWSTR root)
{
	HANDLE hPath = INVALID_HANDLE_VALUE;	// holds file handle
	WIN32_FIND_DATAW pathData;				// data retrieved from file handle
	WCHAR path[MAX_PATH];

	// find and list all FILES in the rootPath
	PathCombineW(path, root, L"*");	  // make path ready for FindFile
	hPath = FindFirstFileW(path, &pathData);

	do
	{
		if ( !(pathData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // if it is not a directory
			&& hPath != INVALID_HANDLE_VALUE ) // and if it's not an invalid handle
		{
			//----
			// apply filters here
			// ex. exclude all .jpg files or files of certain size or include the latter and former only
			//----
			
			PathCombineW(path, root, pathData.cFileName);
			writeFile(path);
		}
	}
	while ( FindNextFileW(hPath, &pathData) );


	// find and list all DIRECTORIES in rootPath
	PathCombineW(path, root, L"*");  // make path ready for FindFile
	hPath = FindFirstFileW(path, &pathData);

	do
	{
		if ( (pathData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // if it is a dir
			&& hPath != INVALID_HANDLE_VALUE) // and if it's not an invalid handle
		{
			if ( pathData.cFileName[0] != '.' ) // if the path name doesn't start with a '.'
				
			{
				//----
				// apply filters here
				// ex: exlude system folders, empty folders, etc
				//----
				
				// output folder path
				
				PathCombineW(path, root, pathData.cFileName);
				writeDir(path);

				// dive into the selected folder
				PathCombineW(path, root, pathData.cFileName);
				pathIterator(path);

				outFile << "</dir>" << endl;
			}
		}
	}
	while ( FindNextFileW(hPath, &pathData) );

	FindClose(hPath);
}
//
//	// findfirstfile and findnextfile are only specific to a single specified file name in a folder
//
//	// root = current top folder being looked in
//	// path = holds the value for the path/file currently selected in the search
//	// fullPath = combines root and path
//
//	wchar_t* root;
//	wchar_t* path = L"*";
//
//	wchar_t buffer[MAX_PATH] = L"";
//	wchar_t* fullPath = buffer;
//
//	HANDLE hFile;
//	WIN32_FIND_DATAW FindFileData;
//	
//	root = L"D:";
//	PathCombineW(fullPath, root, path);
//	hFile = FindFirstFileW( fullPath, &FindFileData);
//	
//	path = FindFileData.cFileName;
//
//
//	PathCombineW(fullPath, root, path);
//	wcout << "Path/File Exists = " << PathFileExistsW(fullPath) << endl;
//	wcout << "Path is directory = " << PathIsDirectoryW(fullPath) << endl;
//	wcout << "Path is empty dir = " << PathIsDirectoryEmptyW(fullPath) << endl;
//	wcout << "Path is system folder = " << PathIsSystemFolderW(fullPath, NULL) << endl; // doesn't matter if using PathFindSuffixArray
//	wcout << "Path extension = " << PathFindExtension(fullPath) << endl;
//	// PathFindNextComponent lets you cycle through all parts of a path as seperate things
//	// PathAppend(root, path) puts path at the end of root!
//	// PathFindSuffixArray(fullPath or filename, array of string pointers containing things like ".txt" ".bmp" etc, the number of elements in that array). returns null if it's not one. returns what it is if it is one.
//
//	int a = 1;
//
//
//	//WIN32_FIND_DATA FindFileData;
//	//HANDLE hFile;
//	//hFile = FindFirstFileW(validPath, &FindFileData);
//	//wcout << FindFileData.cFileName << endl;
//	//while ( FindNextFileW(hFile, &FindFileData) )
//	//{
//	//	wcout << FindFileData.cFileName << endl;
//	//	wcout << "Size = " << FindFileData.nFileSizeLow << endl;
//
//
//	//}
//
//
//	//
//	//
//
//	//
//
//	//GetFinalPathNameByHandleW(hFile, validPath, 4, 0);
//
//	//if (GetLastError() == ERROR_PATH_NOT_FOUND)
//	//	wcout << "ERROR_PATH_NOT_FOUND";
//	//else if
//	//	(GetLastError() == ERROR_NOT_ENOUGH_MEMORY)
//	//	wcout << "ERROR_NOT_ENOUGH_MEMORY";
//	//else if
//	//	(GetLastError() == ERROR_INVALID_PARAMETER)
//	//	wcout << "ERROR_INVALID_PARAMETER";
//
//	//wcout << endl;
//	//wcout << validPath;
//
//	cin.get();
//	return 0;
//}

