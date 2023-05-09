#include "SDK.h"

SDK sdk;

#if defined(MODE_EXEC_CODE_PY)
static const char code[1024] =
    "\n\n\n"
    "# -----------------------------------------------\n"
    "# CODE BLOCK BEGIN (you can modify it right here)\n"
    "import os"                               "\n"
    "with open(\"code.py\",\"r\") as file:"   "\n"
    "   data = file.read()"                   "\n"
    "exec(data)"                              "\n"
    "                                          \n"
    "                                          \n"
    "                                          \n"
    "                                          \n"
    "                                          \n"
    "                                          \n"
    "                                          \n"
    "                                          \n"
    "                                          \n"
    "                                          \n"
    "                                          \n"
    "                                          \n"
    "                                          \n"
    "                                          \n"
    "                                          \n"
    "                                          \n"
    "                                          \n"
    "                                          \n"
    "# CODE BLOCK END\n"
    "# -----------------------------------------------\n\n\n";
#elif defined(MODE_SPAWN_PYSHELL)
static const char code[] = "\n\n\n"
    "import traceback                                 " "\r\n"
    "import sys                                       " "\r\n"
    "                                                 " "\r\n"
    "while(True):                                     " "\r\n"
    "  s = input('pyshell >>> ')                      " "\r\n"
    "  cs = s                                         " "\r\n"
    "  while(cs.endswith(':') or cs.startswith(' ')): " "\r\n"
    "    cs = input('pyshell ... ')                   " "\r\n"
    "    s += '\\n' + cs                              " "\r\n"
    "  if(not s.strip()): continue                    " "\r\n"
    "  try:                                           " "\r\n"
    "    code = compile(s,'<string>','single')        " "\r\n"
    "    eval(code)                                   " "\r\n"
    "  except:                                        " "\r\n"
    "    traceback.print_exception(*sys.exc_info())   " "\r\n\n\n";
#else
#error "Please, define MODE_XXX macro or write python code to inject in the 'code' variable"
//static const char code[] = "python code text";
#endif 

void run_python_code()
{
    if (!sdk.InitCPython()) {
        ::MessageBoxW(0, L"Unable to initialize python (python3x.dll was not found)", L"Error", 0);
    }
    Py_SetProgramName(L"PyInjector");
    PyEval_InitThreads();

    PyGILState_STATE s = PyGILState_Ensure();
#ifdef MODE_SPAWN_PYSHELL
    // We need to access the interactive shell, so stdin, stdout, stderr must be assigned to the console
    PyRun_SimpleString(
        "import sys, io"                                                       "\n"
        "sys.stdin  = io.open(\"CONIN$\",  \"r\")" "\n"
        "sys.stdout = io.open(\"CONOUT$\", \"w\")" "\n"
        "sys.stderr = io.open(\"CONOUT$\", \"w\")" "\n");
#endif // MODE_SPAWN_PYSHELL

    PyRun_SimpleString(code);
    PyGILState_Release(s);
}

#ifdef MODE_SPAWN_PYSHELL
bool show_hidden_console_window()
{
    ::AllocConsole(); // Allocate console if there is no console
    HWND hWnd = ::GetConsoleWindow();
    
    if (!hWnd)
    {
        // The second way to allocate console
        ::FreeConsole();
        ::AllocConsole();
        hWnd = ::GetConsoleWindow();
    }

    if(!hWnd)
    {
        // The third way to allocate console
        char command[] = "cmd.exe";

        PROCESS_INFORMATION pi = {};
        STARTUPINFOA si = {};
        si.cb = sizeof(si);
        si.wShowWindow = SW_SHOWNORMAL;
        si.dwFlags = STARTF_USESHOWWINDOW;

        BOOL success = CreateProcessA(
            NULL,               // absolute path to the application
            command,            // command line 
            NULL,               // process security attributes 
            NULL,               // primary thread security attributes 
            FALSE,              // handles are inherited 
            CREATE_NEW_CONSOLE, // creation flags 
            NULL,               // use parent's environment 
            NULL,               // use parent's current directory 
            &si,                // STARTUPINFO pointer 
            &pi);               // receives PROCESS_INFORMATION
        if (success) 
        {
            ::WaitForSingleObject(pi.hProcess, 1000);
            ::AttachConsole(pi.dwProcessId);
            hWnd = ::GetConsoleWindow();
            ::TerminateProcess(pi.hProcess, 0);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }
    if (hWnd) {
        SetConsoleTitleA("PyShell");
        ::ShowWindow(hWnd, 4);
        return true;
    }
    else {
        ::MessageBoxW(0, L"Unable to attach console", L"Error", 0);
        return false;
    }
}
#else
#define show_hidden_console_window(...) 1
#endif

DWORD WINAPI MainThread(HMODULE hModule)
{
    do
    {
        if (!show_hidden_console_window()) {
#ifdef MODE_SPAWN_PYSHELL
            break;
#endif
        }
        run_python_code();
        break;
    } while (true);
    FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        ::CloseHandle(::CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, 0));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

