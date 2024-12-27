#pragma once

#include <string>
#include <vector>
using std::wstring;
using std::vector;

wstring ArgvQuote(const wstring& Argument);

struct CommandLineData
{
    wstring launcherExe;
    wstring launcherDirectory;
    wstring startupDirectory;
    wstring currentDirectory;
    wstring targetExe;
    wstring targetDirectory;
    vector<wstring> targetParameters;
    wstring targetFullCommandLine;
    vector<wstring> switches;


    void BuildFullCommandLine();
};

struct CommandLinePiece
{
    wstring str;
    int startIndex;
    int endIndex;
    bool containsQuotes;
    bool isQuoted;
};

wstring LocateFile(const wstring& path_in, const CommandLineData& cmd);
wstring LocateFile2(const wstring& path_in, const CommandLineData& cmd);
CommandLineData ParseCommandLine();
wstring AppendSlash(const wstring& path);
wstring GetDirectory(const wstring& path);
wstring GetFile(const wstring& path);
