# anishell - A simple shell project

## Running the Code

An executable is in the home folder for you convinience.
Just execute and start on the shell:
```shell script
./anishell
```

To compile and run the code with cmake do the following:
```shell script
cd cmake-build-debug
cmake -S .. -B .
make
./anishell
```
And start typing away.

You can also build this project with GCC.
```shell script
gcc -o anishell *.c utils/*.* processor/*.* commands/*.*
./anishell
```

## Code flow and organization

There are 3 directories:
* utils: Contains basic data structures and system interface
* processor: Implements the parsing and terminal logic
* commands: Implements all system commands needed

The code flow is the following:
* Execution starts in main
* It calls prompt to take input
* prompt calls parser once a line of input is obtained
* parser calls any of the commands it needs

All logic for background processes and forking is in `commands/execvp`.

`String` and `Strmat` (List of Strings) classes are used everywhere to maintain data.

## Specifications

All the things that the code should do are specified in the assignment document in the root directory Assignment2.pdf.
