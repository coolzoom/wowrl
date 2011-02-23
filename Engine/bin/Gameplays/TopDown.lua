TopDownGameplay = {
    ["camera"] = nil,                       -- The camera handled by this gameplay

    ["key"] = {
        ["leadToward"] = Vector(0, 0, 0),   -- The direction vector
        ["acceleration"] = Vector(0, 0, 0), -- The acceleration vector
        ["speed"] = Vector(0, 0, 0),        -- The speed vector
        ["prevSpeed"] = Vector(0, 0, 0),    -- The previous speed vector
        ["maxSpeed"] = 20.0,                -- The camera's max speed
        ["maxAcceleration"] = 5.0,          -- The camera's max acceleration
    },
    
    ["mouse"] = {
        ["rightPressed"] = false,           -- 'true' if the mouse right button is pressed
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
    this:RegisterEvent("MOUSE_MOVED_SMOOTH");
    this:RegisterEvent("MOUSE_PRESSED");
    this:RegisterEvent("MOUSE_RELEASED");
    this:RegisterEvent("MOUSE_WHEEL_SMOOTH");
    
    TopDownGameplay.camera = this:CreateCamera(2, 8, 2);
    TopDownGameplay.camera:OrbitAround(0, 0, 0);
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
    elseif (event == "MOUSE_MOVED_SMOOTH") then
        if (TopDownGameplay.mouse.rightPressed) then
            TopDownGameplay.camera:Yaw(-arg3);
            TopDownGameplay.camera:Pitch(-arg4);
        end
    elseif (event == "MOUSE_PRESSED") then
        if (arg1 == MOUSE_RIGHT) then
            TopDownGameplay.mouse.rightPressed = true;
        end
        Frost:ForceWorldClicksAllowed(true);
    elseif (event == "MOUSE_RELEASED") then
        if (arg1 == MOUSE_RIGHT) then
            TopDownGameplay.mouse.rightPressed = false;
        end
        Frost:ForceWorldClicksAllowed(false);
    elseif (event == "MOUSE_WHEEL_SMOOTH") then
        TopDownGameplay.camera:Translate(-2.0*arg1*Vector.UNIT_Z, true);
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

function TopDownGameplay.OnUpdate()
    TopDownGameplay.key.prevSpeed = TopDownGameplay.key.speed;
    TopDownGameplay.key.speed = TopDownGameplay.key.speed + arg1*TopDownGameplay.key.acceleration;
    
    TopDownGameplay.UpdateKeyAcceleration("x");
    TopDownGameplay.UpdateKeyAcceleration("z");
    
    local e1 = TopDownGameplay.camera:GetDirection(); e1.y = 0; e1:Normalize();
    local e2 = e1 ^ Vector.UNIT_Y;
    
    TopDownGameplay.camera:TranslateOrbitNode(arg1*TopDownGameplay.key.maxSpeed*(TopDownGameplay.key.speed.z*e1 + TopDownGameplay.key.speed.x*e2));
end
