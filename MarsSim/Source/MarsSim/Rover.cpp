// Fill out your copyright notice in the Description page of Project Settings.

#include "Rover.h"
#include "Components/InputComponent.h"


// Sets default values
ARover::ARover()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARover::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARover::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Translation = 100 * Velocity * DeltaTime;

	AddActorWorldOffset(Translation);

}

// Called to bind functionality to input
void ARover::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ARover::MoveForward);

}


void ARover::MoveForward(float Value)
{
	Velocity = GetActorForwardVector()* Value * 20;
}
