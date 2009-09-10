TopDownGameplay = {
    ["camera"] = nil,                -- The camera handled by this gameplay
    ["mouseRightPressed"] = false,   -- 'true' if the mouse right button is pressed
    
    ["maxSpeed"] = 20,               -- The camera movement speed
    ["translate"] = Vector(0, 0, 0), -- Translation vector
    
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
        ["lastWheel"] = 0,
        ["averagedYaw"] = 0,
        ["averagedPitch"] = 0,
        ["averagedWheel"] = 0,
        ["yawHistoric"] = {},
        ["pitchHistoric"] = {},
        ["wheelHistoric"] = {},
        
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
-- [MouseRight] : Yaw and pitch the camera when dragged
-- [MouseWheel] : Zoom in and out
-----------------------------------

function TopDownGameplay.OnLoad()
    this:RegisterEvent("KEY_PRESSED");
    this:RegisterEvent("KEY_RELEASED");
    this:RegisterEvent("MOUSE_MOVED");
    this:RegisterEvent("MOUSE_PRESSED");
    this:RegisterEvent("MOUSE_RELEASED");
    this:RegisterEvent("MOUSE_WHEEL");
    
    TopDownGameplay.camera = this:CreateCamera(2, 8, 2);
    TopDownGameplay.camera:OrbitAround(0, 0, 0, true);
    this:SetCamera(TopDownGameplay.camera);
end

function TopDownGameplay.OnEvent()
    if (event == "KEY_PRESSED") then
        if (arg1 == KEY_W) then
            TopDownGameplay.key.leadToward.z = TopDownGameplay.key.leadToward.z + 1;
        elseif (arg1 == KEY_S) then
            TopDownGameplay.key.leadToward.z = TopDownGameplay.key.leadToward.z - 1;
        elseif (arg1 == KEY_A) then
            TopDownGameplay.key.leadToward.x = TopDownGameplay.key.leadToward.x - 1;
        elseif (arg1 == KEY_D) then
            TopDownGameplay.key.leadToward.x = TopDownGameplay.key.leadToward.x + 1;
        end
    elseif (event == "KEY_RELEASED") then
        if (arg1 == KEY_W) then
            TopDownGameplay.key.leadToward.z = TopDownGameplay.key.leadToward.z - 1;
        elseif (arg1 == KEY_S) then
            TopDownGameplay.key.leadToward.z = TopDownGameplay.key.leadToward.z + 1;
        elseif (arg1 == KEY_A) then
            TopDownGameplay.key.leadToward.x = TopDownGameplay.key.leadToward.x + 1;
        elseif (arg1 == KEY_D) then
            TopDownGameplay.key.leadToward.x = TopDownGameplay.key.leadToward.x - 1;
        end
    elseif (event == "MOUSE_MOVED") then
        if (TopDownGameplay.mouseRightPressed) then
            TopDownGameplay.mouse.lastYaw = arg3;
            TopDownGameplay.mouse.lastPitch = arg4;
        end
    elseif (event == "MOUSE_PRESSED") then
        if (arg1 == MOUSE_RIGHT) then
            TopDownGameplay.mouseRightPressed = true;
        end
    elseif (event == "MOUSE_RELEASED") then
        if (arg1 == MOUSE_RIGHT) then
            TopDownGameplay.mouseRightPressed = false;
        end
    elseif (event == "MOUSE_WHEEL") then
        --TopDownGameplay.camera:Translate(-arg1/120*Vector.UNIT_Z, true);
        TopDownGameplay.mouse.lastWheel = arg1/120;
    end
end

function TopDownGameplay.UpdateKeyAcceleration(axis)
    local input = TopDownGameplay.key;
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

function TopDownGameplay.UpdateMouseMovement()
    local input = TopDownGameplay.mouse;
    
    -- Yaw
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
    
    -- Pitch
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
    
    -- Wheel
    table.insert(input.wheelHistoric, 1, input.lastWheel);
    local wheelNbr = table.maxn(input.wheelHistoric);
    if (wheelNbr > input.historicLength) then
        table.remove(input.wheelHistoric);
        wheelNbr = wheelNbr - 1;
    end
    
    factor = 1;
    totalWeight = 0;
    
    input.averagedWheel = 0;
    for i=1, wheelNbr do
        input.averagedWheel = input.averagedWheel + input.wheelHistoric[i]*factor;
        totalWeight = totalWeight + factor;
        factor = factor * input.historicSmoothFactor;
    end
    input.averagedWheel = input.averagedWheel / totalWeight;
end

function TopDownGameplay.OnUpdate()
    TopDownGameplay.UpdateMouseMovement();
    
    TopDownGameplay.mouse.lastYaw = 0;
    TopDownGameplay.mouse.lastPitch = 0;
    TopDownGameplay.mouse.lastWheel = 0;
    
    TopDownGameplay.camera:Yaw(-TopDownGameplay.mouse.averagedYaw);
    TopDownGameplay.camera:Pitch(-TopDownGameplay.mouse.averagedPitch);
    TopDownGameplay.camera:Translate(-2.0*TopDownGameplay.mouse.averagedWheel*Vector.UNIT_Z, true);
    
    TopDownGameplay.key.prevSpeed = TopDownGameplay.key.speed;
    TopDownGameplay.key.speed = TopDownGameplay.key.speed + arg1*TopDownGameplay.key.acceleration;
    
    TopDownGameplay.UpdateKeyAcceleration("x");
    TopDownGameplay.UpdateKeyAcceleration("z");
    
    local e1 = TopDownGameplay.camera:GetDirection(); e1.y = 0; e1:Normalize();
    local e2 = e1 ^ Vector.UNIT_Y;
    
    TopDownGameplay.camera:TranslateOrbitNode(arg1*TopDownGameplay.key.maxSpeed*(TopDownGameplay.key.speed.z*e1 + TopDownGameplay.key.speed.x*e2));
end
