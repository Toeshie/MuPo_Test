// Fill out your copyright notice in the Description page of Project Settings.

#include "InstrumentSelectionWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "UIGameManager.h"
#include "Kismet/GameplayStatics.h"
#include "OverworldConcertActor.h"
#include "GameFramework/Character.h"

void UInstrumentSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ConcertCharacterButton)
	{
		ConcertCharacterButton->OnClicked.AddDynamic(this, &UInstrumentSelectionWidget::OnConcertCharacterButtonClicked);
		ConcertCharacterButton->SetKeyboardFocus();
	}
	if (MarimbaCharacterButton)
	{
		MarimbaCharacterButton->OnClicked.AddDynamic(this, &UInstrumentSelectionWidget::OnMarimbaCharacterButtonClicked);
	}
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		ACharacter* PlayerCharacter = Cast<ACharacter>(PlayerController->GetPawn());
		if (PlayerCharacter)
		{
			PlayerCharacter->DisableInput(PlayerController);
		}

		// Set input mode to UI only, so the player can only interact with the UI
		PlayerController->SetInputMode(FInputModeUIOnly());

		// Optionally, show and lock the mouse cursor
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
	}
}

void UInstrumentSelectionWidget::InitializeInstrumentSelectionWidget(UTexture2D* CharacterImage)
{
	if (SelectedCharacterImage && CharacterImage)
	{
		SelectedCharacterImage->SetBrushFromTexture(CharacterImage);
	}
}

void UInstrumentSelectionWidget::SetCachedLevelName(const FString& LevelName)
{
	CachedLevelName = LevelName;
}

void UInstrumentSelectionWidget::OnConcertCharacterButtonClicked()
{
	// Handle the selection of the concert character
	UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GameInstance)
	{
		GameInstance->SetSelectedInstrument(0);  // 0 for Concert Character
	}

	// Notify the UIGameManager to proceed with the next step (like loading a level)
	UUIGameManager* GameManager = GameInstance->GetUIGameManager();
	if (GameManager)
	{
		GameManager->OnInstrumentSelected(0);
	}

	// Optionally, remove this widget if you're done with it
	RemoveFromParent();
	UGameplayStatics::OpenLevel(this, FName(*CachedLevelName));
}

void UInstrumentSelectionWidget::OnMarimbaCharacterButtonClicked()
{
	// Handle the selection of the marimba character
	UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GameInstance)
	{
		GameInstance->SetSelectedInstrument(1);  // 1 for Marimba Character
	}

	// Notify the UIGameManager to proceed with the next step (like loading a level)
	UUIGameManager* GameManager = GameInstance->GetUIGameManager();
	if (GameManager)
	{
		GameManager->OnInstrumentSelected(1);
	}

	// Optionally, remove this widget if you're done with it
	RemoveFromParent();
	UGameplayStatics::OpenLevel(this, FName(*CachedLevelName));
}

