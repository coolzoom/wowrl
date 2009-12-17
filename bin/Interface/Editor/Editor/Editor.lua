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

function AddOns.Editor:RegisterTextColorFunction(object, func)
    if (not self.textColorFuncList) then
        self.textColorFuncList = {};
    end
    
    self.textColorFuncList[object] = func;
end

function AddOns.Editor:RegisterSecondaryTextColorFunction(object, func)
    if (not self.sndTextColorFuncList) then
        self.sndTextColorFuncList = {};
    end
    
    self.sndTextColorFuncList[object] = func;
end

function AddOns.Editor:RegisterBackgroundColorFunction(object, func)
    if (not self.bgColorFuncList) then
        self.bgColorFuncList = {};
    end
    
    self.bgColorFuncList[object] = func;
end
