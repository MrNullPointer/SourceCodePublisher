cd .\Debug
start ServerPrototype.exe localhost:8080
cd ..\GUI\bin\Debug
start GUI.exe localhost:8082 localhost:8080 ../TestFolder AutoDemo1
start GUI.exe localhost:8081 localhost:8080 ../TestFolder AutoDemo2
