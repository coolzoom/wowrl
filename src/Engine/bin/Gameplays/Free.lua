FreeGameplay = {
    ["camera"] = nil,                          -- The camera handled by this gameplay
    
    ["key"] = {
        ["leadToward"] = Vector(0, 0, 0),      -- The direction vector
        ["acceleration"] = Vector(0, 0, 0),    -- The acceleration vector
        ["speed"] = Vector(0, 0, 0),           -- The speed vector
        ["prevSpeed"] = Vector(0, 0, 0),       -- The previous speed vector
        ["maxSpeed"] = 20.0,                   -- The camera's max speed
        ["maxAcceleration"] = 5.0,             -- The camera's max acceleration
    },
    
    ["mouse"] = {
        ["rightPressed"] = false,              -- 'true' if the mouse right button is pressed
    },
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
    this:RegisterEvent("MOUSE_MOVED_SMOOTH");
    this:RegisterEvent("MOUSE_PRESSED");
    this:RegisterEvent("MOUSE_RELEASED");
    
    FreeGameplay.camera = this:CreateCamera(3, 4, 3);
    FreeGameplay.camera:SetDirection(-3, -4, -3);
    this:SetCamera(FreeGameplay.camera);
   
end

function FreeGameplay.UpdateKeyAcceleration(axis)
    local input = FreeGameplay.key;
    if (input.leadToward[axis] == 0.0 and (input.speed[axis] * input.prevSpeed[axis] < 0.0)) then
        input.acceleration[axis] = 0.0;
        input.speed[axis] = 0.0;
    else
        if (input.leadToward[axis] - input.speed[axis] > 0.0) then
            input.acceleration[axis] = input.maxAcceleration;
        elseif (input.speed[axis] - input.leadToward[axis] > 0.0) then
            input.acceleration[axis] = -input.maxAcceleration;
        end
    end
end

function FreeGameplay.OnEvent()
    if (event == "KEY_PRESSED") then
        if (arg1 == KEY_W) then
            FreeGameplay.key.leadToward.z = FreeGameplay.key.leadToward.z - 1;
        elseif (arg1 == KEY_S) then
            FreeGameplay.key.leadToward.z = FreeGameplay.key.leadToward.z + 1;
        elseif (arg1 == KEY_A) then
            FreeGameplay.key.leadToward.x = FreeGameplay.key.leadToward.x - 1;
        elseif (arg1 == KEY_D) then
            FreeGameplay.key.leadToward.x = FreeGameplay.key.leadToward.x + 1;
        elseif (arg1 == KEY_Q) then
            FreeGameplay.key.leadToward.y = FreeGameplay.key.leadToward.y + 1;
        elseif (arg1 == KEY_E) then
            FreeGameplay.key.leadToward.y = FreeGameplay.key.leadToward.y - 1;
        end
    elseif (event == "KEY_RELEASED") then
        if (arg1 == KEY_W) then
            FreeGameplay.key.leadToward.z = FreeGameplay.key.leadToward.z + 1;
        elseif (arg1 == KEY_S) then
            FreeGameplay.key.leadToward.z = FreeGameplay.key.leadToward.z - 1;
        elseif (arg1 == KEY_A) then
            FreeGameplay.key.leadToward.x = FreeGameplay.key.leadToward.x + 1;
        elseif (arg1 == KEY_D) then
            FreeGameplay.key.leadToward.x = FreeGameplay.key.leadToward.x - 1;
        elseif (arg1 == KEY_Q) then
            FreeGameplay.key.leadToward.y = FreeGameplay.key.leadToward.y - 1;
        elseif (arg1 == KEY_E) then
            FreeGameplay.key.leadToward.y = FreeGameplay.key.leadToward.y + 1;
        end
    elseif (event == "MOUSE_MOVED_SMOOTH") then
        if (FreeGameplay.mouse.rightPressed) then
            FreeGameplay.camera:Yaw(-arg3);
            FreeGameplay.camera:Pitch(-arg4);
        end
    elseif (event == "MOUSE_PRESSED") then
        if (arg1 == MOUSE_RIGHT) then
            FreeGameplay.mouse.rightPressed = true;
        end
        Frost:ForceWorldClicksAllowed(true);
    elseif (event == "MOUSE_RELEASED") then
        if (arg1 == MOUSE_RIGHT) then
            FreeGameplay.mouse.rightPressed = false;
        end
        Frost:ForceWorldClicksAllowed(false);
    end
end

function FreeGameplay.OnUpdate()
    FreeGameplay.key.prevSpeed = FreeGameplay.key.speed;
    FreeGameplay.key.speed = FreeGameplay.key.speed + arg1*FreeGameplay.key.acceleration;
    
    FreeGameplay.UpdateKeyAcceleration("x");
    FreeGameplay.UpdateKeyAcceleration("y");
    FreeGameplay.UpdateKeyAcceleration("z");

    FreeGameplay.camera:Translate(arg1*FreeGameplay.key.maxSpeed*FreeGameplay.key.speed, true);
end
