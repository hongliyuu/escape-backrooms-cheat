#include "item.h"

#include "gvalue.h"

item* item::get()
{
    static item inst;
    return &inst;
}

void item::spawn(SDK::TSubclassOf<SDK::AActor> actor_class)
{
    if (!gvalue::world || !gvalue::controller)
    {
        return;
    }

    if (!gvalue::controller->Pawn || !gvalue::controller->Pawn->IsA(SDK::ABPCharacter_Demo_C::StaticClass()))
    {
        return;
    }

    SDK::ABPCharacter_Demo_C* my_player = static_cast<SDK::ABPCharacter_Demo_C*>(gvalue::controller->Pawn);

    int32_t slot_index = 0;
    bool found = false;
    my_player->InvFindFreeSlot(&slot_index, &found);

    SDK::FTransform trans;
    trans.Rotation = SDK::FQuat(0.0f, 0.0f, 0.0f, 1.0f);
    trans.Translation = gvalue::controller->PlayerCameraManager->GetCameraLocation();
    trans.Scale3D = SDK::FVector(1.0f, 1.0f, 1.0f);

    SDK::AActor* new_actor = SDK::UGameplayStatics::BeginDeferredActorSpawnFromClass(
        gvalue::world,
        actor_class,
        trans,
        SDK::ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
        nullptr
    );
    SDK::UGameplayStatics::FinishSpawningActor(new_actor, trans);
    SDK::ABP_DroppedItem_C* item = static_cast<SDK::ABP_DroppedItem_C*>(new_actor);

    if (found)
    {
        my_player->InvAdd(item);
    }
    else
    {
        if (item->ItemMesh)
        {
            item->ItemMesh->SetCollisionEnabled(SDK::ECollisionEnabled::QueryAndPhysics);
            item->ItemMesh->SetEnableGravity(true);
            item->ItemMesh->SetSimulatePhysics(true);
        }
    }
}

void item::interact_all(const std::string& name)
{
    SDK::FVector trace_start = gvalue::controller->PlayerCameraManager->GetCameraLocation();
    SDK::FVector trace_end = trace_start + SDK::UKismetMathLibrary::GetForwardVector(gvalue::controller->GetControlRotation()) * 200;
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
    const SDK::FVector location = result.bBlockingHit ? result.Location : result.TraceEnd;

    SDK::TArray<SDK::AActor*> actor_list;
    SDK::UGameplayStatics::GetAllActorsOfClass(gvalue::world, SDK::AInteractableActor::StaticClass(), &actor_list);
    for (SDK::AActor* actor : actor_list)
    {
        if (actor->Class->Name.ToString() != name)
        {
            continue;
        }

        SDK::AInteractableActor* interact = static_cast<SDK::AInteractableActor*>(actor);
        interact->K2_SetActorLocation(location, true, nullptr, true);
    }
}
