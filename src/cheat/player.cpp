#include "player.h"

#include "gvalue.h"
#include "_sdk.h"
#include "render.h"
#include "kismet.h"

SDK::ABPCharacter_Demo_C* my_player = nullptr;
SDK::AMP_PS_C* my_state = nullptr;

float def_sensitivity = -1;
SDK::FVector fly_location = SDK::FVector(-114514, -114514, -114514);

SDK::AActor* take_actor = nullptr;
float take_distance = 0.0f;

namespace
{
	bool is_valid_object(SDK::UObject* object)
	{
		return object && SDK::UKismetSystemLibrary::IsValid(object);
	}
}

player* player::get()
{
	static player inst;
	return &inst;
}

void player::init()
{
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02;
	rid.dwFlags = RIDEV_INPUTSINK;
	rid.hwndTarget = kismet::get_window();
	RegisterRawInputDevices(&rid, 1, sizeof(rid));
}

void player::main()
{
	if (!gvalue::world || !gvalue::controller)
	{
		return;
	}
	get_player();
}

void player::get_player()
{
	if (my_player != gvalue::controller->Pawn)
	{
		fly_location = SDK::FVector(-114514, -114514, -114514);
	}
	if (!gvalue::controller->Pawn || !gvalue::controller->Pawn->IsA(SDK::ABPCharacter_Demo_C::StaticClass()))
	{
		my_player = nullptr;
		def_sensitivity = -1;
		fly_location = SDK::FVector(-114514, -114514, -114514);
		return;
	}
	if (!gvalue::controller->PlayerState || !gvalue::controller->PlayerState->IsA(SDK::AMP_PS_C::StaticClass()))
	{
		my_state = nullptr;
		def_sensitivity = -1;
		fly_location = SDK::FVector(-114514, -114514, -114514);
		return;
	}
	my_player = static_cast<SDK::ABPCharacter_Demo_C*>(gvalue::controller->Pawn);
	my_state = static_cast<SDK::AMP_PS_C*>(gvalue::controller->PlayerState);
	if (def_sensitivity == -1)
	{
		def_sensitivity = my_player->MouseSensitivity;
	}
	domain();
	domain_team();
}

