#define RMLUI_STATIC_LIB

#include <RmlUi/Core/Input.h>
#include <RmlUi/Core/SystemInterface.h>
#include <RmlUi/Core/Types.h>
#include <raylib.h>

class SystemInterface_Raylib : public Rml::SystemInterface {
public:
		double GetElapsedTime() override;
};
