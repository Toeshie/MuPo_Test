// Fill out your copyright notice in the Description page of Project Settings.

#include "ConcertGameInstance.h"
#include "NoteData.h"
#include "HighScoreSaveGame.h"
#include "SongDataParserSubsystem.h"
#include "Kismet/GameplayStatics.h"

DECLARE_LOG_CATEGORY_EXTERN(LogConcertGameInstance, Log, All);
DEFINE_LOG_CATEGORY(LogConcertGameInstance);

void UConcertGameInstance::SaveLevelScore(const FString& LevelName, int32 Score, float SuccessPercentage)
{
    if (HighScoreSaveGame)
    {
        FLevelScoreData* LevelScoreData = HighScoreSaveGame->LevelScores.Find(LevelName);
        if (LevelScoreData)
        {
            LevelScoreData->HighScore = FMath::Max(LevelScoreData->HighScore, Score);
            LevelScoreData->SuccessPercentage = FMath::Max(LevelScoreData->SuccessPercentage, SuccessPercentage);
        }
        else
        {
            FLevelScoreData NewScoreData;
            NewScoreData.HighScore = Score;
            NewScoreData.SuccessPercentage = SuccessPercentage;
            HighScoreSaveGame->LevelScores.Add(LevelName, NewScoreData);
        }
        UGameplayStatics::SaveGameToSlot(HighScoreSaveGame, SaveSlotName, UserIndex);
    }
}

int32 UConcertGameInstance::GetBestStarsForLevel(const FString& LevelName) const
{
    if (HighScoreSaveGame)
    {
        const FLevelScoreData* LevelScoreData = HighScoreSaveGame->LevelScores.Find(LevelName);
        if (LevelScoreData)
        {
            UE_LOG(LogTemp, Warning, TEXT("Retrieved Stars: %d for Level: %s"), LevelScoreData->GetStarRating(), *LevelName);
            return LevelScoreData->GetStarRating();
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("No data found for Level: %s, returning 0 stars"), *LevelName);
    return 0;
}

void UConcertGameInstance::Init()
{
    Super::Init();
    SaveSlotName = TEXT("HighScoreSlot");
    UserIndex = 0;

    LoadHighScoreData();
    LoadAllSongData();
}

void UConcertGameInstance::LoadAllSongData()
{
    USongDataParserSubsystem* ParserSubsystem = GetSubsystem<USongDataParserSubsystem>();

    if (ParserSubsystem)
    {
        if (ParserSubsystem->ParseSongData(TEXT("LagosCSV.csv")))
        {
            ConcertLocation1Data.NotesData = ParserSubsystem->GetParsedNotesData(); 
            UE_LOG(LogTemp, Warning, TEXT("Successfully loaded song data for ConcertLocation_1, total notes: %d"), ConcertLocation1Data.NotesData.Num());
        }
        
        ParserSubsystem->NotesData.Empty();
        
        if (ParserSubsystem->ParseSongData(TEXT("ChankuraCSV.csv")))
        {
            ConcertLocation2Data.NotesData = ParserSubsystem->GetParsedNotesData(); 
            UE_LOG(LogTemp, Warning, TEXT("Successfully loaded song data for ConcertLocation_2, total notes: %d"), ConcertLocation2Data.NotesData.Num());
        }
    }
}

float UConcertGameInstance::GetSongDuration(FName LevelName) const
{
    if (LevelName == "ConcertLocation_1")
    {
        if (ConcertLocation1Data.NotesData.Num() > 0)
        {
            return ConcertLocation1Data.NotesData.Last().TimeMs / 1000.0f;
        }
    }
    else if (LevelName == "ConcertLocation_2")
    {
        if (ConcertLocation2Data.NotesData.Num() > 0)
        {
            return ConcertLocation2Data.NotesData.Last().TimeMs / 1000.0f;
        }
    }
    return 0.0f;
}

void UConcertGameInstance::ResetAllHighScores()
{
    if (HighScoreSaveGame)
    {
        for (auto& LevelScorePair : HighScoreSaveGame->LevelScores)
        {
            LevelScorePair.Value.HighScore = 0;
            LevelScorePair.Value.SuccessPercentage = 0.0f;
        }

        UGameplayStatics::SaveGameToSlot(HighScoreSaveGame, SaveSlotName, UserIndex);
    }
}

const TArray<FNoteData>& UConcertGameInstance::GetSongDataForLevel(FName LevelName) const
{
    if (LevelName == "ConcertLocation_1")
    {
        return ConcertLocation1Data.NotesData;
    }
    else if (LevelName == "ConcertLocation_2")
    {
        return ConcertLocation2Data.NotesData;
    }

    static TArray<FNoteData> EmptyArray;
    return EmptyArray;
}

const TArray<FNoteData>& UConcertGameInstance::GetConcertLocation1Data() const
{
    return ConcertLocation1Data.NotesData;
}

const TArray<FNoteData>& UConcertGameInstance::GetConcertLocation2Data() const
{
    return ConcertLocation2Data.NotesData;
}

void UConcertGameInstance::SetSelectedCharacterMesh(UStaticMesh* Mesh)
{
    SelectedCharacterMesh = Mesh;
}

UStaticMesh* UConcertGameInstance::GetSelectedCharacterMesh() const
{
    return SelectedCharacterMesh;
}

FString UConcertGameInstance::GetSongNameForLevel(FName LevelName) const
{
    if (LevelName == "ConcertLocation_1")
    {
        return TEXT("Lagos");
    }
    else if (LevelName == "ConcertLocation_2")
    {
        return TEXT("Chankura");
    }
    
    return TEXT("Unknown Song");
}

void UConcertGameInstance::SetSelectedSong(const FString& SongName)
{
    SelectedSong = SongName; 
}

FString UConcertGameInstance::GetSelectedSong() const
{
    return SelectedSong;
}

TArray<FString> UConcertGameInstance::GetAvailableCustomSongs() const
{
    TArray<FString> AvailableSongs;
    FString Path = FPaths::ProjectContentDir();
    Path = FPaths::Combine(Path, TEXT("CustomSongs/"));

    IFileManager& FileManager = IFileManager::Get();
    FileManager.FindFiles(AvailableSongs, *Path, TEXT("*.csv"));

    return AvailableSongs;
}

TArray<FString> UConcertGameInstance::GetAvailableHiddenSongs() const
{
    TArray<FString> AvailableSongs;
    FString Path = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("HiddenSongs/"));

    IFileManager& FileManager = IFileManager::Get();
    FileManager.FindFiles(AvailableSongs, *Path, TEXT("*.csv"));

    return AvailableSongs;
}

void UConcertGameInstance::LoadHighScoreData()
{
    HighScoreSaveGame = Cast<UHighScoreSaveGame>(UGameplayStatics::LoadGameFromSlot("HighScoresSlot", 0));
    if (!HighScoreSaveGame)
    {
        HighScoreSaveGame = Cast<UHighScoreSaveGame>(UGameplayStatics::CreateSaveGameObject(UHighScoreSaveGame::StaticClass()));
    }
}
