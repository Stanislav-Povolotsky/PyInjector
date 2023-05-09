# PyInjector
PyInjector - Inject python-code into any python process or spawn interactive python-shell inside the target process.

## How to use it ?

### Injecting own code
1. Write the code you want to inject into a file called _code.py_ and place it in the same folder of the software you want to inject it (in the _current folder_ of this software).   
2. Inject the PyInjector_x64.dll / PyInjector_x86.dll into the target process using [Process Hacker](https://processhacker.sourceforge.io/) or any other [DLL injection tool](https://github.com/nefarius/Injector).

You can also write the code directly in the DLL and recompile it or modify python code inside PyInjector_x??.dll in your favorite binary editor.   

#### Why injecting python code into python script ?
This project is made for reverse-engineer and malware analyst, the main aspect of this project is to reverse-engineer any python script/executable easily.   
I put some usefull script in the py-code-examples/ folder that can be used to reverse-engineer any script (even with pyarmor).

### Demo: injecting code.py
![injecting-code py](https://github.com/Stanislav-Povolotsky/PyInjector/assets/19610545/9d2cd5c5-97c9-4b31-bee3-3c346a3c1b43)

### Spawning python-shell
Spawning interactive python shell in the any python process is easy - just inject PyInjector_x64_shell.dll / PyInjector_x86_shell.dll DLL in the target process. It should be enough to see interactive console window with python shell.   
You can spawn interactive python shell even in GUI applications.

### Demo: spawning python-shell
![spawning-pyshell py](https://github.com/Stanislav-Povolotsky/PyInjector/assets/19610545/3dd58cd9-811d-40e8-a84a-752c30fd51fa)

## Compatibilities
Requirements:
- OS: Windows XP or later.
- Target process must use Python x86/x64 3.7, 3.8, 3.9, 3.10, 3.11.

## Example
In this [video](https://youtu.be/NkFs7A0q4DM) [@call-042PE](https://github.com/call-042PE) is unpacking a function in a pyarmor protected script with PyInjector.
