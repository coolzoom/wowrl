MenuBar:AddMenu("View");

MenuBar:AddMenuCheckItem("View", "Wireframe", "F2", false):SetScript("OnClick", function ()
    Editor:ToggleWireframeView();
    MenuBar:CloseCurrentDropDown();
end);
MenuBar:AddMenuCheckItem("View", "Shading", "F3", true):SetScript("OnClick", function ()
    Editor:ToggleShading();
    MenuBar:CloseCurrentDropDown();
end);
MenuBar:AddMenuCheckItem("View", "Decal", "F4", true):SetScript("OnClick", function ()
    if (AddOns.Editor:IsMouseDecalEnabled()) then
        AddOns.Editor:EnableMouseDecal(false);
    else
        AddOns.Editor:EnableMouseDecal(true);
    end
    MenuBar:CloseCurrentDropDown();
end);
MenuBar:AddMenuItem("View", "DecalColor"):SetScript("OnClick", function ()
    ColorSelector:SetColor(Color(Frost:GetMouseDecalColor()));
    ColorSelector:SetOnColorUpdateFunc(function (color)
        AddOns.Editor.Config.mouseDecalColor = Color(color);
        Frost:SetMouseDecalColor(color:Unpack());
    end);
    ColorSelector:Show();
    MenuBar:CloseCurrentDropDown();
end);
MenuBar:AddMenuItem("View", "BGColor"):SetScript("OnClick", function ()
    ColorSelector:SetColor(Color(Editor:GetBackgroundColor()));
    ColorSelector:SetOnColorUpdateFunc(function (color)
        AddOns.Editor.Config.worldBackgroundColor = Color(color);
        Editor:SetBackgroundColor(color:Unpack());
    end);
    ColorSelector:Show();
    MenuBar:CloseCurrentDropDown();
end);
