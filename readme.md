# Software System

### Installation:
This software requires a linux machine with 
- Linux Machine (Preferably Ubuntu)
- GNU Make (Preferably version 4)
- g++ (Perferably >=5.2.1)
- 

There is an issue with svn that keeps deleting one of my source files (/src/ObjectGeneration/ClassValueReturnable.h).
Should this happens, there is another file in the same location (ClassValueReturnable.h.backup) that holds 
the same source code. Please put the contents of this file into a new ClassValueReturnable.h file if it goes missing.

From this folder (with all the contents of the folder) run the following commands from the terminal:
```sh
make clean
make
```
This will generate the compiler in a file called "main".
This file can be copied anywhere although running tests requires the TPL_code folder to be in the same folder as the executable.

### Running the Executable:
This program can be run in multiple ways.

```sh
./main input_file.tpl
```
The first argument ("input_file.tpl") is the file path to the file that is intented to be compiled. This fill be compiled into the "default-output.xml" file in the directory where the compiler "main" exists.

```sh
./main input_file.tpl output_file.xml
```
This method of execution allows for the user to set what output file should be written to ("output_file.xml") rather than using the default output ("default-output.xml").
