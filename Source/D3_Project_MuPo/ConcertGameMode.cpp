// Fill out your copyright notice in the Description page of Project Settings.

#include "ConcertGameMode.h"
#include "ConcertCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "ScoreHUD.h"
#include "NoteSpawner.h"
#include "Camera/CameraActor.h"
#include "ConcertGameInstance.h"
#include "Sound/SoundWave.h"
#include "Blueprint/UserWidget.h"

AConcertGameMode::AConcertGameMode()
{
    DefaultPawnClass = AConcertCharacter::StaticClass();
    HUDClass = AScoreHUD::StaticClass();
    Player1Score = 0;
    CurrentStreak = 1;
    ScoreMultiplier = 1;
    TotalNotes = 0;
    GoodHits = 0;
    PerfectHits = 0;
    TotalNotesInSong = 0; // Initialize total notes in song

    static ConstructorHelpers::FClassFinder<UUserWidget> MenuWidgetClass(TEXT("/Game/Blueprints/BP_EndGameMenu"));
    if (MenuWidgetClass.Succeeded())
    {
        EndGameMenuClass = MenuWidgetClass.Class;
    }
    
}

void AConcertGameMode::ScheduleEndGameMenu(float Delay)
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AConcertGameMode::ShowEndGameMenu, Delay, false);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to schedule end game menu - World context is null"));
    }
}

void AConcertGameMode::UpdatePlayer1Score(int32 ScoreDelta)
{
    Player1Score += ScoreDelta * ScoreMultiplier; // Apply the multiplier to the score delta
    UE_LOG(LogTemp, Log, TEXT("Player 1 Score: %d"), Player1Score);
    DisplayScore();
}

int32 AConcertGameMode::GetPlayer1Score() const
{
    return Player1Score;
}

void AConcertGameMode::NoteHit(bool bIsCorrect, bool bIsPerfect)
{
    if (bIsCorrect)
    {
        CurrentStreak++;
        if (CurrentStreak % StreakToIncreaseMultiplier == 0 && ScoreMultiplier < MaxMultiplier)
        {
            ScoreMultiplier++;
            UE_LOG(LogTemp, Log, TEXT("Multiplier increased to %d"), ScoreMultiplier);
        }

        if (bIsPerfect)
        {
            PerfectHits++;
        }
        else
        {
            GoodHits++;
        }
    }
    else
    {
        CurrentStreak = 0;
        ScoreMultiplier = 1;
        UE_LOG(LogTemp, Log, TEXT("Streak reset. Multiplier reset to 1"));
    }

    // Update the HUD with the new streak, multiplier, and hit counts
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        AScoreHUD* HUD = Cast<AScoreHUD>(PlayerController->GetHUD());
        if (HUD)
        {
            HUD->SetStreak(CurrentStreak);
            HUD->SetMultiplier(ScoreMultiplier);
            HUD->SetGoodHits(GoodHits);
            HUD->SetPerfectHits(PerfectHits);
            HUD->SetTotalNotes(TotalNotes);
        }
    }
}

void AConcertGameMode::DisplayScore()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        AScoreHUD* HUD = Cast<AScoreHUD>(PlayerController->GetHUD());
        if (HUD)
        {
            HUD->SetScore(Player1Score);
        }
    }
}

