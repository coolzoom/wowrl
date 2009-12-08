UIParent:SetFrameStrata("BACKGROUND");
UIParent:EnableMouse(true);
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
