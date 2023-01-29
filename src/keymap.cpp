#include <regex>
#include "ftxui/component/captured_mouse.hpp"      // for ftxui
#include "keymap.hpp"
#include <iostream>

namespace StartUp {
  using namespace ftxui;
  Event keymap_transform(const std::string& keymap) {
    if(std::regex_match(keymap, std::regex("<[a-zA-Z]>"))) {
        return Event::Character(keymap[1]);
    }
    else if(std::regex_match(keymap, std::regex("<C-[a-zA-Z]>"))) {
      return Event::Special({static_cast<char>(std::tolower(keymap[3]) - 'a' + 1)});
    }
    else if(std::regex_match(keymap, std::regex("<M-[a-zA-Z]>"))) {
      return Event::Special({27, static_cast<char>(std::tolower(keymap[3]) - '\0')});
    }
    else if (keymap == "<Esc>" | keymap == "<esc>" | keymap == "<ESC>"){
      return Event::Escape;
    }
  }
  // const Event KEY_CLOSE = keymap_transform(conf.keymap_list[5]);

  // const Event KEY_CLOSE = keymap_transform(conf.keymap_list[5]);
  bool KeyCallback(ScreenInteractive& screen, UI& ui, Event& event ) {
    const Event& KEY_CLOSE =  keymap_transform(conf.keymap_list[5]);
    if (!history_files_shown && !dotfiles_shown && !paths_shown && event == KEY_CLOSE ) {
      screen.ExitLoopClosure()();
      return true;
    }
    if (history_files_shown && event == KEY_CLOSE) {
      history_files_shown = false;
      return true;

    }
    if (dotfiles_shown && event == KEY_CLOSE ) {
      dotfiles_shown = false;
      return true;
    }
    if (paths_shown && event == KEY_CLOSE ) {
      paths_shown = false;
      return true;
    }
    if ( !(history_files_shown ^ dotfiles_shown ^ paths_shown) && event == keymap_transform(conf.keymap_list[0])) {
      history_files_shown = true;
      return true;
    }
    if ( !(history_files_shown ^ dotfiles_shown ^ paths_shown) && event == keymap_transform(conf.keymap_list[3])) {
      dotfiles_shown = true;
      return true;
    }
    if ( !(history_files_shown ^ dotfiles_shown ^ paths_shown) && event == keymap_transform(conf.keymap_list[4])) {
      paths_shown = true;
      return true;
    }
    if ( !(history_files_shown ^ dotfiles_shown ^ paths_shown) && event == keymap_transform(conf.keymap_list[1])) {
      screen.ExitLoopClosure()();
      system(conf.find_file_cmd);
      return true;
    }
    if ( !(history_files_shown ^ dotfiles_shown ^ paths_shown) && event == keymap_transform(conf.keymap_list[2])) {
      screen.ExitLoopClosure()();
      system(conf.file_browser_cmd);
      return true;
    }
    // <enter> select a item
    if (!history_files_shown && !dotfiles_shown && !paths_shown && event == Event::Return) {
      ui.options_box->OnEvent(event);
      switch(conf.radiobox_selected) {
        case 0:
          history_files_shown = true;
          break;
        case 1:
          screen.ExitLoopClosure()();
          system(conf.find_file_cmd);
          break;
        case 2:
          screen.ExitLoopClosure()();
          system(conf.file_browser_cmd);
          break;
        case 3:
          dotfiles_shown = true;
          break;
        case 4:
          paths_shown = true;
          break;
        case 5:
          screen.ExitLoopClosure()();
          break;
      }
      return true;
    }
    if (history_files_shown && event == Event::Return) {
      ui.history_files_box->OnEvent(event);
      history_files_shown = false;
      screen.ExitLoopClosure()();
      system((conf.editor + " " + ui.history_files_list_bak[conf.history_files_selected]).c_str());
      return true;
    }
    if (dotfiles_shown && event == Event::Return) {
      ui.dotfiles_box->OnEvent(event);
      dotfiles_shown = false;
      screen.ExitLoopClosure()();
      system((conf.editor + " " + ui.dotfiles_list_bak[conf.dotfiles_selected]).c_str());
      return true;
    }
    if (paths_shown && event == Event::Return) {
      ui.paths_box->OnEvent(event);
      screen.ExitLoopClosure()();
      paths_shown = false;
      system((std::string("echo cd ") + ui.paths_list_bak[conf.paths_selected] + "> /tmp/startup_cd.sh").c_str());
      return true;
    }
    return false;
  }
}
