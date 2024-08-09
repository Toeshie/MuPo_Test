// Fill out your copyright notice in the Description page of Project Settings.


#include "ConcertGameInstance.h"
#include "NoteData.h"
#include "SongDataParserSubsystem.h"
#include "Misc/DefaultValueHelper.h"

DECLARE_LOG_CATEGORY_EXTERN(LogConcertGameInstance, Log, All);
DEFINE_LOG_CATEGORY(LogConcertGameInstance);

void UConcertGameInstance::Init()
{
    Super::Init();
    
    FString LevelName = GetWorld()->GetMapName();
    if (LevelName.Contains("ConcertLocation_1"))
    {
        LoadSongDataFromCSV("D:/MuPo_Test/MuPo_Test/CSV/Nobody, Not Even the RainCSV.csv");
    }
    else if (LevelName.Contains("ConcertLocation_2"))
    {
        LoadSongDataFromCSV("D:/MuPo_Test/MuPo_Test/CSV/ChankuraCSV.csv");
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Unknown Level! No song data loaded."));
    }
}

void UConcertGameInstance::LoadAllSongData()
{
    USongDataParserSubsystem* ParserSubsystem = GetSubsystem<USongDataParserSubsystem>();

    // Clear existing data before loading new data
    ConcertLocation1Data.NotesData.Empty();
    ConcertLocation2Data.NotesData.Empty();
    ConcertLocation3Data.NotesData.Empty();

    if (ParserSubsystem)
    {
        // Load data for ConcertLocation_1
        if (ParserSubsystem->ParseSongData(TEXT("D:/MuPo_Test/MuPo_Test/CSV/Nobody, Not Even the RainCSV.csv")))
        {
            ConcertLocation1Data.NotesData = ParserSubsystem->GetParsedNotesData();
            UE_LOG(LogTemp, Warning, TEXT("Successfully loaded song data for ConcertLocation_1, total notes: %d"), ConcertLocation1Data.NotesData.Num());
        }

        // Load data for ConcertLocation_2
        if (ParserSubsystem->ParseSongData(TEXT("D:/MuPo_Test/MuPo_Test/CSV/ChankuraCSV.csv")))
        {
            ConcertLocation2Data.NotesData = ParserSubsystem->GetParsedNotesData();
            UE_LOG(LogTemp, Warning, TEXT("Successfully loaded song data for ConcertLocation_2, total notes: %d"), ConcertLocation2Data.NotesData.Num());
        }

        // If there's data for ConcertLocation_3 or other levels, load similarly
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
    else if (LevelName == "ConcertLocation_3")
    {
        if (ConcertLocation3Data.NotesData.Num() > 0)
        {
            return ConcertLocation3Data.NotesData.Last().TimeMs / 1000.0f;
        }
    }

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
    else if (LevelName == "ConcertLocation_3")
    {
        return ConcertLocation3Data.NotesData;
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

void UConcertGameInstance::LoadSongDataFromCSV(FString CSVFilePath)
{
    // Clear any previously loaded data to avoid overlapping
    ScheduledNotes.Empty();

    // Check if the file exists
    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*CSVFilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("File not found: %s"), *CSVFilePath);
        return;
    }

    FString FileData;

    // Load the file content into a string
    if (!FFileHelper::LoadFileToString(FileData, *CSVFilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load file: %s"), *CSVFilePath);
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("File read successfully from: %s"), *CSVFilePath);

    TArray<FString> Lines;
    FileData.ParseIntoArrayLines(Lines);

    for (int32 LineIndex = 0; LineIndex < Lines.Num(); LineIndex++)
    {
        const FString& Line = Lines[LineIndex];
        TArray<FString> Columns;

        Line.ParseIntoArray(Columns, TEXT(","), true);

        if (Columns.Num() != 4)
        {
            UE_LOG(LogTemp, Error, TEXT("Line %d: Invalid format. Found %d columns, expected 4."), LineIndex + 1, Columns.Num());
            continue;
        }

        FNoteData NewNote;
        if (!FDefaultValueHelper::ParseInt(Columns[0], NewNote.TimeMs) ||
            !FDefaultValueHelper::ParseInt(Columns[1], NewNote.NoteNumber) ||
            !FDefaultValueHelper::ParseInt(Columns[2], NewNote.Track) ||
            !Columns[3].Equals(TEXT("spawn"), ESearchCase::IgnoreCase))
        {
            UE_LOG(LogTemp, Error, TEXT("Line %d: Failed to parse note data."), LineIndex + 1);
            continue;
        }

        NewNote.Action = ENoteAction::Spawn;

        // Add the new note to the scheduled notes array
        ScheduledNotes.Add(NewNote);

        UE_LOG(LogTemp, Warning, TEXT("Line %d: Note data added: TimeMs: %d, NoteNumber: %d, Track: %d, Action: spawn"), 
            LineIndex + 1, NewNote.TimeMs, NewNote.NoteNumber, NewNote.Track);
    }

    UE_LOG(LogTemp, Warning, TEXT("Success parsing data. Total notes added: %d"), ScheduledNotes.Num());
}
