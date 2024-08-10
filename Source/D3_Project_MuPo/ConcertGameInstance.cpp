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

        // Load data for ConcertLocation_3
        /*if (ParserSubsystem->ParseSongData(TEXT("D:/MuPo_Test/MuPo_Test/CSV/DeadNeverStavDeadCSV.csv")))
        {
            ConcertLocation3Data.NotesData = ParserSubsystem->GetParsedNotesData();
            UE_LOG(LogTemp, Warning, TEXT("Successfully loaded song data for ConcertLocation_3, total notes: %d"), ConcertLocation3Data.NotesData.Num());
        }*/
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


