UIParent:SetFrameStrata("BACKGROUND");
UIParent:EnableMouse(true);
UIParent:SetScript("OnMouseUp", function ()
    if (arg1 == "LeftButton") then
        if (this.activeDropDown) then
            this.activeDropDown:Hide();
            this.activeDropDown = nil;
        end
    elseif (arg1 == "RightButton") then
        -- Handle right clicking the 3D view
    end
end);
