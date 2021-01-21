FirstPersonGameplay = {
    ["camera"] = nil,                -- The camera handled by this gameplay
    ["defaultCamera"] = nil,         -- The camera to use if something goes wrong
    ["translate"] = Vector(0, 0, 0), -- Translation vector

    ["unit"] = nil,                  -- The followed unit
    ["ready"] = false,               -- 'true' if this gameplay can be used
    
    ["key"] = {
        ["mouseForwardPressed"] = false, -- 'true' if the "move forward" key is pressed
    },
    
    ["mouse"] = {
        ["leftPressed"] = false,     -- 'true' if the mouse left button is pressed
        ["rightPressed"] = false,    -- 'true' if the mouse right button is pressed
        ["bothPressed"] = false,     -- 'true' if both mouse button are pressed
    },
};

-----------------------------------
-- CONTROLS : (only moves a camera)
-----------------------------------
-- [W] : Move unit forward
-- [S] : Move unit backward
-- [A] : Strafe unit left / turn unit left
-- [D] : Strafe unit right / turn unit right
-- [Space] : Make the unit jump
-- [Divide (num pad "/")] : Toggle walking/running
-- [MouseLeft] : Yaw and pitch the unit and its camera when dragged
-- [MouseRight] : Yaw and pitch the camera when dragged
-- [MouseWheel] : Zoom the unit's camera in and out
-----------------------------------

function FirstPersonGameplay.OnLoad()
    this:RegisterEvent("KEY_PRESSED");
    this:RegisterEvent("KEY_RELEASED");
    this:RegisterEvent("MOUSE_MOVED_SMOOTH");
    this:RegisterEvent("MOUSE_PRESSED");
    this:RegisterEvent("MOUSE_RELEASED");
    this:RegisterEvent("MOUSE_WHEEL_SMOOTH");
    
    FirstPersonGameplay.defaultCamera = this:CreateCamera();
end

function FirstPersonGameplay.SetUnit(unit)
    if (FirstPersonGameplay.unit ~= nil) then
        FirstPersonGameplay.unit:EnableMotionBlur();
        FirstPersonGameplay.unit:SetPlayerControlled(false);
    end
    
    FirstPersonGameplay.unit = unit;
    
    if (FirstPersonGameplay.unit ~= nil) then
        FirstPersonGameplay.unit:DisableMotionBlur();
        FirstPersonGameplay.unit:SetPlayerControlled(true);
        FirstPersonGameplay.camera = FirstPersonGameplay.unit:GetCamera();
        this:SetCamera(FirstPersonGameplay.camera);
        FirstPersonGameplay.ready = true;
    else
        this:SetCamera(nil);
        FirstPersonGameplay.ready = false;
    end
end

function FirstPersonGameplay.OnEnabled()

    -- Unit selection is not available for now, so we just pick the first unit
    if (FirstPersonGameplay.unit == nil) then
        FirstPersonGameplay.SetUnit(U_0);
    else
        FirstPersonGameplay.unit:DisableMotionBlur();
    end
    
    --[[
    -- First try to follow the selected unit
    FirstPersonGameplay.unit = UnitManager:GetSelectedUnit("FRIENDLY");
    if (FirstPersonGameplay.unit == nil) then
        -- If there is none, try to follow the nearest alive unit
        FirstPersonGameplay.unit = UnitManager:GetNearestUnit(FirstPersonGameplay.camera:GetPosition(), "FRIENDLY, ALIVE");
        if (FirstPersonGameplay.unit == nil) then
            -- If none, try to follow the nearest dead unit
            FirstPersonGameplay.unit = UnitManager:GetNearestUnit(FirstPersonGameplay.camera:GetPosition(), "FRIENDLY");
            if (FirstPersonGameplay.unit == nil) then
                -- If none, you have no unit... You can't use this gameplay.
                FirstPersonGameplay.ready = false;
                this:SetCamera(FirstPersonGameplay.defaultCamera);
            end
        end
    end
    ]]--
end

