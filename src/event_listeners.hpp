#include <RmlUi/Core/EventListener.h>
#include <RmlUi/Core/Types.h>

class ResetListener : public Rml::EventListener {
    public:
        void ProcessEvent(Rml::Event& event);
};

class StartListener : public Rml::EventListener {
    public:
        void ProcessEvent(Rml::Event& event);
};

class MenuAnimationListener : public Rml::EventListener {
    public:
        void ProcessEvent(Rml::Event& event);
};
