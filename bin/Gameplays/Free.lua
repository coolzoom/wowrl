FreeGameplay = {
    ["camera"] = nil,                -- The camera handled by this gameplay
    ["maxSpeed"] = 20,               -- The camera movement speed
    ["translate"] = Vector(0, 0, 0), -- Translation vector
    ["MRPressed"] = false,           -- 'true' if the mouse right button is pressed
};

-----------------------------------
-- CONTROLS : (only moves a camera)
-----------------------------------
-- [W] : move forward
-- [S] : move backward
-- [A] : strafe left
-- [D] : strafe right
-- [Q] : move up
-- [E] : move down
-- [MouseRight] : Yaw and pitch the camera when dragged
-----------------------------------

function FreeGameplay.OnLoad()
    this:RegisterEvent("KEY_PRESSED");
    this:RegisterEvent("KEY_RELEASED");
    this:RegisterEvent("MOUSE_MOVED");
    this:RegisterEvent("MOUSE_PRESSED");
    this:RegisterEvent("MOUSE_RELEASED");
    FreeGameplay.camera = this:CreateCamera(3, 4, 3);
    FreeGameplay.camera:SetDirection(-3, -4, -3);
    this:SetCamera(FreeGameplay.camera); 
end

function FreeGameplay.OnEvent()
    if (event == "KEY_PRESSED") then
        if (arg1 == KEY_W) then
            FreeGameplay.translate.z = FreeGameplay.translate.z - 1;
        elseif (arg1 == KEY_S) then
            FreeGameplay.translate.z = FreeGameplay.translate.z + 1;
        elseif (arg1 == KEY_A) then
            FreeGameplay.translate.x = FreeGameplay.translate.x - 1;
        elseif (arg1 == KEY_D) then
            FreeGameplay.translate.x = FreeGameplay.translate.x + 1;
        elseif (arg1 == KEY_Q) then
            FreeGameplay.translate.y = FreeGameplay.translate.y + 1;
        elseif (arg1 == KEY_E) then
            FreeGameplay.translate.y = FreeGameplay.translate.y - 1;
        end
    elseif (event == "KEY_RELEASED") then
        if (arg1 == KEY_W) then
            FreeGameplay.translate.z = FreeGameplay.translate.z + 1;
        elseif (arg1 == KEY_S) then
            FreeGameplay.translate.z = FreeGameplay.translate.z - 1;
        elseif (arg1 == KEY_A) then
            FreeGameplay.translate.x = FreeGameplay.translate.x + 1;
        elseif (arg1 == KEY_D) then
            FreeGameplay.translate.x = FreeGameplay.translate.x - 1;
        elseif (arg1 == KEY_Q) then
            FreeGameplay.translate.y = FreeGameplay.translate.y - 1;
        elseif (arg1 == KEY_E) then
            FreeGameplay.translate.y = FreeGameplay.translate.y + 1;
        end
    elseif (event == "MOUSE_MOVED") then
        if (FreeGameplay.MRPressed) then
            FreeGameplay.camera:Yaw(-arg3);
            FreeGameplay.camera:Pitch(-arg4);
        end
    elseif (event == "MOUSE_PRESSED") then
        if (arg1 == MOUSE_RIGHT) then
            FreeGameplay.MRPressed = true;
        end
    elseif (event == "MOUSE_RELEASED") then
        if (arg1 == MOUSE_RIGHT) then
            FreeGameplay.MRPressed = false;
        end
    end
end

function FreeGameplay.OnUpdate()
    local norm = FreeGameplay.translate:GetNorm();
    if (norm ~= 0) then
        local speed = arg1*FreeGameplay.maxSpeed/norm;
        FreeGameplay.camera:Translate(speed*FreeGameplay.translate, true);
    end
end