void AConcertGameMode::BeginPlay()
{
    Super::BeginPlay();

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        if (Actor->GetName().Contains(TEXT("MainConcertCameraActor")))
        {
            APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            if (PlayerController)
            {
                PlayerController->SetViewTarget(Actor);
                break;
            }
        }
    }

    // Initialize the NoteSpawner
    AActor* CameraActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass());
    if (CameraActor)
    {
        NoteSpawner = Cast<UNoteSpawner>(CameraActor->GetComponentByClass(UNoteSpawner::StaticClass()));
        if (NoteSpawner)
        {
            NoteSpawner->OnNoteSpawned.AddDynamic(this, &AConcertGameMode::HandleNoteSpawned);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("NoteSpawner component not found on CameraActor"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("CameraActor not found"));
    }

    if (MusicWave)
    {
        SongDuration = MusicWave->Duration;
        UE_LOG(LogTemp, Log, TEXT("MusicWave duration: %f seconds"), SongDuration);

        // Adding a buffer time after the song ends before showing the end game menu
        float EndGameMenuDelay = SongDuration + 5.0f;
        UE_LOG(LogTemp, Log, TEXT("Scheduling end game menu to appear in: %f seconds"), EndGameMenuDelay);
        ScheduleEndGameMenu(EndGameMenuDelay);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("MusicWave is null. Cannot schedule end game menu."));
    }

    FString LevelName = GetWorld()->GetMapName();
    LevelName.RemoveFromStart(TEXT("UEDPIE_0_"));

    // Load the appropriate MusicWave based on the level name
    if (LevelName == "ConcertLocation_1")
    {
        MusicWave = LoadObject<USoundWave>(nullptr, TEXT("/Game/Sounds/Nobody__Not_Even_the_Rain_｜_La_Dispute__2018_.Nobody__Not_Even_the_Rain_｜_La_Dispute__2018_"));
    }
    else if (LevelName == "ConcertLocation_2")
    {
        MusicWave = LoadObject<USoundWave>(nullptr, TEXT("/Game/Sounds/Rhythmic_Vol2_Chankura_MainMarks.Rhythmic_Vol2_Chankura_MainMarks"));
    }

    // Check if the MusicWave is successfully loaded
    if (MusicWave)
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully loaded MusicWave for %s"), *LevelName);
        SongDuration = MusicWave->Duration;
        ScheduleEndGameMenu(SongDuration + 5.0f);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load MusicWave for level %s"), *LevelName);
    }

    // Load the song data for this level
    LoadSongData();
}

void AConcertGameMode::LoadSongData()
{
    GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (GameInstance)
    {
        FString CurrentLevelName = GetWorld()->GetMapName();
        CurrentLevelName.RemoveFromStart(TEXT("UEDPIE_0_"));

        FName LevelName = FName(*CurrentLevelName);
        UE_LOG(LogTemp, Warning, TEXT("Current Level Name (stripped): %s"), *LevelName.ToString());

        const TArray<FNoteData>& NotesData = GameInstance->GetSongDataForLevel(LevelName);
        UE_LOG(LogTemp, Warning, TEXT("Notes Data Count: %d for level: %s"), NotesData.Num(), *LevelName.ToString());

        TotalNotesInSong = NotesData.Num(); // Set the total number of notes in the current song

        if (NoteSpawner)
        {
            NoteSpawner->SetNotesData(NotesData);
            UE_LOG(LogTemp, Warning, TEXT("SetNotesData called. Notes count: %d"), NotesData.Num());
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("NoteSpawner is null. Cannot set notes data."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GameInstance not found"));
    }
}

void AConcertGameMode::HandleNoteSpawned()
{
    TotalNotes++;
    UE_LOG(LogTemp, Warning, TEXT("Note spawned. TotalNotes: %d, TotalNotesInSong: %d"), TotalNotes, TotalNotesInSong);

    // Update the HUD with the new total notes
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        AScoreHUD* HUD = Cast<AScoreHUD>(PlayerController->GetHUD());
        if (HUD)
        {
            HUD->SetTotalNotes(TotalNotes);
        }
    }

    if (TotalNotes == TotalNotesInSong)
    {
        UE_LOG(LogTemp, Warning, TEXT("All notes spawned. Setting timer to show end game menu."));
        ScheduleEndGameMenu(12.0f); // Show end game menu after 5 seconds when all notes are spawned
    }
}

void AConcertGameMode::ShowEndGameMenu()
{
    UE_LOG(LogTemp, Warning, TEXT("Attempting to show end game menu."));
    if (EndGameMenuClass)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PlayerController)
        {
            UUserWidget* EndGameMenu = CreateWidget<UUserWidget>(PlayerController, EndGameMenuClass);
            if (EndGameMenu)
            {
                UE_LOG(LogTemp, Warning, TEXT("End game menu widget created."));
                EndGameMenu->AddToViewport();
                // Set input mode to UI only and show the mouse cursor
                FInputModeUIOnly InputMode;
                InputMode.SetWidgetToFocus(EndGameMenu->TakeWidget());
                PlayerController->SetInputMode(InputMode);
                PlayerController->bShowMouseCursor = true;
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to create end game menu widget."));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("PlayerController not found."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("EndGameMenuClass is null."));
    }
}