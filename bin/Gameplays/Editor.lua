EditorGameplay = {
    ["camera"] = nil,                -- The camera handled by this gameplay
    ["maxSpeed"] = 20,               -- The camera movement speed
    ["translate"] = Vector(0, 0, 0), -- Translation vector
    ["MRPressed"] = false,           -- 'true' if the mouse right button is pressed
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

function EditorGameplay.OnLoad()
    this:RegisterEvent("KEY_PRESSED");
    this:RegisterEvent("KEY_RELEASED");
    this:RegisterEvent("MOUSE_MOVED");
    this:RegisterEvent("MOUSE_PRESSED");
    this:RegisterEvent("MOUSE_RELEASED");
    
    EditorGameplay.camera = this:CreateCamera(3, 4, 3);
    EditorGameplay.camera:SetDirection(-3, -4, -3);
    this:SetCamera(EditorGameplay.camera); 
end

function EditorGameplay.OnEvent()
    if (event == "KEY_PRESSED") then
        if (arg1 == KEY_W) then
            EditorGameplay.translate.z = EditorGameplay.translate.z - 1;
        elseif (arg1 == KEY_S) then
            EditorGameplay.translate.z = EditorGameplay.translate.z + 1;
        elseif (arg1 == KEY_A) then
            EditorGameplay.translate.x = EditorGameplay.translate.x - 1;
        elseif (arg1 == KEY_D) then
            EditorGameplay.translate.x = EditorGameplay.translate.x + 1;
        elseif (arg1 == KEY_Q) then
            EditorGameplay.translate.y = EditorGameplay.translate.y + 1;
        elseif (arg1 == KEY_E) then
            EditorGameplay.translate.y = EditorGameplay.translate.y - 1;
        end
    elseif (event == "KEY_RELEASED") then
        if (arg1 == KEY_W) then
            EditorGameplay.translate.z = EditorGameplay.translate.z + 1;
        elseif (arg1 == KEY_S) then
            EditorGameplay.translate.z = EditorGameplay.translate.z - 1;
        elseif (arg1 == KEY_A) then
            EditorGameplay.translate.x = EditorGameplay.translate.x + 1;
        elseif (arg1 == KEY_D) then
            EditorGameplay.translate.x = EditorGameplay.translate.x - 1;
        elseif (arg1 == KEY_Q) then
            EditorGameplay.translate.y = EditorGameplay.translate.y - 1;
        elseif (arg1 == KEY_E) then
            EditorGameplay.translate.y = EditorGameplay.translate.y + 1;
        end
    elseif (event == "MOUSE_MOVED") then
        if (EditorGameplay.MRPressed) then
            EditorGameplay.camera:Yaw(-arg3);
            EditorGameplay.camera:Pitch(-arg4);
        end
    elseif (event == "MOUSE_PRESSED") then
        if (arg1 == MOUSE_RIGHT) then
            EditorGameplay.MRPressed = true;
        end
    elseif (event == "MOUSE_RELEASED") then
        if (arg1 == MOUSE_RIGHT) then
            EditorGameplay.MRPressed = false;
        end
    end
end

function EditorGameplay.OnUpdate()
    local norm = EditorGameplay.translate:GetNorm();
    if (norm ~= 0) then
        local speed = arg1*EditorGameplay.maxSpeed/norm;
        EditorGameplay.camera:Translate(speed*EditorGameplay.translate, true);
    end
end
