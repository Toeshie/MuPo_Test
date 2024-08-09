// Fill out your copyright notice in the Description page of Project Settings.


#include "EndGameMenu.h"

#include "ConcertCharacter.h"
#include "OverworldCarCharacter.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"


void UEndGameMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (PlayAgainButton)
	{
		PlayAgainButton->SetKeyboardFocus();
		PlayAgainButton->OnClicked.AddDynamic(this, &UEndGameMenu::OnPlayAgainClicked);
	}

	if (QuitToOverworldButton)
	{
		QuitToOverworldButton->OnClicked.AddDynamic(this, &UEndGameMenu::OnQuitToOverworldClicked);
	}
}

void UEndGameMenu::OnPlayAgainClicked()
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


void UEndGameMenu::OnQuitToOverworldClicked()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(nullptr);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("Overworld"), false);
}

void UEndGameMenu::SetupInputContexts()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AConcertCharacter* ConcertCharacter = Cast<AConcertCharacter>(PlayerController->GetPawn()))
		{
			ConcertCharacter->InitializeInputMappings();
		}
		else if (AOverworldCarCharacter* OverworldCarCharacter = Cast<AOverworldCarCharacter>(PlayerController->GetPawn()))
		{
			OverworldCarCharacter->InitializeInputMappings();
		}
	}
}