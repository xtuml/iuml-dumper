echo Running Dumper on domain ...
@echo off
call D:\iUML_Dumper\build\setupenv.bat

for /F "tokens=*" %%A in (domains.txt) do (
  
  echo 9 | %%A

  for /F "tokens=3" %%B in ("%%A") do (

    call move output %%B

  )

)
echo Finished.
@echo on