import subprocess
import os
import random
import sys

# Name of the C executable
ELF = "./big_tester.out"

# C source files
C_FILES = ["big_tester.c", "4096_t.c"]

# Check if we need to recompile (compile every time before running tests)
def compile_c_program():
    print("🔄 Compiling C program...")
    compile_cmd = ["gcc", *C_FILES, "-o", ELF]
    result = subprocess.run(compile_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    if result.returncode != 0:
        print("❌ Compilation failed!")
        print(result.stderr.decode())  # Print compilation errors
        exit(1)
    print("✅ Compilation successful.")

# Ensure the executable is compiled before running tests
compile_c_program()

# Ensure the user provided at least one operation to test
if len(sys.argv) < 2:
    print("Usage: python3 interface.py <operation(s)>")
    print("Available operations: ADD, SUB, MUL, QUO, REM")
    exit(1)

# Generate two large random numbers
bigone = random.randint(2 ** 1024, 2 ** 2047)
bigtwo = random.randint(2 ** 1024, 2 ** random.randint(1025, 2047))
hexone = hex(bigone)
hextwo = hex(bigtwo)

# Define supported operations
from operator import add, sub, mul, floordiv as quo, mod as rem
operations = {
    "ADD": ("A", add),
    "SUB": ("S", sub),
    "MUL": ("M", mul),
    "QUO": ("Q", quo),
    "REM": ("R", rem)
}

# Get operations from command-line arguments
selected_ops = [op.upper() for op in sys.argv[1:]]

# Validate input operations
for op in selected_ops:
    if op not in operations:
        print(f"❌ Error: Unknown operation '{op}'. Available operations: ADD, SUB, MUL, QUO, REM")
        exit(1)

# Run the tests for selected operations
for op_name in selected_ops:
    op_symbol, python_op = operations[op_name]

    try:
        # Special handling for subtraction to avoid negative results
        if op_name == "SUB" and bigone < bigtwo:
            print(f"⚠️ Skipping SUB test because {bigone} < {bigtwo} (would underflow in unsigned math)")
            continue

        # Call C program with hex inputs
        result_hex = subprocess.check_output([ELF, hexone, hextwo, op_symbol], text=True).strip()
        result_int = int(result_hex, 16)  # Convert from hex to integer
        expected_result = python_op(bigone, bigtwo)  # Compute expected result in Python

        # Compare the results
        if result_int != expected_result:
            print(f"❌ {op_name} failed!")
            print(f"Expected: {hex(expected_result)}")
            print(f"Received: {hex(result_int)}")
            exit(1)
        else:
            print(f"✅ {op_name} passes.")

    except subprocess.CalledProcessError as e:
        print(f"Error running C program for {op_name}: {e}")
        exit(1)

