@ECHO OFF

REM 
REM @file env.bat
REM @author Maurício Spinardi (mauricio.spinardi@gmail.com)
REM @platform cygwin64
REM @brief Auxiliary script. Defines the development environment.
REM @date 2020-09-03
REM 
REM 

:ENVIRONMENT

SET ROOT_DIR=^
C:\cygwin64

SET PATH=^
%ROOT_DIR%\bin;^
C:\Program Files\Git\bin;

EXIT /B 0
