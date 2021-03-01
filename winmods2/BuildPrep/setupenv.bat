echo off
set path=D:\cmake\cmake-3.13.4-win64-x64\bin;%PATH%
set TOOL_HOME=C:\PROGRA~2\KENNED~1\IUML2~1.4
set UML_HOME=%TOOL_HOME%\Modeller
set UML_ROOT=%TOOL_HOME%\Modeller

call %UML_HOME%\uml_login.bat
call %UML_HOME%\etc\environment.bat

set LD_LIBRARY_PATH=%LD_LIBRARY_PATH%;%UML_HOME%/api/lib
set TBK_SCHEMATA=UML_schema tbk os
set DBI_MAX_CONCURRENT_FSDS=16
set TBK_HASH=%UML_HOME%\tools\UML.tool
set TBK_PATH=%UML_HOME%\api\tools\API.tool
set TCL_VERSION=84
set TCL_ROOT=%UML_HOME%\tcl
set TCL_LIBRARY=%UML_HOME%\tcl\lib\tcl8.4
set TCLLIBPATH=%UML_HOME%\tcl\lib
set OOA_PERL_EXEC=%UML_HOME%\perl\bin
echo on