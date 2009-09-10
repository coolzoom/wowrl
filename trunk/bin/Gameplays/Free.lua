FreeGameplay = {
    ["camera"] = nil,                -- The camera handled by this gameplay
    ["mouseRightPressed"] = false,           -- 'true' if the mouse right button is pressed
    
    ["key"] = {
        ["leadToward"] = Vector(0, 0, 0),      -- The direction vector
        ["acceleration"] = Vector(0, 0, 0),    -- The acceleration vector
        ["speed"] = Vector(0, 0, 0),           -- The speed vector
        ["prevSpeed"] = Vector(0, 0, 0),       -- The previous speed vector
        ["maxSpeed"] = 20.0,                   -- The camera's max speed
        ["maxAcceleration"] = 5.0,             -- The camera's max acceleration
    },
    
    ["mouse"] = {
        ["lastYaw"] = 0,
        ["lastPitch"] = 0,
        ["averagedYaw"] = 0,
        ["averagedPitch"] = 0,
        ["yawHistoric"] = {},
        ["pitchHistoric"] = {},
        
        ["historicLength"] = 50,
        ["historicSmoothFactor"] = 1,
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
    this:RegisterEvent("MOUSE_MOVED");
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

function FreeGameplay.UpdateMouseMovement()
    local input = FreeGameplay.mouse;
    
    table.insert(input.yawHistoric, 1, input.lastYaw);
    local yawNbr = table.maxn(input.yawHistoric);
    if (yawNbr > input.historicLength) then
        table.remove(input.yawHistoric);
        yawNbr = yawNbr - 1;
    end
    
    local factor = 1;
    local totalWeight = 0;
    
    input.averagedYaw = 0;
    for i=1, yawNbr do
        input.averagedYaw = input.averagedYaw + input.yawHistoric[i]*factor;
        totalWeight = totalWeight + factor;
        factor = factor * input.historicSmoothFactor;
    end
    input.averagedYaw = input.averagedYaw / totalWeight;
    
    table.insert(input.pitchHistoric, 1, input.lastPitch);
    local pitchNbr = table.maxn(input.pitchHistoric);
    if (pitchNbr > input.historicLength) then
        table.remove(input.pitchHistoric);
        pitchNbr = pitchNbr - 1;
    end
    
    factor = 1;
    totalWeight = 0;
    
    input.averagedPitch = 0;
    for i=1, pitchNbr do
        input.averagedPitch = input.averagedPitch + input.pitchHistoric[i]*factor;
        totalWeight = totalWeight + factor;
        factor = factor * input.historicSmoothFactor;
    end
    input.averagedPitch = input.averagedPitch / totalWeight;
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
    elseif (event == "MOUSE_MOVED") then
        if (FreeGameplay.mouseRightPressed) then
            FreeGameplay.mouse.lastYaw = arg3;
            FreeGameplay.mouse.lastPitch = arg4;
        end
    elseif (event == "MOUSE_PRESSED") then
        if (arg1 == MOUSE_RIGHT) then
            FreeGameplay.mouseRightPressed = true;
        end
    elseif (event == "MOUSE_RELEASED") then
        if (arg1 == MOUSE_RIGHT) then
            FreeGameplay.mouseRightPressed = false;
        end
    end
end

function FreeGameplay.OnUpdate()
    FreeGameplay.UpdateMouseMovement();
    
    FreeGameplay.mouse.lastYaw = 0;
    FreeGameplay.mouse.lastPitch = 0;
    
    FreeGameplay.camera:Yaw(-FreeGameplay.mouse.averagedYaw);
    FreeGameplay.camera:Pitch(-FreeGameplay.mouse.averagedPitch);

    FreeGameplay.key.prevSpeed = FreeGameplay.key.speed;
    FreeGameplay.key.speed = FreeGameplay.key.speed + arg1*FreeGameplay.key.acceleration;
    
    FreeGameplay.UpdateKeyAcceleration("x");
    FreeGameplay.UpdateKeyAcceleration("y");
    FreeGameplay.UpdateKeyAcceleration("z");

    FreeGameplay.camera:Translate(arg1*FreeGameplay.key.maxSpeed*FreeGameplay.key.speed, true);
end
