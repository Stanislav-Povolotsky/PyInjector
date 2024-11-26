#pragma once
#include <iostream>
#include <Windows.h>
#include <fstream>

class SDK
{
public:
    static bool InitCPython();
};

typedef
enum { PyGILState_LOCKED, PyGILState_UNLOCKED }
PyGILState_STATE;

typedef struct {
    int cf_flags;  /* bitmask of CO_xxx flags relevant to future */
    int cf_feature_version;  /* minor Python version (PyCF_ONLY_AST) */
} PyCompilerFlags;

#define PyRun_SimpleString(s) PyRun_SimpleStringFlags(s, NULL)

typedef const char*(__stdcall* _Py_GetVersion)();
typedef void(__stdcall* _Py_SetProgramName)(const wchar_t*);
typedef void(__stdcall* _PyEval_InitThreads)();
typedef int(__stdcall* _PyEval_ThreadsInitialized)();
typedef PyGILState_STATE(__stdcall* _PyGILState_Ensure)();
typedef void(__stdcall* _PyGILState_Release)(PyGILState_STATE);
typedef int(__stdcall* _PyRun_SimpleStringFlags)(const char*, PyCompilerFlags*);

extern _Py_GetVersion Py_GetVersion;
extern _Py_SetProgramName Py_SetProgramName;
extern _PyEval_InitThreads PyEval_InitThreads;
extern _PyEval_ThreadsInitialized PyEval_ThreadsInitialized;
extern _PyGILState_Ensure PyGILState_Ensure;
extern _PyGILState_Release PyGILState_Release;
extern _PyRun_SimpleStringFlags PyRun_SimpleStringFlags;