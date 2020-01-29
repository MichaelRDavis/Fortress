// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "BGameModeBase.h"
#include "BGameMode.generated.h"

class ABCharacter;
class ABPlayerState;
class APlayerStart;

UCLASS()
class BLITZBALL_API ABGameMode : public ABGameModeBase
{
	GENERATED_BODY()
	
public:
	ABGameMode();

	virtual void PreInitializeComponents() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void InitGameState() override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleMatchHasStarted() override;
	virtual void RestartPlayer(AController* NewPlayer) override;

	UFUNCTION(BlueprintCallable, Category = GameMode)
	void StartMatchTimer();

	UFUNCTION(BlueprintCallable, Category = GameMode)
	void RestartMatch();

	UFUNCTION(BlueprintCallable, Category = GameMode)
	void FinishMatch();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<ABCharacter> DefaultCharacterClass;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GameMode)
	int32 GetTimeRemaining() const;

protected:
	int32 ChooseTeam(ABPlayerState* PlayerState) const;
	void DetermineMatchWinner();
	bool IsSpawnPointAllowed(APlayerStart* Start, AController* Player) const;

	UFUNCTION(BlueprintCallable, Category=GameMode)
	bool IsWinner(ABPlayerState* PlayerState) const;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = GameMode)
	int32 NumTeams;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = GameMode)
	int32 WinningTeam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameMode)
	int32 WarmupTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameMode)
	int32 MatchTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameMode)
	int32 TimeBetweenMatches;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameMode)
	int32 GoalScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameMode)
	int32 SaveScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameMode)
	TSubclassOf<UUserWidget> EndMatchWidget;

	UPROPERTY(BlueprintReadOnly, Category = GameMode)
	UUserWidget* CurrentWidget;

	FTimerHandle MatchTimer;
};
