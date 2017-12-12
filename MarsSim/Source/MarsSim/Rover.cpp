// Fill out your copyright notice in the Description page of Project Settings.

#include "Rover.h"
#include "Components/InputComponent.h"
#include "UnrealNetwork.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"


FString GetEnumText(ENetRole Role)
{
	switch (Role)
	{
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "SimulatedProxy";
	case ROLE_AutonomousProxy:
		return "Autonomous";
	case ROLE_Authority:
		return "Authority";
	default:
		return "default: error";
	}
}

// Sets default values
ARover::ARover()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	MovementComponent = CreateDefaultSubobject<URoverMovementComponent>(TEXT("Rover Movement Component"));
	Replicator = CreateDefaultSubobject<URoverReplicator>(TEXT("Rover Replicator"));
}

// Called when the game starts or when spawned
void ARover::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		NetUpdateFrequency = 1;
	}
}

// Called every frame
void ARover::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Draw string on screen 
	DrawDebugString(GetWorld(), FVector(0, 200, 200), GetEnumText(Role), this, FColor::Blue, DeltaTime);
}

// Called to bind functionality to input
void ARover::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ARover::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARover::MoveRight);
}

void ARover::MoveForward(float Value)
{
	if (MovementComponent == nullptr) return;

	MovementComponent->SetThrottle(Value);
}

void ARover::MoveRight(float Value)
{
	if (MovementComponent == nullptr) return;

	MovementComponent->SetSteeringThrow(Value);
}