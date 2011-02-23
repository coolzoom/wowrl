==============================================================
                    FROST ENGINE README
==============================================================

Summary :
==============================================================

 [0] Forewords

 [1] Installation
   [1.1] Dependencies
       [1.1.1] Lua
       [1.1.2] FreeType
       [1.1.3] OIS
       [1.1.4] OGRE
   [1.2] Building the engine

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
cess. I use Code::Blocks, so I suggest you do the same ! You
can use other IDEs if you want, but you'll have to translate
the build process by yourself. Maybe there is a tool somewhere
that can convert a Code::Blocks project to your IDE's format.

 >-----------------------------------------------------------<
 |    Note : altough Code::Blocks can use almost any com-    |
 | piler, I'll suppose you have chosen gcc (I use v.4.5.2).  |
 >-----------------------------------------------------------<

==============================================================
= [1] Installation ===========================================

    In the following, I'll assume that you have extracted FE
source code in a directory that I'll call INSTALL_DIR. For ex-
ample :
      * Windows : C:\Programmation
      * Unix    : ~/programmation/

=== [1.1] Dependencies =======================================

    Before you compile anything, you'll need to get your hands
on the different dependencies required by FE :
      * Lua 5.1 :         http://www.lua.org/
      * FreeType 2.3.11 : http://www.freetype.org/index2.html
      * OIS 1.2 :         http://www.wreckedgames.com/
      * OGRE 1.6.4 :      http://www.ogre3d.org/

    All these libraries are free to use and open source. You
can usually download them from the provided websites. If you
cannot find the required version number, try with the closest
one (the most important thing is that the main version number
is the same, so the API has not been changed).

 >-----------------------------------------------------------<
 |    Note : If you don't want to compile these library by   |
 | yourself, you still have the option to donwload the pre-  |
 | built package available on FE's sourceforge page :        |

https://sourceforge.net/projects/wowrl/files/frost/Prebuilt%20Dependencies/

 |   Be carefull though : it may not be entierly up to date  |
 | and has not been fully tested. It should work though.     |
 >-----------------------------------------------------------<

    Unpack all the archives you downloaded in the INSTALL_DIR
directory. You should end up with something like :

     - INSTALL_DIR/
         - lua5.1/
         - freetype-2.3.11/
         - ois/
         - ogre/

======= [1.1.1] Lua ==========================================

    Lua is a scripting language that FE uses in both its GUI
system and for game logics.

    FE uses a slightly modified version of Lua 5.1, with a
custom configuration file. You'll have to copy the following
file :

     INSTALL_DIR/Frost/Lunar/luaconf.h

... into :

     INSTALL_DIR/lua5.1/include

... and then compile Lua. There is no big deal here : Lua has
no dependency and is very well coded. It should compile like
a charm !
    If you're using Code::Blocks, create a new static library
project and all the .c files that are in :

     INSTALL_DIR/lua5.1/src

    Make sure that Code::Blocks will compile Lua into :
      * liblua5.1.a   (Release)
      * liblua5.1_d.a (Debug)

... then build the whole thing.
    Once that is done, copy these new library files into :

     INSTALL_DIR/Frost/lib/

======= [1.1.2] FreeType =====================================

    FreeType is used by the GUI to render text on the screen.

    No modification has been done to FreeType, so you should
be able to use a prebuilt version.
    If you prefer to build it by yourself, FreeType doesn't
provide any project file for Code::Blocks, so you'll have to
import one of the Visual Studion project and build it.
    Again, no big deal : there is no dependency.

    At last, you should end up with two files :
      * libfreetype2311.a   (Release)
      * libfreetype2311_d.a (Debug)

... that need to be copied into :

     INSTALL_DIR/Frost/lib/


======= [1.1.3] OIS ==========================================

    OIS is a platform independent input library, for mouse and
keyboard.

    No modification has been done to OIS either.
    There is a Code::Blocks project shipped with the source
code, so there should be no problem.

    OIS can be built as a static or dynamic library. I prefer
the static way, but you're free to choose the one your want :)

    If you built is as a static library, you should have two
files :
      * libOIS.a   (Release)
      * libOIS_d.a (Debug)

... that you need to copy to :

     INSTALL_DIR/Frost/lib/

    If you went for the dynamic library way, you may have to
copy these files as well
    If you're using Windows, also copy the created DLLs into :

     INSTALL_DIR/Frost/Engine/bin


    Note for Windows users : OIS relies on DirectInput.
    If you compiled it as a static library you probably didn't
notice it, but that means you'll have to link to two more
files when building FE :
      * dinput8.lib
      * dxguid.lib

    Usually, these are provided with your compiler (yes, even
GCC). If that's your case, you'll have to copy these to the
'lib' folder as well.


======= [1.1.4] OGRE =========================================
 
    OGRE is the heaviest dependency of FE, as it takes care of
all the rendering stuff. As such, it can be a real pain to get
to compile.

    There are SDKs with pre-compiled version of OGRE, but
these are very unlikely to be v.1.6.2.

    You're best chance here is to refer to OGRE's website.

    A little addition has been made to OGRE though, but it
doesn't require re-compilation. You'll have to copy the
following file :

     INSTALL_DIR/Frost/OgreMain/include/OgreGpuCommandBufferFlush.h

... into :

     INSTALL_DIR/ogre/OgreMain/include

    You'll only need to compile OgreMain, along with the
Direct3D9 and OpenGL render systems.

    Once compilation is over, and if you're using Windows, go
to the 'lib' directory and copy the following files :
        - OgreMain.dll
        - OgreMain_d.dll
        - RenderSystem_Direct3D9.dll
        - RenderSystem_Direct3D9_d.dll
        - RenderSystem_GL.dll
        - RenderSystem_GL_d.dll

... to :

     INSTALL_DIR/Frost/Engine/bin

==============================================================
=== [1.2] Building the engine ================================

    So you've decided to use Code::Blocks. Good :) This is
the simplest (and the best) choice.
    
    The first thing you'll need to do, is to open the file :

        INSTALL_DIR/Frost/Frost.CodeBlocks.workspace

    If you've installed Code::Blocks properly, it should pop
up and display the Frost Workspace.
    This "workspace" contains several projects :
        - Frost
        - Frost - Utils
        - Frost - GUI
        - Frost - Input
        - Frost - XML

    These are "parts" of the engine that we will need to com-
pile. We will start by 'Frost - Utils'.
    
    If you installed the dependencies as suggested by this
readme, then you should be ready to compile.

    Depending on your system, choose the appropriate build
target :
      * Linux (Unix)
      * Win32 (Windows)

    You can then start compiling.
    If you encounter any error, you're welcome to report them
back in the forum.

    The build process is exactly the same for the 'Input' and
'GUI' libraries. The latter can take a few minutes to build
depending on your system and setup, but otherwise there should
be no surprises.

    The XML library compiles the same way on Linux and Windows.
    Just compile the 'All' build target.

    Now to the engine itself.
    Pick up the appropriate build target :
      * Release_Linux (Unix)
      * Release_Win32 (Windows)
... and build ! This will take a few minutes.

    When the process is completed, you should have a valid
executable in the 'bin' folder called either "Frost.exe" for
Windows, or simply "Frost" for Linux.

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