void player::domain()
{
	//无限耐力
	if (gvalue::inf_energy)
	{
		my_player->Stamina = 45.0f;
	}

	//自动平衡
	if (gvalue::auto_balance)
	{
		if (is_valid_object(my_player->BalanceTimeline))
		{
			my_player->BalanceTimeline->TheTimeline.Position = 0.0f;
		}
	}

	//无限san值
	if (gvalue::inf_san)
	{
		my_state->Sanity = my_state->MaxSanity;
	}

	//无限跳跃
	if (gvalue::inf_jump)
	{
		my_player->JumpCurrentCount = 0;
		my_player->JumpMaxCount = 2;
	}
	else
	{
		my_player->JumpMaxCount = 1;
	}

	//跑步速度
	if (my_player->SprintSpeed != gvalue::run_speed * 5500)
	{
		my_player->SprintSpeed = gvalue::run_speed * 5500;
		my_player->SetSprintSpeedServer(gvalue::run_speed * 5500);
	}
	//走路速度
	if (my_player->WalkSpeed != gvalue::walk_speed * 2750)
	{
		my_player->WalkSpeed = gvalue::walk_speed * 2750;
		my_player->SetWalkSpeedServer(gvalue::walk_speed * 2750);
	}

	//通用加速
	const float time_scale = gvalue::global_speed > 0.0f ? gvalue::global_speed * 10.0f : 0.01f;
	my_player->CustomTimeDilation = time_scale;
	my_player->MouseSensitivity = def_sensitivity / time_scale;

	//跳跃速度
	if (is_valid_object(my_player->CharacterMovement))
	{
		my_player->CharacterMovement->JumpZVelocity = gvalue::jump_speed * 4000;
		my_player->CharacterMovement->AirControl = gvalue::air_control * 5;
	}

	//灵魂出窍
	my_player->SetReplicateMovement(!gvalue::ghost_mode);

	if (fly_location == SDK::FVector(-114514, -114514, -114514))
	{
		fly_location = my_player->K2_GetActorLocation();
	}
	//飞行模式
	if (gvalue::fly_mode)
	{
		const SDK::FRotator con_rot = my_player->GetControlRotation();
		const SDK::FRotator act_rot = my_player->K2_GetActorRotation();

		if (kismet::is_key_down('W'))
		{
			fly_location += SDK::UKismetMathLibrary::GetForwardVector(con_rot) * gvalue::fly_speed * 100;
		}
		if (kismet::is_key_down('S'))
		{
			fly_location += SDK::UKismetMathLibrary::GetForwardVector(con_rot) * -1 * gvalue::fly_speed * 100;
		}
		if (kismet::is_key_down('A'))
		{
			fly_location += SDK::UKismetMathLibrary::GetRightVector(con_rot) * -1 * gvalue::fly_speed * 100;
		}
		if (kismet::is_key_down('D'))
		{
			fly_location += SDK::UKismetMathLibrary::GetRightVector(con_rot) * gvalue::fly_speed * 100;
		}
		if (kismet::is_key_down(VK_SPACE))
		{
			fly_location += SDK::UKismetMathLibrary::GetUpVector(act_rot) * gvalue::fly_speed * 100;
		}
		if (kismet::is_key_down(VK_CONTROL))
		{
			fly_location += SDK::UKismetMathLibrary::GetUpVector(act_rot) * -1 * gvalue::fly_speed * 100;
		}
		my_player->K2_SetActorLocation(fly_location, false, nullptr, true);
	}
	else
	{
		fly_location = my_player->K2_GetActorLocation();
	}

	//X键删除
	if (gvalue::x_delete && is_valid_object(gvalue::controller->PlayerCameraManager))
	{
		SDK::FVector trace_start = gvalue::controller->PlayerCameraManager->GetCameraLocation();
		SDK::FVector trace_end = trace_start + SDK::UKismetMathLibrary::GetForwardVector(my_player->GetControlRotation()) * 10000;
		SDK::TArray<SDK::AActor*> ignore_actors;
		ignore_actors.Add(my_player);
		SDK::FHitResult result;
		SDK::UKismetSystemLibrary::LineTraceSingle(
			gvalue::world,
			trace_start,
			trace_end,
			SDK::ETraceTypeQuery::TraceTypeQuery1,
			false,
			ignore_actors,
			SDK::EDrawDebugTrace::None,
			&result,
			true,
			SDK::FLinearColor(1.0f, 0.0f, 0.0f, 1.0f),
			SDK::FLinearColor(0.0f, 1.0f, 0.0f, 1.0f),
			0.0f
		);

		SDK::AActor* del_actor = result.bBlockingHit ? result.Actor.Get() : nullptr;
		if (is_valid_object(del_actor))
		{
			draw_extent(del_actor, SDK::FLinearColor(1.0f, 0.2f, 0.0f, 1.0f), L"[X键] 删除 " + SDK::UKismetStringLibrary::Conv_NameToString(del_actor->Name).ToWString());
		}

		static bool x_down = false;
		if (kismet::is_key_down('X'))
		{
			if (!x_down)
			{
				x_down = true;
				if (del_actor)
				{
					del_actor->K2_DestroyActor();
				}
			}
		}
		else
		{
			x_down = false;
		}
	}

	//无敌模式
	if (gvalue::inf_health)
	{
		my_player->CanKill = false;
		//my_player->CapsuleComponent->SetCollisionResponseToChannel(SDK::ECollisionChannel::ECC_WorldDynamic, SDK::ECollisionResponse::ECR_Ignore);
	}
	else
	{
		my_player->CanKill = true;
		//my_player->CapsuleComponent->SetCollisionResponseToChannel(SDK::ECollisionChannel::ECC_WorldDynamic, SDK::ECollisionResponse::ECR_Block);
	}
	
	//旋转
	if (is_valid_object(my_player->Mesh) && gvalue::spin)
	{
		SDK::FRotator rot = my_player->Mesh->RelativeRotation + SDK::FRotator(0.0f, (gvalue::spin_speed * 10000 * gvalue::delta_time), 0.0f);
		my_player->Mesh->K2_SetRelativeRotation(rot, true, nullptr, true);
	}
	else if (is_valid_object(my_player->Mesh))
	{
		my_player->Mesh->K2_SetRelativeRotation(SDK::FRotator(0, -90, 0), true, nullptr, true);
	}

	//T Pose
	if (is_valid_object(my_player->Mesh) && gvalue::t_pos)
	{
		my_player->Mesh->SetAnimClass(nullptr);
	}
	else if (is_valid_object(my_player->Mesh))
	{
		my_player->Mesh->SetAnimClass(SDK::UPlayer_AnimBP_C::StaticClass());
	}

	//右键移动
	if (gvalue::rb_move && is_valid_object(gvalue::controller->PlayerCameraManager))
	{
		SDK::FVector trace_start = gvalue::controller->PlayerCameraManager->GetCameraLocation();
		SDK::FVector trace_end = trace_start + SDK::UKismetMathLibrary::GetForwardVector(my_player->GetControlRotation()) * 10000;
		SDK::TArray<SDK::AActor*> ignore_actors;
		ignore_actors.Add(my_player);
		SDK::FHitResult result;
		SDK::UKismetSystemLibrary::LineTraceSingle(
			gvalue::world,
			trace_start,
			trace_end,
			SDK::ETraceTypeQuery::TraceTypeQuery1,
			false,
			ignore_actors,
			SDK::EDrawDebugTrace::None,
			&result,
			true,
			SDK::FLinearColor(1.0f, 0.0f, 0.0f, 1.0f),
			SDK::FLinearColor(0.0f, 1.0f, 0.0f, 1.0f),
			0.0f
		);
		if (take_actor && !is_valid_object(take_actor))
		{
			take_actor = nullptr;
			take_distance = 0.0f;
		}

		SDK::AActor* hovered_actor = result.bBlockingHit ? result.Actor.Get() : nullptr;
		std::wstring move_label = L"[右键] 移动";
		if (is_valid_object(hovered_actor))
		{
			move_label += L" " + SDK::UKismetStringLibrary::Conv_NameToString(hovered_actor->Name).ToWString();
		}
		draw_extent(take_actor ? take_actor : hovered_actor,
			SDK::FLinearColor(0.0f, 1.0f, 0.2f, 1.0f),
			move_label
		);

		static bool rb_down = false;
		if (kismet::is_key_down(VK_RBUTTON))
		{
			if (!rb_down)
			{
				rb_down = true;
				if (is_valid_object(hovered_actor))
				{
					take_actor = hovered_actor;
					take_distance = result.Distance;
				}
			}
		}
		else
		{
			rb_down = false;
			take_actor = nullptr;
			take_distance = 0.0f;
		}

		if (take_actor)
		{

			SDK::FVector trace_start = gvalue::controller->PlayerCameraManager->GetCameraLocation();
			SDK::FVector trace_end = trace_start + SDK::UKismetMathLibrary::GetForwardVector(my_player->GetControlRotation()) * take_distance;
			SDK::TArray<SDK::AActor*> ignore_actors;
			ignore_actors.Add(my_player);
			ignore_actors.Add(take_actor);
			SDK::FHitResult result;
			SDK::UKismetSystemLibrary::LineTraceSingle(
				gvalue::world,
				trace_start,
				trace_end,
				SDK::ETraceTypeQuery::TraceTypeQuery1,
				false,
				ignore_actors,
				SDK::EDrawDebugTrace::None,
				&result,
				true,
				SDK::FLinearColor(1.0f, 0.0f, 0.0f, 1.0f),
				SDK::FLinearColor(0.0f, 1.0f, 0.0f, 1.0f),
				0.0f
			);

			if (is_valid_object(take_actor))
			{
				take_actor->K2_SetActorLocation(result.TraceEnd, true, nullptr, true);
			}
		}
	}
}

