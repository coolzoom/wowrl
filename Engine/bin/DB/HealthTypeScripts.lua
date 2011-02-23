-- Contains functions used in the health types scripts.

function GetSpiritHealthRegenCoef( unit )
    if (unit) then
        local sClass = unit:GetClass();
        if (sClass == "DRUID") then
            return 0.09;
        elseif (sClass == "HUNTER") then
            return 0.25;
        elseif (sClass == "MAGE") then
            return 0.1;
        elseif (sClass == "PALADIN") then
            return 0.25;
        elseif (sClass == "PRIEST") then
            return 0.1;
        elseif (sClass == "ROGUE") then
            return 0.5;
        elseif (sClass == "SHAMAN") then
            return 0.11;
        elseif (sClass == "WARLOCK") then
            return 0.07;
        elseif (sClass == "WARRIOR") then
            return 0.50;
        else
            return 1.0;
        end
    end
end
