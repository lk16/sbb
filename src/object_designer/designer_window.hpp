#include <gtkmm/box.h>
#include <gtkmm/window.h>

class t_engine;

class design_window:
public Gtk::Window
{
	
	public:
		design_window();
		virtual ~design_window();
		virtual bool on_key_press_event(GdkEventKey* k);
		virtual bool on_key_release_event(GdkEventKey* k);
		
	protected:
		Gtk::VBox m_VBox;
		t_engine& engine;
};
