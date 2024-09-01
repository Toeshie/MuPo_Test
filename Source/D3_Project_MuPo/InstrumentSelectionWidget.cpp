// Fill out your copyright notice in the Description page of Project Settings.

#include "InstrumentSelectionWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "UIGameManager.h"
#include "ConcertGameInstance.h"
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

void UInstrumentSelectionWidget::OnConcertCharacterButtonClicked()
{
	UUIGameManager* GameManager = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this))->GetUIGameManager();
	if (GameManager)
	{
		GameManager->OnInstrumentSelected(0);  // Assuming 0 is for Concert Character
	}

	RemoveFromParent();
}

void UInstrumentSelectionWidget::OnMarimbaCharacterButtonClicked()
{
	UUIGameManager* GameManager = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this))->GetUIGameManager();
	if (GameManager)
	{
		GameManager->OnInstrumentSelected(1);  // Assuming 1 is for Marimba Character
	}

	RemoveFromParent();
}