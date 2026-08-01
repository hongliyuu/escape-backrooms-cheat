#include "entity.h"

#include "gvalue.h"
#include "kismet.h"
#include <set>

SDK::APawn* last_pawn = nullptr;
SDK::APawn* control_entity = nullptr;
SDK::ACameraActor* tpp_camera = nullptr;

// 已冻结实体集合（用指针地址跟踪，避免依赖 SDK 内部状态查询）
static std::set<SDK::ACharacter*> frozen_entities;

// StopLogic 参数布局（与 SDK::Params::BrainComponent_StopLogic 兼容）
struct StopLogicParams
{
	SDK::FString reason;
};

entity* entity::get()
{
	static entity inst;
	return &inst;
}

void entity::init()
{
}

void entity::main()
{
	__try
	{
		domain();
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		control_entity = nullptr;
		printf("control_entity error\n");
	}
}

void entity::kill_all()
{
	SDK::TArray<SDK::AActor*> list;
	SDK::UGameplayStatics::GetAllActorsOfClass(gvalue::world, SDK::ACharacter::StaticClass(), &list);
	for (SDK::AActor* actor : list)
	{
		if (actor->IsA(SDK::ABP_Explorer_C::StaticClass()) || actor->IsA(SDK::ABPCharacter_Demo_C::StaticClass()))
		{
			continue;
		}
		actor->K2_DestroyActor();
	}
	frozen_entities.clear();
}

void entity::kill(const std::string& name)
{
	SDK::TArray<SDK::AActor*> list;
	SDK::UGameplayStatics::GetAllActorsOfClass(gvalue::world, SDK::ACharacter::StaticClass(), &list);
	for (SDK::AActor* actor : list)
	{
		if (actor->Class->Name.ToString() == name)
		{
			actor->K2_DestroyActor();
		}
	}
	frozen_entities.clear();
}

void entity::freeze(SDK::ACharacter* character)
{
	if (!character)
		return;
	if (character->CharacterMovement)
	{
		character->CharacterMovement->StopMovementImmediately();
	}
	SDK::AController* controller = character->GetController();
	if (controller && controller->IsA(SDK::AAIController::StaticClass()))
	{
		SDK::AAIController* ai_controller = static_cast<SDK::AAIController*>(controller);
		if (ai_controller->BrainComponent)
		{
			static SDK::UFunction* func = nullptr;
			if (!func)
				func = ai_controller->BrainComponent->Class->GetFunction("BrainComponent", "StopLogic");
			if (func)
			{
				StopLogicParams params;
				params.reason = SDK::FString(L"frozen");
				auto flgs = func->FunctionFlags;
				func->FunctionFlags |= 0x400;
				ai_controller->BrainComponent->ProcessEvent(func, &params);
				func->FunctionFlags = flgs;
			}
		}
	}
	frozen_entities.insert(character);
}

void entity::unfreeze(SDK::ACharacter* character)
{
	if (!character)
		return;
	SDK::AController* controller = character->GetController();
	if (controller && controller->IsA(SDK::AAIController::StaticClass()))
	{
		SDK::AAIController* ai_controller = static_cast<SDK::AAIController*>(controller);
		if (ai_controller->BrainComponent)
		{
			static SDK::UFunction* func = nullptr;
			if (!func)
				func = ai_controller->BrainComponent->Class->GetFunction("BrainComponent", "StartLogic");
			if (func)
			{
				auto flgs = func->FunctionFlags;
				func->FunctionFlags |= 0x400;
				ai_controller->BrainComponent->ProcessEvent(func, nullptr);
				func->FunctionFlags = flgs;
			}
		}
	}
	frozen_entities.erase(character);
}

bool entity::is_frozen(SDK::ACharacter* character)
{
	if (!character)
		return false;
	return frozen_entities.count(character) > 0;
}

void entity::freeze_all()
{
	SDK::TArray<SDK::AActor*> list;
	SDK::UGameplayStatics::GetAllActorsOfClass(gvalue::world, SDK::ACharacter::StaticClass(), &list);
	for (SDK::AActor* actor : list)
	{
		if (actor->IsA(SDK::ABP_Explorer_C::StaticClass()) || actor->IsA(SDK::ABPCharacter_Demo_C::StaticClass()))
		{
			continue;
		}
		freeze(static_cast<SDK::ACharacter*>(actor));
	}
}

void entity::unfreeze_all()
{
	SDK::TArray<SDK::AActor*> list;
	SDK::UGameplayStatics::GetAllActorsOfClass(gvalue::world, SDK::ACharacter::StaticClass(), &list);
	for (SDK::AActor* actor : list)
	{
		if (actor->IsA(SDK::ABP_Explorer_C::StaticClass()) || actor->IsA(SDK::ABPCharacter_Demo_C::StaticClass()))
		{
			continue;
		}
		unfreeze(static_cast<SDK::ACharacter*>(actor));
	}
}

void entity::spawn(SDK::TSubclassOf<SDK::AActor> cls)
{
	if (!gvalue::world || !gvalue::controller)
	{
		return;
	}

	SDK::FVector trace_start = gvalue::controller->PlayerCameraManager->GetCameraLocation();
	SDK::FVector trace_end = trace_start + SDK::UKismetMathLibrary::GetForwardVector(gvalue::controller->GetControlRotation()) * 2000;
	SDK::TArray<SDK::AActor*> ignore_actors;
	ignore_actors.Add(gvalue::controller->Pawn);
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

	SDK::FVector location = result.bBlockingHit ? result.Location : result.TraceEnd;
	SDK::FTransform trans;
	trans.Rotation = SDK::FQuat(0.0f, 0.0f, 0.0f, 1.0f);
	trans.Translation = location;
	trans.Scale3D = SDK::FVector(1.0f, 1.0f, 1.0f);

	SDK::AActor* new_actor = SDK::UGameplayStatics::BeginDeferredActorSpawnFromClass(
		gvalue::world,
		cls,
		trans,
		SDK::ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
		nullptr
	);
	if (new_actor)
	{
		SDK::UGameplayStatics::FinishSpawningActor(new_actor, trans);

		if (new_actor->IsA(SDK::APawn::StaticClass()))
		{
			SDK::APawn* pawn = static_cast<SDK::APawn*>(new_actor);
			pawn->SpawnDefaultController();
		}
	}
}

