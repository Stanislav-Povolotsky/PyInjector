# PyInjector
PyInjector - Inject python-code into any python process or spawn interactive python-shell inside the target process.

## How to use it ?

### Injecting own code
1. Write the code you want to inject into a file called code.py and place it in the same folder of the software you want to inject it (in the current folder of this software).   
2. Inject the PyInjector_x64.dll / PyInjector_x86.dll into the target process using [Process Hacker](https://processhacker.sourceforge.io/) or any other [DLL injection tool](https://github.com/nefarius/Injector).

You can also write the code directly in the DLL and recompile it or modify python code inside PyInjector_x??.dll in your favorite binary editor.   

#### Why injecting python code into python script ?
This project is made for reverse-engineer and malware analyst, the main aspect of this project is to reverse-engineer any python script/executable easily.   
I put some usefull script in the py-code-examples/ folder that can be used to reverse-engineer any script (even with pyarmor).

### Spawning python-shell
Spawning interactive python shell in the any python process is easy - just inject PyInjector_x64_shell.dll / PyInjector_x86_shell.dll DLL in the target process. It should be enough to see interactive console window with python shell.   
You can spawn interactive python shell even in GUI applications.

## Compatibilities
Requirements:
- OS: Windows XP or later.
- Target process must use Python x86/x64 3.7, 3.8, 3.9, 3.10, 3.11.

## Example
In this [video](https://youtu.be/NkFs7A0q4DM) [@call-042PE](https://github.com/call-042PE) is unpacking a function in a pyarmor protected script with PyInjector.
