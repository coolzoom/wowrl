FreeGameplay = {
    ["camera"] = nil,                -- The camera handled by this gameplay
    ["maxSpeed"] = 20,               -- The camera movement speed
    ["MRPressed"] = false,           -- 'true' if the mouse right button is pressed
    ["leadToward"] = Vector(0, 0, 0),
    ["prevLeadToward"] = Vector(0, 0, 0),
    ["acceleration"] = Vector(0, 0, 0),
    ["speed"] = Vector(0, 0, 0),
    ["prevSpeed"] = Vector(0, 0, 0),
    ["maxAcceleration"] = 5,
};

-----------------------------------
-- CONTROLS : (only moves a camera)
-----------------------------------
-- [W] : Move forward
-- [S] : Move backward
-- [A] : Strafe left
-- [D] : Strafe right
-- [Q] : Move up
-- [E] : Move down
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

function FreeGameplay.UpdateAcceleration(axis)
    if (FreeGameplay.leadToward[axis] == 0.0 and (FreeGameplay.speed[axis] * FreeGameplay.prevSpeed[axis] < 0.0)) then
        FreeGameplay.acceleration[axis] = 0.0;
        FreeGameplay.speed[axis] = 0.0;
    else
        if (FreeGameplay.leadToward[axis] - FreeGameplay.speed[axis] > 0.0) then
            FreeGameplay.acceleration[axis] = FreeGameplay.maxAcceleration;
        elseif (FreeGameplay.speed[axis] - FreeGameplay.leadToward[axis] > 0.0) then
            FreeGameplay.acceleration[axis] = -FreeGameplay.maxAcceleration;
        end
    end
end

function FreeGameplay.OnEvent()
    if (event == "KEY_PRESSED") then
        if (arg1 == KEY_W) then
            FreeGameplay.leadToward.z = FreeGameplay.leadToward.z - 1;
        elseif (arg1 == KEY_S) then
            FreeGameplay.leadToward.z = FreeGameplay.leadToward.z + 1;
        elseif (arg1 == KEY_A) then
            FreeGameplay.leadToward.x = FreeGameplay.leadToward.x - 1;
        elseif (arg1 == KEY_D) then
            FreeGameplay.leadToward.x = FreeGameplay.leadToward.x + 1;
        elseif (arg1 == KEY_Q) then
            FreeGameplay.leadToward.y = FreeGameplay.leadToward.y + 1;
        elseif (arg1 == KEY_E) then
            FreeGameplay.leadToward.y = FreeGameplay.leadToward.y - 1;
        end
    elseif (event == "KEY_RELEASED") then
        if (arg1 == KEY_W) then
            FreeGameplay.leadToward.z = FreeGameplay.leadToward.z + 1;
        elseif (arg1 == KEY_S) then
            FreeGameplay.leadToward.z = FreeGameplay.leadToward.z - 1;
        elseif (arg1 == KEY_A) then
            FreeGameplay.leadToward.x = FreeGameplay.leadToward.x + 1;
        elseif (arg1 == KEY_D) then
            FreeGameplay.leadToward.x = FreeGameplay.leadToward.x - 1;
        elseif (arg1 == KEY_Q) then
            FreeGameplay.leadToward.y = FreeGameplay.leadToward.y - 1;
        elseif (arg1 == KEY_E) then
            FreeGameplay.leadToward.y = FreeGameplay.leadToward.y + 1;
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
    FreeGameplay.prevSpeed = FreeGameplay.speed;
    FreeGameplay.speed = FreeGameplay.speed + arg1*FreeGameplay.acceleration;
    
    FreeGameplay.UpdateAcceleration("x");
    FreeGameplay.UpdateAcceleration("y");
    FreeGameplay.UpdateAcceleration("z");

    FreeGameplay.camera:Translate(arg1*FreeGameplay.maxSpeed*FreeGameplay.speed, true);
    
    FreeGameplay.prevLeadToward = FreeGameplay.leadToward;
end
