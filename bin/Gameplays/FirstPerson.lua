FirstPersonGameplay = {
    ["camera"] = nil,                -- The camera handled by this gameplay
    ["defaultCamera"] = nil,         -- The camera to use if something goes wrong
    ["translate"] = Vector(0, 0, 0), -- Translation vector
    ["mouseLeftPressed"] = false,    -- 'true' if the mouse left button is pressed
    ["mouseRightPressed"] = false,   -- 'true' if the mouse right button is pressed
    ["unit"] = nil,                  -- The followed unit
    ["ready"] = false,               -- 'true' if this gameplay can be used
    
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

function FirstPersonGameplay.SetUnit(unit)
    if (FirstPersonGameplay.unit ~= nil) then
        FirstPersonGameplay.unit:EnableMotionBlur();
    end
    
    FirstPersonGameplay.unit = unit;
    
    if (FirstPersonGameplay.unit ~= nil) then
        FirstPersonGameplay.unit:DisableMotionBlur();
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
            if (FirstPersonGameplay.mouseLeftPressed or FirstPersonGameplay.mouseRightPressed) then
                FirstPersonGameplay.mouse.lastYaw = arg3;
                FirstPersonGameplay.mouse.lastPitch = arg4;
            end
        elseif (event == "MOUSE_PRESSED") then
            if (arg1 == MOUSE_RIGHT) then
                FirstPersonGameplay.unit:ToggleTurning();
                FirstPersonGameplay.unit:RotateModel(0, 0);
                FirstPersonGameplay.mouseRightPressed = true;
            elseif (arg1 == MOUSE_LEFT) then
                FirstPersonGameplay.mouseLeftPressed = true;
            end
        elseif (event == "MOUSE_RELEASED") then
            if (arg1 == MOUSE_RIGHT) then
                FirstPersonGameplay.unit:ToggleTurning();
                FirstPersonGameplay.mouseRightPressed = false;
            elseif (arg1 == MOUSE_LEFT) then
                FirstPersonGameplay.mouseLeftPressed = false;
            end
        elseif (event == "MOUSE_WHEEL") then
            FirstPersonGameplay.unit:ZoomCamera(-arg1/120);
        end
    end
end

function FirstPersonGameplay.UpdateMouseMovement()
    local input = FirstPersonGameplay.mouse;
    
    -- Yaw
    if (table.maxn(input.yawHistoric) == 0) then
        table.insert(input.yawHistoric, input.lastYaw);
    else
        table.insert(input.yawHistoric, 1, input.lastYaw);
    end
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
    if (table.maxn(input.pitchHistoric) == 0) then
        table.insert(input.pitchHistoric, input.lastPitch);
    else
        table.insert(input.pitchHistoric, 1, input.lastPitch);
    end
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

function FirstPersonGameplay.OnUpdate()
    FirstPersonGameplay.UpdateMouseMovement();
    
    FirstPersonGameplay.mouse.lastYaw = 0;
    FirstPersonGameplay.mouse.lastPitch = 0;
    
    if (FirstPersonGameplay.mouseLeftPressed) then
        FirstPersonGameplay.unit:RotateCamera(-FirstPersonGameplay.mouse.averagedYaw, -FirstPersonGameplay.mouse.averagedPitch);
    elseif (FirstPersonGameplay.mouseRightPressed) then
        FirstPersonGameplay.unit:RotateModel(-FirstPersonGameplay.mouse.averagedYaw, -FirstPersonGameplay.mouse.averagedPitch);
    end
end
