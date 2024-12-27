struct IUnknown;
#define NOMINMAX
#include <Windows.h>
#include <string>
#pragma comment(lib, "Shlwapi.lib")

#include <Shlwapi.h>
using std::wstring;

#include "CommandLineArguments.h"

wstring ArgvQuote(const wstring& Argument);

void CommandLineData::BuildFullCommandLine()
{
    if (!targetExe.empty())
    {
        targetFullCommandLine += ArgvQuote(targetExe);
        for (int i = 0; i < targetParameters.size(); i++)
        {
            targetFullCommandLine += L" ";
            targetFullCommandLine += ArgvQuote(targetParameters[i]);
        }
    }
}

BOOL CALLBACK WndEnumProc(HWND hwnd, LPARAM lParam)
{
    vector<HWND>* vec = (vector<HWND>*)lParam;
    vec->push_back(hwnd);
    return true;
}

vector<HWND> GetChildWindows(HWND hwnd)
{
    vector<HWND> vec;
    EnumChildWindows(hwnd, WndEnumProc, (LPARAM)&vec);
    return vec;
}

//wstring GetParentExplorerDirectory()
//{
//    HWND foregroundWindow = GetForegroundWindow();
//    wchar_t buffer[256];
//    GetClassNameW(foregroundWindow, buffer, 256);
//    if (0 == wcscmp(buffer, L"CabinetWClass") || 0 == wcscmp(buffer, L"ExplorerWClass"))
//    {
//        auto vec = GetChildWindows(foregroundWindow);
//        for (int i = 0; i < vec.size(); i++)
//        {
//            GetClassNameW(vec[i], buffer, 256);
//            if (0 == wcscmp(buffer, L"Edit") || 0 == wcscmp(buffer, L"ToolbarWindow32"))
//            {
//                GetWindowTextW(vec[i], buffer, 256);
//                wchar_t* found = NULL;
//                found = wcsstr(buffer, L":\\");
//                if (found)
//                {
//                    int charPos = found - buffer;
//                    if (charPos > 0)
//                    {
//                        wchar_t* path = buffer + charPos - 1;
//                        if (PathFileExistsW(path))
//                        {
//                            wchar_t buffer2[MAX_PATH];
//                            PathCanonicalizeW(buffer2, path);
//                            return wstring(buffer2);
//                        }
//                    }
//                }
//            }
//        }
//    }
//    return wstring();
//}

wstring GetCurrentDirectory()
{
    wstring currentDirectory;
    int length = GetCurrentDirectoryW(0, NULL);
    currentDirectory.resize(length + 1);
    GetCurrentDirectoryW(length, &currentDirectory[0]);
    currentDirectory.resize(length - 1);
    return currentDirectory;
}

wstring GetDirectory(LPCWSTR path)
{
    if (PathIsDirectoryW(path))
    {
        return path;
    }
    wstring str = path;
    for (int i = str.length() - 1; i >= 0; i--)
    {
        if (str[i] == '/' || str[i] == '\\')
        {
            str.resize(i + 1);
            if (PathIsDirectoryW(str.c_str()))
            {
                return str;
            }
        }
    }
    return wstring();
}

wstring GetFile(LPCWSTR path)
{
    if (PathFileExistsW(path) && !PathIsDirectoryW(path))
    {
        return path;
    }
    wstring str = path;
    for (int i = str.length() - 1; i >= 0; i--)
    {
        if (str[i] == ' ')
        {
            str.resize(i);
            if (PathFileExistsW(str.c_str()) && !PathIsDirectoryW(str.c_str()))
            {
                return str;
            }
        }
    }
    return wstring();
}

wstring GetDirectory(const wstring& path)
{
    return GetDirectory(path.c_str());
}

wstring GetFile(const wstring& path)
{
    return GetFile(path.c_str());
}

wstring AppendSlash(const wstring& path)
{
    int lastChar = 0;
    if (path.size() >= 1)
    {
        lastChar = path[path.size() - 1];
    }
    if (lastChar == '/' || lastChar == '\\') return path;
    return path + L'\\';
}

