#include "frost_te_gtkogrewindow.h"
#include "frost_te_mainwindow.h"
#include "gui/frost_spritemanager.h"

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreStringConverter.h>

#ifdef WIN32
#  include <gdk/gdkwin32.h>
#else
#  include <gdk/gdkx.h>
#endif

int GtkOgreWidget::m_ID = 0;
extern Frost::TE::MainWindow* pMainWindow;

GtkOgreWidget::GtkOgreWidget(void)
        : Glib::ObjectBase("GtkOgre"), Gtk::Widget(), m_pWindow(NULL)
{
    set_flags(Gtk::NO_WINDOW);
}

GtkOgreWidget::~GtkOgreWidget(void)
{
}

void GtkOgreWidget::on_size_request(Gtk::Requisition* requisition)
{
    //Initialize the output parameter:
    *requisition = Gtk::Requisition();

    //Discover the total amount of minimum space needed by this widget.

    //Let's make this simple example widget always need 50 by 50:
    requisition->height = 400;
    requisition->width = 400;
}

void GtkOgreWidget::on_size_allocate(Gtk::Allocation& allocation)
{
    //Do something with the space that we have actually been given:
    //(We will not be given heights or widths less than we have requested, though we might get more)

    set_allocation(allocation);

    if (m_refGdkWindow)
        m_refGdkWindow->move_resize(allocation.get_x(), allocation.get_y(), allocation.get_width(), allocation.get_height());

    if (m_pWindow)
    {
        /* Let Ogre update the viewport dimensions. */
#ifdef FROST_LINUX
        getRenderWindow()->resize(allocation.get_width(), allocation.get_height());
#else
        getRenderWindow()->windowMovedOrResized();
#endif

        Frost::SpriteManager::GetSingleton()->SetWindowSize(
            allocation.get_width(),
            allocation.get_height()
        );

        on_expose_event(NULL);
    }
}

void GtkOgreWidget::on_map()
{
    //Call base class:
    Gtk::Widget::on_map();
}

void GtkOgreWidget::on_unmap()
{
    //Call base class:
    Gtk::Widget::on_unmap();
}

void GtkOgreWidget::on_realize()
{
    //Call base class:
    Gtk::Widget::on_realize();
    if (!m_refGdkWindow)
    {
        //Create the GdkWindow:
        GdkWindowAttr attributes;
        memset(&attributes, 0, sizeof(attributes));

        Gtk::Allocation allocation = get_allocation();

        //Set initial position and size of the Gdk::Window:
        attributes.x = allocation.get_x();
        attributes.y = allocation.get_y();
        attributes.width = allocation.get_width();
        attributes.height = allocation.get_height();
        attributes.event_mask = GDK_EXPOSURE_MASK | GDK_POINTER_MOTION_MASK;
        attributes.window_type = GDK_WINDOW_CHILD;
        attributes.wclass = GDK_INPUT_OUTPUT;

        m_refGdkWindow = Gdk::Window::create(get_window(), &attributes, GDK_WA_X | GDK_WA_Y);

        unset_flags(Gtk::NO_WINDOW);
        set_window(m_refGdkWindow);
        set_double_buffered(false);
        //make the widget receive expose events
        m_refGdkWindow->set_user_data(gobj());
        m_refGdkWindow->set_back_pixmap(Glib::RefPtr<Gdk::Pixmap>(), false);
        //Allocate a GC for use in on_expose_event():
        Ogre::NameValuePairList params;

#ifdef WIN32
        params["externalWindowHandle"] = Ogre::StringConverter::toString((unsigned long)GDK_WINDOW_HWND(m_refGdkWindow->gobj()));

#else
        unsigned long handle = static_cast < unsigned long> ( GDK_WINDOW_XID(m_refGdkWindow->gobj() ));
        std::ostringstream os;
        os << handle;
        params["externalWindowHandle"] = os.str();
        XSync(GDK_WINDOW_XDISPLAY(    m_refGdkWindow->gobj() ), False);
#endif

        m_pWindow = Ogre::Root::getSingleton().createRenderWindow("GtkOgreWidget" + Ogre::StringConverter::toString(++m_ID),
                        allocation.get_width(), allocation.get_height(), false, &params);
        m_pWindow->setAutoUpdated(false);

        pMainWindow->Initialize(m_pWindow, allocation.get_width(), allocation.get_height());
    }
}

void GtkOgreWidget::on_unrealize()
{
    m_refGdkWindow.clear();

    //Call base class:
    Gtk::Widget::on_unrealize();
}

bool GtkOgreWidget::on_expose_event(GdkEventExpose* event)
{
    getRenderWindow()->update(true);
    return true;
}

Ogre::RenderWindow* GtkOgreWidget::getRenderWindow(void) throw()
{
    return m_pWindow;
}

void GtkOgreWidget::render()
{
    on_expose_event(NULL);
}
