from pathlib import Path
import subprocess
import shutil
import os


def execute(cmd: str):
    print(f"Executing {cmd}")
    proc = subprocess.Popen(cmd, stderr=subprocess.PIPE, shell=True)
    _, err = proc.communicate()
    if err:
        raise Exception(
            f"Error while building example: {err.decode('utf-8')}. Executed command: {cmd}"
        )


# remove build folder and re-create
build_path = Path(__file__).parent.parent.joinpath("build")
build_path.mkdir(exist_ok=True, parents=True)

# change current working directory
os.chdir(str(build_path))
# Build project
try:
    cmake_pre_build_cmd = "cmake .."
    execute(cmake_pre_build_cmd)

    cmake_build_cmd = "cmake --build ."
    execute(cmake_build_cmd)

except Exception as e:
    print(f"CMAKE ERROR: {e}")

print("Examples build successfully!")
