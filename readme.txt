==============================================================
                    FROST ENGINE README
==============================================================

Summary :
==============================================================

 [0] Forewords

 [1] Installation
   [1.1] Dependencies
       [1.1.1] OGRE
   [1.2] Building the engine
       [1.2.1] ... using Code::Blocks
             [1.2.1.1] Frost - Utils
             [1.2.1.2] Frost - XML
             [1.2.1.3] Frost
       [1.2.2] ... using CodeLite
             [1.2.2.1] Frost - Utils
             [1.2.2.2] Frost - XML
             [1.2.2.3] Frost
       [1.2.3] ... using Visual C++ 2008
             [1.2.3.1] Frost - Utils
             [1.2.3.2] Frost - XML
             [1.2.3.3] Frost
       [1.2.4] ... using MakeFile
   [1.3] Post-steps

==============================================================
= [0] Forewords ==============================================

    Thank you for downloading Frost Engine (FE) !
    To ensure you've got the latest version, please visit :
              http://wowrl.sourceforge.net/
    
    As you might be aware, FE is a "game engine". That means
that you will have to "compile" it to get it running. If you
are new to the world of programming or to C++, I might sug-
gest practicing a little on simpler projects before you start
playing with FE.
    If you think you can handle all of this, then come on and
follow me into the dark, horrible world of C++ compilers !

    If you don't know how to compile a C++ program, you might
want to download an IDE that will help you through the pro-
cess. I use Code::Blocks, so I suggest you do the same ! But
you can use other IDEs if you want : I've included the build
details in this readme.
    Once you have your IDE installed, you can go to the next
section.

==============================================================
= [1] Installation ===========================================
=== [1.1] Dependencies =======================================

    Before you compile anything, you'll need to get your hands
on the different dependencies required by FE.
    To simplify this task, I've uploaded an archive containing
two of the required libraries (OIS and Lua) on SourceForge.
You can get it here :
                          -------
    Once you've dowloaded it, extract it's content in this
folder and you're done !

    The third and last library, the most important one, is the
3D rendering engine : OGRE. Because of its huge size, I could 
not upload a prebuilt version like the other two, so you'll
have to work a little here :)

======= [1.1.1] OGRE =========================================
 
    OGRE is an open source graphics engine. It only takes care
of displaying things on your screen, but this task requires
lots and lots of source code !
    You can see that by yourself by downloading their source
code from SourceForge :
          http://www.ogre3d.org/download/source
    But if you dont want to get your hands dirty, you can
download one of their prebuilt SDK (Windows/Mac only) :
           http://www.ogre3d.org/download/sdk
    The only drawback of using these is that you can't take a
look at the source code to see how it works, nor can you debug
your application deep into OGRE's code. Some SDKs are also a
bit outdated...

    If you chose to download a SDK, you'll have very few 
things to do : the archive contains the necessary include
files and the DLL/lib files. Just unpack it somewhere near
this directory. The recommended extraction folder would be :

    ...
      + Frost (the directory containing this readme)
      - ogre (unpack the SDK here)
        + ...

    Locate the folders named "include" and "lib". I will call
these folders "<OgreInclude>" and "<OgreLib>" later.
    At last go to the "bin/release" directory and copy :
        - OgreMain.dll
        - RenderSystem_Direct3D9.dll
        - RenderSystem_GL.dll
... to the "Frost/bin" folder. Do the same with the DLLs in
the "bin/debug" directory (there is no need to copy the
OIS.dll file).

    If you chose to download the source, unpack it the same
way as the SDK, and refer to OGRE's website for instructions
on how to build it. Note that we'll only need OgreMain and its
two rendering systems (OpenGL and DirectX if available).
    You should the end up with quite the same thing than with
the SDK.
    Locate the folders named "OgreMain/include" and "lib". I
will call these folders "<OgreInclude>" and "<OgreLib>" later.
    If you're using Windows, go to the "lib" directory and
copy the following files :
        - OgreMain.dll
        - OgreMain_d.dll
        - RenderSystem_Direct3D9.dll
        - RenderSystem_Direct3D9_d.dll
        - RenderSystem_GL.dll
        - RenderSystem_GL_d.dll
... to the "Frost/bin" folder.

    Congratulations, you've installed OGRE successfully !

