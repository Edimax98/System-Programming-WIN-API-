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

bool ListDirectory(TCHAR *spath) {
	
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

	_tcscpy(pathWithStar, spath);
	_tcscat(pathWithStar, _T("*"));
	if (INVALID_HANDLE_VALUE==(h = FindFirstFile(pathWithStar, &fileData))) {
		DWORD e = GetLastError();
		if (e != ERROR_FILE_NOT_FOUND) {
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, e, MAKELANGID
				(LANG_ENGLISH, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBup, 0, NULL);

			_tprintf((LPTSTR)(lpMsgBup));

			LocalFree(lpMsgBup);
		}
	}

	_tcscpy(path, spath);
	long long sizeOfFile = fileData.nFileSizeHigh << 32 + fileData.nFileSizeLow;

	if (h != INVALID_HANDLE_VALUE) {

			do {
				if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

					if (_tcscmp(fileData.cFileName, _T(".")) != 0 && _tcscmp(fileData.cFileName, _T("..")) != 0) {
						_tcscpy(path, spath);
						_tcscat(path, fileData.cFileName);
						_tcscat(path, _T("\\"));
						ListDirectory(path);
						_tcscpy(path, _T(""));
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

							_tcscat(nameBufer,path);
							_tcscat(nameBufer, "\\");
							_tcscat(nameBufer, fileData.cFileName);

							_tcscat(newNameBufer, path);
							_tcscat(newNameBufer, "\\");
							_tcscat(newNameBufer, TcharBufer.c_str());

							if (MoveFile(nameBufer, newNameBufer)) {
								_tprintf(_T("rename success\n"));
							}
							else {
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
		{
			DWORD e = GetLastError();
			if (e != ERROR_NO_MORE_FILES) {
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
	else {
		return false;
	}
	return true;
}

int main() {
	TCHAR path[300] = _T("C:\\test\\");

	if (ListDirectory(path)) {
		_tprintf("Successful");
	}
	else {
		_tprintf("Unsuccessful");
	}

	_getch();
	return 0;
}
