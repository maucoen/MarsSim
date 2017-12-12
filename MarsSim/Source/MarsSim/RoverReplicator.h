// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RoverMovementComponent.h"
#include "RoverReplicator.generated.h"

USTRUCT()
struct FRoverState
{
	GENERATED_BODY()

	UPROPERTY()
		FRoverMoves LastMove;

	UPROPERTY()
		FVector Velocity;

	UPROPERTY()
		FTransform Transform;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MARSSIM_API URoverReplicator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URoverReplicator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void ClearOldMoves(FRoverMoves LastMove);

	UFUNCTION()
		void OnRep_ServerState();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SendMove(FRoverMoves Move);

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
		FRoverState ServerState;

	TArray<FRoverMoves> UnacknowledgedMoves;
	
	UPROPERTY()
		URoverMovementComponent* MovementComponent;

	void UpdateServerState(const FRoverMoves& Move);
};
