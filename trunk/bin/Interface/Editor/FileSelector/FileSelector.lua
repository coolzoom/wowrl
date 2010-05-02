function FileSelector:SetOnOkFunc(func)
    self.onOk = func;
end

function FileSelector:OnOk()
    if (self.onOK) then
        self.onOK();
    end

    self.onOK = nil;
end

function FileSelector:SetOnCancelFunc(func)
    self.onCancel = func;
end

function FileSelector:OnCancel()
    if (self.onCancel) then
        self.onCancel();
    end

    self.onCancel = nil;
end

function FileSelector:Error()
    FileSelectorNotFoundErrorDialog:Show();
end

function FileSelector:GetSelectedFile()
    if (self.currentFile) then
        return self.currentFile.file;
    else
        return nil;
    end
end

function FileSelector:InitRootFolder()
    if (not self.initialized) then
        self.folders = {};
        self.files = {};

        local rootFolder = CreateFrame("Button", "$parentRoot", self.Splitter.FolderFrame.Scroll, "ButtonTemplate_FolderButton");
        if (rootFolder) then
            self.lastID = 1;
            rootFolder.id = self.lastID;
            rootFolder.developed = true;
            rootFolder.parent = nil;
            rootFolder.name = "";
            rootFolder.folder = "";
            rootFolder.level = 0;
            rootFolder.folders = {};
            rootFolder.folderNum = 0;

            rootFolder:SetText("/");
            rootFolder:SetPoint("RIGHT", self.Splitter.FolderFrame.Scroll, "RIGHT", -5, 0);
            rootFolder:SetPoint("TOPLEFT", self.Splitter.FolderFrame.Scroll, "TOPLEFT", 5, 5);

            rootFolder.Develop.Plus:Hide();

            self.folders[self.lastID] = rootFolder;
        else
            return;
        end

        local folderList = {Frost:GetFolderList("")};

        if (#folderList == 0) then
            return;
        end

        rootFolder.Develop.Minus:Show();

        self.folderScrollChildHeight = 5 + 18 + 5;

        for i, folder in pairs(folderList) do
            local folderButton = CreateFrame("Button", "$parent"..folder, rootFolder, "ButtonTemplate_FolderButton");
            if (folderButton) then
                self.lastID = self.lastID + 1;
                folderButton.id = self.lastID;
                folderButton.developed = false;
                folderButton.parent = rootFolder;
                folderButton.name = folder;
                folderButton.folder = folder;
                folderButton.level = 1;
                folderButton.folders = {};
                folderButton.folderNum = 0;

                folderButton:SetText(folder);

                if (rootFolder.lastFolder) then
                    rootFolder.lastFolder.nextFolder = folderButton;
                    folderButton:SetPoint("TOPLEFT", rootFolder.lastFolder, "BOTTOMLEFT");
                else
                    folderButton:SetPoint("TOPLEFT", rootFolder, "BOTTOMLEFT", 16, 0);
                end

                self.folders[self.lastID] = folderButton;
                rootFolder.folders[self.lastID] = folderButton;
                rootFolder.folderNum = rootFolder.folderNum + 1;
                rootFolder.lastFolder = folderButton;
                self.folderScrollChildHeight = self.folderScrollChildHeight + 18;
            end
        end

        rootFolder.folderNum = #rootFolder.folders;

        self.Splitter.FolderFrame.Scroll:SetHeight(self.folderScrollChildHeight);
        self.Splitter.FolderFrame.Slider:SetMinMaxValues(
            0, math.max(0, self.folderScrollChildHeight - self.Splitter.FolderFrame:GetHeight())
        );

        self.initialized = true;
    end
end

function FileSelector:DevelopFolder(id, toggle)
    if (self.folders[id]) then
        local parentFolder = self.folders[id];
        if (not parentFolder.developed) then
            parentFolder.Develop.Plus:Hide();
            parentFolder.developed = true;

            local folderList = {Frost:GetFolderList(parentFolder.folder)};

            if (#folderList == 0) then
                parentFolder.isEmpty = true;
                parentFolder.Develop:Hide();
                return;
            end

            parentFolder.isEmpty = false;
            parentFolder.Develop.Minus:Show();

            for i, folder in pairs(folderList) do
                local folderButton = CreateFrame("Button", "$parent"..folder, parentFolder, "ButtonTemplate_FolderButton");
                if (folderButton) then
                    self.lastID = self.lastID + 1;
                    folderButton.id = self.lastID;
                    folderButton.developed = false;
                    folderButton.parent = parentFolder;
                    folderButton.name = folder;
                    if (#parentFolder.folder ~= 0) then
                        folderButton.folder = parentFolder.folder.."/"..folder;
                    else
                        folderButton.folder = folder;
                    end

                    folderButton.level = parentFolder.level + 1;
                    folderButton.folders = {};
                    folderButton.folderNum = 0;

                    folderButton:SetText(folder);

                    if (parentFolder.lastFolder) then
                        parentFolder.lastFolder.nextFolder = folderButton;
                        folderButton:SetPoint("TOPLEFT", parentFolder.lastFolder, "BOTTOMLEFT");
                    else
                        folderButton:SetPoint("TOPLEFT", parentFolder, "BOTTOMLEFT", 16, 0);
                    end

                    self.folders[self.lastID] = folderButton;
                    parentFolder.folders[self.lastID] = folderButton;
                    parentFolder.folderNum = parentFolder.folderNum + 1;
                    parentFolder.lastFolder = folderButton;
                end
            end

            local parent = parentFolder.parent;
            while (parent) do
                parent.folderNum = parent.folderNum + parentFolder.folderNum;
                parent = parent.parent;
            end

            self.folderScrollChildHeight = self.folderScrollChildHeight + 18*parentFolder.folderNum;
            self.Splitter.FolderFrame.Scroll:SetHeight(self.folderScrollChildHeight);
            self.Splitter.FolderFrame.Slider:SetMinMaxValues(
                0, math.max(0, self.folderScrollChildHeight - self.Splitter.FolderFrame:GetHeight())
            );

            if (parentFolder.nextFolder) then
                parentFolder.nextFolder:SetPoint(
                    "TOPLEFT", parentFolder.lastFolder, "BOTTOMLEFT",
                    -16*(parentFolder.lastFolder.level - parentFolder.nextFolder.level), 0
                );

                parentFolder.lastFolder.nextFolder = parentFolder.nextFolder;
            end
        elseif (not parentFolder.isEmpty and toggle) then
            parentFolder.Develop.Minus:Hide();
            parentFolder.Develop.Plus:Show();
            parentFolder.developed = false;

            if (parentFolder.lastFolder.nextFolder) then
                parentFolder.lastFolder.nextFolder:SetPoint("TOPLEFT", parentFolder, "BOTTOMLEFT");
                parentFolder.nextFolder = parentFolder.lastFolder.nextFolder;
            end

            parentFolder.lastFolder = nil;

            local parent = parentFolder.parent;
            while (parent) do
                parent.folderNum = parent.folderNum - parentFolder.folderNum;
                parent = parent.parent;
            end

            self.folderScrollChildHeight = self.folderScrollChildHeight - 18*parentFolder.folderNum;
            self.Splitter.FolderFrame.Scroll:SetHeight(self.folderScrollChildHeight);
            self.Splitter.FolderFrame.Slider:SetMinMaxValues(
                0, math.max(0, self.folderScrollChildHeight - self.Splitter.FolderFrame:GetHeight())
            );

            if (self.currentFolder) then
                local parent = self.currentFolder.parent;
                while (parent) do
                    if (parent == parentFolder) then
                        self:SetFolder(parentFolder.id);
                        self:SetFile(nil);
                        break;
                    end
                    parent = parent.parent;
                end
            end

            for i, folder in pairs(parentFolder.folders) do
                DeleteFrame(folder);
            end

            parentFolder.folders = {};
            parentFolder.folderNum = 0;
        end
    else
        Warning("FileSelector : No folder with ID : "..id);
    end
end

function FileSelector:SetFolder(id)
    if (not id and self.currentFolder) then
        self:SetFile(nil);

        for i, file in pairs(self.files) do
            DeleteFrame(file);
        end

        self.files = {};

        self.currentFolder:SetBackdrop(nil);

        self.currentFolder = nil;
        self.lastFile = nil;

        return;
    end

    if (self.folders[id]) then
        local parentFolder = self.folders[id];

        if (parentFolder == self.currentFolder) then
            return;
        end

        self:SetFile(nil);

        for i, file in pairs(self.files) do
            DeleteFrame(file);
        end

        self.lastFile = nil;
        self.files = {};

        local fileList = {Frost:GetFileList(parentFolder.folder)};

        if (self.currentFolder) then
            self.currentFolder:SetBackdrop(nil);
        end

        self.currentFolder = parentFolder;
        self.fileScrollChildHeight = 10;

        parentFolder:SetBackdrop({
            edgeFile = "Interface/Editor/Templates/Border1PX.png"
        });

        parentFolder:SetBackdropColor(1, 1, 1, 0.35);

        if (#fileList == 0) then
            parentFolder.isFileEmpty = true;
            return;
        end

        local fileID = 0;

        for i, file in pairs(fileList) do
            local adjustedName = string.gsub(file, "%.", "");
            local dotPos = string.find(file, ".", 0, true);
            local extension;
            if (dotPos) then
                extension = string.sub(file, dotPos+1);
            end
            local fileButton = CreateFrame("Button", "$parent"..adjustedName, self.Splitter.FileFrame.Scroll, "ButtonTemplate_FileButton");
            if (fileButton) then
                fileID = fileID + 1;
                fileButton.id = fileID;
                fileButton.name = file;
                if (#parentFolder.folder ~= 0) then
                    fileButton.file = parentFolder.folder.."/"..file;
                else
                    fileButton.file = file;
                end
                fileButton:SetText(file);

                if (self.lastFile) then
                    fileButton:SetPoint("TOPLEFT", self.lastFile, "BOTTOMLEFT");
                else
                    fileButton:SetPoint("TOPLEFT", self.Splitter.FileFrame.Scroll, "TOPLEFT", 5, 5);
                end

                if (extension) then
                    if (extension == "lua" or extension == "xml" or extension == "toc" or extension == "def"
                        or extension == "txt" or extension == "glsl" or extension == "hlsl") then
                        fileButton.IconFrame.Icon:SetTexture("Interface/Editor/FileSelector/FileSelectorFileText.png");
                    elseif (extension == "png" or extension == "jpg" or extension == "tga" or extension == "bmp"
                            or extension == "gif") then
                        fileButton.IconFrame.Icon:SetTexture("Interface/Editor/FileSelector/FileSelectorFilePicture.png");
                    elseif (extension == "fm" or extension == "ft" or extension == "mesh") then
                        fileButton.IconFrame.Icon:SetTexture("Interface/Editor/FileSelector/FileSelectorFile.png");
                    else
                        fileButton.IconFrame.Icon:SetTexture("Interface/Editor/FileSelector/FileSelectorFileUnknown.png");
                    end
                else
                    fileButton.IconFrame.Icon:SetTexture("Interface/Editor/FileSelector/FileSelectorFileUnknown.png");
                end

                self.files[fileID] = fileButton;
                self.lastFile = fileButton;
                self.fileScrollChildHeight = self.fileScrollChildHeight + 18;
            end
        end

        self.Splitter.FileFrame.Scroll:SetHeight(self.fileScrollChildHeight);
        self.Splitter.FileFrame.Slider:SetMinMaxValues(
            0, math.max(0, self.fileScrollChildHeight - self.Splitter.FileFrame:GetHeight())
        );
    else
        Warning("FileSelector : No folder with ID : "..id);
    end
end

function FileSelector:SetFile(id)
    if (not id and self.currentFile) then
        self.currentFile:SetBackdrop(nil);
        self.File:SetText("");
        self.currentFile = nil;
    else
        local file;
        if (id) then
            file = self.files[id];
            if (not file) then
                Warning("FileSelector : No file with ID : "..id);
            end
        else
            file = nil
        end

        if (self.currentFile ~= file) then
            if (self.currentFile) then
                self.currentFile:SetBackdrop(nil);
            end

            self.currentFile = file;

            if (self.currentFile) then

                self.currentFile:SetBackdrop({
                    edgeFile = "Interface/Editor/Templates/Border1PX.png"
                });

                self.currentFile:SetBackdropColor(1, 1, 1, 0.35);

                self.File:SetText(file.file);
            else
                self.File:SetText("");
            end
        end
    end
end

function FileSelector:SelectFile(fileName)
    self:InitRootFolder();
    local info = Frost:CutFilePath(fileName);

    self.error = nil;
    local currentFolder = self.folders[1];
    for i, folder in pairs(info.folders) do
        self:DevelopFolder(currentFolder.id, false);
        local nextFolder = nil;
        for j, subFolder in pairs(currentFolder.folders) do
            if (subFolder.name == folder) then
                nextFolder = subFolder;
                break;
            end
        end
        if (nextFolder) then
            currentFolder = nextFolder;
        else
            if (#currentFolder.folder == 0) then
                self.error = folder;
            else
                self.error = currentFolder.folder.."/"..folder;
            end
            self:Error();
            break;
        end
    end

    if (not self.error) then
        self:SetFolder(currentFolder.id);

        local found = false;
        for i, file in pairs(self.files) do
            if (file.name == info.file) then
                self:SetFile(file.id);
                found = true;
                break;
            end
        end

        if (not found) then
            if (#self.currentFolder.folder == 0) then
                self.error = info.file;
            else
                self.error = self.currentFolder.folder.."/"..info.file;
            end
            self:Error();
        end
    end
end

function FileSelector:Reset()
    for i, folder in pairs(rootFolder.folders) do
        if (folder.developed) then
            if (folder.isEmpty) then
                folder.Develop:Show();
                folder.Develop.Plus:Show();
                folder.developed = false;
            else
                self:DevelopFolder(folder.id);
            end
        end
    end
end

SetKeyBinding(KEY_T, "FileSelector:SelectFile(\"Interface/Editor/FileSelector/FileSelector.lua\"); FileSelector:Show();");
