// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MountainDefensePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MOUNTAINDEFENSE_API AMountainDefensePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(ReplicatedUsing=OnRep_Lives, Category=PlayerState, BlueprintGetter=GetLives, BlueprintSetter=SetLives)
	int Lives = 10;

	UFUNCTION(BlueprintGetter)
	int GetLives() const;

	UFUNCTION(BlueprintSetter)
	void SetLives(const int NewLives);
	
	UFUNCTION()
	virtual void OnRep_Lives();

	UFUNCTION(BlueprintCallable)
	void DecreaseLives();
	
	UFUNCTION(BlueprintCallable)
	void IncreaseScore();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
};
