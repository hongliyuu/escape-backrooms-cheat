#include "menu.h"

#include "entity.h"
#include "visual.h"


void menu::entity()
{
	param::size.X = 560;
	param::size.Y = 480;

	function::pice(SDK::FVector2D(110, 10), SDK::FVector2D(160, param::size.Y - 20));
	function::pice(SDK::FVector2D(280, 10), SDK::FVector2D(270, param::size.Y - 20));

	if (function::button_color_text(" ", SDK::FVector2D(120, 20), SDK::FVector2D(140, 30), L"干死所有实体"))
	{
		entity::get()->kill_all();
	}

	if (function::button_color_text(" ", SDK::FVector2D(120, 55), SDK::FVector2D(140, 30), L"删除细菌"))
	{
		entity::get()->kill("Bacteria_Roaming_BP_C");
		entity::get()->kill("Bacteria_BP_C");
	}

	if (function::button_color_text(" ", SDK::FVector2D(120, 90), SDK::FVector2D(140, 30), L"删除笑魇"))
	{
		entity::get()->kill("Smiler_BP2_C");
		entity::get()->kill("BP_Roaming_Smiler_C");
		entity::get()->kill("BP_Smiler_Dash_C");
	}

	if (function::button_color_text(" ", SDK::FVector2D(120, 125), SDK::FVector2D(140, 30), L"删除派对客"))
	{
		entity::get()->kill("BP_RoamingPartygoer_Idle_C");
		entity::get()->kill("BP_RoamingPartygoer_C");
		entity::get()->kill("BP_RoamingPartygoer_Slow_C");
		entity::get()->kill("BP_DarkPartyGoer_C");
		entity::get()->kill("BP_HidingPartyGoer_C");
	}

	if (function::button_color_text(" ", SDK::FVector2D(120, 160), SDK::FVector2D(140, 30), L"删除死亡飞蛾"))
	{
		entity::get()->kill("BP_Moth_C");
		entity::get()->kill("BP_Cave_Moth_C");
	}

	if (function::button_color_text(" ", SDK::FVector2D(120, 195), SDK::FVector2D(140, 30), L"删除窃皮者"))
	{
		entity::get()->kill("BP_SkinStealer_C");
		entity::get()->kill("BP_SkinStealer_Cave_C");
		entity::get()->kill("BP_SkinStealer_Level07_C");
		entity::get()->kill("BP_SkinStealer_Hotel_C");
	}

	if (function::button_color_text(" ", SDK::FVector2D(120, 230), SDK::FVector2D(140, 30), L"删除猎犬"))
	{
		entity::get()->kill("BP_Hound_C");
		entity::get()->kill("BP_Hound_Hotel_C");
	}

	if (function::button_color_text(" ", SDK::FVector2D(120, 265), SDK::FVector2D(140, 30), L"删除悲尸"))
	{
		entity::get()->kill("BP_Wretch_C");
		entity::get()->kill("BP_Wretch_House_C");
	}

	if (function::button_color_text(" ", SDK::FVector2D(120, 300), SDK::FVector2D(140, 30), L"删除偷骨者"))
	{
		entity::get()->kill("BP_BoneThief_C");
	}

	if (function::button_color_text(" ", SDK::FVector2D(120, 335), SDK::FVector2D(140, 30), L"删除鱼"))
	{
		entity::get()->kill("BP_Fish_C");
		entity::get()->kill("BP_Fish_Scare_C");
	}

	if (function::button_color_text(" ", SDK::FVector2D(120, 370), SDK::FVector2D(140, 30), L"冻结所有实体"))
	{
		entity::get()->freeze_all();
	}

	if (function::button_color_text(" ", SDK::FVector2D(120, 405), SDK::FVector2D(140, 30), L"解冻所有实体"))
	{
		entity::get()->unfreeze_all();
	}

#define ETB_BUTTON(_name_,_class_,_x_,_y_) \
if (function::button_color_text(" ", SDK::FVector2D(_x_, _y_), SDK::FVector2D(120, 30), L#_name_)) \
{ \
	entity::get()->spawn(SDK::_class_::StaticClass()); \
}

	ETB_BUTTON(细菌1, ABacteria_BP_C, 290, 20);
	ETB_BUTTON(细菌2, ABacteria_Roaming_BP_C, 290, 55);
	ETB_BUTTON(窃皮者1, ABP_SkinStealer_C, 290, 90);
	ETB_BUTTON(窃皮者2, ABP_SkinStealer_Level07_C, 290, 125);
	ETB_BUTTON(偷骨者, ABP_BoneThief_C, 290, 160);
	ETB_BUTTON(笑魇, ABP_Roaming_Smiler_C, 290, 195);
	ETB_BUTTON(动画, ABP_Animation_C, 420, 20);
	ETB_BUTTON(死亡飞蛾, ABP_Moth_C, 420, 55);
	ETB_BUTTON(猎犬, ABP_Hound_C, 420, 90);
	ETB_BUTTON(鱼, ABP_Fish_C, 420, 125);
	ETB_BUTTON(惊吓鱼, ABP_Fish_Scare_C, 420, 160);

#undef ETB_BUTTON

#define ETB_BUTTON(_name_,_class_,_x_,_y_) \
if (function::button_color_text(" ", SDK::FVector2D(_x_, _y_), SDK::FVector2D(120, 30), L#_name_)) \
{ \
	entity::get()->spawn(SDK::UObject::FindClassFast(#_class_)); \
}

	ETB_BUTTON(Kitty, BP_Entity974_C, 290, 230);
	ETB_BUTTON(派对客1, BP_RoamingPartygoer_Idle_C, 290, 265);
	ETB_BUTTON(派对客2, BP_RoamingPartygoer_C, 290, 300);
	ETB_BUTTON(派对客3, BP_DarkPartyGoer_C, 290, 335);
	ETB_BUTTON(派对客4, BP_HidingPartyGoer_C, 290, 370);
	ETB_BUTTON(大眼怪, BP_NPC_Type01_C, 420, 195);
	ETB_BUTTON(池核肢团, BP_Clump_Poolrooms_C, 420, 230);
	ETB_BUTTON(悲尸, BP_Wretch_House_C, 420, 265);
	ETB_BUTTON(无面灵, BP_Faceling_C, 420, 300);
	ETB_BUTTON(蜘蛛怪, BP_Membri_C, 420, 335);

#undef ETB_BUTTON

	auto flush_entity=[&]()
	{
		param::entity_list.clear();
		if (!entity::get()->is_valid(gvalue::world))
			return;

		SDK::TArray<SDK::AActor*> actor_list;
		SDK::UGameplayStatics::GetAllActorsOfClass(gvalue::world, SDK::ACharacter::StaticClass(), &actor_list);
		for (SDK::AActor* actor : actor_list)
		{
			if (!entity::get()->is_valid(actor))
				continue;
			if (actor->IsA(SDK::ABP_Explorer_C::StaticClass()) ||
				actor->IsA(SDK::ABPCharacter_Demo_C::StaticClass()))
			{
				continue;
			}
			SDK::ACharacter* cur_pawn = static_cast<SDK::ACharacter*>(actor);
			param::entity_list.emplace_back(cur_pawn);
		}
	};

	auto entity_box = [&](SDK::ACharacter* pawn, SDK::FVector2D pos)
		{
			if (!entity::get()->is_valid(pawn) || !pawn->IsA(SDK::ACharacter::StaticClass()))
			{
				flush_entity();
				return;
			}
			function::pice(pos, SDK::FVector2D(290, 40.0f));
			const std::wstring name = visual::get()->find_name(pawn->Class->Name.ToString());
			function::text(
				pos + SDK::FVector2D(10, 12),
				name.c_str()
			);

			if (entity::get()->can_control(pawn))
			{
				if (function::button_color_text(" ", pos + SDK::FVector2D(140, 10), SDK::FVector2D(40, 20), L"控制"))
				{
					entity::get()->poss(pawn);
				}
			}
			else
			{
				function::text(pos + SDK::FVector2D(145, 12), L"不可控");
			}

			if (function::button_color_text(" ", pos + SDK::FVector2D(185, 10), SDK::FVector2D(40, 20), L"删除"))
			{
				entity::get()->destroy(pawn);
				flush_entity();
				return;
			}

			if (entity::get()->is_frozen(pawn))
			{
				if (function::button_color_text(" ", pos + SDK::FVector2D(230, 10), SDK::FVector2D(55, 20), L"解冻"))
				{
					entity::get()->unfreeze(pawn);
				}
			}
			else
			{
				if (function::button_color_text(" ", pos + SDK::FVector2D(230, 10), SDK::FVector2D(55, 20), L"冻结"))
				{
					entity::get()->freeze(pawn);
				}
			}
		};

	render::fill_box(
		SDK::FVector2D(param::pos.X + (param::size.X + 18) * gvalue::menu_scale, param::pos.Y - 2 * gvalue::menu_scale),
		SDK::FVector2D((300 + 4) * gvalue::menu_scale, (150 + param::entity_list.size() * 50 + 4) * gvalue::menu_scale), 
		color::get()->outline_col
	);

	render::fill_box(
		SDK::FVector2D(param::pos.X + (param::size.X + 20) * gvalue::menu_scale, param::pos.Y),
		SDK::FVector2D(300 * gvalue::menu_scale, (150 + param::entity_list.size() * 50) * gvalue::menu_scale), 
		color::get()->back_col
	);

	if (function::button_color_text(" ", SDK::FVector2D(param::size.X + 30, 10), SDK::FVector2D(240, 30), L"刷新实体列表"))
	{
		flush_entity();
	}

	if (function::button_color_text(" ", SDK::FVector2D(param::size.X + 30, 50), SDK::FVector2D(240, 30), L"取消控制实体"))
	{
		entity::get()->unposs();
	}

	{
		const int speed = 4000 * gvalue::entity_speed;
		const std::wstring wstr = L"被控实体速度：" + std::to_wstring(speed);
		function::text(SDK::FVector2D(param::size.X + 30, 90), wstr.c_str());
		function::drag_bar("entity_speed", SDK::FVector2D(param::size.X + 30, 120), SDK::FVector2D(240, 20), SDK::FVector2D(10, 20), &gvalue::entity_speed);
	}

	for (int i = 0; i < param::entity_list.size(); i++)
	{
		entity_box(param::entity_list[i], SDK::FVector2D(param::size.X + 30, 150 + 50 * i));
	}
}
