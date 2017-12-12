// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RoverMovementComponent.generated.h"


USTRUCT()
struct FRoverMoves
{
	GENERATED_BODY()

	UPROPERTY()
		float Throttle;

	UPROPERTY()
		float SteeringThrow;

	UPROPERTY()
		float DeltaTime;

	UPROPERTY()
		float Time;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MARSSIM_API URoverMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URoverMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SimulateMove(const FRoverMoves& Move);
	
	FVector GetVelocity() { return Velocity; }

	void SetVelocity(FVector Val) { Velocity = Val; }

	void SetThrottle(float Val) { Throttle = Val; }
	void SetSteeringThrow(float Val) { SteeringThrow = Val; }

	FRoverMoves GetLastMove() { return LastMove; }

private:
	
	FVector Velocity;

	// mass of rover. kgs
	UPROPERTY(EditAnywhere)
		float Mass = 1000;

	UPROPERTY(EditAnywhere)
		float MaxForce = 10000;

	UPROPERTY(EditAnywhere)
		float MaxDegreesPerS = 90;

	UPROPERTY(EditAnywhere)
		float MinTurnRadius = 10;

	UPROPERTY(EditAnywhere)
		float DragCoeff = 15;

	UPROPERTY(EditAnywhere)
		float RollCoeff = 0.1;

	float Throttle;

	float SteeringThrow;

	void UpdateLocationFromVelocity(float DeltaTime);

	void ApplyRotation(float DeltaTime, float SteeringThrow);

	FVector GetAirResistance();
	FVector GetRollingResistance();

	FRoverMoves LastMove;

	FRoverMoves SetMove(float DeltaTime);
};
