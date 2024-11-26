# Script to test the python injection (with multithreading to test it with Python 2)
# Test 1: you can inject 'code.py' file from the current directory and it will call internal_function()
# Test 2: you can spawn python shell and type "internal_function()" there to execute internal_function
import threading
import time

def internal_function():
    print("Internal function call. How did u do this?")

# These lines are required because in Python 2 threads should be initialized in the main thread
t = threading.Thread(target=(lambda: 0))
t.start()
t.join()

while(True):
    print("I'm python process. Nobody can use my internal_function()")
    time.sleep(20)