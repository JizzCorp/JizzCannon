#include<FL/Fl.H>
#include<FL/Fl_Double_Window.H>
#include<FL/fl_ask.H>
#include<FL/fl_string_functions.h>
#include<FL/Fl_Menu_Bar.H>
#include<FL/Fl_Text_Editor.H>
#include<FL/Fl_Text_Buffer.H>
#include<FL/Fl_Native_File_Chooser.H>
#include<FL/platform.H>

#include<iostream>
#include<errno.h>

#define EDITOR_NAME "JIZZ CANNON"
#define QUIT_EDITOR "File/Quit"
#define OPEN_FILE "File/Open"
#define NEW_FILE "File/New"
#define SAVE_FILE "File/Save"
#define SAVE_FILE_AS "File/Save as..."
#define FILE_CHOOSER_TITLE "JIZZ CANNON - Save as..."
#define OPEN_FILE_FILE_CHOOSER_TITLE "JIZZ CANNON - Open file..."
#define CURRENT_FILE_NOT_SAVED_MESSAGE "The current file has not been saved.\nWould you like to save before quitting?"
#define FAILED_TO_LOAD_FILE_ERROR "JIZZ CANNON ERROR - Failed to load file! Err number: %s, %s"
#define FAILED_TO_SAVE_FILE_ERROR "JIZZ CANNON ERROR - Failed to save file! Err number: %s, %s"

Fl_Double_Window *app_window = NULL;
Fl_Menu_Bar *app_menu_bar = NULL;
Fl_Text_Editor *app_editor = NULL;
Fl_Text_Editor *app_split_editor = NULL;
Fl_Text_Buffer *app_text_buffer = NULL;

bool text_changed = false;
char app_filename[FL_PATH_MAX] = "";

void build_app_window();
void build_app_menu_bar();
void build_main_editor();
void add_file_support();
int handle_commandline_and_run(int &argc, char **argv);

void menu_quit_callback(Fl_Widget *, void *);
void menu_open_callback(Fl_Widget *, void *);
void menu_new_callback(Fl_Widget *, void *);
void menu_save_callback(Fl_Widget *, void *);
void menu_save_as_callback(Fl_Widget *, void *);
void text_changed_callback(int, int n_inserted, int n_deleted, int, const char *, void *);

void set_changed(bool v);
void update_title();
void set_filename(const char *new_filename);
void load(const char *filename);

int main(int argc, char **argv) {
  build_app_window();
  build_app_menu_bar();
  build_main_editor();
  // add_file_support();
  // return handle_commandline_and_run(argc, argv);
  app_window->show();
  return Fl::run();
}

void build_app_window() {
  app_window = new Fl_Double_Window(640, 480, EDITOR_NAME);
}

void build_app_menu_bar() {
  app_window->begin();
  app_menu_bar = new Fl_Menu_Bar(0, 0, app_window->w(), 25); // x, y, width, height
  app_menu_bar->add(QUIT_EDITOR, FL_COMMAND+'q', menu_quit_callback);

  app_window->callback(menu_quit_callback);

  app_menu_bar->add(OPEN_FILE, FL_COMMAND+'o', menu_open_callback, NULL, FL_MENU_DIVIDER);
  app_menu_bar->add(NEW_FILE, FL_COMMAND+'n', menu_new_callback, NULL);
  app_menu_bar->add(SAVE_FILE, FL_COMMAND+'s', menu_save_callback);
  app_menu_bar->add(SAVE_FILE_AS, FL_COMMAND+'S', menu_save_as_callback, NULL, FL_MENU_DIVIDER);
  app_window->end();
}

void build_main_editor() {
  app_window->begin();
  app_text_buffer = new Fl_Text_Buffer();
  app_text_buffer->add_modify_callback(text_changed_callback, NULL);
  
  app_editor = new Fl_Text_Editor(
      0, 
      app_menu_bar->h(),
      app_window->w(), 
      app_window->h() - app_menu_bar->h()
    );
  app_editor->buffer(app_text_buffer);
  app_editor->textfont(FL_COURIER);
  
  app_window->resizable(app_editor);
  app_window->end();
}

