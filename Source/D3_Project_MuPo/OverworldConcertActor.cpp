// Fill out your copyright notice in the Description page of Project Settings.

#include "OverworldConcertActor.h"
#include "ConcertSelectionWidget.h"
#include "D3_Project_MuPoCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ConcertGameInstance.h"
#include "ConcertSelectionSongChoiceWidget.h"

// Sets default values
AOverworldConcertActor::AOverworldConcertActor()
{
    PrimaryActorTick.bCanEverTick = true;

    // Initialize the root component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    // Initialize the collider
    ConcertCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Concert Collider"));
    ConcertCollider->SetupAttachment(RootComponent);

    // Set default values for the collider, such as its extent
    ConcertCollider->InitBoxExtent(FVector(50.f, 50.f, 50.f));  // Example size
    ConcertCollider->SetCollisionProfileName(TEXT("Trigger"));  // Set the collision profile to Trigger (adjust as needed)

    // Attach overlap events (if necessary)
    ConcertCollider->OnComponentBeginOverlap.AddDynamic(this, &AOverworldConcertActor::OnBeginOverlap);
    ConcertCollider->OnComponentEndOverlap.AddDynamic(this, &AOverworldConcertActor::OnEndOverlap);
}

void AOverworldConcertActor::BeginPlay()
{
    Super::BeginPlay();
}

void AOverworldConcertActor::ShowWidget()
{
    if (WidgetClass && !WidgetInstance)
    {
        // Assume LevelToLoad determines which concert level is going to be loaded
        FString CurrentLevelName = GetWorld()->GetMapName();
        CurrentLevelName.RemoveFromStart(TEXT("UEDPIE_0_"));

        UE_LOG(LogTemp, Log, TEXT("Current Level Name: %s"), *CurrentLevelName);

        if (CurrentLevelName == "Overworld")
        {
            if (LevelToLoad == "ConcertLocation_1")
            {
                // Show widget for ConcertLocation_1
                UE_LOG(LogTemp, Log, TEXT("Showing normal widget for ConcertLocation_1"));

                WidgetInstance = CreateWidget<UConcertSelectionWidget>(GetWorld(), WidgetClass);
                if (WidgetInstance)
                {
                    UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
                    FString SongName = TEXT("Concert Location 1 Song"); // Hardcoded or fetched for ConcertLocation_1

                    WidgetInstance->InitializeWidget(SongName, TArray<FString>({ TEXT("Concert Character") }));
                    WidgetInstance->OnConfirm.AddDynamic(this, &AOverworldConcertActor::LoadLevel);

                    WidgetInstance->AddToViewport();

                    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
                    {
                        PlayerController->bShowMouseCursor = true;
                        PlayerController->bEnableClickEvents = true;
                        PlayerController->bEnableMouseOverEvents = true;
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to create widget instance for ConcertLocation_1."));
                }
            }
            else if (LevelToLoad == "ConcertLocation_2")
            {
                // Show widget for ConcertLocation_2
                UE_LOG(LogTemp, Log, TEXT("Showing normal widget for ConcertLocation_2"));

                WidgetInstance = CreateWidget<UConcertSelectionWidget>(GetWorld(), WidgetClass);
                if (WidgetInstance)
                {
                    UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
                    FString SongName = TEXT("Concert Location 2 Song"); // Hardcoded or fetched for ConcertLocation_2

                    WidgetInstance->InitializeWidget(SongName, TArray<FString>({ TEXT("Concert Character") }));
                    WidgetInstance->OnConfirm.AddDynamic(this, &AOverworldConcertActor::LoadLevel);

                    WidgetInstance->AddToViewport();

                    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
                    {
                        PlayerController->bShowMouseCursor = true;
                        PlayerController->bEnableClickEvents = true;
                        PlayerController->bEnableMouseOverEvents = true;
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to create widget instance for ConcertLocation_2."));
                }
            }
            else
            {
                // Show the custom song selection widget for other cases (like ConcertLocation_CustomSongs)
                UE_LOG(LogTemp, Log, TEXT("Showing Widget for custom song selection"));

                WidgetInstance = CreateWidget<UConcertSelectionSongChoiceWidget>(GetWorld(), WidgetClass);
                if (WidgetInstance)
                {
                    TArray<FString> AvailableSongs;
                    IFileManager& FileManager = IFileManager::Get();
                    FString Path = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("HiddenSongs/"));
                    UE_LOG(LogTemp, Log, TEXT("Looking for songs in directory: %s"), *Path);
                    FileManager.FindFiles(AvailableSongs, *Path, TEXT("*.csv"));

                    if (AvailableSongs.Num() == 0)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("No songs found in directory: %s"), *Path);
                    }
                    else
                    {
                        for (const FString& Song : AvailableSongs)
                        {
                            UE_LOG(LogTemp, Log, TEXT("Found song: %s"), *Song);
                        }
                    }

                    UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
                    if (GameInstance)
                    {
                        FString SongName = GameInstance->GetSongNameForLevel(LevelToLoad);
                        UE_LOG(LogTemp, Log, TEXT("Initializing widget with song name: %s"), *SongName);
                        Cast<UConcertSelectionSongChoiceWidget>(WidgetInstance)->InitializeWidgetWithSongs(SongName, TArray<FString>({ TEXT("Concert Character") }), AvailableSongs);
                        Cast<UConcertSelectionSongChoiceWidget>(WidgetInstance)->OnSongChosen.AddDynamic(this, &AOverworldConcertActor::OnSongChosen);
                    }

                    WidgetInstance->AddToViewport();

                    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
                    {
                        PlayerController->bShowMouseCursor = true;
                        PlayerController->bEnableClickEvents = true;
                        PlayerController->bEnableMouseOverEvents = true;
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to create widget instance."));
                }
            }
        }
    }
}

