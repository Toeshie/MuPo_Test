// Fill out your copyright notice in the Description page of Project Settings.


#include "UIGameManager.h"

#include "CharacterSelectionWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ConcertGameInstance.h"

UUIGameManager::UUIGameManager()
{
	// Load the Character Selection Widget Blueprint
	static ConstructorHelpers::FClassFinder<UCharacterSelectionWidget> WidgetClassFinder(TEXT("/Game/Blueprints/UI/CharacterSelectionWDG"));
	if (WidgetClassFinder.Succeeded())
	{
		CharacterSelectionWidgetClass = WidgetClassFinder.Class;
	}
}

void UUIGameManager::LoadLevel(const FName& LevelName)
{
	if (LevelName != NAME_None)
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelName);
	}
}

void UUIGameManager::SetSelectedCharacter(int32 CharacterIndex)
{
	UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GameInstance)
	{
		GameInstance->SetSelectedCharacter(CharacterIndex);
	}
}

void UUIGameManager::SetSelectedInstrument(int32 InstrumentIndex)
{
	UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GameInstance)
	{
		GameInstance->SetSelectedInstrument(InstrumentIndex);
	}
}

void UUIGameManager::LoadCharacterSelectionWidget()
{
	if (CharacterSelectionWidgetClass)
	{
		UCharacterSelectionWidget* CharacterSelectionWidget = CreateWidget<UCharacterSelectionWidget>(GetWorld(), CharacterSelectionWidgetClass);
		if (CharacterSelectionWidget)
		{
			CharacterSelectionWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create CharacterSelectionWidget instance"));
		}
	}
}