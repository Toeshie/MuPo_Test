// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenuWidget::TogglePauseMenu()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (!bIsPaused)
	{
		// Pause the game
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		bIsPaused = true;

		// Add the pause menu widget to the viewport
		AddToViewport();
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetInputMode(FInputModeUIOnly());
	}
	else
	{
		// Unpause the game
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		bIsPaused = false;

		// Remove the pause menu widget from the viewport
		RemoveFromParent();
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
}

void UPauseMenuWidget::RestartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(nullptr);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetName()), false);
}