wstring LocateFile2(const wstring& path_in, const CommandLineData& cmd)
{
    if (!PathIsRelativeW(path_in.c_str())) return GetFile(path_in);
    if (!cmd.launcherDirectory.empty())
    {
        wstring path = GetFile(AppendSlash(cmd.launcherDirectory) + path_in);
        if (!path.empty()) return path;
    }
    if (!cmd.targetDirectory.empty())
    {
        wstring path = GetFile(AppendSlash(cmd.targetDirectory) + path_in);
        if (!path.empty()) return path;
    }
    if (!cmd.currentDirectory.empty())
    {
        wstring path = GetFile(AppendSlash(cmd.currentDirectory) + path_in);
        if (!path.empty()) return path;
    }
    if (!cmd.startupDirectory.empty())
    {
        wstring path = GetFile(AppendSlash(cmd.startupDirectory) + path_in);
        if (!path.empty()) return path;
    }
    return wstring();
}
wstring LocateFile(const wstring& path_in, const CommandLineData& cmd)
{
    if (!PathIsRelativeW(path_in.c_str())) return GetFile(path_in);
    if (!cmd.currentDirectory.empty())
    {
        wstring path = GetFile(AppendSlash(cmd.currentDirectory) + path_in);
        if (!path.empty()) return path;
    }
    if (!cmd.launcherDirectory.empty())
    {
        wstring path = GetFile(AppendSlash(cmd.launcherDirectory) + path_in);
        if (!path.empty()) return path;
    }
    if (!cmd.targetDirectory.empty())
    {
        wstring path = GetFile(AppendSlash(cmd.targetDirectory) + path_in);
        if (!path.empty()) return path;
    }
    if (!cmd.startupDirectory.empty())
    {
        wstring path = GetFile(AppendSlash(cmd.startupDirectory) + path_in);
        if (!path.empty()) return path;
    }
    return wstring();
}

int EatWhiteSpace(LPCWSTR str, int startIndex)
{
    int i = startIndex;
    while (str[i] == ' ' || str[i] == '\t')
    {
        i++;
    }
    return i;
}

CommandLinePiece ParseCommandLinePiece(LPCWSTR lpCmdLine, int& i, int stringLength)
{
    CommandLinePiece piece = CommandLinePiece();
    wstring& result = piece.str;
    if (i < 0) i = 0;
    if (i >= stringLength) return piece;

    //first, eat white space
    i = EatWhiteSpace(lpCmdLine, i);
    piece.startIndex = i;
    //
    int backslashCount = 0;
    bool insideQuote = false;
    while (true)
    {
        wchar_t c = lpCmdLine[i];
        if (c == '\0')
        {
            break;
        }
        if (c == '"')
        {
            piece.containsQuotes = true;
            if (backslashCount == 1)
            {
                result.push_back('"');
                backslashCount = 0;
            }
            else
            {
                insideQuote = !insideQuote;
            }
        }
        else if (c == '\\')
        {
            backslashCount++;
            if (backslashCount == 2)
            {
                result.push_back('\\');
                backslashCount = 0;
            }
        }
        else
        {
            if (backslashCount > 0)
            {
                result.push_back('\\');
            }
            if (!insideQuote && (c == ' ' || c == '\t'))
            {
                break;
            }
            result.push_back(c);
            backslashCount = 0;
        }
        i++;
    }
    piece.endIndex = i;
    i = EatWhiteSpace(lpCmdLine, i);
    if (piece.containsQuotes)
    {
        if (lpCmdLine[piece.startIndex] == '"' && lpCmdLine[piece.endIndex - 1] == '"')
        {
            piece.isQuoted = true;
        }
    }
    return piece;
}

bool StringStartsWith(const wstring& str, const wstring& lookFor)
{
    if (lookFor.length() > str.length()) return false;
    for (int i = 0; i < lookFor.length(); i++)
    {
        if (str[i] != lookFor[i]) return false;
    }
    return true;
}

wstring GetStartupDirectory()
{
    STARTUPINFOW startupInfo = {};
    startupInfo.cb = sizeof(startupInfo);
    GetStartupInfoW(&startupInfo);
    return GetDirectory(startupInfo.lpTitle);
}

