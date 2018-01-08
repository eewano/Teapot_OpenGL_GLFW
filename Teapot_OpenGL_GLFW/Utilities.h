#ifndef Utility_h
#define Utility_h

//カレントディレクトリの設定の際、WindowsとMacで使用するヘッダーファイルが異なる
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd

#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

void ErrorCallback(int error, const char *description) {
    std::cerr << "Error: " << description << ".\n";
}

//カレントディレクトリの設定
std::string GetCurrentWorkingDir() {
    char buff[FILENAME_MAX];
    GetCurrentDir(buff, FILENAME_MAX);
    std::string currentWorkingDir(buff);
    return currentWorkingDir;
}

#endif /* Utility_h */
