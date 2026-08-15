#include "menu.h"

void menu::level()
{
#define ETB_BUTTON(_name_,_lvname_,_x_,_y_) \
if (function::button_color_text("", SDK::FVector2D(_x_, _y_), SDK::FVector2D(150, 30), L#_name_)) \
{ \
	const std::wstring cmd = std::wstring(L"ServerTravel ") + std::wstring(L#_lvname_); \
	SDK::UKismetSystemLibrary::ExecuteConsoleCommand(gvalue::world, cmd.c_str(), gvalue::controller); \
}

	// 列1
	ETB_BUTTON(Level0, Level0, 135, 72);
	ETB_BUTTON(Level1, TopFloor, 135, 106);
	ETB_BUTTON(Level2, Pipes, 135, 140);
	ETB_BUTTON(Level3, ElectricalStation, 135, 174);
	ETB_BUTTON(Level4, Office, 135, 208);
	ETB_BUTTON(Level5, Hotel, 135, 242);
	ETB_BUTTON(LevelFun, LevelFun, 135, 276);
	ETB_BUTTON(Level37, Poolrooms, 135, 310);
	ETB_BUTTON(Level!, LevelRun, 135, 344);
	ETB_BUTTON(LevelEnd, TheEnd, 135, 378);

	// 列2
	ETB_BUTTON(Level94, Level94, 305, 72);
	ETB_BUTTON(Level6, LightsOut, 305, 106);
	ETB_BUTTON(Level7, OceanMap, 305, 140);
	ETB_BUTTON(Level8, CaveLevel, 305, 174);
	ETB_BUTTON(Level0.11, Level05, 305, 208);
	ETB_BUTTON(Level9, Level9, 305, 242);
	ETB_BUTTON(Level10, Level10, 305, 276);
	ETB_BUTTON(Level3999, Level3999, 305, 310);
	ETB_BUTTON(Level0.2, Level07, 305, 344);
	ETB_BUTTON(零食屋, Snackrooms, 305, 378);

	// 列3
	ETB_BUTTON(Level!-!, LevelDash, 485, 72);
	ETB_BUTTON(Level188, Level188_Expanded, 485, 106);
	ETB_BUTTON(Level37.2, Poolrooms_Expanded, 485, 140);
	ETB_BUTTON(LevelFun + , LevelFun_Expanded, 485, 174);
	ETB_BUTTON(Level52, Level52, 485, 208);
	ETB_BUTTON(Level55.1, TunnelLevel, 485, 242);
	ETB_BUTTON(Level922, Level922, 485, 276);
	ETB_BUTTON(Level974, Level974, 485, 310);
	ETB_BUTTON(过度生长, GraffitiLevel, 485, 344);
	ETB_BUTTON(草屋, Grassrooms_Expanded, 485, 378);

#undef ETB_BUTTON
}
