#ifndef FROST_TE_GTKOGREWINDOW_H
#define FROST_TE_GTKOGREWINDOW_H


#include <gtkmm/widget.h>

#include <OgrePrerequisites.h>

class GtkOgreWidget : public Gtk::Widget
{
    // Attributes
private :
    //! GDK window.
    Glib::RefPtr<Gdk::Window> m_refGdkWindow;
    //! Ogre render window.
    Ogre::RenderWindow* m_pWindow;
    //! ID of window.
    static int m_ID;

    // Operations
public :
    //! Get ogre render window.
    Ogre::RenderWindow* getRenderWindow(void) throw();
    void                render();

protected :
    virtual void on_size_request(Gtk::Requisition* requisition);
    virtual void on_size_allocate(Gtk::Allocation& allocation);
    virtual void on_map();
    virtual void on_unmap();
    virtual void on_realize();
    virtual void on_unrealize();
    virtual bool on_expose_event(GdkEventExpose* event);


    // Construction/Destruction
public :
    GtkOgreWidget(void);
    virtual ~GtkOgreWidget(void);
};

#endif
