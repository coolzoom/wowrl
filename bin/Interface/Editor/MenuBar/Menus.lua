
MenuBar:AddMenu("File");
MenuBar:AddMenuItem("File", "New", "Ctrl-N");
MenuBar:AddMenuItem("File", "Open", "Ctrl-O");
MenuBar:AddMenuItem("File", "Save", "Ctrl-S");
MenuBar:AddMenuItem("File", "SaveAs");
MenuBar:AddMenuItem("File", "Quit", "Esc."):SetScript("OnClick", function() Exit(); end);

MenuBar:AddMenu("Edit");
MenuBar:AddMenuItem("Edit", "Undo", "Ctrl-Z");
MenuBar:AddMenuItem("Edit", "Redo", "Ctrl-Shift-Z");
MenuBar:AddMenuItem("Edit", "Copy", "Ctrl-C");
MenuBar:AddMenuItem("Edit", "Paste", "Ctrl-V");

MenuBar:AddMenu("Add");
MenuBar:AddMenuItem("Add", "Doodad", "Ctrl-D");
MenuBar:AddMenuItem("Add", "Light", "Ctrl-L");
MenuBar:AddMenuItem("Add", "Unit", "Ctrl-U");

MenuBar:AddMenu("View");
MenuBar:AddMenuCheckItem("View", "Wireframe", "F2", false):SetScript("OnClick", function() base:OnClick(); Frost:ToggleWireframeView(); end);
MenuBar:AddMenuCheckItem("View", "Shading", "F3", true):SetScript("OnClick", function() base:OnClick(); Frost:ToggleShading(); end);
MenuBar:AddMenuItem("View", "BGColor");

MenuBar:AddMenu("Settings");

MenuBar:AddMenu("Help");
MenuBar:AddMenuItem("Help", "Show", "F1");
MenuBar:AddMenuItem("Help", "About");
