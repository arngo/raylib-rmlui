#include <iostream>
#include "event_listeners.hpp"
#include "raylib_logo.hpp"
void ResetListener::ProcessEvent(Rml::Event &event) {
    gameState = 0;
    logoState = { 0 };
}
