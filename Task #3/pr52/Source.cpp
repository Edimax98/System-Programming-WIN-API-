#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <conio.h>
#include <vector>
#include <List>
using namespace std;
LPVOID lpMsgBup;

void reverseTchar(TCHAR* str) {
	
	const size_t len = _tcslen(str);
	for (size_t i = 0; i < len / 2; i++) {
		swap(str[i], str[len - i - 1]);
	}
}

void showLastError() {
	
	DWORD e = GetLastError();
	if (e != ERROR_FILE_NOT_FOUND) {
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, e, MAKELANGID
			(LANG_ENGLISH, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBup, 0, NULL);

		_tprintf((LPTSTR)(lpMsgBup));
		LocalFree(lpMsgBup);
	}
}

DWORD WINAPI myThread(LPVOID t) {

	WIN32_FIND_DATA fileData;
	TCHAR pathWithStar[300] = _T("");
	TCHAR nameBufer[300] = _T("");
	TCHAR newNameBufer[300] = _T("");
	HANDLE h;

	int j = 0;
	TCHAR path[500] = _T("");
	TCHAR nameOfFile[255] = _T("");
	TCHAR newNameOFFile[255] = _T("");
	TCHAR size;
	TCHAR str[255] = _T("");
	TCHAR bufer[255] = _T("");
	int n = 255;
	std::basic_string<TCHAR> TcharBufer;

	list<HANDLE> listOfThreads;
	vector<TCHAR*> vectorOfpaths;

	HANDLE hThread;

	_tcscpy(pathWithStar, (TCHAR*)t);
	_tcscat(pathWithStar, _T("*"));
	if (INVALID_HANDLE_VALUE == (h = FindFirstFile(pathWithStar, &fileData))) {
		showLastError();
		return 0;
	}

	_tcscpy(path, (TCHAR*)t);
	long long sizeOfFile = fileData.nFileSizeHigh << 32 + fileData.nFileSizeLow;

	if (h != INVALID_HANDLE_VALUE) {
		do {
			if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

				if (_tcscmp(fileData.cFileName, _T(".")) != 0 && _tcscmp(fileData.cFileName, _T("..")) != 0) {
					
					TCHAR *newPath = new TCHAR[300];

					if (newPath != NULL) {
						_tcscpy(newPath, (TCHAR*)t);
						_tcscat(newPath, fileData.cFileName);
						_tcscat(newPath, _T("\\"));

						hThread = CreateThread(NULL, 0, &myThread, newPath, 0, NULL);
						if (hThread == NULL) {
							showLastError();
							return 0;
						}

						listOfThreads.push_back(hThread);
						vectorOfpaths.push_back(newPath);
					}
				}
			}
			
			_tcscpy(nameOfFile, fileData.cFileName);
			
			if (!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DEVICE) &&
				!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

				TcharBufer = fileData.cFileName;

				for (int i = 0; i < TcharBufer.length(); i++) {
					j = 0;
					_tcscpy(newNameOFFile, _T(""));
					if (TcharBufer[i] == _T('$')) {
						int b = 0;
						long long a = (((long long)fileData.nFileSizeHigh) << 32) + fileData.nFileSizeLow;

						if (sizeOfFile == 0) {
							newNameOFFile[j] = _T('0');
						}

						while (a > 0) {
							b = a % 10;
							size = b + _T('0');
							newNameOFFile[j++] = size;
							a /= 10;
						}

						newNameOFFile[++j] = 0;
						_tcscpy(bufer, newNameOFFile);

						reverseTchar(bufer);
			
						TcharBufer.erase(TcharBufer.begin() + i, TcharBufer.begin() + i + 1);
						TcharBufer.insert(i, bufer);
					}
					_tprintf(bufer);
				}

				_tcscat(nameBufer, path);
				_tcscat(nameBufer, _T("\\"));
				_tcscat(nameBufer, fileData.cFileName);

				_tcscat(newNameBufer, path);
				_tcscat(newNameBufer, _T("\\"));
				_tcscat(newNameBufer, TcharBufer.c_str());

				if (MoveFile(nameBufer, newNameBufer)) {
					_tprintf(_T("rename success\n"));
				}
				else {
					showLastError();
				}
			}
		} while (FindNextFile(h, &fileData));
		showLastError();
	}

	if (!FindClose(h)) showLastError();
	
	if (!listOfThreads.empty()) {
		for (auto i = listOfThreads.begin(); i != listOfThreads.end(); ++i) {
			if (WaitForSingleObject(*i, INFINITE) == WAIT_FAILED) {
				showLastError();
			} else {
				_tprintf(_T("OK"));
			}
		}
	}

	if (!vectorOfpaths.empty())  {
		for (int i = 0; i < vectorOfpaths.size; ++i) {
			delete vectorOfpaths[i];
		}
	}

	if (CloseHandle(hThread)) {
		_tprintf(_T("OK"));
	} else {
		showLastError();
	}

	return 0;
}


int main() {
	TCHAR path[300] = _T("C:\\test\\");
	HANDLE hThread;
	
	myThread(path);

	_getch();
	return 0;
}
