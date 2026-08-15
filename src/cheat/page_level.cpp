#include "menu.h"

void menu::level()
{
	param::size.X = 650;

	function::pice(SDK::FVector2D(110, 10), SDK::FVector2D(param::size.X - 120, param::size.Y - 20));

#define ETB_BUTTON(_name_,_lvname_,_x_,_y_) \
if (function::button_color_text("", SDK::FVector2D(_x_, _y_), SDK::FVector2D(120, 30), L#_name_)) \
{ \
	const std::wstring cmd = std::wstring(L"ServerTravel ") + std::wstring(L#_lvname_); \
	SDK::UKismetSystemLibrary::ExecuteConsoleCommand(gvalue::world, cmd.c_str(), gvalue::controller); \
}

	//line 1
	ETB_BUTTON(Level0, Level0, 120, 20);
	ETB_BUTTON(Level1, TopFloor, 120, 60);
	ETB_BUTTON(Level2, Pipes, 120, 100);
	ETB_BUTTON(Level3, ElectricalStation, 120, 140);
	ETB_BUTTON(Level4, Office, 120, 180);
	ETB_BUTTON(Level5, Hotel, 120, 220);
	ETB_BUTTON(LevelFun, LevelFun, 120, 260);
	ETB_BUTTON(Level37, Poolrooms, 120, 300);
	ETB_BUTTON(Level!, LevelRun, 120, 340);

	//line 2
	ETB_BUTTON(LevelEnd, TheEnd, 250, 20);
	ETB_BUTTON(Level94, Level94, 250, 60);
	ETB_BUTTON(Level6, LightsOut, 250, 100);
	ETB_BUTTON(Level7, OceanMap, 250, 140);
	ETB_BUTTON(Level8, CaveLevel, 250, 180);
	ETB_BUTTON(Level0.11, Level05, 250, 220);
	ETB_BUTTON(Level9, Level9, 250, 260);
	ETB_BUTTON(Level10, Level10, 250, 300);
	ETB_BUTTON(Level3999, Level3999, 250, 340);

	//line 3
	ETB_BUTTON(Level0.2, Level07, 380, 20);
	ETB_BUTTON(零食屋, Snackrooms, 380, 60);
	ETB_BUTTON(Level!- !, LevelDash, 380, 100);
	ETB_BUTTON(Level188, Level188_Expanded, 380, 140);
	ETB_BUTTON(Level37.2, Poolrooms_Expanded, 380, 180);
	ETB_BUTTON(LevelFun + , LevelFun_Expanded, 380, 220);
	ETB_BUTTON(Level52, Level52, 380, 260);
	ETB_BUTTON(Level55.1, TunnelLevel, 380, 300);
	ETB_BUTTON(Level922, Level922, 380, 340);

	//line 4
	ETB_BUTTON(Level974, Level974, 510, 20);
	ETB_BUTTON(过度生长, GraffitiLevel, 510, 60);
	ETB_BUTTON(草屋, Grassrooms_Expanded, 510, 100);

#undef ETB_BUTTON
}