void AOverworldConcertActor::LoadLevel()
{
    if (WidgetInstance)
    {
        FString SelectedCharacter = WidgetInstance->GetSelectedCharacter();
        UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
        if (GameInstance)
        {
            GameInstance->SetSelectedCharacter(SelectedCharacter);
        }

        // Remove the widget
        WidgetInstance->RemoveFromParent();
        WidgetInstance = nullptr;

        // Load the level
        if (LevelToLoad != NAME_None)
        {
            UGameplayStatics::OpenLevel(this, LevelToLoad);
        }
    }
}

void AOverworldConcertActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && OtherComp)
    {
        if (AD3_Project_MuPoCharacter* PlayerCharacter = Cast<AD3_Project_MuPoCharacter>(OtherActor))
        {
            ShowWidget();
        }
    }
}

void AOverworldConcertActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor != this && OtherComp)
    {
        if (AD3_Project_MuPoCharacter* PlayerCharacter = Cast<AD3_Project_MuPoCharacter>(OtherActor))
        {
            if (WidgetInstance)
            {
                WidgetInstance->RemoveFromParent();
                WidgetInstance = nullptr;

                APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
                if (PlayerController)
                {
                    PlayerController->bShowMouseCursor = false;
                    PlayerController->bEnableClickEvents = false;
                    PlayerController->bEnableMouseOverEvents = false;
                }
            }
        }
    }
}

void AOverworldConcertActor::OnSongChosen(const FString& SelectedSongName, const FString& SelectedCharacter)
{
    if (SelectedSongName.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("No song selected for ConcertLocation_CustomSongs."));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Song chosen: %s, Character: %s"), *SelectedSongName, *SelectedCharacter);

    UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (GameInstance)
    {
        // Store the selected song and character in the game instance to be used in the next level
        GameInstance->SetSelectedSong(SelectedSongName);
        GameInstance->SetSelectedCharacter(SelectedCharacter);
    }

    // Use the class member LevelToLoad
    LevelToLoad = "ConcertLocation_CustomSongs";
    UE_LOG(LogTemp, Log, TEXT("Loading level: %s with selected song: %s"), *LevelToLoad.ToString(), *SelectedSongName);
    UGameplayStatics::OpenLevel(this, FName(LevelToLoad));
}