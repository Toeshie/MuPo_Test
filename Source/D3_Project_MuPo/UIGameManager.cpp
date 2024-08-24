// Fill out your copyright notice in the Description page of Project Settings.

#include "UIGameManager.h"
#include "CharacterSelectionWidget.h"
#include "InstrumentSelectionWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ConcertGameInstance.h"
#include "OverworldConcertActor.h"

UUIGameManager::UUIGameManager()
{
    // Load the Character Selection Widget Blueprint
    static ConstructorHelpers::FClassFinder<UCharacterSelectionWidget> CharacterWidgetClassFinder(TEXT("/Game/Blueprints/UI/CharacterSelectionWDG"));
    if (CharacterWidgetClassFinder.Succeeded())
    {
        CharacterSelectionWidgetClass = CharacterWidgetClassFinder.Class;
    }

    // Load the Instrument Selection Widget Blueprint
    static ConstructorHelpers::FClassFinder<UInstrumentSelectionWidget> InstrumentWidgetClassFinder(TEXT("/Game/Blueprints/UI/InstrumentSelectionWDG"));
    if (InstrumentWidgetClassFinder.Succeeded())
    {
        InstrumentSelectionWidgetClass = InstrumentWidgetClassFinder.Class;
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

void UUIGameManager::LoadInstrumentSelectionWidget(UTexture2D* CharacterImage, AOverworldConcertActor* OverworldConcertActor)
{
    if (InstrumentSelectionWidgetClass)
    {
        UInstrumentSelectionWidget* InstrumentWidget = CreateWidget<UInstrumentSelectionWidget>(GetWorld(), InstrumentSelectionWidgetClass);
        if (InstrumentWidget)
        {
            CachedOverworldConcertActor = OverworldConcertActor;  // Cache the actor for level loading

            InstrumentWidget->InitializeInstrumentSelectionWidget(CharacterImage);
            InstrumentWidget->OnInstrumentSelected.AddDynamic(this, &UUIGameManager::OnInstrumentSelected);
            InstrumentWidget->AddToViewport();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to create InstrumentSelectionWidget instance"));
        }
    }
}

void UUIGameManager::OnInstrumentSelected(int32 InstrumentIndex)
{
    SetSelectedInstrument(InstrumentIndex);

    // Load the level using the cached OverworldConcertActor
    if (CachedOverworldConcertActor)
    {
        CachedOverworldConcertActor->LoadLevel();
    }
}