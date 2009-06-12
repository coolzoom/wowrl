TopDownGameplay = {
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
            TopDownGameplay.translate.z = TopDownGameplay.translate.z + 1;
        elseif (arg1 == KEY_S) then
            TopDownGameplay.translate.z = TopDownGameplay.translate.z - 1;
        elseif (arg1 == KEY_A) then
            TopDownGameplay.translate.x = TopDownGameplay.translate.x - 1;
        elseif (arg1 == KEY_D) then
            TopDownGameplay.translate.x = TopDownGameplay.translate.x + 1;
        end
    elseif (event == "KEY_RELEASED") then
        if (arg1 == KEY_W) then
            TopDownGameplay.translate.z = TopDownGameplay.translate.z - 1;
        elseif (arg1 == KEY_S) then
            TopDownGameplay.translate.z = TopDownGameplay.translate.z + 1;
        elseif (arg1 == KEY_A) then
            TopDownGameplay.translate.x = TopDownGameplay.translate.x + 1;
        elseif (arg1 == KEY_D) then
            TopDownGameplay.translate.x = TopDownGameplay.translate.x - 1;
        end
    elseif (event == "MOUSE_MOVED") then
        if (TopDownGameplay.MRPressed) then
            TopDownGameplay.camera:Yaw(-arg3);
            TopDownGameplay.camera:Pitch(-arg4);
        end
    elseif (event == "MOUSE_PRESSED") then
        if (arg1 == MOUSE_RIGHT) then
            TopDownGameplay.MRPressed = true;
        end
    elseif (event == "MOUSE_RELEASED") then
        if (arg1 == MOUSE_RIGHT) then
            TopDownGameplay.MRPressed = false;
        end
    elseif (event == "MOUSE_WHEEL") then
        TopDownGameplay.camera:Translate(-arg1/120*Vector.UNIT_Z, true);
    end
end

function TopDownGameplay.OnUpdate()
    local norm = TopDownGameplay.translate:GetNorm();
    if (norm ~= 0) then
        local speed = arg1*TopDownGameplay.maxSpeed/norm;
        
        local axis1 = TopDownGameplay.camera:GetDirection();
        axis1.y = 0; axis1:Normalize();
        
        local axis2 = axis1^Vector.UNIT_Y;

        TopDownGameplay.camera:TranslateOrbitNode(speed*(TopDownGameplay.translate.z*axis1 + TopDownGameplay.translate.x*axis2));
    end
end
