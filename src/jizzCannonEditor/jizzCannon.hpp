#pragma once

#include<FL/Fl.H>
#include<FL/Fl_Double_Window.H>
#include<FL/fl_ask.H>
#include<FL/fl_string_functions.h>
#include<FL/Fl_Menu_Bar.H>
#include<FL/Fl_Text_Editor.H>
#include<FL/Fl_Text_Buffer.H>
#include<FL/Fl_Native_File_Chooser.H>
#include<FL/platform.H>

#include<errno.h>

#include"labels.hpp"

class JizzCannon {
  
  private:
    const int WIDTH = 640;
    const int HEIGHT = 480;
    const int MENU_HEIGHT = 25;
    
    Fl_Double_Window *jc_window = NULL;
    Fl_Menu_Bar *jc_menu_bar = NULL;
    Fl_Text_Editor *jc_editor = NULL;
    Fl_Text_Editor *jc_split_editor = NULL;
    Fl_Text_Buffer *jc_text_buffer = NULL;
    
    void init_widgets();
    void populate_widgets();
     
    // NOTE: from here on, it's just callbacks 
    void callback_text_changed(int, int n_inserted, int n_deleted, int, const char *, void *);
    void callback_editor_quit(Fl_Widget *, void *);
  
  public:
    JizzCannon();
    ~JizzCannon();
    int run(int &argc, char **argv);
};
