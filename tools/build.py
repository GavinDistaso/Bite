# this file compiles and builds all modules and standalone sources
# im using python as its platform independent
'''
HOW TO USE

easy,

if you want to build just run `python build.py` 

if you want to clean run `python build.py clean`
'''

from pathlib import Path
import os
import sys
from sys import platform

BIN_DIR = r'../bin'
BUILD_DIR = r'../build'
SRC_DIR = r'../src'

# this is to create the directories if they dont exist
os.system(f'mkdir {BIN_DIR} && mkdir {BUILD_DIR} && mkdir {SRC_DIR}')

ENTRY_FILEPATH = r'../src/main.c'

COMPILE_FLAGS = '-Wall -Wextra -g -I../src'

OUTPUT_NAME = 'cal' # `compile A-lang`
LIB_OUTPUT_NAME = r'libmodules.a'

def command(cmd: str):
    print(f'\u001b[35m[cmd]\u001b[0m {cmd}')
    os.system(cmd)

def status(msg: str):
    print(f'\u001b[32m[status]\u001b[0m {msg}')

''' clean '''
if(len(sys.argv) > 1 and sys.argv[1] == 'clean'):
    status("cleaning")
    if(sys.platform == 'win32'):
        command(f'del "{BUILD_DIR}/*.o"')
    else:
        command(f'rm "{BUILD_DIR}/*.o"')
        
    exit()

''' build '''

''' compile all modules '''

status('building modules')

objPaths = []

# blacklist removes all files not in a folder
# that is removes all non-module files
pathBlacklist = Path(SRC_DIR).glob('*.c')

for path in Path(SRC_DIR).rglob('*.c'):
    if(path in pathBlacklist): continue
    
    
    objFile = str(path)[len(SRC_DIR)+1:-1].replace('\\', '-').replace('/', '-')
    objPath = f'{BUILD_DIR}/{objFile}o'
    
    cmd = f'gcc -c -o {objPath} {path}'
    
    command(cmd)
    
    objPaths.append(objPath)

''' build static library '''

status('building static library')

cmd = f"ar rcs {BIN_DIR}/{LIB_OUTPUT_NAME} {' '.join(objPaths)}"

command(cmd)

''' build source '''

status("building source")

command(f'gcc {COMPILE_FLAGS} {ENTRY_FILEPATH} -o {BIN_DIR}/{OUTPUT_NAME} {BIN_DIR}/{LIB_OUTPUT_NAME}')
