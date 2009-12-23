UIParent:SetFrameStrata("BACKGROUND");
UIParent:EnableMouse(true, true);

UIParent:SetScript("OnMouseUp", function ()
    if (arg1 == "LeftButton") then
        if (this.activeDropdown) then
            this.activeDropdown:Hide();
            this.activeDropdown = nil;
        end
    elseif (arg1 == "RightButton") then
        -- Handle right clicking the 3D view
    end
end);

function AddOns.Editor:NotifyDataChanged()
    self.dataSaved = false;
end

function AddOns.Editor:NotifyDataSaved()
    self.dataSaved = true;
end

function AddOns.Editor:IsDataSaved()
    if (self.dataSaved == nil) then
        self.dataSaved = true;
    end
    return self.dataSaved;
end

function AddOns.Editor:EnableMouseDecal(enabled)
    if (self.mouseDecalEnabled == nil) then
        self.mouseDecalEnabled = false;
    end
    
    if (self.mouseDecalEnabled ~= enabled) then
        if (enabled) then
            Frost:EnableMouseDecal(true,
                "Textures/EditorCrosshair.png", 5, self.Config.mouseDecalColor:Unpack()
            );
        else
            Frost:EnableMouseDecal(false);
        end
        
        self.mouseDecalEnabled = enabled;
    end
end

function AddOns.Editor:IsMouseDecalEnabled()
    if (self.mouseDecalEnabled == nil) then
        self.mouseDecalEnabled = true;
    end
    return self.mouseDecalEnabled;
end

function AddOns.Editor:SetCurrentDropdown(dropdown)
    if (self.activeDropdown) then
        if (self.activeDropdown ~= dropdown) then
            self.activeDropdown:Hide();
            self.activeDropdown = dropdown;
            self.activeDropdown:Show();
        else
            self.activeDropdown:Hide();
            self.activeDropdown = nil;
        end
    else
        self.activeDropdown = dropdown;
        self.activeDropdown:Show();
    end
end

function AddOns.Editor:CloseCurrentDropdown()
    if (self.activeDropdown) then
        self.activeDropdown:Hide();
        self.activeDropdown = nil;
    end
end

function AddOns.Editor:RegisterTextColorFunction(object, func)
    if (self.textColorFuncList == nil) then
        self.textColorFuncList = {};
    end
    
    self.textColorFuncList[object] = func;
end

function AddOns.Editor:RegisterSecondaryTextColorFunction(object, func)
    if (self.sndTextColorFuncList == nil) then
        self.sndTextColorFuncList = {};
    end
    
    self.sndTextColorFuncList[object] = func;
end

function AddOns.Editor:RegisterBackgroundColorFunction(object, func)
    if (self.bgColorFuncList == nil) then
        self.bgColorFuncList = {};
    end
    
    self.bgColorFuncList[object] = func;
end
