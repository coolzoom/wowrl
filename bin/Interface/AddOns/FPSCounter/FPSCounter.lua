FPSCounter = {
    ["updateTimer"] = 0.5, -- Time to wait between each update of the FPS
    ["frameNbr"]    = 0,   -- Frame counter
    ["FPS"]         = 0,   -- The FPS as it is displayed on the screen
};

function FPSCounter.OnUpdate()
    FPSCounter.updateTimer = FPSCounter.updateTimer + arg1;
    FPSCounter.frameNbr = FPSCounter.frameNbr + 1;
    if (FPSCounter.updateTimer >= 0.5) then
        FPSCounter.FPS = math.floor(FPSCounter.frameNbr / FPSCounter.updateTimer);
        FPSCounter_Text:SetText("FPS : "..FPSCounter.FPS);
        FPSCounter.updateTimer = 0;
        FPSCounter.frameNbr = 0;
    end
end