==============================================================
=== [1.2] Building the engine ================================
======= [1.2.1] ... using Code::Blocks =======================

    So you've decided to use Code::Blocks. Good :) This is
the simplest (and the best) choice.
    
    The first thing you'll need to do, is to open the file :
                  Frost.CodeBlocks.workspace
... located in the same folder as this readme. If you've in-
stalled Code::Blocks properly, it should pop up and display
the Frost Workspace.
    This "workspace" contains several projects :
        - Frost
        - Frost - XML
        - Frost - Utils
    These are "parts" of the engine that we will need to com-
pile. We will start by "Frost - Utils".
    Note : altough Code::Blocks can use almost any compiler,
I'll suppose you have chosen g++.

============= [1.2.1.1] Frost - Utils ========================

    Double click on the Frost - Utils project to activate it.
    Then right click on it and choose "Build options...". This
will pop up a new window with a lot of things on it, but don't
panic :) Click on the "Search directories" tab and look at the
white zone containing 3 folder names. The last one should be
changed if you have installed the OGRE SDK, or if you have
installed it somewhere else than wherer I suggested. Anyway,
double click it : it will show a little dialog. Press the
[...] button and navigate to the <OgreInclude> folder (see the
[1.1.1] section if you've forgot). Press OK twice and exit the
"Build option" window.

    The Utils library should now be properly configured.
    Now search for a field called "Build target" in C::B's
toolbar. It should contain "Linux", which may not be appropri-
ate... If you're using Windows, click on the arrow next to it
and chose Win32.
    At last, go to the "Build" menu and click on "Build" (or
press Ctrl+F9), and wait for the compilation to end.

    If you encounter any error, you're welcome to report them
in the forum.

============= [1.2.1.2] Frost - XML ==========================

    Now to the XML lib. This is the smallest one :)
    Double click on "Frost - XML" in the project panel to act-
ivate it. Good news : this one doesn't need any configuring !
Just select the proper build target ("Linux" or "Win32", like
with the Utils library) and build it.

============= [1.2.1.3] Frost ================================

    The last but not the least...
    Double click on "Frost" in the project panel. Right click
it and go to "Build options...". Click on the "Search directo-
ries" tab and edit the first folder in the white zone to make
it point to the "<OgreInclude>" directory.
    If you've carefully followed the steps in OGRE's instal-
lation, there is nothing else to do !
    
    Pick up the appropriate build target ("Release_Win32" for
Windows, "Release_Linux" for Linux), and build ! This might
take some time (~2 minutes on my machine).

    When the process is completed, you should have a valid
executable in the "bin" folder called either "Frost.exe" for
Windows, or simply "Frost" for Linux.

==============================================================
======= [1.2.2] ... using CodeLite ===========================

   CodeLite is an IDE that looks much like Code::Blocks. So
the build steps are likely to be the same...

   The first thing you'll need to do is to open the file :
                Frost.CodeLite.workspace
... located in the same folder as this readme. If you've in-
stalled CodeLite properly, it should pop up and display the
Frost workspace.
    This "workspace" contains several projects :
        - Frost
        - Frost_XML
        - Frost_Utils
    These are "parts" of the engine that we will need to com-
pile. We will start by "Frost_Utils".
    Note : altough CodeLite can use almost any compiler, I'll
suppose you have chosen g++.

============= [1.2.1.1] Frost - Utils ========================

    Double click on the Fros_Utils project to activate it.
    Then right click on it and choose "Settings...". This will
