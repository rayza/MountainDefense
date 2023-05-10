// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class MOUNTAINDEFENSE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void RotateLeftRight(float InputValue);
	void MoveVertically(float InputValue);

	UPROPERTY(BlueprintReadWrite)
	FVector OrbitCenter;

	UPROPERTY(BlueprintReadWrite)
	float OrbitingScale = 0.5;

	UPROPERTY(BlueprintReadWrite)
	float MoveVerticallyScale = 15;
};
