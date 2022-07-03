#include <windows.h>
#pragma comment(lib, "Winmm.lib")
#include <iostream>

void playSound(std::string str) {
    std::wstring stemp = std::wstring(str.begin(), str.end());
    LPCWSTR sw = stemp.c_str();
    PlaySound(sw, 0, SND_FILENAME | SND_ASYNC);
}
void playSoundSyn(std::string str) {
    std::wstring stemp = std::wstring(str.begin(), str.end());
    LPCWSTR sw = stemp.c_str();
    PlaySound(sw, 0, SND_FILENAME | SND_SYNC);
}