// Fill out your copyright notice in the Description page of Project Settings.

#include "BPlayerState.h"
#include "Net/UnrealNetwork.h"

ABPlayerState::ABPlayerState()
{
	TeamNumber = 0;
	Goals = 0;
}

void ABPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABPlayerState, TeamNumber, COND_None);
	DOREPLIFETIME_CONDITION(ABPlayerState, Goals, COND_None);
}

void ABPlayerState::SetTeamNumber(int32 NewTeamNumber)
{
	TeamNumber = NewTeamNumber;
}

void ABPlayerState::ScoreGoal(ABPlayerState* ScoredBy, int32 Points)
{
	Goals++;
}

int32 ABPlayerState::GetTeamNumber()
{
	return TeamNumber;
}

int32 ABPlayerState::GetGoals()
{
	return Goals;
}

float ABPlayerState::GetScore()
{
	return Score;
}