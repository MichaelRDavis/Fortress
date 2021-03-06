// Fill out your copyright notice in the Description page of Project Settings.

#include "BPlayerController.h"
#include "BCharacter.h"
#include "BPlayerCameraManager.h"
#include "BPlayerState.h"
#include "BHUD.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/Engine.h"

ABPlayerController::ABPlayerController()
{
	PlayerCameraManagerClass = ABPlayerCameraManager::StaticClass();
	BaseTurnRate = 45.0f;
	BaseLookUpRate = 45.0f;
	bIsInputAllowed = true;
}

void ABPlayerController::SetPawn(APawn* InPawn)
{
	AController::SetPawn(InPawn);
	BCharacter = Cast<ABCharacter>(InPawn);
}

void ABPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ABPlayerController::MoveForward);
	InputComponent->BindAxis("MoveBackward", this, &ABPlayerController::MoveBackward);
	InputComponent->BindAxis("MoveRight", this, &ABPlayerController::MoveRight);
	InputComponent->BindAxis("MoveLeft", this, &ABPlayerController::MoveLeft);
	InputComponent->BindAxis("Turn", this, &ABPlayerController::AddYawInput);
	InputComponent->BindAxis("TurnRate", this, &ABPlayerController::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &ABPlayerController::AddPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ABPlayerController::LookUpAtRate);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ABPlayerController::OnJump);
	InputComponent->BindAction("Jump", IE_Released, this, &ABPlayerController::OnStopJumping);
	InputComponent->BindAction("Crouch", IE_Pressed, this, &ABPlayerController::OnCrouch);
	InputComponent->BindAction("Crouch", IE_Released, this, &ABPlayerController::OnUnCrouch);
	InputComponent->BindAction("ToggleCrouch", IE_Pressed, this, &ABPlayerController::OnToggleCrouch);
	InputComponent->BindAction("Sprint", IE_Pressed, this, &ABPlayerController::OnSprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &ABPlayerController::OnStopSprinting);

	InputComponent->BindAction("Kick", IE_Pressed, this, &ABPlayerController::OnKick);

	InputComponent->BindAction("Scorebaord", IE_Pressed, this, &ABPlayerController::OnShowScoreboard);
	InputComponent->BindAction("Scorebaord", IE_Released, this, &ABPlayerController::OnHideScoreboard);
	InputComponent->BindAction("Pause", IE_Pressed, this, &ABPlayerController::OnShowPauseMenu);
}

void ABPlayerController::MoveForward(float Value)
{
	if (BCharacter != nullptr && Value != 0.0f && IsInputAllowed())
	{
		BCharacter->MoveForward(Value);
	}
}

void ABPlayerController::MoveBackward(float Value)
{
	MoveForward(Value * -1);
}

void ABPlayerController::MoveRight(float Value)
{
	if (BCharacter != nullptr && Value != 0.0f && IsInputAllowed())
	{
		BCharacter->MoveRight(Value);
	}
}

void ABPlayerController::MoveLeft(float Value)
{
	MoveRight(Value * -1);
}

void ABPlayerController::TurnAtRate(float Rate)
{
	AddYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABPlayerController::LookUpAtRate(float Rate)
{
	AddPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABPlayerController::OnJump()
{
	if (BCharacter && IsInputAllowed())
	{
		BCharacter->Jump();
	}
}

void ABPlayerController::OnStopJumping()
{
	if (BCharacter && IsInputAllowed())
	{
		BCharacter->StopJumping();
	}
}

void ABPlayerController::OnCrouch()
{
	if (BCharacter && IsInputAllowed())
	{
		BCharacter->Crouch();
	}
}

void ABPlayerController::OnUnCrouch()
{
	if (BCharacter && IsInputAllowed())
	{
		BCharacter->UnCrouch();
	}
}

void ABPlayerController::OnToggleCrouch()
{
	if (GetCharacter() && IsInputAllowed())
	{
		GetCharacter()->bIsCrouched ? OnUnCrouch() : OnCrouch();
	}
}

void ABPlayerController::OnSprint()
{
	if (BCharacter && IsInputAllowed())
	{
		BCharacter->StartSprinting();
	}
}

void ABPlayerController::OnStopSprinting()
{
	if (BCharacter && IsInputAllowed())
	{
		BCharacter->StopSprinting();
	}
}

void ABPlayerController::OnKick()
{
	if (BCharacter && IsInputAllowed())
	{
		BCharacter->Kick();
	}
}

void ABPlayerController::OnShowScoreboard()
{
	if (ScoreboardWidget)
	{
		CurrentWidget = CreateWidget<UUserWidget>(this, ScoreboardWidget);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void ABPlayerController::OnHideScoreboard()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
	}
}

void ABPlayerController::OnShowPauseMenu()
{
	if (PauseMenuWidget)
	{
		CurrentWidget = CreateWidget<UUserWidget>(this, PauseMenuWidget);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
			UWidgetBlueprintLibrary::SetInputMode_UIOnly(this, CurrentWidget);
			bShowMouseCursor = true;
		}
	}
}

void ABPlayerController::SetGameInputAllowed(bool bNewGameInput)
{
	bIsInputAllowed = bNewGameInput;
}

bool ABPlayerController::IsInputAllowed() const
{
	return bIsInputAllowed;
}

void ABPlayerController::OnScored()
{
	ABHUD* HUD = Cast<ABHUD>(GetHUD());
	if (HUD)
	{
		HUD->NotifyPlayerScored();
	}

	PlayerScoredDelegate.Broadcast();
}

void ABPlayerController::OnEndMatch()
{
	if (EndMatchWidget)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), EndMatchWidget);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}
