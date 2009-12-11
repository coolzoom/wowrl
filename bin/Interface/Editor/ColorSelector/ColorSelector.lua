local onColorUpdate = nil;

function ColorSelector:SetOnColorUpdateFunc(func)
    onColorUpdate = func;
end

function ColorSelector:SetColor(color)
   ColorSelector.SliderA.Slider:SetValue(color.a*255.0);
   ColorSelector.SliderR.Slider:SetValue(color.r*255.0);
   ColorSelector.SliderG.Slider:SetValue(color.g*255.0);
   ColorSelector.SliderB.Slider:SetValue(color.b*255.0);
end

function ColorSelector:OnColorUpdate()
    local temp = {};
    temp.a = ColorSelector.currentColor.a/255.0;
    temp.r = ColorSelector.currentColor.r/255.0;
    temp.g = ColorSelector.currentColor.g/255.0;
    temp.b = ColorSelector.currentColor.b/255.0;
    
    ColorSelector.Swatch.ColorZone:SetBackdropColor(UnpackColor(temp));
    
    if (onColorUpdate) then
        onColorUpdate(temp);
    end
end