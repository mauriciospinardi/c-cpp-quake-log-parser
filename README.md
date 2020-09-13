# Hello from CloudWalk: Quake Log Parser

Skills presentation for CloudWalk, Inc.  

## Summary

- [Development environment](#development-environment)
- [Build](#build)
  - [Defining the environment](#defining-the-environment)
  - [Build commands](#build-commands)
  - [Visual Studio Code](#visual-studio-code)
    - [Example](#example)
- [Execution and debugging](#execution-and-debugging)
  - [Unit tests](#unit-tests)
- [Development notes](#development-notes)

## Development environment

File ```.\misc\env.bat``` defines the development environment. Its highlight is
the tools directory ```C:\cygwin64```.  

The [Cygwin](https://www.cygwin.com) distribution provides all required
dependencies:  

- GNU Compiler Collection
- The GNU version of the 'make' utility

This repository was originally created and tested under Windows 10 Pro 2004
19041.450.  

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
```./.vscode``` can be sniffed for references to the most important commands
related to the build process.  

#### Example

For the correspondent of ```bash -c "make CFG_NAME=debug"```,
file ```./.vscode/tasks.json``` has:  

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

The executable is built to ```./bin/<config>/*.exe``` and takes as input only
one argument: a log file.  

- ```<config>``` can be either ```release``` or ```debug```.
- ```*.exe``` references the ```PROJECT_NAME``` env. variable from the
  Makefile.
- Path to the input can be either complete or relative to the executable path
  itself.
- Output should be either a JSON stream or a error message.

### Unit tests

Similarly to what is in the [Build commands](#build-commands) section, unit
tests can be run by:

```cmd
<...\c-cpp-quake-log-parser>bash -c "make -B CFG_NAME=tests"
```

An optional [Visual Studio Code](#visual-studio-code) integration is also
available.

## Development notes

- Commit messages for this repository were written (and should always)
  following the guidelines summary from
  [robertpainsi](https://gist.github.com/robertpainsi/b632364184e70900af4ab688decf6f53).
- Source code was procedurally written in C (under the C99 standard).
- Despite procedurally written, code has some characteristics usual to the
  object oriented approach, as its hinted by the prefixes in libqlp's API:
    - \<"class"\>_\<"public method"\>
- JSON data handling happens through Dave Gamble's
  [cJSON](https://github.com/DaveGamble/cJSON), under the MIT License.
- Structure was defined to highlight knowledge over C/C++ language and
  programming overall (not only with the functional goal in mind).
- EN_ERROR was kept minimal, but it could be extended to improve analysis in
  cases of errors.