CommandLineData ParseCommandLine(LPWSTR lpCmdLine)
{
    int stringLength = wcslen(lpCmdLine);
    CommandLineData cmd;
    cmd.startupDirectory = GetStartupDirectory();
    cmd.currentDirectory = GetCurrentDirectory();
    //int argc;
    //LPWSTR* argv;
    //argv = CommandLineToArgvW(lpCmdLine, &argc);

    vector<CommandLinePiece> pieces;
    int textIndex = 0;
    while (textIndex < stringLength)
    {
        pieces.emplace_back(ParseCommandLinePiece(lpCmdLine, textIndex, stringLength));
    }

    cmd.launcherExe = GetFile(pieces[0].str);
    cmd.launcherDirectory = GetDirectory(cmd.launcherExe);


    for (int pieceNumber = 1; pieceNumber < pieces.size(); pieceNumber++)
    {
        const CommandLinePiece& piece = pieces[pieceNumber];
        if (cmd.targetExe.empty())
        {
            if (StringStartsWith(piece.str, L"--"))
            {
                bool wantArgument = false;
                cmd.switches.push_back(piece.str);
                if (piece.str == L"--attach")
                {
                    wantArgument = true;
                }

                if (wantArgument)
                {
                    pieceNumber++;
                    if (pieceNumber < pieces.size())
                    {
                        const CommandLinePiece& piece2 = pieces[pieceNumber];
                        cmd.targetExe = piece2.str;
                    }
                }
            }
            else
            {
                cmd.targetExe = LocateFile(piece.str, cmd);
                cmd.targetDirectory = GetDirectory(cmd.targetExe);
                if (cmd.targetExe.empty() && !piece.isQuoted)
                {
                    int startIndex = piece.startIndex;
                    //re-parse command line using other rules
                    for (int i = pieceNumber; i < pieces.size(); i++)
                    {
                        int endIndex = pieces[i].endIndex;
                        wstring exeName = wstring(lpCmdLine + startIndex, endIndex - startIndex);
                        cmd.targetExe = LocateFile(exeName, cmd);
                        cmd.targetDirectory = GetDirectory(cmd.targetExe);
                        if (!cmd.targetExe.empty())
                        {
                            CommandLinePiece newPiece = CommandLinePiece();
                            newPiece.startIndex = startIndex;
                            newPiece.endIndex = endIndex;
                            newPiece.str = exeName;

                            pieces[pieceNumber] = newPiece;
                            pieces.erase(pieces.begin() + pieceNumber + 1, pieces.begin() + i + 1);
                            break;
                        }
                    }
                }
            }
        }
        else
        {
            cmd.targetParameters.push_back(piece.str);
        }
    }
    cmd.BuildFullCommandLine();
    return cmd;
}

CommandLineData ParseCommandLine()
{
    LPWSTR lpCmdLine = GetCommandLineW();
    return ParseCommandLine(lpCmdLine);
}


void ArgvQuote(const std::wstring& Argument, std::wstring& CommandLine, bool Force);
wstring ArgvQuote(const wstring& Argument);
//wstring ArgvToCommandLine(LPWSTR* argv, int startIndex, int argc);

