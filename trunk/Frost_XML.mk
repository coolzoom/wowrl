##
## Auto Generated makefile, please do not edit
##
ProjectName:=Frost_XML

## Release_Win32
ConfigurationName      :=Release_Win32
IntermediateDirectory  :=obj_XML/Release_CL_Win32
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "C:\Programmation2\Frost"
ProjectPath            := "C:\Programmation2\Frost"
CurrentFileName        :=
CurrentFulePath        :=
CurrentFileFullPath    :=
User                   :=Administrateur
Date                   :=11/15/09
CodeLitePath           :="C:\Programmation\CodeLite"
LinkerName             :=g++
ArchiveTool            :=ar rcus
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
CompilerName           :=g++
OutputFile             :=lib/libxml.a
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=
CmpOptions := -O3 -Wall $(Preprocessors)
LinkOptions :=  -s
IncludePath :=  "$(IncludeSwitch)." "$(IncludeSwitch)include/xml" "$(IncludeSwitch)include/utils" 
RcIncludePath :=
Libs :=
LibPath := "$(LibraryPathSwitch)." "$(LibraryPathSwitch)lib" "$(LibraryPathSwitch)bin" 


Objects=$(IntermediateDirectory)/frost_xml_document$(ObjectSuffix) $(IntermediateDirectory)/frost_xml_block$(ObjectSuffix) 

##
## Main Build Tragets 
##
all: $(IntermediateDirectory) $(OutputFile)

$(OutputFile):  $(Objects)
	@makedir $(@D)
	$(ArchiveTool) $(ArchiveOutputSwitch)$(OutputFile) $(Objects)

obj_XML/Release_CL_Win32:
	@makedir "obj_XML/Release_CL_Win32"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/frost_xml_document$(ObjectSuffix): src/xml/frost_xml_document.cpp $(IntermediateDirectory)/frost_xml_document$(DependSuffix)
	@makedir "obj_XML/Release_CL_Win32"
	$(CompilerName) $(SourceSwitch) "C:/Programmation2/Frost/src/xml/frost_xml_document.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/frost_xml_document$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/frost_xml_document$(DependSuffix): src/xml/frost_xml_document.cpp
	@makedir "obj_XML/Release_CL_Win32"
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/frost_xml_document$(ObjectSuffix) -MF$(IntermediateDirectory)/frost_xml_document$(DependSuffix) -MM "C:/Programmation2/Frost/src/xml/frost_xml_document.cpp"

$(IntermediateDirectory)/frost_xml_block$(ObjectSuffix): src/xml/frost_xml_block.cpp $(IntermediateDirectory)/frost_xml_block$(DependSuffix)
	@makedir "obj_XML/Release_CL_Win32"
	$(CompilerName) $(SourceSwitch) "C:/Programmation2/Frost/src/xml/frost_xml_block.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/frost_xml_block$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/frost_xml_block$(DependSuffix): src/xml/frost_xml_block.cpp
	@makedir "obj_XML/Release_CL_Win32"
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/frost_xml_block$(ObjectSuffix) -MF$(IntermediateDirectory)/frost_xml_block$(DependSuffix) -MM "C:/Programmation2/Frost/src/xml/frost_xml_block.cpp"

##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/frost_xml_document$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/frost_xml_document$(DependSuffix)
	$(RM) $(IntermediateDirectory)/frost_xml_document$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/frost_xml_block$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/frost_xml_block$(DependSuffix)
	$(RM) $(IntermediateDirectory)/frost_xml_block$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) $(OutputFile)

-include $(IntermediateDirectory)/*$(DependSuffix)


