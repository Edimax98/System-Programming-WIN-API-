#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <conio.h>
#include <vector>
using namespace std;
LPVOID lpMsgBup;

void reverseTchar(TCHAR* str) {
	const size_t len = _tcslen(str);
	for (size_t i = 0; i < len / 2; i++) {
		swap(str[i], str[len - i - 1]);
	}
}

int main() {

	WIN32_FIND_DATA fileData;
	HANDLE h = FindFirstFile(_T("C:\\Users\\Даниил\\Documents\\Visual Studio 2015\\Projects\\dollar\\file\\*"), &fileData);
	int j = 0;
	TCHAR path[300] = _T("C:\\Users\\Даниил\\Documents\\Visual Studio 2015\\Projects\\dollar\\file\\");
	TCHAR nameOfFile[255] = _T("");
	TCHAR newNameOFFile[255] = _T("");
	TCHAR size;
	TCHAR str[255] = _T("");
	TCHAR bufer[255] = _T("");
	int n = 255;
	std::basic_string<TCHAR> TcharBufer;
	_tcscat_s(path, fileData.cFileName);

	long long sizeOfFile = fileData.nFileSizeHigh << 32 + fileData.nFileSizeLow;

	if (h != INVALID_HANDLE_VALUE) {

		if (!FindNextFile(h, &fileData)) {
			DWORD e = GetLastError();
			if (e == ERROR_NO_MORE_FILES) {
				FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, e, MAKELANGID
					(LANG_ENGLISH, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBup, 0, NULL);

				_tprintf((LPTSTR)(lpMsgBup));

				LocalFree(lpMsgBup);
			}
		}

		do {
			_tcscpy(nameOfFile, fileData.cFileName);
			if (!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DEVICE) &&
				!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

				for (int i = 0; i < _tcslen(nameOfFile); i++) {
					j = 0;
					_tcscpy(newNameOFFile, _T(""));
					TcharBufer = fileData.cFileName;
					
					if (nameOfFile[i] == _T('$')) {
						int b = 0;
						long long a = (((long long)fileData.nFileSizeHigh) << 32) + fileData.nFileSizeLow;

						while (a > 0) {
							b = a % 10;
							size = b + _T('0');
							newNameOFFile[j++] = size;
							a /= 10;
						}
						if (sizeOfFile == 0) {
							newNameOFFile[j] = '0';
						}

						newNameOFFile[j] = 0;
						_tcscpy(bufer, newNameOFFile);
						reverseTchar(bufer);
				
						TcharBufer.erase(TcharBufer.begin() + i, TcharBufer.begin() + i + 1);
						TcharBufer.insert(i, bufer);

					}
					_tprintf(bufer);
				}
				
				if (MoveFile(path, TcharBufer.c_str())) {
					_tprintf(_T("rename success"));
				} else {
					DWORD e = GetLastError();
					_tprintf(_T("rename error"));
					FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
						FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, e, MAKELANGID
						(LANG_ENGLISH, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBup, 0, NULL);

					_tprintf((LPTSTR)(lpMsgBup));

					LocalFree(lpMsgBup);
				}
			}
		} while (FindNextFile(h, &fileData));

		if (!FindNextFile(h, &fileData)) {
			DWORD e = GetLastError();
			
			if (e == ERROR_NO_MORE_FILES) {
				FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, e, MAKELANGID
					(LANG_ENGLISH, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBup, 0, NULL);

				_tprintf((LPTSTR)(lpMsgBup));
				LocalFree(lpMsgBup);
			}
		}

		if (!FindClose(h)) {
			DWORD e = GetLastError();
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, e, MAKELANGID
				(LANG_ENGLISH, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBup, 0, NULL);

			_tprintf((LPTSTR)(lpMsgBup));

			LocalFree(lpMsgBup);
		}
	}
	_getch();
	return 0;
}
