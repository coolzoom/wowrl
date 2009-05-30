FPSCounter = {
    ["updateTimer"] = 0.5,
    ["FPS"]         = 0,
};

function FPSCounter.OnUpdate()
    FPSCounter.updateTimer = FPSCounter.updateTimer + arg1;
    if (FPSCounter.updateTimer >= 0.5) then
        FPSCounter.FPS = math.floor(1/arg1);
        FPSCounter.updateTimer = 0;
    end
    FPSCounter_Text:SetText("FPS : "..FPSCounter.FPS);
end
