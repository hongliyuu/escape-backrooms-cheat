#include "entity.h"

#include "gvalue.h"
#include "kismet.h"
#include <set>

namespace
{
	SDK::APawn* last_pawn = nullptr;
	SDK::APawn* control_entity = nullptr;
	SDK::AAIController* last_ai_controller = nullptr;
	SDK::ACameraActor* tpp_camera = nullptr;
	SDK::UWorld* camera_world = nullptr;
	std::set<SDK::ACharacter*> frozen_entities;

	struct StopLogicParams
	{
		SDK::FString reason;
	};

	bool is_valid_object(SDK::UObject* object)
	{
		if (!object)
			return false;

		__try
		{
			return SDK::UKismetSystemLibrary::IsValid(object);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			return false;
		}
	}

	bool is_player_character(SDK::ACharacter* character)
	{
		return character->IsA(SDK::ABP_Explorer_C::StaticClass()) ||
			character->IsA(SDK::ABPCharacter_Demo_C::StaticClass());
	}

	bool is_player_pawn(SDK::APawn* pawn)
	{
		return is_valid_object(pawn) &&
			(pawn->IsA(SDK::ABP_Explorer_C::StaticClass()) ||
				pawn->IsA(SDK::ABPCharacter_Demo_C::StaticClass()));
	}

	bool is_control_target(SDK::ACharacter* character)
	{
		if (!is_valid_object(character))
			return false;

		__try
		{
			return character->IsA(SDK::ACharacter::StaticClass()) &&
				!is_player_character(character) &&
				!character->IsActorBeingDestroyed() &&
				is_valid_object(character->CharacterMovement);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			return false;
		}
	}

	void clear_control_state()
	{
		last_pawn = nullptr;
		control_entity = nullptr;
		last_ai_controller = nullptr;
		gvalue::x_offset = 0.0f;
		gvalue::y_offset = 0.0f;
	}

	void release_control()
	{
		SDK::APawn* player_pawn = last_pawn;
		SDK::APawn* controlled_pawn = control_entity;
		SDK::AAIController* ai_controller = last_ai_controller;
		SDK::APlayerController* player_controller = gvalue::controller;
		clear_control_state();

		__try
		{
			if (is_valid_object(player_controller) && is_valid_object(player_pawn))
			{
				player_controller->Possess(player_pawn);
				if (player_controller->Pawn == player_pawn)
				{
					player_controller->SetViewTargetWithBlend(
						player_pawn,
						0.3f,
						SDK::EViewTargetBlendFunction::VTBlend_EaseInOut,
						2.0f,
						false
					);
				}
			}

			if (is_valid_object(ai_controller) && is_valid_object(controlled_pawn) &&
				ai_controller->Pawn != controlled_pawn)
			{
				ai_controller->Possess(controlled_pawn);
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
		}
	}

	bool ensure_tpp_camera()
	{
		if (camera_world != gvalue::world)
		{
			tpp_camera = nullptr;
			camera_world = gvalue::world;
		}
		if (is_valid_object(tpp_camera))
			return true;
		if (!is_valid_object(gvalue::world))
			return false;

		SDK::FTransform transform{};
		transform.Rotation = SDK::FQuat(0.0f, 0.0f, 0.0f, 1.0f);
		transform.Scale3D = SDK::FVector(1.0f, 1.0f, 1.0f);
		SDK::AActor* new_camera = SDK::UGameplayStatics::BeginDeferredActorSpawnFromClass(
			gvalue::world,
			SDK::ACameraActor::StaticClass(),
			transform,
			SDK::ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
			nullptr
		);
		if (!is_valid_object(new_camera))
			return false;

		SDK::UGameplayStatics::FinishSpawningActor(new_camera, transform);
		if (!is_valid_object(new_camera) || !new_camera->IsA(SDK::ACameraActor::StaticClass()))
			return false;

		tpp_camera = static_cast<SDK::ACameraActor*>(new_camera);
		return true;
	}
}

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
		release_control();
		printf("control_entity error\n");
	}
}