function FirstPersonGameplay.OnDisabled()
    if (FirstPersonGameplay.unit ~= nil) then
        FirstPersonGameplay.unit:EnableMotionBlur();
    end
end

function FirstPersonGameplay.OnEvent()
    if (FirstPersonGameplay.ready) then
        if (event == "KEY_PRESSED") then
            if (arg1 == KEY_W) then
                if (not FirstPersonGameplay.mouse.bothPressed) then
                    FirstPersonGameplay.unit:SetMoveForward(true);
                end
                FirstPersonGameplay.key.mouseForwardPressed = true;
            elseif (arg1 == KEY_S) then
                FirstPersonGameplay.unit:SetMoveBackward(true);
            elseif (arg1 == KEY_A) then
                FirstPersonGameplay.unit:SetMoveLeft(true);
            elseif (arg1 == KEY_D) then
                FirstPersonGameplay.unit:SetMoveRight(true);
            elseif (arg1 == KEY_SPACE) then
                FirstPersonGameplay.unit:Jump();
            elseif (arg1 == KEY_DIVIDE) then
                FirstPersonGameplay.unit:ToggleWalking();
            end
        elseif (event == "KEY_RELEASED") then
            if (arg1 == KEY_W) then
                if (not FirstPersonGameplay.mouse.bothPressed) then
                    FirstPersonGameplay.unit:SetMoveForward(false);
                end
                FirstPersonGameplay.key.mouseForwardPressed = false;
            elseif (arg1 == KEY_S) then
                FirstPersonGameplay.unit:SetMoveBackward(false);
            elseif (arg1 == KEY_A) then
                FirstPersonGameplay.unit:SetMoveLeft(false);
            elseif (arg1 == KEY_D) then
                FirstPersonGameplay.unit:SetMoveRight(false);
            end
        elseif (event == "MOUSE_MOVED_SMOOTH") then
            if (FirstPersonGameplay.mouse.rightPressed) then
                FirstPersonGameplay.unit:RotateModel(-arg3, -arg4);
            elseif (FirstPersonGameplay.mouse.leftPressed) then
                FirstPersonGameplay.unit:RotateCamera(-arg3, -arg4);
            end
        elseif (event == "MOUSE_PRESSED") then
            if (arg1 == MOUSE_RIGHT) then
                FirstPersonGameplay.unit:ToggleTurning();
                FirstPersonGameplay.unit:RotateModel(0, 0);
                FirstPersonGameplay.mouse.rightPressed = true;
                if (FirstPersonGameplay.mouse.leftPressed and not FirstPersonGameplay.key.mouseForwardPressed) then
                    FirstPersonGameplay.mouse.bothPressed = true;
                    FirstPersonGameplay.unit:SetMoveForward(true);
                end
            elseif (arg1 == MOUSE_LEFT) then
                FirstPersonGameplay.mouse.leftPressed = true;
                if (FirstPersonGameplay.mouse.rightPressed and not FirstPersonGameplay.key.mouseForwardPressed) then
                    FirstPersonGameplay.mouse.bothPressed = true;
                    FirstPersonGameplay.unit:SetMoveForward(true);
                end
            end
            Frost:ForceWorldClicksAllowed(true);
        elseif (event == "MOUSE_RELEASED") then
            if (FirstPersonGameplay.mouse.bothPressed and not FirstPersonGameplay.key.mouseForwardPressed) then
                FirstPersonGameplay.unit:SetMoveForward(false);
            end
            FirstPersonGameplay.mouse.bothPressed = false;
            if (arg1 == MOUSE_RIGHT) then
                FirstPersonGameplay.unit:ToggleTurning();
                FirstPersonGameplay.mouse.rightPressed = false;
            elseif (arg1 == MOUSE_LEFT) then
                FirstPersonGameplay.mouse.leftPressed = false;
            end
            Frost:ForceWorldClicksAllowed(false);
        elseif (event == "MOUSE_WHEEL_SMOOTH") then
            FirstPersonGameplay.unit:ZoomCamera(-arg1);
        end
    end
end

function FirstPersonGameplay.OnUpdate()

end
