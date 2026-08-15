#include "menu.h"

#include "item.h"

void menu::item()
{
	// 消耗品
	function::section(SDK::FVector2D(120, layout::TOP), SDK::FVector2D(120, 260), L"消耗品");

	// 工具（两列）
	function::section(SDK::FVector2D(250, layout::TOP), SDK::FVector2D(120, 260), L"工具");
	function::section(SDK::FVector2D(380, layout::TOP), SDK::FVector2D(120, 260), L"工具");

	// 武器
	function::section(SDK::FVector2D(510, layout::TOP), SDK::FVector2D(120, 260), L"武器");

	// 其他
	function::section(SDK::FVector2D(120, 320), SDK::FVector2D(550, 140), L"其他");

#define ETB_BUTTON(_name_,_class_,_x_,_y_) \
if (function::button_color_text(" ", SDK::FVector2D(_x_, _y_), SDK::FVector2D(110, 30), L#_name_)) \
{ \
	item::get()->spawn(SDK::_class_::StaticClass()); \
}

	// 消耗品
	ETB_BUTTON(果汁, ABP_DroppedItem_Juice_C, 125, 72);
	ETB_BUTTON(杏仁水, ABP_DroppedItem_AlmondWater_C, 125, 104);
	ETB_BUTTON(液态痛苦, ABP_DroppedItem_LiquidPain_C, 125, 136);
	ETB_BUTTON(能量棒, ABP_DroppedItem_EnergyBar_C, 125, 168);
	ETB_BUTTON(果冻, ABP_DroppedItem_Jelly_C, 125, 200);

	// 工具（左列）
	ETB_BUTTON(手电筒, ABP_DroppedItem_Flashlight_C, 255, 72);
	ETB_BUTTON(信号枪, ABP_DroppedItem_FlareGun_C, 255, 104);
	ETB_BUTTON(绳子, ABP_DroppedItem_Rope_C, 255, 136);
	ETB_BUTTON(对讲机, ABP_DroppedItem_WalkieTalkie_C, 255, 168);
	ETB_BUTTON(撬棍, ABP_DroppedItem_Crowbar_C, 255, 200);
	ETB_BUTTON(相机, ABP_DroppedItem_Camera_C, 255, 232);

	// 工具（右列）
	ETB_BUTTON(温度计, ABP_DroppedItem_Thermometer_C, 385, 72);
	ETB_BUTTON(扫描仪, ABP_DroppedItem_LiDAR_C, 385, 104);
	ETB_BUTTON(潜水头盔, ABP_DroppedItem_DivingHelmet_C, 385, 136);
	ETB_BUTTON(黄色荧光棒, ABP_DroppedItem_Glowstick_Yellow_C, 385, 168);
	ETB_BUTTON(蓝色荧光棒, ABP_DroppedItem_Glowstick_Blue_C, 385, 200);
	ETB_BUTTON(红色荧光棒, ABP_DroppedItem_Glowstick_Red_C, 385, 232);
	ETB_BUTTON(荧光棒, ABP_DroppedItem_Glowstick_C, 385, 264);

	// 武器
	ETB_BUTTON(电锯, ABP_DroppedItem_Chainsaw_C, 515, 72);
	ETB_BUTTON(快速电锯, ABP_DroppedItem_Chainsaw_Fast_C, 515, 104);
	ETB_BUTTON(刀, ABP_DroppedItem_Knife_C, 515, 136);
	ETB_BUTTON(杀虫喷雾, ABP_DroppedItem_BugSpray_C, 515, 168);
	ETB_BUTTON(烟花, ABP_DroppedItem_Firework_C, 515, 200);

#undef ETB_BUTTON

	// 其他
	if (function::button_color_text(" ", SDK::FVector2D(125, 352), SDK::FVector2D(110, 30), L"玩具"))
	{
		item::get()->spawn(SDK::ABP_DroppedItem_Toy_C::StaticClass());
	}

	if (function::button_color_text(" ", SDK::FVector2D(245, 352), SDK::FVector2D(110, 30), L"门票"))
	{
		item::get()->spawn(SDK::ABP_DroppedItem_Ticket_C::StaticClass());
	}

	if (function::button_color_text(" ", SDK::FVector2D(515, 352), SDK::FVector2D(140, 30), L"吸附所有磁带"))
	{
		item::get()->interact_all("Tape_BP_C");
	}
}
