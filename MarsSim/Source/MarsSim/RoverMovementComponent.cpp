// Fill out your copyright notice in the Description page of Project Settings.

#include "RoverMovementComponent.h"
#include "Engine/World.h"


// Sets default values for this component's properties
URoverMovementComponent::URoverMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void URoverMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void URoverMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwnerRole() == ROLE_AutonomousProxy || GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
	{
		LastMove = SetMove(DeltaTime);
		SimulateMove(LastMove);
	}
}

FVector URoverMovementComponent::GetAirResistance()
{
	return -DragCoeff * Velocity.SizeSquared() * Velocity.GetSafeNormal();
}

FVector URoverMovementComponent::GetRollingResistance()
{
	float NormalForce = Mass * GetWorld()->GetGravityZ() / 100;
	return RollCoeff * NormalForce * Velocity.GetSafeNormal();
}

void URoverMovementComponent::ApplyRotation(float DeltaTime, float SteeringThrow)
{
	//dot to have no steering flip on reverse
	float DeltaLocation = FVector::DotProduct(GetOwner()->GetActorForwardVector(), Velocity) * DeltaTime;
	float RotationAngle = DeltaLocation / MinTurnRadius * SteeringThrow;
	FQuat RotationDelta(GetOwner()->GetActorUpVector(), RotationAngle);
	GetOwner()->AddActorWorldRotation(RotationDelta);

	Velocity = RotationDelta.RotateVector(Velocity);
}

void URoverMovementComponent::UpdateLocationFromVelocity(float DeltaTime)
{
	FVector Translation = 100 * Velocity * DeltaTime;

	FHitResult Hit;
	GetOwner()->AddActorWorldOffset(Translation, true, &Hit);  //true sweep for collision check
	if (Hit.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;
	}
}

FRoverMoves URoverMovementComponent::SetMove(float DeltaTime)
{
	FRoverMoves Move;
	Move.DeltaTime = DeltaTime;
	Move.SteeringThrow = SteeringThrow;
	Move.Throttle = Throttle;
	Move.Time = GetWorld()->TimeSeconds; // timestamp

	return Move;
}

void URoverMovementComponent::SimulateMove(const FRoverMoves& Move)
{
	FVector Force = MaxForce * GetOwner()->GetActorForwardVector() *Move.Throttle;
	Force += GetAirResistance();
	Force += GetRollingResistance();

	FVector Acceleration = Force / Mass;

	//UE_LOG(LogTemp, Warning, TEXT("gravity %f"),GetWorld()->GetGravityZ());

	Velocity += Acceleration * Move.DeltaTime;

	ApplyRotation(Move.DeltaTime, Move.SteeringThrow);

	UpdateLocationFromVelocity(Move.DeltaTime);
}