/*
wstring FindInitialDirectory(const wstring& commandLine)
{
    if (PathFileExistsW(commandLine.c_str()))
    {
        LPCWSTR arg1 = commandLine.c_str();
        //look for last '/' or '\'
        int len = (int)wcslen(arg1);
        int i;
        for (i = len - 1; i >= 0; i--)
        {
            if (arg1[i] == '/' || arg1[i] == '\\')
            {
                return wstring(&arg1[0], &arg1[i + 1]);
            }
        }
    }
    return FindInitialDirectory();
}
wstring FindInitialDirectory()
{
    int argc;
    LPWSTR* argv;
    LPWSTR lpCmdLine = GetCommandLineW();

    argv = CommandLineToArgvW(lpCmdLine, &argc);
    if (argc <= 1)
    {
        return L"";
    }
    else
    {
        LPWSTR arg1 = argv[1];
        //look for last '/' or '\'
        int len = (int)wcslen(arg1);
        int i;
        for (i = len - 1; i >= 0; i--)
        {
            if (arg1[i] == '/' || arg1[i] == '\\')
            {
                return wstring(&arg1[0], &arg1[i + 1]);
            }
        }

        //try Launcher.exe directory next
        arg1 = argv[0];
        //look for last '/' or '\'
        len = (int)wcslen(arg1);
        for (i = len - 1; i >= 0; i--)
        {
            if (arg1[i] == '/' || arg1[i] == '\\')
            {
                return wstring(&arg1[0], &arg1[i + 1]);
            }
        }

    }
    return L"";

}
*/
/*
wstring FindLauncherDirectory()
{
    int argc;
    LPWSTR* argv;
    LPWSTR lpCmdLine = GetCommandLineW();
    argv = CommandLineToArgvW(lpCmdLine, &argc);
    if (argc <= 1)
    {
        return L"";
    }
    else
    {
        LPWSTR arg1 = argv[0];
        //look for last '/' or '\'
        int len = (int)wcslen(arg1);
        int i;
        for (i = len - 1; i >= 0; i--)
        {
            if (arg1[i] == '/' || arg1[i] == '\\')
            {
                return wstring(&arg1[0], &arg1[i + 1]);
            }
        }
    }
    return L"";

}
*/
/*
wstring GetProcessCommandLine()
{
    int argc;
    LPWSTR* argv;
    LPWSTR lpCmdLine = GetCommandLineW();
    argv = CommandLineToArgvW(lpCmdLine, &argc);
    
    wstring commandLine;

    if (argc <= 1)
    {

    }
    else
    {
        commandLine = ArgvToCommandLine(argv, 1, argc);
    }
    LocalFree(argv);
    return commandLine;
}
*/

wstring ArgvQuote(const wstring& Argument)
{
    wstring commandLine;
    ArgvQuote(Argument, commandLine, false);
    return commandLine;
}
/*
wstring ArgvToCommandLine(LPWSTR* argv, int startIndex, int argc)
{
    wstring commandLine;
    commandLine.reserve(MAX_PATH);
    for (int i = startIndex; i < argc; i++)
    {
        ArgvQuote(argv[i], commandLine, false);
        if (i + 1 < argc)
        {
            commandLine.append(L" ");
        }
    }
    return commandLine;
}
*/

//https://blogs.msdn.microsoft.com/twistylittlepassagesallalike/2011/04/23/everyone-quotes-command-line-arguments-the-wrong-way/


void
ArgvQuote(
    const std::wstring& Argument,
    std::wstring& CommandLine,
    bool Force
)

/*++

Routine Description:

    This routine appends the given argument to a command line such
    that CommandLineToArgvW will return the argument string unchanged.
    Arguments in a command line should be separated by spaces; this
    function does not add these spaces.

Arguments:

    Argument - Supplies the argument to encode.

    CommandLine - Supplies the command line to which we append the encoded argument string.

    Force - Supplies an indication of whether we should quote
            the argument even if it does not contain any characters that would
            ordinarily require quoting.

Return Value:

    None.

Environment:

    Arbitrary.

--*/

{
    //
    // Unless we're told otherwise, don't quote unless we actually
    // need to do so --- hopefully avoid problems if programs won't
    // parse quotes properly
    //

    if (Force == false &&
        Argument.empty() == false &&
        Argument.find_first_of(L" \t\n\v\"") == Argument.npos)
    {
        CommandLine.append(Argument);
    }
    else {
        CommandLine.push_back(L'"');

        for (auto It = Argument.begin(); ; ++It) {
            size_t NumberBackslashes = 0;

            while (It != Argument.end() && *It == L'\\') {
                ++It;
                ++NumberBackslashes;
            }

            if (It == Argument.end()) {

                //
                // Escape all backslashes, but let the terminating
                // double quotation mark we add below be interpreted
                // as a metacharacter.
                //

                CommandLine.append(NumberBackslashes * 2, L'\\');
                break;
            }
            else if (*It == L'"') {

                //
                // Escape all backslashes and the following
                // double quotation mark.
                //

                CommandLine.append(NumberBackslashes * 2 + 1, L'\\');
                CommandLine.push_back(*It);
            }
            else {

                //
                // Backslashes aren't special here.
                //

                CommandLine.append(NumberBackslashes, L'\\');
                CommandLine.push_back(*It);
            }
        }

        CommandLine.push_back(L'"');
    }
}
