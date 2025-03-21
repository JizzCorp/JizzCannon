#include "jizzCannon.hpp"

JizzCannon::JizzCannon() {
  this->init_widgets();
  this->populate_widgets();
}

JizzCannon::~JizzCannon() {}

int JizzCannon::run(int &argc, char **argv) {
  this->jc_window->show();
  return Fl::run();
}

void JizzCannon::init_widgets() {
  this->jc_window = new Fl_Double_Window(
      this->WIDTH,
      this->HEIGHT
    );
  
  this->jc_window->begin();
  
  this->jc_menu_bar = new Fl_Menu_Bar(0, 0, this->jc_window->w(), this->MENU_HEIGHT);

  this->jc_text_buffer = new Fl_Text_Buffer();

  this->jc_editor = new Fl_Text_Editor(
    0,
    this->jc_menu_bar->h(),
    this->jc_window->w(),
    this->jc_window->h() - this->jc_menu_bar->h()
  );

  this->jc_window->end();
}

void JizzCannon::populate_widgets() {
  this->jc_window->begin();

  this->jc_menu_bar->add(EDITOR_QUIT, FL_COMMAND+'q', this->callback_editor_quit, NULL, FL_MENU_DIVIDER);

}