pop up a new window with a lot of things on it, but don't
panic :) In the field at the top of the window ("Configuration
Type"), chose "Global Settings". Locate the "Additional Search
Path" field and click on the [...] button just next to it.
This will pop up a dialog window with several folders listed.
Edit the last one to point to the <OgreInclude> folder (see the
[1.1.1] section if you've forgot). Press OK and exit the "Set-
tings" window.

    The Utils library should now be properly configured.
    Now search for a field called "Selected Configuration", it
should be just above the projects list. Depending on your cur-
rent OS, select either "Release_Linux" or "Release_Win32".
    At last, go to the "Build" menu and click on "Build pro-
ject" (or press F7), and wait for the compilation to end.

    If you encounter any error, you're welcome to report them
in the forum.

============= [1.2.2.2] Frost - XML ==========================

    Now to the XML lib. This is the smallest one :)
    Double click on "Frost_XML" in the project panel to act-
ivate it. Good news : this one doesn't need any configuring !
Just build it.

============= [1.2.2.3] Frost ================================

    The last but not the least...
    Double click on "Frost" in the project panel. Right click
it and go to "Settings...". Change the "Configuration Type" to
"Global settings" and click the [...] button near "Additional
Search Path". Now, edit the "../ogre/OgreMain/include" to make
it point to the "<OgreInclude>" directory.
    If you've carefully followed the steps in OGRE's instal-
lation, there is nothing else to do !
    
    Now, build ! This might take some time (~2 minutes on my
machine).

    When the process is completed, you should have a valid
executable in the "bin" folder called either "Frost.exe" for
Windows, or simply "Frost" for Linux.

==============================================================
======= [1.2.3] ... using Visual C++ 2008 ====================

    Visual C++ is Microsoft's IDE. Even though I don't really
like it (for various reasons, including irrationnal ones), FE
comes with working project files for Visual C++ 2008 (tested
with the Express, free edition), mainly because it is a widely
used IDE.
    
    Anyway, the first thing you'll need to do is to open :
                      Frost.sln
... located in the same folder as this readme. VC++ should pop
up and display the Frost "solution".
    This "solution" contains several projects :
        - Frost
        - Frost - XML
        - Frost - Utils
    These are "parts" of the engine that we will need to com-
pile. We will start by "Frost - Utils".
    Note : I'll only cover the Release build here. If you ever
need to compile the Debug version, follow the exact same steps
but replace all occurences of "Release" by "Debug", that's
all :).

============= [1.2.3.1] Frost - Utils ========================

    First, make sure the current build target is set to "Re-
lease" : in the toolbar, just above the main menu, there
should be 3 drop downs. Set the first one to "Release".
    Now, double click on the "Frost - Utils" project to act-
ivate it, then right click on it and choose "Properties". This
will pop up a new window with a lot of things on it, but don't
panic :) On the left panel, click the cross near "C/C++" to
expand it and click on "General".
    Then, click on the "Additional Include Directories" and
push the little [...] that just appeared on the right. That
should pop up a small dialog window containing 3 folder names.
Double click on the last one (..\ogre\OgreMain\include), click
on the [...] button that just appeared, and navigate to the
"<OgreInclude>" directory. Press OK twice and close the "Pro-
perties" window.

    The Utils library should now be properly configured !
    At last, go to the "Build" menu, click "Build Frost -
Utils", and wait for the compilation to end.

    If you encounter any error, you're welcome to report them
in the forum.

============= [1.2.3.2] Frost - XML ==========================

    Now to the XML lib. This is the smallest one :)
    Double click on "Frost - XML" in the project panel to act-
ivate it. Good news : this one doesn't need any configuring !
Just build it.

============= [1.2.3.3] Frost ================================

    The last but not the least...
    Double click on "Frost" in the project panel. Right click
it and go to "Properties". On the left panel, navigate to
"Configuration Properties", "C/C++" and "General".
    Then, click on the "Additional Include Directories" and
push the little [...] that just appeared on the right. That
should pop up a small dialog window containing 3 folder names.
Double click on "..\ogre\OgreMain\include", click on the [...]
button that just appeared, and navigate to the "<OgreInclude>"
directory. Press OK twice.
    Now, navigate to "Configuration Properties", "Linker" and
"General". Click on the "Additional Library Directories" and
push the [...] button. In the window that just poped up, edit
the "..\ogre\lib" folder to make it point to the "<OgreLib>"
folder. Press OK and close the property window.
    If you've carefully followed the steps in OGRE's instal-
lation, there is nothing else to do !
    
    Now, build ! This might take some time (several minutes on
my machine, which could be improved by using pre-compiled
headers, but I don't know how do this with VC++).

    When the process is completed, you should have a valid
executable in the "bin" folder called "Frost.exe".

==============================================================
======= [1.2.4] ... using MakeFile ===========================

    WIP

==============================================================
=== [1.3] Post-steps =========================================

    Now that you have an executable built, run it to see if
everything went fine. If something strange happens (or if no-
thing happens), please report back to the forums with the er-
ror message if any, and the content of the "Frost.log" file.
    If it works properly (no error message and the windows
doesn't close), then congratulation : you managed to build
Frost Engine !

==============================================================

                      Frost Engine
               Copyright 2008-2012 Kalith