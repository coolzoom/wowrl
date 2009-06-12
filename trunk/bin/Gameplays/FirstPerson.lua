FirstPersonGameplay = {
    ["camera"] = nil,      -- The camera handled by this gameplay
    ["maxSpeed"] = 20,     -- The camera movement speed
    ["translateX"] = 0,    -- Negative X is "left", positive is "right"
    ["translateY"] = 0,    -- Negative Y is "down", positive is "up"
    ["translateZ"] = 0,    -- Negative Z is "forward", positive is "backward"
    ["MRPressed"] = false, -- 'true' if the mouse right button is pressed
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

function FirstPersonGameplay.OnLoad()
    this:RegisterEvent("KEY_PRESSED");
    this:RegisterEvent("KEY_RELEASED");
    this:RegisterEvent("MOUSE_MOVED");
    this:RegisterEvent("MOUSE_PRESSED");
    this:RegisterEvent("MOUSE_RELEASED");
    FirstPersonGameplay.camera = this:CreateCamera(3, 4, 3);
    this:SetCamera(FirstPersonGameplay.camera);
end

function FirstPersonGameplay.OnEvent()
    if (event == "KEY_PRESSED") then
        if (arg1 == KEY_W) then
            FirstPersonGameplay.translateZ = FirstPersonGameplay.translateZ - 1;
        elseif (arg1 == KEY_S) then
            FirstPersonGameplay.translateZ = FirstPersonGameplay.translateZ + 1;
        elseif (arg1 == KEY_A) then
            FirstPersonGameplay.translateX = FirstPersonGameplay.translateX - 1;
        elseif (arg1 == KEY_D) then
            FirstPersonGameplay.translateX = FirstPersonGameplay.translateX + 1;
        elseif (arg1 == KEY_Q) then
            FirstPersonGameplay.translateX = FirstPersonGameplay.translateY + 1;
        elseif (arg1 == KEY_E) then
            FirstPersonGameplay.translateX = FirstPersonGameplay.translateY - 1;
        end
    elseif (event == "KEY_RELEASED") then
        if (arg1 == KEY_W) then
            FirstPersonGameplay.translateZ = FirstPersonGameplay.translateZ + 1;
        elseif (arg1 == KEY_S) then
            FirstPersonGameplay.translateZ = FirstPersonGameplay.translateZ - 1;
        elseif (arg1 == KEY_A) then
            FirstPersonGameplay.translateX = FirstPersonGameplay.translateX + 1;
        elseif (arg1 == KEY_D) then
            FirstPersonGameplay.translateX = FirstPersonGameplay.translateX - 1;
        elseif (arg1 == KEY_Q) then
            FirstPersonGameplay.translateX = FirstPersonGameplay.translateY - 1;
        elseif (arg1 == KEY_E) then
            FirstPersonGameplay.translateX = FirstPersonGameplay.translateY + 1;
        end
    elseif (event == "MOUSE_MOVED") then
        if (FirstPersonGameplay.MRPressed) then
            FirstPersonGameplay.camera:Yaw(-arg3);
            FirstPersonGameplay.camera:Pitch(-arg4);
        end
    elseif (event == "MOUSE_PRESSED") then
        if (arg1 == MOUSE_RIGHT) then
            FirstPersonGameplay.MRPressed = true;
        end
    elseif (event == "MOUSE_RELEASED") then
        if (arg1 == MOUSE_RIGHT) then
            FirstPersonGameplay.MRPressed = false;
        end
    end
end

function FirstPersonGameplay.OnUpdate()
    local speed = maxSpeed*math.sqrt(FirstPersonGameplay.translateX*FirstPersonGameplay.translateX + FirstPersonGameplay.translateY*FirstPersonGameplay.translateY + FirstPersonGameplay.translateZ*FirstPersonGameplay.translateZ);
    if (speed ~= 0) then
        FirstPersonGameplay.camera:Translate(
            FirstPersonGameplay.translateX*speed,
            FirstPersonGameplay.translateY*speed,
            FirstPersonGameplay.translateZ*speed
        );
    end
end
