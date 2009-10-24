-- Frost Engine configuration file
GameOptions = {

    -- Generic
    ["GameVersion"] = "0.045",
    ["GameLocale"] = "enGB",
    
    -- Screen resolution
    ["FullScreen"] = false,
    ["WindowWidth"] = 1024,
    ["WindowHeight"] = 768,
    ["ScreenWidth"] = 1280,
    ["ScreenHeight"] = 1024,
    
    -- Renderer
    ["RenderSystem"] = "OpenGL",
    --["RenderSystem"] = "DirectX",
    
    -- Advanced graphics options
    ["TerrainTextureFiltering"] = "ANISOTROPIC";
    --["TerrainTextureFiltering"] = "BILINEAR";
    ["AntiAliasingLevel"] = 0,
    ["EnableVSync"] = true,
    ["EnableSpecular"] = true,
    
    ["EnablePostProcessing"] = true,
    ["EnableMotionBlur"] = true,
    
    -- Defaults
    ["DefaultFont"] = "Fonts/Vera.ttf",
    ["DefaultGameplay"] = "FirstPerson",
    
    -- Input
    ["DoubleClickTime"] = 0.25,
    ["MouseBufferLength"] = 0.05,
    ["MouseSensibility"] = 1.0,
    
};
