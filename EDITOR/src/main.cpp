#include "Application.h"

extern "C"
{
	__declspec( dllexport ) unsigned long NvOptimusEnablement = 0x00000001;
}

int main()
{
	auto& app = Application::GetInstance();
	app.Run();
	return 0;
}
