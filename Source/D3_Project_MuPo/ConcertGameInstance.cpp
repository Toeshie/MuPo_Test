// Fill out your copyright notice in the Description page of Project Settings.


#include "ConcertGameInstance.h"
#include "NoteData.h"
#include "SongDataParserSubsystem.h"

DECLARE_LOG_CATEGORY_EXTERN(LogConcertGameInstance, Log, All);
DEFINE_LOG_CATEGORY(LogConcertGameInstance);

void UConcertGameInstance::Init()
{
    Super::Init();
    UE_LOG(LogTemp, Warning, TEXT("Game Instance Initialized."));
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
        
        ParserSubsystem->NotesData.Empty();

       /*
        if (ParserSubsystem->ParseSongData(TEXT("DeadNeverStavDeadCSV.csv")))
        {
            ConcertLocation3Data.NotesData = ParserSubsystem->GetParsedNotesData();  
            UE_LOG(LogTemp, Warning, TEXT("Successfully loaded song data for ConcertLocation_3, total notes: %d"), ConcertLocation3Data.NotesData.Num());
        }
        */
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
    /*
    else if (LevelName == "ConcertLocation_CustomSongs")
    {
        if (ConcertLocation2Data.NotesData.Num() > 0)
        {
            return ConcertLocation2Data.NotesData.Last().TimeMs / 1000.0f;
        }
    }
    */
    return 0.0f;
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

const TArray<::FNoteData>& UConcertGameInstance::GetConcertLocation1Data() const
{
    return ConcertLocation1Data.NotesData;
}

const TArray<::FNoteData>& UConcertGameInstance::GetConcertLocation2Data() const
{
    return ConcertLocation2Data.NotesData;
}

const TArray<::FNoteData>& UConcertGameInstance::GetConcertLocation3Data() const
{
    return ConcertLocation3Data.NotesData;
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

void UConcertGameInstance::SetSelectedCharacter(const FString& CharacterName)
{
    SelectedCharacter = CharacterName;
}

FString UConcertGameInstance::GetSelectedCharacter() const
{
    return SelectedCharacter;
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

    // Ensure the path points directly to the CustomSongs folder inside the Content directory
    FString Path = FPaths::ProjectContentDir();  // Get the Content directory
    Path = FPaths::Combine(Path, TEXT("CustomSongs/"));  // Combine with CustomSongs subfolder

    IFileManager& FileManager = IFileManager::Get();
    FileManager.FindFiles(AvailableSongs, *Path, TEXT("*.csv"));

    // Log the path for debugging purposes
    UE_LOG(LogTemp, Log, TEXT("Looking for songs in directory: %s"), *Path);

    return AvailableSongs;
}

TArray<FString> UConcertGameInstance::GetAvailableHiddenSongs() const
{
    TArray<FString> AvailableSongs;
    // Correct path to the CustomSongs folder within the Content directory
    FString Path = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("HiddenSongs/"));

    IFileManager& FileManager = IFileManager::Get();
    FileManager.FindFiles(AvailableSongs, *Path, TEXT("*.csv"));

    return AvailableSongs;
}


