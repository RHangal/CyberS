if __name__ != "__main__":
    exit()
    
import subprocess, os
ELF = "tester.out"
if os.path.isfile(ELF):
    result = subprocess.run(["rm", ELF])
result = subprocess.run(["gcc", "tester.c", "ops_ui.c", "../4096_t/4096_t.c", "-Wall", "-Wextra", "-Werror", "-Wpedantic", "-o", ELF])
if not os.path.isfile(ELF):
    print("Compilation failed.")
    exit()
#result = subprocess.check_output(["head", "-c", "4", ELF])
#if not "ELF" in str(result)[1:]:
  #  print("Executable not valid.")
  #  exit()
import random
big = random.randint(2 ** 1024, 2 ** 2047)
lil = random.randint(2 ** 16, 2 ** 63)
strbig = hex(big)
strlil = str(lil)

from operator import add, sub, mul, floordiv as quo, mod as rem
# We don't deal with negative results from sub, so, just don't test it.
ops = {'ADD':add,'SUB':sub, 'MUL':mul,'QUO':quo,'REM':rem} # 
for op in ops:        
    result = int(subprocess.check_output(["./"+ELF, strbig, strlil, op]),16)
    answer = ops[op](big,lil)
    if result != answer:
        print("Operator", op, "failed.")
        print("Expected:")
        print(hex(answer))
        print("Received:")
        print(hex(result))
        exit()
    else:
        print(op, "passes.")
