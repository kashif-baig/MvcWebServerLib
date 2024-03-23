# On Linux, it may be necessary to use fully qualified paths for
# dotnet and the code generator assembly, if environment variables
# are blank when invoked from Visual Studio Code.

dotnet "$HOME/Arduino.MVC/Arduino.MVC.CodeGen.dll" "./asp_files" "."
