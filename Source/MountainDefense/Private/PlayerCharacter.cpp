// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Kismet/KismetMathLibrary.h"

// #include "Kismet/KismetMathLibrary.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!OrbitCenter.IsNearlyZero())
	{
		FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),OrbitCenter);
		GetController()->SetControlRotation(LookAtRotator);
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("RotateLeftRight", this, &APlayerCharacter::RotateLeftRight);
	PlayerInputComponent->BindAxis("MoveVertically", this, &APlayerCharacter::MoveVertically);
}

void APlayerCharacter::RotateLeftRight(float InputValue)
{
	if (InputValue)
	{
		FVector OrbitCenterToChar = GetActorLocation() - OrbitCenter;	
		FVector NewOrbitCenterToChar = OrbitCenterToChar.RotateAngleAxis(InputValue * OrbitingScale, FVector::UpVector);
		FVector NewActorLocation = OrbitCenter + NewOrbitCenterToChar;
		SetActorLocation(NewActorLocation);
	}
}

void APlayerCharacter::MoveVertically(float InputValue)
{
	if (InputValue)
	{
		FVector NewActorLocation = GetActorLocation();
		NewActorLocation += FVector::UpVector * InputValue * MoveVerticallyScale;
		OrbitCenter += FVector::UpVector * InputValue * MoveVerticallyScale;
		SetActorLocation(NewActorLocation);
	}
}
