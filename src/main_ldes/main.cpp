#include "main_window.hpp"
#include <gtkmm/main.h>

int main(int argc, char** argv){
  Gtk::Main kit(argc, argv);
  Gtk::GL::init(argc, argv);
  ldes::main_window window;
  kit.run(window);
  return 0;
}