from pathlib import Path
import subprocess
import os
from build_example import execute

# remove build folder and re-create
build_path = Path(__file__).parent.parent.joinpath("build")

if not build_path.exists():
    print(
        "Error: build folder not found. Make sure running 'python build_example.py' first!"
    )
    exit(-1)

program_path = build_path.joinpath("examples/metal_demo")
os.chdir(program_path)

try:
    execute("./metal_example")
except Exception as e:
    print(f"Error while running the example: {e}")
