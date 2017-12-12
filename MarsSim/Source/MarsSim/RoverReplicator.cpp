// Fill out your copyright notice in the Description page of Project Settings.

#include "RoverReplicator.h"
#include "UnrealNetwork.h"


// Sets default values for this component's properties
URoverReplicator::URoverReplicator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	//replicate as default
	SetIsReplicated(true);

	// ...
}

// Called when the game starts
void URoverReplicator::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent = GetOwner()->FindComponentByClass<URoverMovementComponent>();
	
}

// Called every frame
void URoverReplicator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MovementComponent == nullptr) return;

	FRoverMoves  LastMove = MovementComponent->GetLastMove();

	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		UnacknowledgedMoves.Add(LastMove);
		Server_SendMove(LastMove);
	}
	// server and in control of pawn. (non-player)
	if (GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
	{
		UpdateServerState(LastMove);
	}
	if (GetOwnerRole() == ROLE_SimulatedProxy)
	{
		MovementComponent->SimulateMove(ServerState.LastMove);
	}
}

void URoverReplicator::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const //DOREPLIFETIME macro uses outlifetime
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(URoverReplicator, ServerState);
}

void URoverReplicator::OnRep_ServerState()
{
	if (MovementComponent == nullptr) return;
	GetOwner()->SetActorTransform(ServerState.Transform);
	MovementComponent->SetVelocity(ServerState.Velocity);

	ClearOldMoves(ServerState.LastMove);

	for (const FRoverMoves& Move : UnacknowledgedMoves)
	{
		MovementComponent->SimulateMove(Move);
	}
}

bool URoverReplicator::Server_SendMove_Validate(FRoverMoves Move)
{
	return true; ///better make
}
void URoverReplicator::Server_SendMove_Implementation(FRoverMoves Move)
{
	if (MovementComponent == nullptr) return;
	MovementComponent->SimulateMove(Move);
	UpdateServerState(Move);
}



void URoverReplicator::ClearOldMoves(FRoverMoves LastMove)
{
	TArray<FRoverMoves> NewMoves;

	for (const FRoverMoves& Move : UnacknowledgedMoves)
	{
		if (Move.Time > LastMove.Time)
		{
			NewMoves.Add(Move);
		}
	}
	UnacknowledgedMoves = NewMoves;
}

void URoverReplicator::UpdateServerState(const FRoverMoves& Move)
{
	ServerState.Transform = GetOwner()->GetActorTransform();
	ServerState.Velocity = MovementComponent->GetVelocity();
	ServerState.LastMove = Move;
}
