
MenuBar:AddMenu("File");
MenuBar:AddMenuItem("File", "New", "Ctrl-N");
MenuBar:AddMenuItem("File", "Open", "Ctrl-O"):SetScript("OnClick", function() Frost:LoadZoneFile("Zones/Test/Test.xml"); end);
MenuBar:AddMenuItem("File", "Save", "Ctrl-S");
MenuBar:AddMenuItem("File", "SaveAs");
MenuBar:AddMenuItem("File", "Close", "Ctrl-W"):SetScript("OnClick", function() Frost:UnloadZone(); end);
MenuBar:AddMenuItem("File", "Quit", "Esc."):SetScript("OnClick", function() Exit(); end);

MenuBar:AddMenu("Edit");
MenuBar:AddMenuItem("Edit", "Undo", "Ctrl-Z");
MenuBar:AddMenuItem("Edit", "Redo", "Ctrl-Shift-Z");
MenuBar:AddMenuItem("Edit", "Clone", "Ctrl-C");
MenuBar:AddMenuItem("Edit", "Delete", "Del.");

MenuBar:AddMenu("View");
MenuBar:AddMenuCheckItem("View", "Wireframe", "F2", false):SetScript("OnClick", function() Frost:ToggleWireframeView(); end);
MenuBar:AddMenuCheckItem("View", "Shading", "F3", true):SetScript("OnClick", function() Frost:ToggleShading(); end);
MenuBar:AddMenuItem("View", "BGColor");

MenuBar:AddMenu("Settings");

MenuBar:AddMenu("Help");
MenuBar:AddMenuItem("Help", "Show", "F1");
MenuBar:AddMenuItem("Help", "About");
