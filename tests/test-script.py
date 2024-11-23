import time

def internal_function():
    print("Internal function call. How did u do this?")

while(True):
    print("I'm python process. Nobody can use my internal_function()")
    time.sleep(20)
