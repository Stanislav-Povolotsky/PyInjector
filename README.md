# PyInjector
PyInjector - Inject python-code into any python process or spawn interactive python-shell inside the target process.
Based on [PyInjector](https://github.com/call-042PE/PyInjector) by [@call-042PE](https://github.com/call-042PE)

## How to use it?

1. Find out what platform uses the target process (where you will inject your python code or in which context you want to spawn shell).   
   You can find out it by checking "Platform" column in the task manager.  
   Then select the correct version of the PyInjector:
   - on **64-bit** platform
     - PyInjector_**x64**.dll to inject you own python code
     - PyInjector_**x64**_shell.dll to spawn shell in the context of the target process
   - on **32-bit** platform
     - PyInjector_**x86**.dll to inject you own python code
     - PyInjector_**x86**_shell.dll to spawn shell in the context of the target process
   - on **ARM64** platform
     - PyInjector_**arm**.dll to inject you own python code
     - PyInjector_**arm**_shell.dll to spawn shell in the context of the target process

2. If you want to inject your own custom python code, write it into a file called _code.py_ and 
   place it in the same folder where PyInjector DLLs are located or in the current folder of the target app.

2. Inject PyInjector DLL into the target process by using:
   - [Injector](https://github.com/nefarius/Injector) ([Download](https://github.com/nefarius/Injector/releases/latest))
   - [System Informer](https://github.com/winsiderss/systeminformer) ([Download](https://systeminformer.sourceforge.io/downloads.php))
   - [Process Hacker](https://processhacker.sourceforge.io/) ([Download](https://processhacker.sourceforge.io/downloads.php))
   - or any other DLL injection tool

## Compatibilities
Requirements:

- Target process must use **Python 3** (any version) or **Python 2** (implemented only limited support)
- OS: Windows 7 or later  
  (for Windows XP support: you still can compile it with Visual Studio 2015 or use precompiled binaries from [PyInjector 1.1](https://github.com/Stanislav-Povolotsky/PyInjector/releases/tag/v1.1))
  
Limits:

- Release binaries are compiled for Windows 7+, please use Visual Studio 2015 to build solution with Windows XP support.
- **Python 2** support limits: 
  - Only processes, which have **more than a one thread** are supported (because multithreading support should be initialized in the main thread. See [example](https://github.com/Stanislav-Povolotsky/PyInjector/blob/main/tests/test-script.python2.py))
  - SpawnShell mode: stdout is locked while processing raw_input(), so target output of the target application will be suspended until you press enter.
 
## Why would somebody need to inject Python code into a Python script?

This project is made for reverse-engineer and malware analyst, the main aspect of this project is to reverse-engineer any python script/executable easily.   
I put some usefull script in the py-code-examples/ folder that can be used to reverse-engineer any script (even with pyarmor).

## Injecting own python code

You have prepared your own _code.py_ and placed it in the same folder where PyInjector DLLs are located or in the current folder of the target app?  
Perfect! Now just inject (load) PyInjector_x64.dll in the target process and and _code.py_ will be executed.  
Reminder: if the target process have no console window, do not try to print something on the screen, you will see nothing. Just open the file and write output there.

### Demo: injecting code.py with System Informer

![injecting code.py with Process Hacker](https://github.com/user-attachments/assets/4214b213-5cfc-430a-bf0e-fa6bd2f98cef)

### Demo: injecting code.py with Process Hacker

![injecting code.py with Process Hacker](https://github.com/Stanislav-Povolotsky/PyInjector/assets/19610545/eb1d55af-b2ba-48c8-9405-572be5dadf1a)

## Spawning python-shell

Spawning interactive python shell in the any python process is easy - just inject PyInjector_x64_shell.dll / PyInjector_x86_shell.dll DLL in the target process. It should be enough to see interactive console window with python shell.   
You can spawn interactive python shell even in GUI applications.

### Demo: spawning python-shell with System Informer

![spawning python shell with System Informer](https://github.com/user-attachments/assets/9ba47489-7521-4400-b02a-6eaf772ba682)

### Demo: spawning python-shell with Process Hacker

![spawning python shell with Process Hacker](https://github.com/Stanislav-Povolotsky/PyInjector/assets/19610545/17c08928-533d-4482-a191-5dbbb5ee951c)

## Examples

- In this [video](https://youtu.be/NkFs7A0q4DM) [@call-042PE](https://github.com/call-042PE) is unpacking a function in a pyarmor protected script with PyInjector.
- Solving [Reverse Engineering CTF tasks using PyInjector](https://blog.ryukk.dev/ctfs/write-up/2024/sas-ctf-quals/reverse-engineering#f-and-c-function) (writeup by [@kos0ng](https://github.com/kos0ng))
- Article: [How to dump packed Python code](https://equinox-shame.github.io/2023/08/31/Python%20%E6%89%93%E5%8C%85%E5%90%8E%E7%9A%84Dump/) (by [Ziyue / Equinox-shame](https://github.com/equinox-shame))

## How to build

- Open solution *PyInjector.sln* in Visual Studio 2015 / 2017 / 2019 / 2022, 
  select required configuration and platform (for example "Release" "x64") and run
  "Build / Build Solution".
- Or just open "Developer Command Prompt" or "Native Tools Command Prompt for VS 20xx" and 
  run [build.windows.ps1](https://github.com/Stanislav-Povolotsky/PyInjector/blob/main/build.windows.ps1) from the project directory.
