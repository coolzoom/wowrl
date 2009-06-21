FirstPersonGameplay = {
    ["camera"] = nil,                -- The camera handled by this gameplay
    ["defaultCamera"] = nil,         -- The camera to use if something goes wrong
    ["maxSpeed"] = 20,               -- The camera movement speed
    ["translate"] = Vector(0, 0, 0), -- Translation vector
    ["MRPressed"] = false,           -- 'true' if the mouse right button is pressed
    ["MLPressed"] = false,           -- 'true' if the mouse left button is pressed
    ["unit"] = nil,                  -- The followed unit
    ["ready"] = false,               -- 'true' if this gameplay can be used
    
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
    this:RegisterEvent("MOUSE_MOVED");
    this:RegisterEvent("MOUSE_PRESSED");
    this:RegisterEvent("MOUSE_RELEASED");
    this:RegisterEvent("MOUSE_WHEEL");
    
    FirstPersonGameplay.defaultCamera = this:CreateCamera();
end

function FirstPersonGameplay.OnEnabled()
    FirstPersonGameplay.ready = true;
    
    -- Unit selection is not available for now, so we just pick the first unit
    FirstPersonGameplay.unit = U_0;
    
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
    
    if (FirstPersonGameplay.ready) then
        FirstPersonGameplay.camera = FirstPersonGameplay.unit:GetCamera();
        this:SetCamera(FirstPersonGameplay.camera);
    end
end

function FirstPersonGameplay.OnEvent()
    if (FirstPersonGameplay.ready) then
        if (event == "KEY_PRESSED") then
            if (arg1 == KEY_W) then
                FirstPersonGameplay.unit:SetMoveForward(true);
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
                FirstPersonGameplay.unit:SetMoveForward(false);
            elseif (arg1 == KEY_S) then
                FirstPersonGameplay.unit:SetMoveBackward(false);
            elseif (arg1 == KEY_A) then
                FirstPersonGameplay.unit:SetMoveLeft(false);
            elseif (arg1 == KEY_D) then
                FirstPersonGameplay.unit:SetMoveRight(false);
            end
        elseif (event == "MOUSE_MOVED") then
            if (FirstPersonGameplay.MLPressed) then
                FirstPersonGameplay.unit:RotateCamera(-arg3, -arg4);
            elseif (FirstPersonGameplay.MRPressed) then
                FirstPersonGameplay.unit:RotateModel(-arg3, -arg4);
            end
        elseif (event == "MOUSE_PRESSED") then
            if (arg1 == MOUSE_RIGHT) then
                FirstPersonGameplay.unit:ToggleTurning();
                FirstPersonGameplay.unit:RotateModel(0, 0);
                FirstPersonGameplay.MRPressed = true;
            elseif (arg1 == MOUSE_LEFT) then
                FirstPersonGameplay.MLPressed = true;
            end
        elseif (event == "MOUSE_RELEASED") then
            if (arg1 == MOUSE_RIGHT) then
                FirstPersonGameplay.unit:ToggleTurning();
                FirstPersonGameplay.MRPressed = false;
            elseif (arg1 == MOUSE_LEFT) then
                FirstPersonGameplay.MLPressed = false;
            end
        elseif (event == "MOUSE_WHEEL") then
            FirstPersonGameplay.unit:ZoomCamera(-arg1/120);
        end
    end
end
