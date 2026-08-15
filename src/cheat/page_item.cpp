#include "menu.h"

#include "item.h"

void menu::item()
{
	param::size.X = 670;
	param::size.Y = 520;

	function::pice(SDK::FVector2D(110, 10), SDK::FVector2D(400, param::size.Y - 20));

#define ETB_BUTTON(_name_,_class_,_x_,_y_) \
if (function::button_color_text(" ", SDK::FVector2D(_x_, _y_), SDK::FVector2D(120, 30),L#_name_)) \
{ \
	item::get()->spawn(SDK::_class_::StaticClass()); \
}

#define ETB_HEADER(_y_,_text_) \
	function::text(SDK::FVector2D(120, _y_), L#_text_);

	// 消耗品
	ETB_HEADER(20, 消耗品);
	ETB_BUTTON(果汁, ABP_DroppedItem_Juice_C, 120, 45);
	ETB_BUTTON(杏仁水, ABP_DroppedItem_AlmondWater_C, 250, 45);
	ETB_BUTTON(液态痛苦, ABP_DroppedItem_LiquidPain_C, 380, 45);
	ETB_BUTTON(能量棒, ABP_DroppedItem_EnergyBar_C, 120, 80);
	ETB_BUTTON(果冻, ABP_DroppedItem_Jelly_C, 250, 80);

	// 工具
	ETB_HEADER(120, 工具);
	ETB_BUTTON(手电筒, ABP_DroppedItem_Flashlight_C, 120, 145);
	ETB_BUTTON(信号枪, ABP_DroppedItem_FlareGun_C, 250, 145);
	ETB_BUTTON(绳子, ABP_DroppedItem_Rope_C, 380, 145);
	ETB_BUTTON(对讲机, ABP_DroppedItem_WalkieTalkie_C, 120, 180);
	ETB_BUTTON(撬棍, ABP_DroppedItem_Crowbar_C, 250, 180);
	ETB_BUTTON(相机, ABP_DroppedItem_Camera_C, 380, 180);
	ETB_BUTTON(温度计, ABP_DroppedItem_Thermometer_C, 120, 215);
	ETB_BUTTON(扫描仪, ABP_DroppedItem_LiDAR_C, 250, 215);
	ETB_BUTTON(潜水头盔, ABP_DroppedItem_DivingHelmet_C, 380, 215);
	ETB_BUTTON(黄色荧光棒, ABP_DroppedItem_Glowstick_Yellow_C, 120, 250);
	ETB_BUTTON(蓝色荧光棒, ABP_DroppedItem_Glowstick_Blue_C, 250, 250);
	ETB_BUTTON(红色荧光棒, ABP_DroppedItem_Glowstick_Red_C, 380, 250);
	ETB_BUTTON(荧光棒, ABP_DroppedItem_Glowstick_C, 120, 285);

	// 武器
	ETB_HEADER(325, 武器);
	ETB_BUTTON(电锯, ABP_DroppedItem_Chainsaw_C, 120, 350);
	ETB_BUTTON(快速电锯, ABP_DroppedItem_Chainsaw_Fast_C, 250, 350);
	ETB_BUTTON(刀, ABP_DroppedItem_Knife_C, 380, 350);
	ETB_BUTTON(杀虫喷雾, ABP_DroppedItem_BugSpray_C, 120, 385);
	ETB_BUTTON(烟花, ABP_DroppedItem_Firework_C, 250, 385);

	// 其他
	ETB_HEADER(425, 其他);
	ETB_BUTTON(玩具, ABP_DroppedItem_Toy_C, 120, 450);
	ETB_BUTTON(门票, ABP_DroppedItem_Ticket_C, 250, 450);

#undef ETB_BUTTON
#undef ETB_HEADER

	function::pice(SDK::FVector2D(520, 10), SDK::FVector2D(140, param::size.Y - 20));

	if (function::button_color_text(" ", SDK::FVector2D(530, 20), SDK::FVector2D(120, 30), L"吸附所有磁带"))
	{
		item::get()->interact_all("Tape_BP_C");
	}
}
