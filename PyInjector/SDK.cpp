#include "SDK.h"
#include <memory>
#include <functional>
#define PSAPI_VERSION 1
#include <psapi.h>
#pragma comment(lib, "psapi.lib")

_Py_SetProgramName Py_SetProgramName;
_PyEval_InitThreads PyEval_InitThreads;
_PyGILState_Ensure PyGILState_Ensure;
_PyGILState_Release PyGILState_Release;
_PyRun_SimpleStringFlags PyRun_SimpleStringFlags;

namespace 
{
    void ForEachLoadedDLL(std::function<bool(HMODULE hMod, const wchar_t* sModFilename, size_t sModFilenameLen)> callback)
    {
        DWORD cbNeeded = 0;
        HMODULE hMods[1024 * 16];
        wchar_t sModFilename[MAX_PATH + 1];
        HANDLE hProcess = ::GetCurrentProcess();
        if (::EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
            for (size_t i = 0; i < (cbNeeded / sizeof(HMODULE)); ++i) {
                HMODULE hMod = hMods[i];

                DWORD dwReturnedLen = ::GetModuleFileNameExW(hProcess, hMod, sModFilename, MAX_PATH);
                sModFilename[dwReturnedLen] = L'\0';
                const wchar_t* sModName = sModFilename;
                for (size_t i = 0; i < dwReturnedLen; ++i) {
                    if (sModFilename[i] == L'\\') {
                        sModName = sModFilename + i + 1;
                    }
                }
                if (!callback(hMod, sModName, &sModFilename[dwReturnedLen] - sModName)) {
                    break;
                }
            }
        }
    }
};

bool SDK::InitCPython()
{
    bool loaded = false;
    auto fnTryToLoad = [&loaded](HMODULE hPython, const wchar_t* sModFilename, size_t sModFilenameLen) -> bool {
        loaded = 
            NULL != (Py_SetProgramName = (_Py_SetProgramName)(GetProcAddress(hPython, "Py_SetProgramName"))) &&
            NULL != (PyEval_InitThreads = (_PyEval_InitThreads)(GetProcAddress(hPython, "PyEval_InitThreads"))) &&
            NULL != (PyGILState_Ensure = (_PyGILState_Ensure)(GetProcAddress(hPython, "PyGILState_Ensure"))) &&
            NULL != (PyGILState_Release = (_PyGILState_Release)(GetProcAddress(hPython, "PyGILState_Release"))) &&
            NULL != (PyRun_SimpleStringFlags = (_PyRun_SimpleStringFlags)(GetProcAddress(hPython, "PyRun_SimpleStringFlags")));
        return !loaded;
    };
    // First we are trying to use the Python3xx.dll dlls
    const wchar_t sPythonPrefix[] = L"Python3";
    const size_t nPythonPrefixLen = sizeof(sPythonPrefix) / sizeof(wchar_t) - 1;
    ForEachLoadedDLL([&](HMODULE hPython, const wchar_t* sModFilename, size_t sModFilenameLen) -> bool {
        int result = ::CompareStringEx(
            LOCALE_NAME_INVARIANT,
            NORM_IGNORECASE,
            sModFilename,
            (int)(sModFilenameLen > nPythonPrefixLen ? nPythonPrefixLen : sModFilenameLen),
            sPythonPrefix,
            (int)nPythonPrefixLen,
            NULL, NULL, 0);

        return (result != CSTR_EQUAL) ? true :
            fnTryToLoad(hPython, sModFilename, sModFilenameLen);
    });
    // Then we are trying to use all the other Python dlls
    if (!loaded) {
        ForEachLoadedDLL(fnTryToLoad);
    }
    return loaded;
}