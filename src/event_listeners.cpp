#include <RmlUi/Core/Element.h>
#include <iostream>
#include "event_listeners.hpp"
#include "raylib_logo.hpp"
#include "globals.hpp"

void ResetListener::ProcessEvent(Rml::Event &event) {
    gameState = LOGO;
    logoState = { 0 };
}

void StartListener::ProcessEvent(Rml::Event &event) {
    menuState = FADEOUT_START;
}

void MenuAnimationListener::ProcessEvent(Rml::Event &event) {
    menuState = INVISIBLE;
    Rml::Element *el = event.GetTargetElement();
    el->SetProperty("opacity", "0");
    std::cout << "anim end" << std::endl;
}
