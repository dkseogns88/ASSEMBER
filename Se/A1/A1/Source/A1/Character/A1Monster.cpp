#include "Character/A1Monster.h"
#include "Player/A1PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "../A1/Network/A1NetworkManager.h"

AA1Monster::AA1Monster()
{

}

void AA1Monster::BeginPlay()
{
	Super::BeginPlay();

}

void AA1Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator NPCRot;
	{
		FVector Location = GetActorLocation();
		PlayerInfo->set_x(Location.X);
		PlayerInfo->set_y(Location.Y);
		PlayerInfo->set_z(Location.Z);
		PlayerInfo->set_yaw(GetControlRotation().Yaw);
	}

	const Protocol::MoveState State = DestInfo->state();

	if (State == Protocol::MOVE_STATE_RUN)
	{
		PlayerInfo->set_state(Protocol::MOVE_STATE_RUN);
		UE_LOG(LogTemp, Warning, TEXT("State == MOVE_STATE_RUN"));


		NPCRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), curDest);

		SetActorRotation(FRotator(0, NPCRot.Yaw, 0));
		AddMovementInput(GetActorForwardVector());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("State == NOT_RUN"));
	}

	curDest.X = DestInfo->x();
	curDest.Y = DestInfo->y();
	curDest.Z = DestInfo->z();

	if (FVector::Dist(GetActorLocation(), curDest) < 100.f)
	{
		//npc 전용 이동 패킷
		Protocol::C_NPCMOVE pkt;
		{
			Protocol::PosInfo* Info = pkt.mutable_info();
			Info->CopyFrom(*PlayerInfo);
			Info->set_yaw(NPCRot.Yaw);
			Info->set_state(PlayerInfo->state());
			Info->set_object_id(PlayerInfo->object_id());
		}

		for (auto World : GEngine->GetWorldContexts())
		{
			if (const UGameInstance* GameInstance = World.World()->GetGameInstance())
			{
				if (UA1NetworkManager* NetworkManager = GameInstance->GetSubsystem<UA1NetworkManager>())
				{
					NetworkManager->SendPacket(pkt);
				}
			}
		}
	}
}
