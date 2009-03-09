-- Frost Engine configuration file

-- GFX
bFullScreen = false;
if (bFullScreen) then
    iScreenWidth = 1280;
    iScreenHeight = 1024;
else
    iScreenWidth = 1024;
    iScreenHeight = 768;
end
bVSync = true;
iScreenDepth = 32;
iFSAA = 4;
--sRenderSystem = "Direct3D9 Rendering Subsystem";
sRenderSystem = "OpenGL Rendering Subsystem";

-- Files


-- Game
sGameVersion = "0.045";
sLocale = "enGB";
iMaxComputedPaths = 2;

-- UI
sDefaultFont = "Fonts/Calibri.ttf";