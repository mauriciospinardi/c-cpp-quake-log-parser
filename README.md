# Hello from CloudWalk: Quake Log Parser

> Truth can only be found in one place: the code. <br/>
> -- Robert C. Martin

Skills presentation for CloudWalk, Inc.  

## Summary

- [Development environment](#development-environment)
- [Build](#build)
  - [Defining the environment](#defining-the-environment)
  - [Build commands](#build-commands)
  - [Visual Studio Code](#visual-studio-code)
    - [Example](#example)
- [Execution and debugging](#execution-and-debugging)
- [Development notes](#development-notes)
  - [Unit tests](#unit-tests)

## Development environment

File ```.\misc\env.bat``` defines the development environment. Its highlight is
the tools directory ```C:\cygwin64```.  

The [Cygwin](https://www.cygwin.com) distribution provides all dependencies
required here:  

- GNU Compiler Collection
- The GNU version of the 'make' utility

This repository was created and tested under Windows 10 Pro 2004 19041.450.  

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

W.I.P.  

## Development notes

- Commit messages for this repository were written (and should always)
  following the guidelines summary from
  [robertpainsi](https://gist.github.com/robertpainsi/b632364184e70900af4ab688decf6f53).
- Source code was procedurally written in C (under the C99 standard).
    - It's the most generic approach for embedded devices wich can deal with
      C/C++ code (tough the target, in this case, is a regular PC).
- Despite procedurally written, code has some characteristics usual to the
  object oriented approach, as its hinted by the prefixes in the public API:
    - \<"class"\>_\<"public method"\>
- cJSON data format handling happens through Dave Gamble's
  [cJSON](github.com/DaveGamble/cJSON), under the MIT License.
- Structure was defined to highlight knowledge over C/C++ language and
  programming overall. That made the final repository more complex than it
  needed to be for the intended functional goal. Also, it would be better to
  have 3 repositories, other than 1:
  - main application
    - libqlp (QLP stands for Quake Log Parser)
    - libcjson (cJSON)

### Unit tests

W.I.P.  
