#include <gtkmm/main.h>
#include <gtkglmm.h>

#include "designer_window.hpp"

int main(int argc, char** argv){
  Gtk::Main kit(argc, argv);
  Gtk::GL::init(argc, argv);
  design_window window;
  kit.run(window);
  return 0;
}

