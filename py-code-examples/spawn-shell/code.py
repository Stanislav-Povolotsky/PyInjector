def _():
  del globals()['_'] # removing current function from the globals (antidetect)
  sys = __import__('sys')
  # if process have redirected stdin/stdout, we can't input/output anything
  # so we need to redirect them back to the console. Such redirection may break 
  # target application or change its behavior, so use shell-mode with caution.
  redirect_stdio = True
  if (redirect_stdio):
    openfn = lambda f,rw: __import__('io').open(f, rw) if sys.version_info[0] != 2 else open(f, rw)
    stdfnames = ('CONIN$', 'CONOUT$', 'CONOUT$') if (__import__('os').name == 'nt') else (('/dev/tty',) * 3)
    sys.stdin  = openfn(stdfnames[0], 'r')
    sys.stdout = openfn(stdfnames[1], 'w')
    sys.stderr = openfn(stdfnames[2], 'w')

  while(True):
    shell_input_fn = input if (sys.version_info[0] != 2) else raw_input
    sys.stdout.write('pyshell >>> ')
    s = shell_input_fn()
    cs = s
    while(cs.endswith(':') or cs.startswith(' ')):
      sys.stdout.write('pyshell ... ')
      cs = shell_input_fn()
      s += '\n' + cs
    if(not s.strip()): continue
    try:
      code = compile(s,'<string>','single')
      eval(code)
    except:
      __import__('traceback').print_exception(*sys.exc_info())
_()