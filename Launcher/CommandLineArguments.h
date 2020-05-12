#pragma once

#include <string>
using std::wstring;
wstring GetProcessCommandLine();
wstring FindInitialDirectory(const wstring& commandLine);
wstring FindInitialDirectory();
wstring FindLauncherDirectory();
