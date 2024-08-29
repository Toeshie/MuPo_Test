// Fill out your copyright notice in the Description page of Project Settings.

#include "ConcertGameMode.h"
#include "ConcertCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "ScoreHUD.h"
#include "NoteSpawner.h"
#include "Camera/CameraActor.h"
#include "ConcertGameInstance.h"
#include "EndGameMenu.h"
#include "HighScoreSaveGame.h"
#include "MarimbaCharacter.h"
#include "Sound/SoundWave.h"
#include "NiagaraComponent.h"
#include "NiagaraActor.h"
#include "NiagaraFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerStart.h"

AConcertGameMode::AConcertGameMode()
{
    DefaultPawnClass = AMarimbaCharacter::StaticClass();
    HUDClass = AScoreHUD::StaticClass();
    Player1Score = 0;
    CurrentStreak = 1;
    ScoreMultiplier = 1;
    TotalNotes = 0;
    GoodHits = 0;
    PerfectHits = 0;
    TotalNotesInSong = 0; 

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
    Player1Score += ScoreDelta * ScoreMultiplier;
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
        }

        if (bIsPerfect)
        {
            PerfectHits++;
            UE_LOG(LogTemp, Log, TEXT("Perfect hit. PerfectHits: %d"), PerfectHits);
        }
        else
        {
            GoodHits++;
            UE_LOG(LogTemp, Log, TEXT("Good hit. GoodHits: %d"), GoodHits);
        }
    }
    else
    {
        CurrentStreak = 0;
        ScoreMultiplier = 1;
    }

    HitPercentage = GetCorrectNotePercentage();
    DisplayScore();
}

int32 AConcertGameMode::GetFinalScore() const
{
    return Player1Score;
}

float AConcertGameMode::GetCorrectNotePercentage() 
{
    if (TotalNotes == 0)
    {
        return 0.0f; 
    }

    int32 TotalHits = GoodHits + PerfectHits;
    HitPercentage = static_cast<float>(TotalHits) / TotalNotes * 100.0f;

    UE_LOG(LogTemp, Log, TEXT("Calculating Hit Percentage. TotalHits: %d, TotalNotes: %d, HitPercentage: %f"), TotalHits, TotalNotes, HitPercentage);

    return HitPercentage;
    
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
            HUD->SetStreak(CurrentStreak);
            HUD->SetMultiplier(ScoreMultiplier);
            HUD->SetGoodHits(GoodHits);
            HUD->SetPerfectHits(PerfectHits);
            HUD->SetTotalNotes(TotalNotes);

            // Update the HUD with the latest hit percentage
            HUD->SetHitPercentage(GetCorrectNotePercentage());
            UE_LOG(LogTemp, Log, TEXT("Updated HUD with HitPercentage: %f"), GetCorrectNotePercentage());
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
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("MusicWave is null. Trying to calculate duration from Note Data."));
    }

    FString LevelName = GetWorld()->GetMapName();
    LevelName.RemoveFromStart(TEXT("UEDPIE_0_"));

    if (SongDuration <= 0.0f) // either or approach
    {
        GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
        if (GameInstance)
        {
            SongDuration = GameInstance->GetSongDuration(FName(*LevelName));
            UE_LOG(LogTemp, Log, TEXT("Calculated song duration from Note Data: %f seconds"), SongDuration);
        }
    }

    // Schedule the end game menu with a buffer time
    if (SongDuration > 0.0f)
    {
        float EndGameMenuDelay = SongDuration + 9.0f;
        UE_LOG(LogTemp, Log, TEXT("Scheduling end game menu to appear in: %f seconds"), EndGameMenuDelay);
        ScheduleEndGameMenu(EndGameMenuDelay);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to determine song duration. End game menu will not be scheduled."));
    }
    
    LoadSongData();
}

void AConcertGameMode::LoadSongData()
{
    GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to cast GameInstance"));
        return;
    }

    FString CurrentLevelName = GetWorld()->GetMapName();
    CurrentLevelName.RemoveFromStart(TEXT("UEDPIE_0_"));

    const TArray<FNoteData>& LevelNotesData = GameInstance->GetSongDataForLevel(FName(*CurrentLevelName));

    if (LevelNotesData.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("No note data found for level: %s"), *CurrentLevelName);
        return;
    }

    if (NoteSpawner)
    {
        NoteSpawner->SetNotesData(LevelNotesData);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("NoteSpawner is null. Cannot set notes data."));
    }
}


void AConcertGameMode::HandleNoteSpawned()
{
    TotalNotes++;
    UE_LOG(LogTemp, Warning, TEXT("Note spawned. TotalNotes: %d"), TotalNotes);

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
        ScheduleEndGameMenu(12.0f); 
    }
}

