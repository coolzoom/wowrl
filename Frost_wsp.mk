.PHONY: clean All

All:
	@echo ----------Building project:[ Frost_Utils - Release_Win32 ]----------
	@"mingw32-make.exe"  -j 3 -f "Frost_Utils.mk" 
clean:
	@echo ----------Cleaning project:[ Frost_Utils - Release_Win32 ]----------
	@"mingw32-make.exe"  -j 3 -f "Frost_Utils.mk" clean
