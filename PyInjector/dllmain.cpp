#include <windows.h>
#include <psapi.h>
#include "SDK.h"

HMODULE g_hModule = NULL;
SDK sdk;

#if defined(MODE_EXEC_CODE_PY)
class CCodeLoader {
    char* m_code = nullptr;
    bool m_initialized = false;

public:
    CCodeLoader() {
    }

    ~CCodeLoader() {
        dispose();
    }

    void dispose() {
        if (m_code) {
            free(m_code);
            m_code = nullptr;
        }
    }

    void loadCodeFromExpectedLocations() {
        static wchar_t sCodeFile[64] = L"code.py";

        wchar_t sPath[MAX_PATH + 1 + _countof(sCodeFile)];
        DWORD len = ::GetModuleFileNameExW(::GetCurrentProcess(), g_hModule, sPath, MAX_PATH);

        while (true) {
            // Remove module extension (c:\path\module.dll -> c:\path\module)
            wchar_t* pos = &sPath[len];
            while (pos > sPath && *pos != L'\\' && *pos != L'.') {
                *(pos--) = L'\0';
            }

            // Loading code from the <module-name>.code.py file
            // Example: "c:\path\module.dll" -> "c:\path\module.code.py"
            wcscat_s(sPath, _countof(sPath), sCodeFile);
            if (load(sPath)) break;

            // Remove module filename from path (c:\path\module.dll -> c:\path\)
            *pos = L'\0';
            while (pos > sPath && *pos != L'\\') {
                *(pos--) = L'\0';
            }

            // Loading code from the same directory as the module
            // Example: "c:\path\module.dll" -> "c:\path\code.py"
            wcscat_s(sPath, _countof(sPath), sCodeFile);
            if (load(sPath)) break;

            // Loading code from the current directory of the application
            // Example: ".\code.py"
            load(sCodeFile);
            break;
        }
    }

    bool load(const wchar_t* filename) {
        bool loaded = false;
        HANDLE hFile = INVALID_HANDLE_VALUE;
        dispose();
        
        while (true) {
            hFile = ::CreateFileW(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hFile == INVALID_HANDLE_VALUE) break;

            DWORD fileSize = ::GetFileSize(hFile, NULL);
            if (fileSize == INVALID_FILE_SIZE) break;

            m_code = (char*)malloc(fileSize + 1);
            if (!m_code) break;

            DWORD bytesRead;
            if (!::ReadFile(hFile, (LPVOID)m_code, fileSize, &bytesRead, NULL)) {
                break;
            }

            m_code[fileSize] = '\0';
            loaded = true;
            break;
        }

        if (hFile != INVALID_HANDLE_VALUE) {
            ::CloseHandle(hFile);
        }
        if (!loaded) {
            dispose();
        }
        return loaded;
    }

    const char* get() {
        if (!m_initialized) {
            m_initialized = true;
            loadCodeFromExpectedLocations();
        }
        return m_code;        
    }
} g_code_loader;
#define code g_code_loader.get()
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
    PyGILState_STATE s = PyGILState_UNLOCKED;
    bool locked = false;
    while (true) {
        if (!sdk.InitCPython()) {
            ::MessageBoxW(0, L"Unable to initialize python (python3x.dll was not found)", L"Error", 0);
            break;
        }
        Py_SetProgramName(L"PyInjector");
        PyEval_InitThreads();

        s = PyGILState_Ensure();
        locked = true;
#ifdef MODE_SPAWN_PYSHELL
        // We need to access the interactive shell, so stdin, stdout, stderr must be assigned to the console
        PyRun_SimpleString(
            "import sys, io"                           "\n"
            "sys.stdin  = io.open(\"CONIN$\",  \"r\")" "\n"
            "sys.stdout = io.open(\"CONOUT$\", \"w\")" "\n"
            "sys.stderr = io.open(\"CONOUT$\", \"w\")" "\n");
#endif // MODE_SPAWN_PYSHELL

        if(!code) {
            ::MessageBoxW(0, L"Python code (code.py) was not found", L"Warning", MB_ICONWARNING);
            break;
        }

        PyRun_SimpleString(code);
        break;
    }
    if (locked) {
        PyGILState_Release(s);
    }
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
        g_hModule = hModule;
        ::CloseHandle(::CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, 0));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

