import traceback
import sys

while(True):
  s = input('pyshell >>> ')
  cs = s
  while(cs.endswith(':') or cs.startswith(' ')):
    cs = input('pyshell ... ')
    s += '\n' + cs
  if(not s.strip()): continue
  try:
    code = compile(s,'<string>','single')
    eval(code)
  except:
    traceback.print_exception(*sys.exc_info())