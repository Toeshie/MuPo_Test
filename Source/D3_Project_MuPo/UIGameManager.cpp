// Fill out your copyright notice in the Description page of Project Settings.

#include "UIGameManager.h"
#include "CharacterSelectionWidget.h"
#include "InstrumentSelectionWidget.h"
#include "ConcertSelectionWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ConcertGameInstance.h"
#include "OverworldConcertActor.h"

UUIGameManager::UUIGameManager()
{
    static ConstructorHelpers::FClassFinder<UCharacterSelectionWidget> CharacterWidgetClassFinder(TEXT("/Game/Blueprints/UI/CharacterSelectionWDG"));
    if (CharacterWidgetClassFinder.Succeeded())
    {
        CharacterSelectionWidgetClass = CharacterWidgetClassFinder.Class;
    }

    // Initialize CachedConcertSelectionWidget as nullptr
    CachedConcertSelectionWidget = nullptr;

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
    UE_LOG(LogTemp, Log, TEXT("Instrument selected: %d"), InstrumentIndex);

    SetSelectedInstrument(InstrumentIndex);

    // Load the level using the cached OverworldConcertActor
    if (CachedOverworldConcertActor)
    {
        UE_LOG(LogTemp, Log, TEXT("CachedOverworldConcertActor is valid, loading level..."));
        CachedOverworldConcertActor->LoadLevel();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("CachedOverworldConcertActor is null, cannot load level"));
    }
}

void UUIGameManager::CacheOverworldConcertActor(AOverworldConcertActor* OverworldConcertActor)
{
    CachedOverworldConcertActor = OverworldConcertActor;

    // Load and display the ConcertSelectionWidget using the cached actor
    if (CachedConcertSelectionWidget == nullptr)
    {
        // Assuming you have a path for the ConcertSelectionWidget
        TSubclassOf<UConcertSelectionWidget> ConcertSelectionWidgetClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr, TEXT("/Game/Blueprints/UI/BP_ConcertSelectionWidget.BP_ConcertSelectionWidget_C"));
        
        if (ConcertSelectionWidgetClass)
        {
            CachedConcertSelectionWidget = CreateWidget<UConcertSelectionWidget>(GetWorld(), ConcertSelectionWidgetClass);
            if (CachedConcertSelectionWidget)
            {
                CachedConcertSelectionWidget->AddToViewport();

                // Now update the widget using the cached actor
                UpdateWidgetFromCachedActor();
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to create ConcertSelectionWidget instance"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load ConcertSelectionWidget class"));
        }
    }
    else
    {
        // If the widget is already cached, just update it
        UpdateWidgetFromCachedActor();
    }
}

void UUIGameManager::RemoveConcertSelectionWidget()
{
    if (CachedConcertSelectionWidget)
    {
        CachedConcertSelectionWidget->RemoveFromParent();
        CachedConcertSelectionWidget = nullptr;  // Optionally reset the cached widget to null
    }
}

void UUIGameManager::UpdateWidgetFromCachedActor()
{
    if (CachedOverworldConcertActor && CachedConcertSelectionWidget)
    {
        FString SongName = CachedOverworldConcertActor->GetSongName();
        FString ConcertName = CachedOverworldConcertActor->GetConcertName();
        int32 BestStars = CachedOverworldConcertActor->GetBestStars();

        UE_LOG(LogTemp, Log, TEXT("UpdateWidgetFromCachedActor - SongName: %s, ConcertName: %s"), *SongName, *ConcertName);

        if (CachedConcertSelectionWidget)  // Ensure this is not null
        {
            CachedConcertSelectionWidget->InitializeWidget(SongName, ConcertName, BestStars);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("CachedConcertSelectionWidget is null"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("CachedOverworldConcertActor or CachedConcertSelectionWidget is null"));
    }
}
