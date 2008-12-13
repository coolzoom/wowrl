#include "frost_te_mainwindow.h"
#include "frost_timemanager.h"

#include <gtkmm/main.h>

#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreEntity.h>

using namespace std;
using namespace Frost;

File mLog("TerrainEditor.log", FILE_O);
TE::MainWindow* pMainWindow = NULL;

void PrintInLog( const s_str& sMessage, const s_bool& bTimeStamps )
{
    s_str sNewMessage;
    if (bTimeStamps)
    {
        s_str sNewMessage;
        if (sMessage.Get(0) == '|' && sMessage.Get(1) == 't')
        {
            sNewMessage = sMessage;
            sNewMessage.Erase(0, 2);
        }
        else
        {

            s_str sTime = TimeManager::GetSingleton()->GetPlayTime() + " : ";

            sNewMessage = sTime+sMessage;
            sNewMessage.Replace("\n", s_str("\n") + sTime);
        }

        sNewMessage += '\n';

        mLog.Write(sNewMessage.c_str(), sNewMessage.Length());
        cout << sNewMessage.Get() << endl;
    }
    else
    {
        sNewMessage = sMessage + '\n';

        mLog.Write(sMessage.c_str(), sMessage.Length());
        cout << sNewMessage.Get() << endl;
    }

    mLog.Flush();
}

int main(int argc, char *argv[])
{
    // GTKMM
    Gtk::Main mMain(argc, argv);

    UtilsManager::GetSingleton()->SetLogFunction(&PrintInLog);
    TimeManager::GetSingleton()->Initialize();

    pMainWindow = new TE::MainWindow();
    pMainWindow->show();

    setlocale(LC_ALL, "C");

    while (pMainWindow->is_visible())
    {
        // Main loop
        if (!pMainWindow->Iteration())
            break;
    }

    delete pMainWindow;

    return 0;
}
