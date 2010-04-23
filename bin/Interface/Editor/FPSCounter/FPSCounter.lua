FPSCounter = {
    ["updateTimer"] = 0.5, -- Time to wait between each update of the FPS
    ["frameNbr"]    = 0,   -- Frame counter
    ["FPS"]         = 0,   -- The FPS as it is displayed on the screen
};

function FPSCounter.OnUpdate()
    FPSCounter.updateTimer = FPSCounter.updateTimer + arg1;
    FPSCounter.frameNbr = FPSCounter.frameNbr + 1;
    local mx, my, mz = Frost:GetMouseDecalPosition();
    if (mx ~= nil and my ~= nil and mz ~= nil) then
        FPSCounter_Text:SetText(string.format("Mouse : %.2f, %.2f, %.2f", mx, my, mz).."\nFPS : "..FPSCounter.FPS);
    else
        FPSCounter_Text:SetText("Mouse : nan, nan, nan\nFPS : "..FPSCounter.FPS);
    end
    if (FPSCounter.updateTimer >= 0.5) then
        FPSCounter.FPS = math.floor(FPSCounter.frameNbr / FPSCounter.updateTimer);
        FPSCounter.updateTimer = 0;
        FPSCounter.frameNbr = 0;
    end
end
