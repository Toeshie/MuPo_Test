// Fill out your copyright notice in the Description page of Project Settings.


#include "ConcertGameInstance.h"
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
	AllSongsData.Empty(); // Clear previous data

	USongDataParserSubsystem* ParserSubsystem = GetSubsystem<USongDataParserSubsystem>();

	if (ParserSubsystem)
	{
		TMap<FString, FName> SongFiles = {
			{TEXT("D:/UnrealProjects/MuPo/MuPo/D3_Project_MuPo/CSV/Nobody, Not Even the RainCSV.csv"),TEXT("ConcertLocation_1")},
			{TEXT("D:/UnrealProjects/MuPo/MuPo/D3_Project_MuPo/CSV/ChankuraCSV.csv"), TEXT("ConcertLocation_2")}
		};

		for (const auto& SongFile : SongFiles)
		{
			UE_LOG(LogTemp, Warning, TEXT("Loading song data for file: %s, level: %s"), *SongFile.Key, *SongFile.Value.ToString());
			if (ParserSubsystem->ParseSongData(SongFile.Key))
			{
				FLevelSongData LevelSongData;
				LevelSongData.LevelName = SongFile.Value;
				LevelSongData.NotesData = ParserSubsystem->GetParsedNotesData();
				AllSongsData.Add(SongFile.Value, LevelSongData);

				UE_LOG(LogTemp, Warning, TEXT("Successfully loaded song data for level: %s, total notes: %d"), *SongFile.Value.ToString(), LevelSongData.NotesData.Num());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to parse song data for file: %s"), *SongFile.Key);
			}
		}
	}
}

const TArray<FNoteData>& UConcertGameInstance::GetSongDataForLevel(FName LevelName) const
{
	if (const FLevelSongData* LevelSongData = AllSongsData.Find(LevelName))
	{
		return LevelSongData->NotesData;
	}
	static TArray<FNoteData> EmptyArray;
	return EmptyArray;
}

float UConcertGameInstance::GetSongDuration(FName LevelName) const
{
	if (const FLevelSongData* LevelSongData = AllSongsData.Find(LevelName))
	{
		if (LevelSongData->NotesData.Num() > 0)
		{
			return LevelSongData->NotesData.Last().TimeMs / 1000.0f; // Convert milliseconds to seconds
		}
	}
	return 0.0f;
}
