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

## Why would somebody need to inject Python code into a Python script?

This project is made for reverse-engineer and malware analyst, the main aspect of this project is to reverse-engineer any python script/executable easily.   
I put some usefull script in the py-code-examples/ folder that can be used to reverse-engineer any script (even with pyarmor).

## Injecting own python code

You have prepared your own _code.py_ and placed it in the same folder where PyInjector DLLs are located or in the current folder of the target app?  
Perfect! Now just inject (load) PyInjector_x64.dll in the target process and and _code.py_ will be executed.  
Reminder: if the target process have no console window, do not try to print something on the screen, you will see nothing. Just open the file and write output there.

### Demo: injecting code.py with Process Hacker

![injecting-code py](https://github.com/Stanislav-Povolotsky/PyInjector/assets/19610545/eb1d55af-b2ba-48c8-9405-572be5dadf1a)

## Spawning python-shell

Spawning interactive python shell in the any python process is easy - just inject PyInjector_x64_shell.dll / PyInjector_x86_shell.dll DLL in the target process. It should be enough to see interactive console window with python shell.   
You can spawn interactive python shell even in GUI applications.

### Demo: spawning python-shell

![spawning-pyshell py](https://github.com/Stanislav-Povolotsky/PyInjector/assets/19610545/17c08928-533d-4482-a191-5dbbb5ee951c)

## Compatibilities
Requirements:
- OS: Windows 7 or later.
- Target process must use Python3 (any version)

## Example
In this [video](https://youtu.be/NkFs7A0q4DM) [@call-042PE](https://github.com/call-042PE) is unpacking a function in a pyarmor protected script with PyInjector.