void menu_quit_callback(Fl_Widget *, void *) {
  if (text_changed) {
    int c = fl_choice(CURRENT_FILE_NOT_SAVED_MESSAGE, "Cancel", "Save", "Don't save");
    switch (c) {
      case 0:
        return;

      case 1:
        menu_save_callback(NULL, NULL);
        return;
    }
  }

  Fl::hide_all_windows();
}

void menu_open_callback(Fl_Widget *, void *) {
  if (text_changed) {
    int r = fl_choice(
        CURRENT_FILE_NOT_SAVED_MESSAGE,
        "Cancel",
        "Save",
        "Don't save"
      );
    std::cout << "valore r: " << r << std::endl;

    switch (r) {
      case 1:
        menu_save_callback(NULL, NULL);
        return;
      case 2:
        break;
    }
  }

  Fl_Native_File_Chooser file_chooser;
  file_chooser.title(OPEN_FILE_FILE_CHOOSER_TITLE);
  file_chooser.type(Fl_Native_File_Chooser::BROWSE_FILE);

  if (app_filename[0]) {
    char temp_filename[FL_PATH_MAX];
    fl_strlcpy(temp_filename, app_filename, FL_PATH_MAX);
    const char *name = fl_filename_name(temp_filename);
    if (name) {
      file_chooser.preset_file(name);
      temp_filename[name - temp_filename] = 0;
      file_chooser.directory(temp_filename);
    }
  }

  if (file_chooser.show() == 0) {
    load(file_chooser.filename());
  }
}

void menu_new_callback(Fl_Widget *, void *) {}

void menu_save_callback(Fl_Widget *, void *) {
  if(!app_filename[0]) {
    menu_save_as_callback(NULL, NULL);
  } else {
    // NOTE: this app_filename in savefile() is not
    // in the documentation example for "simple text editor",
    // where they use file_chooser.filename(). 
    if (app_text_buffer->savefile(app_filename) != 0) {
      std::cout << strerror(errno) << std::endl;
      fl_alert(
          FAILED_TO_SAVE_FILE_ERROR,
          app_filename,
          strerror(errno)
        );

    }
    set_changed(false);
  }
}

void menu_save_as_callback(Fl_Widget *, void *) {
  Fl_Native_File_Chooser file_chooser;
  file_chooser.title(FILE_CHOOSER_TITLE);
  file_chooser.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);

  if(app_filename[0]) {
    char temp_filename[FL_PATH_MAX];
    fl_strlcpy(temp_filename, app_filename, FL_PATH_MAX);

    const char *name = fl_filename_name(temp_filename);

    if (name) {
      file_chooser.preset_file(name);
      temp_filename[name - temp_filename] = 0;
      file_chooser.directory(temp_filename);
    }
  }

  if (file_chooser.show() == 0) {
    app_text_buffer->savefile(file_chooser.filename());
    set_filename(file_chooser.filename());
    set_changed(false);
  }
}

void text_changed_callback(int, int n_inserted, int n_deleted, int, const char *, void *) {
  if (n_inserted || n_deleted) {
    set_changed(true);
  }
}

void set_changed(bool v) {
  if (v != text_changed) {
    text_changed = v;
    update_title();
  }
}

void set_filename(const char *new_filename) {
  if (new_filename) {
    fl_strlcpy(app_filename, new_filename, FL_PATH_MAX);
  } else {
    app_filename[0] = 0;
  }
  
  update_title();
}

void update_title() {
  const char *fname = NULL;
  if (app_filename[0]) {
    fname = fl_filename_name(app_filename);
  }

  if (fname) {
    char buf[FL_PATH_MAX+3];
    buf[FL_PATH_MAX + 2] = '\0';
    
    if (text_changed) {
      snprintf(buf, FL_PATH_MAX + 2, "%s *", fname);
    } else {
      snprintf(buf, FL_PATH_MAX + 2, "%s", fname);
    }
    app_window->copy_label(buf);
  } else {
    app_window->label(EDITOR_NAME);
  }
}

void load(const char *filename) {
  if (app_text_buffer->loadfile(filename) == 0) {
    set_filename(filename);
    set_changed(false);
  } else {
    fl_alert(
          FAILED_TO_LOAD_FILE_ERROR,
          filename,
          strerror(errno)
        );
  }
}
