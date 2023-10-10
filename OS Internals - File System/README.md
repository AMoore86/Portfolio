# OS Internals - File System
## Goal
The goal of this program is to recreate the functionality of an Operating System File System by modifying C++ code. Although the basic shell of the File System was already included for the project, the main goal of this project is the implementation of the majority of File System command line functions.

## The Program
### General Information
For this file system project, I utilize directory.cpp, inodes.cpp, and volume.cpp for the majority of my function calls. In order to create my own custom functions, I modify the directory.cpp and fs33types.hpp files. The inodes.cpp file is utilized regularly to retrieve information (inode, size, type, etc.) about various files. The Makefile is used to update the P0 executable file, which executes the shell.cpp script. After modifying the software, the testscript.txt file is utilized to test the executeable against expected outcomes.

### Custom Functions
The functions that are recreated within this project include:
 - doInode() --- Prints the inode contents of the current directory
 - doInodeString() --- Prints the inode contents of an entered directory name
 - doLsLong() --- Prints contents of a certain directory
 - doHardLn() --- Creates a hard link to the parent directory using the "."
 - doHardLnNoDot() --- Creates a heard link to a specified directory
 - doSoftLn() --- Creates a soft link to a specified directory
 - doLsofDir() --- Prints contents of a certain directory
 - doMkDir() --- Creates a directory inside of the current directory
 - doMv() --- Move one file to another file location
 - doRm() --- Remove a subdirectory from the cwd
 - rmRec() --- Recursively remove directories/subdirectories
 - doPwd() --- Prints the full filepath of the current working directory
 - doChDir() --- Change the cwd using either an absolute or relative path
 - softCh() --- Helper method for recursively finding the pathnames in a soft links and changing the cwd to them  

All of the listed functions are implemented in the shell.cpp file, between lines 185 - 1543. Toward the bottom of these specified lines of code, there is a Command Table (CmdTable) which specifies the command name (which would be entered into the command line to run any of the corresponding functions), the arguments required, whether globals are needed, and the corresponding function.

### Room for Improvement
The main issue with the current code is that the directories are never deconstructed, meaning that memory leaks are possible. Additionally, there are multiple functions that have yet to be completed/implemented.