void AConcertGameMode::SpawnSelectedCharacter()
{
    UConcertGameInstance* LocalGameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (!LocalGameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("No GameInstance found!"));
        return;
    }

    int32 SelectedCharacterIndex = LocalGameInstance->GetSelectedCharacter();
    int32 SelectedInstrumentIndex = LocalGameInstance->GetSelectedInstrument();
    UStaticMesh* SelectedCharacterMesh = Cast<UStaticMesh>(LocalGameInstance->GetSelectedCharacterMesh());

    TSubclassOf<ACharacter> CharacterToSpawn = nullptr;

    if (SelectedInstrumentIndex == 0)  // Assuming 0 is for ConcertCharacter
    {
        CharacterToSpawn = AConcertCharacter::StaticClass();
    }
    else if (SelectedInstrumentIndex == 1)  // Assuming 1 is for MarimbaCharacter
    {
        CharacterToSpawn = AMarimbaCharacter::StaticClass();
    }

    if (CharacterToSpawn)
    {
        AActor* PlayerStart = UGameplayStatics::GetActorOfClass(this, APlayerStart::StaticClass());
        FVector SpawnLocation = FVector::ZeroVector;
        FRotator SpawnRotation = FRotator::ZeroRotator;

        if (PlayerStart)
        {
            SpawnLocation = PlayerStart->GetActorLocation();
            SpawnRotation = PlayerStart->GetActorRotation();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No PlayerStart found! Spawning at default location."));
        }

        ACharacter* SpawnedCharacter = GetWorld()->SpawnActor<ACharacter>(CharacterToSpawn, SpawnLocation, SpawnRotation);
        if (SpawnedCharacter)
        {
            AConcertCharacter* ConcertCharacter = Cast<AConcertCharacter>(SpawnedCharacter);
            if (ConcertCharacter && SelectedCharacterMesh)
            {
                if (ConcertCharacter->CharacterMesh)
                {
                    ConcertCharacter->CharacterMesh->SetStaticMesh(SelectedCharacterMesh);
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Spawned character does not have a valid CharacterMesh component!"));
                }
            }

            APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
            if (PlayerController)
            {
                PlayerController->Possess(SpawnedCharacter);
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to spawn character!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No character class selected to spawn!"));
    }
}

void AConcertGameMode::ActivateFireworkds()
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANiagaraActor::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        UE_LOG(LogTemp, Log, TEXT("Found Actor: %s"), *Actor->GetName());

        UNiagaraComponent* NiagaraComponent = Actor->FindComponentByClass<UNiagaraComponent>();
        if (NiagaraComponent)
        {
            UE_LOG(LogTemp, Log, TEXT("Activating Niagara Component: %s"), *NiagaraComponent->GetName());
            NiagaraComponent->Activate(true); // Activate the Niagara system
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Niagara Component not found on actor: %s"), *Actor->GetName());
        }
    }
}

void AConcertGameMode::ShowEndGameMenu()
{
    ActivateFireworkds();
    
    UHighScoreSaveGame* SaveGameInstance = Cast<UHighScoreSaveGame>(UGameplayStatics::LoadGameFromSlot("HighScoresSlot", 0));
    if (!SaveGameInstance)
    {
        SaveGameInstance = Cast<UHighScoreSaveGame>(UGameplayStatics::CreateSaveGameObject(UHighScoreSaveGame::StaticClass()));
    }

    FString CurrentLevelName = GetWorld()->GetMapName();
    CurrentLevelName.RemoveFromStart(TEXT("UEDPIE_0_"));

    float CurrentPercentage = GetCorrectNotePercentage();

    if (SaveGameInstance->LevelScores.Contains(CurrentLevelName))
    {
        FLevelScoreData& LevelData = SaveGameInstance->LevelScores[CurrentLevelName];
        if (Player1Score > LevelData.HighScore)
        {
            LevelData.HighScore = Player1Score;
        }
        if (CurrentPercentage > LevelData.SuccessPercentage)
        {
            LevelData.SuccessPercentage = CurrentPercentage;
        }
    }
    else
    {
        FLevelScoreData NewLevelData;
        NewLevelData.HighScore = Player1Score;
        NewLevelData.SuccessPercentage = CurrentPercentage;
        SaveGameInstance->LevelScores.Add(CurrentLevelName, NewLevelData);
    }

    UGameplayStatics::SaveGameToSlot(SaveGameInstance, "HighScoresSlot", 0);

    // Display the end game menu as before
    if (EndGameMenuClass)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PlayerController)
        {
            UEndGameMenu* EndGameMenu = CreateWidget<UEndGameMenu>(PlayerController, EndGameMenuClass);
            if (EndGameMenu)
            {
                EndGameMenu->AddToViewport();

                // Set the final score and star rating
                EndGameMenu->SetFinalScore(GetFinalScore());
                EndGameMenu->SetStarsBasedOnPercentage(GetCorrectNotePercentage());

                // Display the high score and percentage
                if (SaveGameInstance->LevelScores.Contains(CurrentLevelName))
                {
                    FLevelScoreData LevelData = SaveGameInstance->LevelScores[CurrentLevelName];
                    EndGameMenu->SetHighScore(LevelData.HighScore);
                    //EndGameMenu->SetStarsBasedOnHighestPercentage(LevelData.SuccessPercentage);
                }

                // Set input mode to UI only and show the mouse cursor
                FInputModeUIOnly InputMode;
                InputMode.SetWidgetToFocus(EndGameMenu->TakeWidget());
                PlayerController->SetInputMode(InputMode);
                PlayerController->bShowMouseCursor = true;
            }
        }
    }
}
