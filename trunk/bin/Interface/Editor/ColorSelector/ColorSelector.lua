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
    ColorSelector.Swatch.ColorZone:SetBackdropColor(ColorSelector.currentColor:Unpack());
    
    if (onColorUpdate) then
        onColorUpdate(ColorSelector.currentColor);
    end
end