void entity::poss(SDK::APawn* pawn)
{
	if (!gvalue::world || !gvalue::controller)
	{
		return;
	}
	if (gvalue::controller->Pawn->IsA(SDK::ABPCharacter_Demo_C::StaticClass()))
	{
		last_pawn = gvalue::controller->Pawn;
	}
	gvalue::controller->Possess(pawn);
	control_entity = pawn;

	// 切换第三人称视角
	if (!tpp_camera)
	{
		SDK::FTransform trans;
		SDK::AActor* new_camera = SDK::UGameplayStatics::BeginDeferredActorSpawnFromClass(
			gvalue::world,
			SDK::ACameraActor::StaticClass(),
			trans,
			SDK::ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
			nullptr
		);
		SDK::UGameplayStatics::FinishSpawningActor(new_camera, trans);
		tpp_camera = static_cast<SDK::ACameraActor*>(new_camera);
	}
	gvalue::controller->SetViewTargetWithBlend(
		tpp_camera,
		0.3f,
		SDK::EViewTargetBlendFunction::VTBlend_EaseInOut,
		2.0f,
		false
	);
}

void entity::unposs()
{
	if (!last_pawn)
	{
		return;
	}
	gvalue::controller->Possess(last_pawn);
	control_entity = nullptr;
	last_pawn = nullptr;

	// 恢复第一人称视角
	gvalue::controller->SetViewTargetWithBlend(
		gvalue::controller->Pawn,
		0.3f,
		SDK::EViewTargetBlendFunction::VTBlend_EaseInOut,
		2.0f,
		false
	);
}

void entity::domain()
{
	if (!gvalue::world || !gvalue::controller|| !control_entity)
	{
		control_entity = nullptr;
		return;
	}

	if (control_entity->IsA(SDK::ACharacter::StaticClass()))
	{
		SDK::ACharacter* character = static_cast<SDK::ACharacter*>(control_entity);
		character->CharacterMovement->MaxWalkSpeed = gvalue::entity_speed * 4000;
		if (character->CharacterMovement->JumpZVelocity <= 0.0f)
		{
			character->CharacterMovement->JumpZVelocity = 500.0f;
		}
	}

	const SDK::FRotator rot = SDK::FRotator(0.0f, control_entity->GetControlRotation().Yaw, 0.0f);
	SDK::FVector fwd = SDK::UKismetMathLibrary::GetForwardVector(rot);
	SDK::FVector right = SDK::UKismetMathLibrary::GetRightVector(rot);

	if (kismet::is_key_down('W'))
	{
		control_entity->AddMovementInput(fwd, 1.0f, true);
	}
	if (kismet::is_key_down('S'))
	{
		control_entity->AddMovementInput(fwd, -1.0f, true);
	}
	if (kismet::is_key_down('A'))
	{
		control_entity->AddMovementInput(right, -1.0f, true);
	}
	if (kismet::is_key_down('D'))
	{
		control_entity->AddMovementInput(right, 1.0f, true);
	}

	// 跳跃
	if (control_entity->IsA(SDK::ACharacter::StaticClass()))
	{
		SDK::ACharacter* character = static_cast<SDK::ACharacter*>(control_entity);
		if (kismet::is_key_down(VK_SPACE))
		{
			character->Jump();
		}
		else
		{
			character->StopJumping();
		}
	}

	const float sen = 0.2f;

	control_entity->AddControllerPitchInput(gvalue::y_offset * sen);
	control_entity->AddControllerYawInput(gvalue::x_offset * sen);
	gvalue::y_offset = 0.0f;
	gvalue::x_offset = 0.0f;

	// 第三人称摄像机跟随
	if (!tpp_camera)
	{
		SDK::FTransform trans;
		SDK::AActor* new_camera = SDK::UGameplayStatics::BeginDeferredActorSpawnFromClass(
			gvalue::world,
			SDK::ACameraActor::StaticClass(),
			trans,
			SDK::ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
			nullptr
		);
		SDK::UGameplayStatics::FinishSpawningActor(new_camera, trans);
		tpp_camera = static_cast<SDK::ACameraActor*>(new_camera);
	}

	if (tpp_camera)
	{
		SDK::FVector cam_start = control_entity->K2_GetActorLocation();
		cam_start.Z += 80.0f;
		SDK::FVector cam_end = cam_start + SDK::UKismetMathLibrary::GetForwardVector(control_entity->GetControlRotation()) * -400.0f;

		SDK::TArray<SDK::AActor*> ignore_actors;
		ignore_actors.Add(control_entity);
		SDK::FHitResult result;
		SDK::UKismetSystemLibrary::LineTraceSingle(
			gvalue::world,
			cam_start,
			cam_end,
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

		tpp_camera->K2_SetActorLocation(result.bBlockingHit ? result.Location : result.TraceEnd, false, nullptr, true);
		tpp_camera->K2_SetActorRotation(control_entity->GetControlRotation(), false);
	}
}
