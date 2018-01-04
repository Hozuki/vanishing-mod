@echo Current directory:
cd
cd sp\src\
@echo Creating projects...
creategameprojects.bat
@echo Building...
msbuild games_vanishing.sln /verbosity:minimal
cd ..\..\
@echo Complete.
