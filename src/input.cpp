#include "input.hpp"
#include "raylib.h"

void ProcessMouseInput(Rml::Context *context) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        context->ProcessMouseButtonDown(MOUSE_LEFT_BUTTON, 0);
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && !context->IsMouseInteracting()) {
        context->ProcessMouseButtonDown(MOUSE_RIGHT_BUTTON, 0);
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        context->ProcessMouseButtonUp(MOUSE_LEFT_BUTTON, 0);
    }
    if (IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)) {
        context->ProcessMouseButtonUp(MOUSE_RIGHT_BUTTON, 0);
    }
    Vector2 mouseDelta = GetMouseDelta();
    if (mouseDelta.x != 0 || mouseDelta.y != 0) {
        context->ProcessMouseMove(GetMouseX()/3, GetMouseY()/3, 0);
    }
}
