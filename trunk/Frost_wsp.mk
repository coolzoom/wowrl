.PHONY: clean All

All:
	@echo ----------Building project:[ Frost - Release_Win32 ]----------
	@"mingw32-make.exe"  -j 3 -f "Frost.mk" 
clean:
	@echo ----------Cleaning project:[ Frost - Release_Win32 ]----------
	@"mingw32-make.exe"  -j 3 -f "Frost.mk" clean
