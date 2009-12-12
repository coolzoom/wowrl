-- AddOn class definition
AddOn = CreateClass(
    function (addon, name, version)
        addon.name = name;
        if (version ~= nil) then addon.version = version; end
    end
);

-- Member functions
function AddOn:GetInfos()
    local str = self.name;
    if (self.version ~= nil) then
        str = str..", version : "..self.version;
    end
    
    return str;
end

AddOns = {};
