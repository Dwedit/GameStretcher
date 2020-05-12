struct IUnknown;
#define NOMINMAX
#include <Windows.h>
#include <string>
#pragma comment(lib, "Shlwapi.lib")

#include <Shlwapi.h>
using std::wstring;

#include "CommandLineArguments.h"

void ArgvQuote(const std::wstring& Argument, std::wstring& CommandLine, bool Force);
wstring ArgvQuote(const wstring& Argument);
wstring ArgvToCommandLine(LPWSTR* argv, int startIndex, int argc);

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
    }
    return L"";

}

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

wstring ArgvQuote(const wstring& Argument)
{
    wstring commandLine;
    ArgvQuote(Argument, commandLine, false);
    return commandLine;
}

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
