#include "SDK.h"

_Py_SetProgramName Py_SetProgramName;
_PyEval_InitThreads PyEval_InitThreads;
_PyGILState_Ensure PyGILState_Ensure;
_PyGILState_Release PyGILState_Release;
_PyRun_SimpleStringFlags PyRun_SimpleStringFlags;

bool SDK::InitCPython()
{
    HMODULE hPython = NULL;
    const char* python_mods[] = {
        "Python311.dll",
        "Python310.dll",
        "Python39.dll",
        "Python38.dll",
        "Python37.dll",
    };
    for (size_t i = 0; !hPython && i < (sizeof(python_mods) / sizeof(python_mods[0])); ++i) {
        hPython = ::GetModuleHandleA(python_mods[i]);
    }
    if(!hPython) {
        return false;
    }
    return
        NULL != (Py_SetProgramName = (_Py_SetProgramName)(GetProcAddress(hPython, "Py_SetProgramName"))) &&
        NULL != (PyEval_InitThreads = (_PyEval_InitThreads)(GetProcAddress(hPython, "PyEval_InitThreads"))) &&
        NULL != (PyGILState_Ensure = (_PyGILState_Ensure)(GetProcAddress(hPython, "PyGILState_Ensure"))) &&
        NULL != (PyGILState_Release = (_PyGILState_Release)(GetProcAddress(hPython, "PyGILState_Release"))) &&
        NULL != (PyRun_SimpleStringFlags = (_PyRun_SimpleStringFlags)(GetProcAddress(hPython, "PyRun_SimpleStringFlags")));
}