void player::domain_team()
{
	// 检查是否有任何全队开关开启
	if (!gvalue::inf_energy_team && !gvalue::inf_san_team &&
		!gvalue::inf_health_team && !gvalue::inf_jump_team &&
		!gvalue::speed_team)
		return;

	if (!gvalue::world || !my_player)
		return;

	// 获取所有玩家
	SDK::TArray<SDK::AActor*> actor_list;
	SDK::UGameplayStatics::GetAllActorsOfClass(gvalue::world, SDK::ABPCharacter_Demo_C::StaticClass(), &actor_list);

	for (SDK::AActor* actor : actor_list)
	{
		if (!actor || actor == my_player)
			continue;

		SDK::ABPCharacter_Demo_C* pawn = static_cast<SDK::ABPCharacter_Demo_C*>(actor);
		if (!pawn->IsA(SDK::ABPCharacter_Demo_C::StaticClass()))
			continue;

		// 无限耐力
		if (gvalue::inf_energy_team)
		{
			pawn->Stamina = 45.0f;
		}

		// 无限跳跃
		if (gvalue::inf_jump_team)
		{
			pawn->JumpCurrentCount = 0;
			pawn->JumpMaxCount = 2;
		}

		// 无敌
		if (gvalue::inf_health_team)
		{
			pawn->CanKill = false;
		}

		// 速度总开关
		if (gvalue::speed_team)
		{
			// 跑步速度
			if (pawn->SprintSpeed != gvalue::run_speed * 5500)
			{
				pawn->SprintSpeed = gvalue::run_speed * 5500;
				pawn->SetSprintSpeedServer(gvalue::run_speed * 5500);
			}
			// 走路速度
			if (pawn->WalkSpeed != gvalue::walk_speed * 2750)
			{
				pawn->WalkSpeed = gvalue::walk_speed * 2750;
				pawn->SetWalkSpeedServer(gvalue::walk_speed * 2750);
			}
			// 通用加速
			pawn->CustomTimeDilation = gvalue::global_speed * 10;
			// 跳跃速度
			if (is_valid_object(pawn->CharacterMovement))
			{
				pawn->CharacterMovement->JumpZVelocity = gvalue::jump_speed * 4000;
			}
		}

		// 无限san值
		if (gvalue::inf_san_team)
		{
			if (pawn->PlayerState && pawn->PlayerState->IsA(SDK::AMP_PS_C::StaticClass()))
			{
				SDK::AMP_PS_C* ps = static_cast<SDK::AMP_PS_C*>(pawn->PlayerState);
				ps->Sanity = ps->MaxSanity;
			}
		}
	}
}

