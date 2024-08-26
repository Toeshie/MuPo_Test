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
    UE_LOG(LogTemp, Log, TEXT("UIGameManager Created"));

    static ConstructorHelpers::FClassFinder<UUserWidget> InstrumentSelectionBPClass(TEXT("/Game/Blueprints/UI/InstrumentSelectionWDG"));
    if (InstrumentSelectionBPClass.Class != nullptr)
    {
        InstrumentSelectionWidgetClass = InstrumentSelectionBPClass.Class;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find InstrumentSelectionWidget class at path: /Game/Blueprints/UI/InstrumentSelectionWDG"));
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
            CharacterSelectionWidget->OnCharacterSelected.AddDynamic(this, &UUIGameManager::OnCharacterSelected);
            CharacterSelectionWidget->AddToViewport();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to create CharacterSelectionWidget instance"));
        }
    }
}

void UUIGameManager::OnCharacterSelected(int32 CharacterIndex, UTexture2D* SelectedCharacterImage)
{
    UE_LOG(LogTemp, Log, TEXT("Character %d selected, image: %s"), CharacterIndex, *SelectedCharacterImage->GetName());

    SetSelectedCharacter(CharacterIndex);
    
    AOverworldConcertActor* OverworldActor = CachedOverworldConcertActor;
    LoadInstrumentSelectionWidget(SelectedCharacterImage, OverworldActor);
}

void UUIGameManager::SetLevelToLoad(const FString& LevelName)
{
    CachedLevelName = LevelName;
    UE_LOG(LogTemp, Log, TEXT("Level to load set to: %s"), *LevelName);
}

void UUIGameManager::LoadInstrumentSelectionWidget(UTexture2D* CharacterImage, AOverworldConcertActor* OverworldConcertActor)
{
    if (!InstrumentSelectionWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("InstrumentSelectionWidgetClass is null"));
        return;
    }

    UInstrumentSelectionWidget* InstrumentWidget = CreateWidget<UInstrumentSelectionWidget>(GetWorld(), InstrumentSelectionWidgetClass);
    if (InstrumentWidget)
    {
        // Additional logic to display the widget
        InstrumentWidget->AddToViewport();

        // Ensure that the level name is correctly passed
        InstrumentWidget->SetCachedLevelName(CachedLevelName);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to create InstrumentSelectionWidget instance"));
    }
}

void UUIGameManager::OnInstrumentSelected(int32 InstrumentIndex)
{
    // Temporary hardcoded level name for testing
    FString TestLevelName = "ConcertLocation_1";

    UE_LOG(LogTemp, Log, TEXT("Loading test level: %s"), *TestLevelName);
    UGameplayStatics::OpenLevel(this, FName(*TestLevelName));
}

