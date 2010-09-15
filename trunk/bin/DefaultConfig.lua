-- Frost Engine configuration file
GameOptions = {

    -- Generic
    ["GameVersion"] = "0045",
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
    ["AntiAliasingLevel"] = 0,
    ["EnableVSync"] = false,
    ["EnableSpecular"] = true,
    
    ["EnablePostProcessing"] = false,
    ["EnableMotionBlur"] = true,
    
    -- Defaults
    ["DefaultFont"] = "Fonts/Vera.ttf",
    ["DefaultGameplay"] = "Free",
    
    -- Input
    ["DoubleClickTime"] = 0.25,
    ["MouseBufferLength"] = 0.05,
    ["MouseSensibility"] = 1.0,
    
    -- GUI
    ["EnableGUICaching"] = true,
    
};
