# This file should be placed in the same folder, where PyInjector DLLs are located
# Place your custom code here:
print("Hello from PyInjector inside python %s" % (__import__('sys')).version)

with open("PyInjector.log", "at") as log:
    log.write("Hello from PyInjector inside python %s\n" % (__import__('sys')).version)
