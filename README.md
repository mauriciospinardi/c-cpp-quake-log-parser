# Hello from CloudWalk: Quake Log Parser

## Summary

- [Development environment]()
- [Build]()
  - [Defining the environment]()
  - [Build commands]()
  - [Visual Studio Code]()
    - [Example]()
- [Execution and debugging]()
- [Development notes]()
  - [Unit tests]()

## Development environment

File ```.\misc\env.bat``` defines the development environment and should be
consider the functional entry point of this repository. In this file, the
highlight is the tools directory ```C:\cygwin64```.  

The [Cygwin](https://www.cygwin.com) distribution provides all dependencies
required in here:  

- GNU Compiler Collection
- The GNU version of the 'make' utility

## Build

### Defining the environment

As pointed by the previous section, ```.\misc\env.bat``` defines the
development environment. For that environemnt to be correctly set, in a new
Command Prompt window, execute:  

```cmd
<...\c-cpp-quake-log-parser>.\misc\env.bat
```

### Build commands

Once the development environment is correctly set, Cygwin tools will be
available. To trigger the build process, execute:  

```cmd
<...\c-cpp-quake-log-parser>bash -c "make CFG_NAME=debug"
```

The above command will trigger the build process in ```debug``` mode.  

### Visual Studio Code

This repository has an optional integration with Visual Studio Code. Files in
```./vscode``` can be sniffed for references to the most important commands
related to the build process.  

#### Example

For the correspondent of ```bash -c "make CFG_NAME=debug"```,
file ```./vscode/tasks.json``` has:  

```json
{
    "label": "--build-debug",
    "type": "shell",
    "command": "& bash -c \"make CFG_NAME=debug\" & exit",
    "group": "build"
}
```

The ```"command"``` key above defines ```bash -c "make CFG_NAME=debug"```, used
to trigger the compilation process in ```debug``` mode.  

## Execution and debugging

T.B.D.  

## Development notes

T.B.D.  

### Unit tests

T.B.D.  