void entity::kill_all()
{
	if (!is_valid(gvalue::world))
		return;

	SDK::TArray<SDK::AActor*> list;
	SDK::UGameplayStatics::GetAllActorsOfClass(gvalue::world, SDK::ACharacter::StaticClass(), &list);
	for (SDK::AActor* actor : list)
	{
		if (!is_valid(actor))
			continue;
		if (actor->IsA(SDK::ABP_Explorer_C::StaticClass()) || actor->IsA(SDK::ABPCharacter_Demo_C::StaticClass()))
		{
			continue;
		}
		if (actor == control_entity)
			unposs();
		frozen_entities.erase(static_cast<SDK::ACharacter*>(actor));
		actor->K2_DestroyActor();
	}
	frozen_entities.clear();
}

void entity::kill(const std::string& name)
{
	if (!is_valid(gvalue::world))
		return;

	SDK::TArray<SDK::AActor*> list;
	SDK::UGameplayStatics::GetAllActorsOfClass(gvalue::world, SDK::ACharacter::StaticClass(), &list);
	for (SDK::AActor* actor : list)
	{
		if (!is_valid(actor))
			continue;
		if (actor->Class->Name.ToString() == name)
		{
			if (actor == control_entity)
				unposs();
			frozen_entities.erase(static_cast<SDK::ACharacter*>(actor));
			actor->K2_DestroyActor();
		}
	}
}

void entity::destroy(SDK::ACharacter* character)
{
	if (!is_valid(character))
		return;

	if (character == control_entity)
		unposs();
	unfreeze(character);
	character->K2_DestroyActor();
}

