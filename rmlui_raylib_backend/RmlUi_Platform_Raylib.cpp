
#include "RmlUi_Platform_Raylib.h"
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Input.h>
#include <RmlUi/Core/SystemInterface.h>
#include <raylib.h>

double SystemInterface_Raylib::GetElapsedTime()
{
	return double(GetTime()) / 1000.0;
}
