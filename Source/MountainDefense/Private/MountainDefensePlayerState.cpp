// Fill out your copyright notice in the Description page of Project Settings.


#include "MountainDefensePlayerState.h"

#include "Net/UnrealNetwork.h"

int AMountainDefensePlayerState::GetLives() const
{
	return Lives;
}

void AMountainDefensePlayerState::SetLives(const int NewLives)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(AMountainDefensePlayerState, Lives, this);
	Lives = NewLives;
}

void AMountainDefensePlayerState::OnRep_Lives()
{
}

void AMountainDefensePlayerState::DecreaseLives()
{
	if (GetLives() > 0)
	{
		SetLives(GetLives()-1);
	}
}

void AMountainDefensePlayerState::IncreaseScore()
{
	SetScore(GetScore()+1);
}

void AMountainDefensePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(AMountainDefensePlayerState, Lives, SharedParams);
}
