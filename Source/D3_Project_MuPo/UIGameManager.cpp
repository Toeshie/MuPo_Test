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


void UUIGameManager::LoadInstrumentSelectionWidget(UTexture2D* CharacterImage, AOverworldConcertActor* OverworldConcertActor)
{
    UE_LOG(LogTemp, Log, TEXT("Attempting to load InstrumentSelectionWidget dynamically..."));

    // Corrected path based on your directory structure
    InstrumentSelectionWidgetClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr, TEXT("/Game/Blueprints/InstrumentSelectionWDG.InstrumentSelectionWDG_C"));

    if (InstrumentSelectionWidgetClass)
    {
        UInstrumentSelectionWidget* InstrumentWidget = CreateWidget<UInstrumentSelectionWidget>(GetWorld(), InstrumentSelectionWidgetClass);
        if (InstrumentWidget)
        {
            UE_LOG(LogTemp, Log, TEXT("Instrument selection widget created and added to viewport."));

            CachedOverworldConcertActor = OverworldConcertActor;

            InstrumentWidget->InitializeInstrumentSelectionWidget(CharacterImage);
            InstrumentWidget->OnInstrumentSelected.AddDynamic(this, &UUIGameManager::OnInstrumentSelected);
            InstrumentWidget->AddToViewport();

            APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            if (PlayerController)
            {
                PlayerController->SetInputMode(FInputModeUIOnly());
                PlayerController->bShowMouseCursor = true;
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to create InstrumentSelectionWidget instance"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to dynamically load InstrumentSelectionWidget class"));
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