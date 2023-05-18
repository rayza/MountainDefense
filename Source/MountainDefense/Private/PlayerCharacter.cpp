// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(LogPlayerCharacter);

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
	
	FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),OrbitCenter);
	SetActorRotation(LookAtRotator);

	if (!IsValid(Mountain))
	{
		UE_LOG(LogPlayerCharacter, Error, TEXT("Mountain reference not set in PlayerCharacter."));
		return;
	}

	FVector NewActorLocation = GetActorLocation();
	FVector MountainOrigin;
	FVector MountainBoxExtent;
	Mountain->GetActorBounds(true, MountainOrigin, MountainBoxExtent, true);

	// Move character vertically
	NewActorLocation += FVector::UpVector * MoveVerticallyAmount * DeltaTime;
	OrbitCenter += FVector::UpVector * MoveVerticallyAmount * DeltaTime;
	FVector CharacterOrigin;
	FVector CharacterBoxExtent;
	GetActorBounds(true, CharacterOrigin, CharacterBoxExtent, true);
	float SafeMountainTop = (MountainBoxExtent.Z + MountainOrigin.Z) - CharacterBoxExtent.Z;
	float SafeMountainBottom = CharacterBoxExtent.Z;
	NewActorLocation.Z = FMath::Clamp(NewActorLocation.Z, SafeMountainBottom, SafeMountainTop);
	OrbitCenter.Z = FMath::Clamp(OrbitCenter.Z, SafeMountainBottom, SafeMountainTop);
	
	// Rotate the character around orbit center
	FVector OrbitCenterToChar = NewActorLocation - OrbitCenter;
	FVector NewOrbitCenterToChar = OrbitCenterToChar.RotateAngleAxis(RotateLeftRightAmount * DeltaTime, FVector::UpVector);
	NewActorLocation = OrbitCenter + NewOrbitCenterToChar;
	
	// Character might be inside the mountain now -> Calculate a safe vector to line trace towards mountain surface
	float MaxDistanceFromCenter = FMath::Max(MountainBoxExtent.X, MountainBoxExtent.Y) + DistanceToSurface;
	FVector CenterToCharMaxDistance = NewOrbitCenterToChar.GetSafeNormal() * MaxDistanceFromCenter;
	
	// Snap character to desired distance from surface
	FHitResult MountainSurfaceHit;
	FCollisionQueryParams CollisionQueryParams;
	Mountain->ActorLineTraceSingle(MountainSurfaceHit, OrbitCenter + CenterToCharMaxDistance, OrbitCenter, ECC_WorldStatic, CollisionQueryParams);
	FVector OrbitCenterToSurface = MountainSurfaceHit.Location - OrbitCenter;
	float RequiredDistanceFromCenter = OrbitCenterToSurface.Size() + DistanceToSurface;
	NewOrbitCenterToChar = OrbitCenterToSurface.GetSafeNormal() * RequiredDistanceFromCenter;
	NewActorLocation = OrbitCenter + NewOrbitCenterToChar;
	
	// Apply movement changes
	SetActorLocation(NewActorLocation);
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
	RotateLeftRightAmount = InputValue * OrbitingScale;
}

void APlayerCharacter::MoveVertically(float InputValue)
{
	MoveVerticallyAmount = InputValue * MoveVerticallyScale;
}
