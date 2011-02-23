
function ObjectList:BuildObjectList()
    if (self.initialized) then
        for i, object in pairs(self.objects) do
            AddOns.Editor:UnregisterColorFunctions(object);
            DeleteFrame(object);
        end
    else
        self.initialized = true;
    end
    
    self.objects = {};
    self.lastID = 0;
    self.objectNum = 0;
    self.lastObject = nil;
    self.currentObject = nil;
    self.folderScrollChildHeight = 10;
    
    local objectList = {Editor:GetObjectList("")};
    
    for i, object in pairs(objectList) do
        local objectButton = CreateFrame("Button", "$parentFolder"..(self.lastID + 1), self.List.Scroll, "ButtonTemplate_ObjectButton");
        if (objectButton) then
            self.lastID = self.lastID + 1;
            objectButton.id = self.lastID;
            objectButton.objectID = object.id;
            objectButton.object = object.name;

            objectButton:SetText(object.name);
            objectButton:GetNormalFontObject():SetWordWrap(false);

            if (self.lastObject) then
                objectButton:SetPoint("TOPLEFT", self.lastObject, "BOTTOMLEFT");
            else
                objectButton:SetPoint("RIGHT",   self.List.Scroll, "RIGHT",  -5, 0);
                objectButton:SetPoint("TOPLEFT", self.List.Scroll, "TOPLEFT", 5, 5);
            end
            
            objectButton.IconFrame.Icon:SetTexture("Interface/Editor/ObjectList/ObjectListIcons.png");

            if (object.type) then
                if (object.type == "Doodad") then
                    objectButton.IconFrame.Icon:SetTexCoord(0.125, 0.25, 0, 1);
                else
                    objectButton.IconFrame.Icon:SetTexCoord(0, 0.125, 0, 1);
                end
            else
                objectButton.IconFrame.Icon:SetTexCoord(0, 0.125, 0, 1);
            end

            self.objects[self.lastID] = objectButton;
            self.objectNum = self.objectNum + 1;
            self.lastObject = objectButton;
            self.folderScrollChildHeight = self.folderScrollChildHeight + 18;
        
            AddOns.Editor:CallColorFunctions(objectButton);
        end
    end
    
    self.List.Scroll:SetHeight(self.folderScrollChildHeight);
    
    if (#objectList == 0) then
        self.List.Slider:SetMinMaxValues(0, 0);
    elseif (self:IsShown()) then
        self.List.Slider:SetMinMaxValues(
            0, math.max(0, self.folderScrollChildHeight - self.List:GetHeight())
        );
    end
    
    self.updateObjectList = true;
end

function ObjectList:PickObject(id)
    if (not id and self.currentObject) then
        self.currentObject:SetBackdrop(nil);
        self.currentObject = nil;
    else
        local object
        if (id) then
            object = self.objects[id];
            if (not object) then
                Warning("ObjectList : No object with ID : "..id);
            end
        else
            object = nil
        end
    
        if (self.currentObject ~= object) then
            if (self.currentObject) then
                self.currentObject:SetBackdrop(nil);
            end

            self.currentObject = object;

            if (self.currentObject) then
                self.currentObject:SetBackdrop({
                    edgeFile = "Interface/Editor/Templates/Border1PX.png"
                });

                self.currentObject:SetBackdropColor(1, 1, 1, 0.35);
            end
        end
    end
end