void entity::freeze(SDK::ACharacter* character)
{
	if (!is_control_target(character))
		return;

	__try
	{
		character->CharacterMovement->StopMovementImmediately();
		SDK::AController* controller = character->GetController();
		if (is_valid_object(controller) && controller->IsA(SDK::AAIController::StaticClass()))
		{
			SDK::AAIController* ai_controller = static_cast<SDK::AAIController*>(controller);
			if (is_valid_object(ai_controller->BrainComponent))
			{
				SDK::UFunction* func = ai_controller->BrainComponent->Class->GetFunction("BrainComponent", "StopLogic");
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
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}
}

void entity::unfreeze(SDK::ACharacter* character)
{
	if (!is_valid(character))
	{
		frozen_entities.erase(character);
		return;
	}
	__try
	{
		SDK::AController* controller = character->GetController();
		if (is_valid_object(controller) && controller->IsA(SDK::AAIController::StaticClass()))
		{
			SDK::AAIController* ai_controller = static_cast<SDK::AAIController*>(controller);
			if (is_valid_object(ai_controller->BrainComponent))
			{
				SDK::UFunction* func = ai_controller->BrainComponent->Class->GetFunction("BrainComponent", "StartLogic");
				if (func)
				{
					auto flgs = func->FunctionFlags;
					func->FunctionFlags |= 0x400;
					ai_controller->BrainComponent->ProcessEvent(func, nullptr);
					func->FunctionFlags = flgs;
				}
			}
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}
	frozen_entities.erase(character);
}

bool entity::is_frozen(SDK::ACharacter* character)
{
	if (!is_valid(character))
	{
		frozen_entities.erase(character);
		return false;
	}
	return frozen_entities.count(character) > 0;
}

bool entity::has_frozen()
{
	return !frozen_entities.empty();
}

void entity::freeze_all()
{
	if (!is_valid(gvalue::world))
		return;

	frozen_entities.clear();
	SDK::TArray<SDK::AActor*> list;
	SDK::UGameplayStatics::GetAllActorsOfClass(gvalue::world, SDK::ACharacter::StaticClass(), &list);
	for (SDK::AActor* actor : list)
	{
		if (!is_valid(actor))
			continue;
		if (actor->IsA(SDK::ABP_Explorer_C::StaticClass()) || actor->IsA(SDK::ABPCharacter_Demo_C::StaticClass()))
		{
			continue;
		}
		freeze(static_cast<SDK::ACharacter*>(actor));
	}
}

void entity::unfreeze_all()
{
	if (!is_valid(gvalue::world))
		return;

	SDK::TArray<SDK::AActor*> list;
	SDK::UGameplayStatics::GetAllActorsOfClass(gvalue::world, SDK::ACharacter::StaticClass(), &list);
	for (SDK::AActor* actor : list)
	{
		if (!is_valid(actor))
			continue;
		if (actor->IsA(SDK::ABP_Explorer_C::StaticClass()) || actor->IsA(SDK::ABPCharacter_Demo_C::StaticClass()))
		{
			continue;
		}
		unfreeze(static_cast<SDK::ACharacter*>(actor));
	}
	frozen_entities.clear();
}

void entity::spawn(SDK::TSubclassOf<SDK::AActor> cls)
{
	if (!is_valid(gvalue::world) || !is_valid(gvalue::controller) ||
		!is_valid(gvalue::controller->PlayerCameraManager))
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
	SDK::FTransform trans{};
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

bool entity::is_valid(SDK::UObject* object)
{
	return is_valid_object(object);
}

bool entity::can_control(SDK::ACharacter* character)
{
	if (!is_valid_object(gvalue::world) || !is_valid_object(gvalue::controller) ||
		!is_control_target(character))
	{
		return false;
	}

	__try
	{
		SDK::APawn* player_pawn = gvalue::controller->Pawn;
		if (!is_player_pawn(player_pawn))
			return false;

		SDK::AController* controller = character->GetController();
		return is_valid_object(controller) && controller->IsA(SDK::AAIController::StaticClass()) &&
			controller->Pawn == character;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}
}

bool entity::poss(SDK::APawn* pawn)
{
	if (!is_valid_object(pawn) || !pawn->IsA(SDK::ACharacter::StaticClass()))
		return false;

	SDK::ACharacter* character = static_cast<SDK::ACharacter*>(pawn);
	if (!can_control(character))
		return false;

	if (control_entity)
		unposs();
	if (!can_control(character))
		return false;

	__try
	{
		SDK::APlayerController* player_controller = gvalue::controller;
		SDK::APawn* player_pawn = player_controller->Pawn;
		SDK::AAIController* ai_controller = static_cast<SDK::AAIController*>(character->GetController());
		if (!is_valid_object(player_pawn) || !is_valid_object(ai_controller) || ai_controller->Pawn != character)
			return false;

		unfreeze(character);
		last_pawn = player_pawn;
		last_ai_controller = ai_controller;
		control_entity = character;
		player_controller->Possess(character);
		if (player_controller->Pawn != character || !ensure_tpp_camera())
		{
			release_control();
			return false;
		}

		player_controller->SetViewTargetWithBlend(
			tpp_camera,
			0.3f,
			SDK::EViewTargetBlendFunction::VTBlend_EaseInOut,
			2.0f,
			false
		);
		return true;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		release_control();
		return false;
	}
}

void entity::unposs()
{
	release_control();
}

void entity::domain()
{
	if (!is_valid_object(gvalue::world) || !is_valid_object(gvalue::controller))
	{
		clear_control_state();
		return;
	}
	if (!control_entity)
		return;

	if (!is_valid_object(control_entity) || !control_entity->IsA(SDK::ACharacter::StaticClass()) ||
		gvalue::controller->Pawn != control_entity)
	{
		unposs();
		return;
	}

	SDK::ACharacter* character = static_cast<SDK::ACharacter*>(control_entity);
	if (!is_control_target(character) || !is_valid_object(character->CharacterMovement))
	{
		unposs();
		return;
	}

	character->CharacterMovement->MaxWalkSpeed = gvalue::entity_speed * 4000;
	if (character->CharacterMovement->JumpZVelocity <= 0.0f)
	{
		character->CharacterMovement->JumpZVelocity = 500.0f;
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

	if (kismet::is_key_down(VK_SPACE))
	{
		character->Jump();
	}
	else
	{
		character->StopJumping();
	}

	const float sen = 0.2f;

	control_entity->AddControllerPitchInput(gvalue::y_offset * sen);
	control_entity->AddControllerYawInput(gvalue::x_offset * sen);
	gvalue::y_offset = 0.0f;
	gvalue::x_offset = 0.0f;

	if (!ensure_tpp_camera())
	{
		unposs();
		return;
	}

	if (is_valid_object(tpp_camera))
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