void player::draw_extent(SDK::AActor* actor, const SDK::FLinearColor& color, const std::wstring& name)
{
	if (!actor)
	{
		return;
	}

	struct line_01
	{
		SDK::FVector2D p1;
		SDK::FVector2D p2;
	};

	SDK::FVector origin, extent;
	actor->GetActorBounds(true, &origin, &extent, true);
	const SDK::FVector point[] =
	{
		origin + SDK::FVector(extent.X, extent.Y, extent.Z),        //0
		origin + SDK::FVector(-extent.X, -extent.Y, -extent.Z),     //1
		origin + SDK::FVector(extent.X, -extent.Y, extent.Z),       //2
		origin + SDK::FVector(extent.X, extent.Y, -extent.Z),       //3
		origin + SDK::FVector(extent.X, -extent.Y, -extent.Z),      //4
		origin + SDK::FVector(-extent.X, extent.Y, extent.Z),       //5
		origin + SDK::FVector(-extent.X, extent.Y, -extent.Z),      //6
		origin + SDK::FVector(-extent.X, -extent.Y, extent.Z)       //7
	};

	SDK::FVector2D point_2d[8];
	for (int i = 0; i < 8; i++)
	{
		SDK::FVector2D ori_2d;
		bool in_view = gvalue::controller->ProjectWorldLocationToScreen(point[i], &ori_2d, true);
		if (!in_view)
		{
			ori_2d = SDK::FVector2D(-114514, -114514);
		}
		point_2d[i] = ori_2d;
	}

	const line_01 line[12] =
	{
		{point_2d[0],point_2d[2]},
		{point_2d[0],point_2d[3]},
		{point_2d[0],point_2d[5]},

		{point_2d[1],point_2d[4]},
		{point_2d[1],point_2d[6]},
		{point_2d[1],point_2d[7]},

		{point_2d[2],point_2d[4]},
		{point_2d[2],point_2d[7]},

		{point_2d[3],point_2d[4]},
		{point_2d[3],point_2d[6]},

		{point_2d[5],point_2d[6]},
		{point_2d[5],point_2d[7]}
	};

	for (int i = 0; i < 12; i++)
	{
		if (line[i].p1 == SDK::FVector2D(-114514, -114514) ||
			line[i].p2 == SDK::FVector2D(-114514, -114514))
		{
			continue;
		}
		render::draw_line(
			line[i].p1,
			line[i].p2,
			1.0f,
			color
		);
	}

	SDK::FVector2D ori_2d;
	bool in_view = gvalue::controller->ProjectWorldLocationToScreen(origin, &ori_2d, true);
	if (in_view)
	{
		const std::wstring str = name;
		render::draw_text(
			gvalue::engine->MediumFont,
			str.c_str(),
			ori_2d,
			SDK::FVector2D(1.0f, 1.0f),
			color,
			1.0f,
			SDK::FLinearColor(0.0f, 0.0f, 0.0f, 0.0f),
			SDK::FVector2D(0.0f, 0.0f),
			false,
			false,
			false,
			SDK::FLinearColor(0.0f, 0.0f, 0.0f, 0.0f)
		);
	}
}
