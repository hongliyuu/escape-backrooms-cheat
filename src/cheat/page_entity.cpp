#include "menu.h"

#include "entity.h"
#include "visual.h"


void menu::entity()
{
	// 列1：删除实体
	function::section(SDK::FVector2D(120, layout::TOP), SDK::FVector2D(170, 420), L"删除实体");

	if (function::button_color_text(" ", SDK::FVector2D(135, 72), SDK::FVector2D(140, 30), L"干死所有实体"))
	{
		entity::get()->kill_all();
	}

	{
		if (function::button_color_text(" ", SDK::FVector2D(135, 104), SDK::FVector2D(140, 30), L"删除细菌"))
		{
			entity::get()->kill("Bacteria_Roaming_BP_C");
			entity::get()->kill("Bacteria_BP_C");
		}
	}

	{
		if (function::button_color_text(" ", SDK::FVector2D(135, 136), SDK::FVector2D(140, 30), L"删除笑魇"))
		{
			entity::get()->kill("Smiler_BP2_C");
			entity::get()->kill("BP_Roaming_Smiler_C");
			entity::get()->kill("BP_Smiler_Dash_C");
		}
	}

	{
		if (function::button_color_text(" ", SDK::FVector2D(135, 168), SDK::FVector2D(140, 30), L"删除派对客"))
		{
			entity::get()->kill("BP_RoamingPartygoer_Idle_C");
			entity::get()->kill("BP_RoamingPartygoer_C");
			entity::get()->kill("BP_RoamingPartygoer_Slow_C");
			entity::get()->kill("BP_DarkPartyGoer_C");
			entity::get()->kill("BP_HidingPartyGoer_C");
		}
	}

	{
		if (function::button_color_text(" ", SDK::FVector2D(135, 200), SDK::FVector2D(140, 30), L"删除死亡飞蛾"))
		{
			entity::get()->kill("BP_Moth_C");
			entity::get()->kill("BP_Cave_Moth_C");
		}
	}

	{
		if (function::button_color_text(" ", SDK::FVector2D(135, 232), SDK::FVector2D(140, 30), L"删除窃皮者"))
		{
			entity::get()->kill("BP_SkinStealer_C");
			entity::get()->kill("BP_SkinStealer_Cave_C");
			entity::get()->kill("BP_SkinStealer_Level07_C");
			entity::get()->kill("BP_SkinStealer_Hotel_C");
		}
	}

	{
		if (function::button_color_text(" ", SDK::FVector2D(135, 264), SDK::FVector2D(140, 30), L"删除猎犬"))
		{
			entity::get()->kill("BP_Hound_C");
			entity::get()->kill("BP_Hound_Hotel_C");
		}
	}

	{
		if (function::button_color_text(" ", SDK::FVector2D(135, 296), SDK::FVector2D(140, 30), L"删除悲尸"))
		{
			entity::get()->kill("BP_Wretch_C");
			entity::get()->kill("BP_Wretch_House_C");
		}
	}

	{
		if (function::button_color_text(" ", SDK::FVector2D(135, 328), SDK::FVector2D(140, 30), L"删除偷骨者"))
		{
			entity::get()->kill("BP_BoneThief_C");
		}
	}

	{
		if (function::button_color_text(" ", SDK::FVector2D(135, 360), SDK::FVector2D(140, 30), L"删除鱼"))
		{
			entity::get()->kill("BP_Fish_C");
			entity::get()->kill("BP_Fish_Scare_C");
		}
	}

	{
		if (function::button_color_text(" ", SDK::FVector2D(135, 392), SDK::FVector2D(140, 30), L"冻结所有实体"))
		{
			entity::get()->freeze_all();
		}
	}

	{
		if (function::button_color_text(" ", SDK::FVector2D(135, 424), SDK::FVector2D(140, 30), L"解冻所有实体"))
		{
			entity::get()->unfreeze_all();
		}
	}

	// 列2：生成实体
	function::section(SDK::FVector2D(300, layout::TOP), SDK::FVector2D(170, 420), L"生成实体");

#define ETB_BUTTON(_name_,_class_,_x_,_y_) \
if (function::button_color_text(" ", SDK::FVector2D(_x_, _y_), SDK::FVector2D(140, 30), L#_name_)) \
{ \
	entity::get()->spawn(SDK::_class_::StaticClass()); \
}

	ETB_BUTTON(细菌1, ABacteria_BP_C, 315, 72);
	ETB_BUTTON(细菌2, ABacteria_Roaming_BP_C, 315, 104);
	ETB_BUTTON(窃皮者1, ABP_SkinStealer_C, 315, 136);
	ETB_BUTTON(窃皮者2, ABP_SkinStealer_Level07_C, 315, 168);
	ETB_BUTTON(偷骨者, ABP_BoneThief_C, 315, 200);
	ETB_BUTTON(笑魇, ABP_Roaming_Smiler_C, 315, 232);
	ETB_BUTTON(动画, ABP_Animation_C, 315, 264);
	ETB_BUTTON(死亡飞蛾, ABP_Moth_C, 315, 296);
	ETB_BUTTON(猎犬, ABP_Hound_C, 315, 328);
	ETB_BUTTON(鱼, ABP_Fish_C, 315, 360);
	ETB_BUTTON(惊吓鱼, ABP_Fish_Scare_C, 315, 392);

#undef ETB_BUTTON

	// 列3：生成实体（第二组）
	function::section(SDK::FVector2D(480, layout::TOP), SDK::FVector2D(170, 400), L"生成实体");

#define ETB_BUTTON(_name_,_class_,_x_,_y_) \
if (function::button_color_text(" ", SDK::FVector2D(_x_, _y_), SDK::FVector2D(140, 30), L#_name_)) \
{ \
	entity::get()->spawn(SDK::UObject::FindClassFast(#_class_)); \
}

	ETB_BUTTON(派对客1, BP_RoamingPartygoer_Idle_C, 495, 72);
	ETB_BUTTON(派对客2, BP_RoamingPartygoer_C, 495, 104);
	ETB_BUTTON(派对客3, BP_DarkPartyGoer_C, 495, 136);
	ETB_BUTTON(派对客4, BP_HidingPartyGoer_C, 495, 168);
	ETB_BUTTON(大眼怪, BP_NPC_Type01_C, 495, 200);
	ETB_BUTTON(池核肢团, BP_Clump_Poolrooms_C, 495, 232);
	ETB_BUTTON(悲尸, BP_Wretch_House_C, 495, 264);
	ETB_BUTTON(无面灵, BP_Faceling_C, 495, 296);
	ETB_BUTTON(蜘蛛怪, BP_Membri_C, 495, 328);
	ETB_BUTTON(Kitty, BP_Entity974_C, 495, 360);

#undef ETB_BUTTON

	{
		const int speed = 4000 * gvalue::entity_speed;
		const std::wstring wstr = L"被控实体速度：" + std::to_wstring(speed);
		function::text(SDK::FVector2D(490, 404), wstr.c_str());
		function::drag_bar("entity_speed", SDK::FVector2D(490, 432), SDK::FVector2D(150, 20), SDK::FVector2D(10, 20), &gvalue::entity_speed);
	}

	auto flush_entity = [&]()
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

	// 页面打开期间每 1 秒自动刷新一次（进入首帧即触发，等价打开即刷新）
	static float list_timer = 0.0f;
	list_timer -= gvalue::delta_time;
	if (list_timer <= 0.0f)
	{
		list_timer = 1.0f;
		flush_entity();
	}

	auto entity_box = [&](SDK::ACharacter* pawn, SDK::FVector2D pos)
		{
			if (!entity::get()->is_valid(pawn) || !pawn->IsA(SDK::ACharacter::StaticClass()))
			{
				flush_entity();
				return;
			}
			function::pice(pos, SDK::FVector2D(460, 40.0f));
			const std::wstring name = visual::get()->find_name(pawn->Class->Name.ToString());
			function::text(
				pos + SDK::FVector2D(10, 12),
				name.c_str()
			);

			if (entity::get()->can_control(pawn))
			{
				if (function::button_color_text(" ", pos + SDK::FVector2D(300, 10), SDK::FVector2D(50, 20), L"控制"))
				{
					entity::get()->poss(pawn);
				}
			}
			else
			{
				function::text(pos + SDK::FVector2D(305, 12), L"不可控");
			}

			if (function::button_color_text(" ", pos + SDK::FVector2D(355, 10), SDK::FVector2D(50, 20), L"删除"))
			{
				entity::get()->destroy(pawn);
				flush_entity();
				return;
			}

			if (entity::get()->is_frozen(pawn))
			{
				if (function::button_color_text(" ", pos + SDK::FVector2D(410, 10), SDK::FVector2D(45, 20), L"解冻"))
				{
					entity::get()->unfreeze(pawn);
				}
			}
			else
			{
				if (function::button_color_text(" ", pos + SDK::FVector2D(410, 10), SDK::FVector2D(45, 20), L"冻结"))
				{
					entity::get()->freeze(pawn);
				}
			}
		};

	// 列表分页：每页最多显示 6 行，超出用 ◀/▶ 翻页
	const int max_rows = 6;
	const int total_rows = static_cast<int>(param::entity_list.size());
	const int max_page = total_rows <= 0 ? 0 : (total_rows - 1) / max_rows;
	static int list_page = 0;
	if (list_page > max_page) list_page = max_page;
	const int visible_rows = total_rows < max_rows ? total_rows : max_rows;
	const int start_row = list_page * max_rows;

	// 实体列表栏：与主面板同高无缝拼接，宽度与玩家列表统一
	render::fill_box(
		SDK::FVector2D(param::pos.X + (param::size.X + 18) * gvalue::menu_scale, param::pos.Y - 2 * gvalue::menu_scale),
		SDK::FVector2D((500 + 4) * gvalue::menu_scale, (param::size.Y + 4) * gvalue::menu_scale),
		color::get()->outline_col
	);

	render::fill_box(
		SDK::FVector2D(param::pos.X + (param::size.X + 20) * gvalue::menu_scale, param::pos.Y),
		SDK::FVector2D(500 * gvalue::menu_scale, param::size.Y * gvalue::menu_scale),
		color::get()->back_col
	);

	// 列表栏标题条（与主面板标题栏统一）
	render::fill_box(
		SDK::FVector2D(param::pos.X + (param::size.X + 20) * gvalue::menu_scale, param::pos.Y),
		SDK::FVector2D(500 * gvalue::menu_scale, 28 * gvalue::menu_scale),
		color::get()->normal_col
	);
	render::fill_box(
		SDK::FVector2D(param::pos.X + (param::size.X + 20) * gvalue::menu_scale, param::pos.Y + 28 * gvalue::menu_scale),
		SDK::FVector2D(500 * gvalue::menu_scale, 2 * gvalue::menu_scale),
		color::get()->outline_col
	);
	function::text(SDK::FVector2D(param::size.X + 30, 7), L"实体列表");

	if (function::button_color_text(" ", SDK::FVector2D(param::size.X + 30, 40), SDK::FVector2D(440, 30), L"取消控制实体"))
	{
		entity::get()->unposs();
	}

	{
		const int speed = 4000 * gvalue::entity_speed;
		const std::wstring wstr = L"被控实体速度：" + std::to_wstring(speed);
		function::text(SDK::FVector2D(param::size.X + 30, 82), wstr.c_str());
		function::drag_bar("entity_speed", SDK::FVector2D(param::size.X + 30, 110), SDK::FVector2D(440, 20), SDK::FVector2D(10, 20), &gvalue::entity_speed);
	}

	// 翻页控件
	if (list_page > 0 && function::button_color_text(" ", SDK::FVector2D(param::size.X + 30, 145), SDK::FVector2D(40, 30), L"上页"))
		list_page--;
	if (list_page < max_page && function::button_color_text(" ", SDK::FVector2D(param::size.X + 480, 145), SDK::FVector2D(40, 30), L"下页"))
		list_page++;
	if (max_page > 0)
	{
		const std::wstring page_str = L"第 " + std::to_wstring(list_page + 1) + L" / " + std::to_wstring(max_page + 1) + L" 页";
		function::text(SDK::FVector2D(param::size.X + 75, 152), page_str.c_str());
	}

	for (int i = 0; i < visible_rows; i++)
	{
		entity_box(param::entity_list[start_row + i], SDK::FVector2D(param::size.X + 30, 190 + 50 * i));
	}
}
