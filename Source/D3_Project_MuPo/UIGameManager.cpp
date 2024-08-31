// Fill out your copyright notice in the Description page of Project Settings.

#include "UIGameManager.h"
#include "CharacterSelectionWidget.h"
#include "InstrumentSelectionWidget.h"
#include "ConcertSelectionWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ConcertGameInstance.h"
#include "Sound/SoundMix.h"     
#include "Sound/SoundClass.h"    
#include "D3_Project_MuPoCharacter.h"
#include "OverworldConcertActor.h"
#include "GameFramework/CharacterMovementComponent.h"


UUIGameManager::UUIGameManager()
{
    static ConstructorHelpers::FClassFinder<UCharacterSelectionWidget> CharacterWidgetClassFinder(TEXT("/Game/Blueprints/UI/CharacterSelectionWDG"));
    if (CharacterWidgetClassFinder.Succeeded())
    {
        CharacterSelectionWidgetClass = CharacterWidgetClassFinder.Class;
    }
    CachedConcertSelectionWidget = nullptr;

    UE_LOG(LogTemp, Log, TEXT("UIGameManager Created"));

    InitializeSoundSettings();
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
            AD3_Project_MuPoCharacter* PlayerCharacter = Cast<AD3_Project_MuPoCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
            if (PlayerCharacter)
            {
                PlayerCharacter->GetCharacterMovement()->DisableMovement();
                CharacterSelectionWidget->OnCharacterSelected.AddDynamic(this, &UUIGameManager::OnCharacterSelected);
                CharacterSelectionWidget->AddToViewport();
            }
            
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
    
    if (CachedConcertSelectionWidget == nullptr)
    {
        TSubclassOf<UConcertSelectionWidget> ConcertSelectionWidgetClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr, TEXT("/Game/Blueprints/UI/BP_ConcertSelectionWidget.BP_ConcertSelectionWidget_C"));
        
        if (ConcertSelectionWidgetClass)
        {
            CachedConcertSelectionWidget = CreateWidget<UConcertSelectionWidget>(GetWorld(), ConcertSelectionWidgetClass);
            if (CachedConcertSelectionWidget)
            {
                CachedConcertSelectionWidget->AddToViewport();
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
        UpdateWidgetFromCachedActor();
    }
}

void UUIGameManager::RemoveConcertSelectionWidget()
{
    if (CachedConcertSelectionWidget)
    {
        CachedConcertSelectionWidget->RemoveFromParent();
        CachedConcertSelectionWidget = nullptr; 
    }
}

void UUIGameManager::UpdateWidgetFromCachedActor()
{
    if (CachedOverworldConcertActor && CachedConcertSelectionWidget)
    {
        FString SongName = CachedOverworldConcertActor->GetSongName();
        FString ConcertName = CachedOverworldConcertActor->GetConcertName();
        int32 BestStars = CachedOverworldConcertActor->GetBestStars(); // Get the best star rating

        UE_LOG(LogTemp, Log, TEXT("UpdateWidgetFromCachedActor - SongName: %s, ConcertName: %s, BestStars: %d"), *SongName, *ConcertName, BestStars);

        if (CachedConcertSelectionWidget) 
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

void UUIGameManager::InitializeSoundSettings()
{
    
    MainSoundMix = LoadObject<USoundMix>(nullptr, TEXT("/Game/Sounds/SoundClasses/MainSoundMixer.MainSoundMixer"));
    MusicSoundClass = LoadObject<USoundClass>(nullptr, TEXT("/Game/Sounds/SoundClasses/Music.Music"));
    FXSoundClass = LoadObject<USoundClass>(nullptr, TEXT("/Game/Sounds/SoundClasses/FX.FX"));
    AmbientSoundClass = LoadObject<USoundClass>(nullptr, TEXT("/Game/Sounds/SoundClasses/Ambient.Ambient"));
    
    LoadSoundSettings();
}

void UUIGameManager::SetMusicVolume(float Volume)
{
    if (MusicSoundClass)
    {
        UGameplayStatics::SetSoundMixClassOverride(this, MainSoundMix, MusicSoundClass, Volume, 1.0f, 0.0f);
        UGameplayStatics::PushSoundMixModifier(this, MainSoundMix);
    }
}

void UUIGameManager::SetFXVolume(float Volume)
{
    if (FXSoundClass)
    {
        UGameplayStatics::SetSoundMixClassOverride(this, MainSoundMix, FXSoundClass, Volume, 1.0f, 0.0f);
        UGameplayStatics::PushSoundMixModifier(this, MainSoundMix);
    }
}

void UUIGameManager::SetAmbientVolume(float Volume)
{
    if (AmbientSoundClass)
    {
        UGameplayStatics::SetSoundMixClassOverride(this, MainSoundMix, AmbientSoundClass, Volume, 1.0f, 0.0f);
        UGameplayStatics::PushSoundMixModifier(this, MainSoundMix);
    }
}

void UUIGameManager::SetMuteState(bool bMute)
{
    if (bMute)
    {
        SetMusicVolume(0.0f);
        SetFXVolume(0.0f);
        SetAmbientVolume(0.0f);
    }
    else
    {
        UHighScoreSaveGame* LoadGameInstance = Cast<UHighScoreSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SoundSettingsSlot"), 0));
        if (LoadGameInstance)
        {
            SetMusicVolume(LoadGameInstance->MusicVolume);
            SetFXVolume(LoadGameInstance->FXVolume);
            SetAmbientVolume(LoadGameInstance->AmbientVolume);
        }
    }
}

void UUIGameManager::SaveSoundSettings()
{
    UHighScoreSaveGame* SaveGameInstance = Cast<UHighScoreSaveGame>(UGameplayStatics::CreateSaveGameObject(UHighScoreSaveGame::StaticClass()));
    if (SaveGameInstance)
    {
        SaveGameInstance->MusicVolume = MusicSoundClass->Properties.Volume;
        SaveGameInstance->FXVolume = FXSoundClass->Properties.Volume;
        SaveGameInstance->AmbientVolume = AmbientSoundClass->Properties.Volume;
        SaveGameInstance->bIsMuted = MusicSoundClass->Properties.Volume == 0.0f;

        UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("SoundSettingsSlot"), 0);
    }
}

void UUIGameManager::LoadSoundSettings()
{
    UHighScoreSaveGame* LoadGameInstance = Cast<UHighScoreSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SoundSettingsSlot"), 0));
    if (LoadGameInstance)
    {
        SetMusicVolume(LoadGameInstance->MusicVolume);
        SetFXVolume(LoadGameInstance->FXVolume);
        SetAmbientVolume(LoadGameInstance->AmbientVolume);
        SetMuteState(LoadGameInstance->bIsMuted);
    }
}