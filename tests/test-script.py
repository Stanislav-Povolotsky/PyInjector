# Script to test the python injection.
# Test 1: you can inject 'code.py' file from the current directory and it will call internal_function()
# Test 2: you can spawn python shell and type "internal_function()" there to execute internal_function
import time

def internal_function():
    print("Internal function call. How did u do this?")

while(True):
    print("I'm python process. Nobody can use my internal_function()")
    time.sleep(20)
