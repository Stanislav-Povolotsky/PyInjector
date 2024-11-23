# This file should be placed in the same folder, where PyInjector DLLs are located
# Place your custom code here:
print("Hello from PyInjector")

with open("PyInjector.log", "at", encoding="utf8") as log:
    print("Hello from PyInjector", file=log)
