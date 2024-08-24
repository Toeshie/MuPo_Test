// Fill out your copyright notice in the Description page of Project Settings.

#include "ConcertGameInstance.h"
#include "SongDataParserSubsystem.h"
#include "UIGameManager.h"
#include "Kismet/GameplayStatics.h"

DECLARE_LOG_CATEGORY_EXTERN(LogConcertGameInstance, Log, All);
DEFINE_LOG_CATEGORY(LogConcertGameInstance);

void UConcertGameInstance::Init()
{
	Super::Init();
	
	SelectedCharacterIndex = -1;
	SelectedInstrumentIndex = -1;
	SelectedCharacterMesh = nullptr;
	
	UIGameManager = NewObject<UUIGameManager>(this);
	
	LoadHighScoreData();
	LoadAllSongData();
}

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
			return LevelScoreData->GetStarRating();
		}
	}
	return 0;
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

float UConcertGameInstance::GetSongDuration(FName LevelName) const
{
	const TArray<FNoteData>* NotesData = nullptr;

	if (LevelName == "ConcertLocation_1")
	{
		NotesData = &ConcertLocation1Data.NotesData;
	}
	else if (LevelName == "ConcertLocation_2")
	{
		NotesData = &ConcertLocation2Data.NotesData;
	}

	if (NotesData && NotesData->Num() > 0)
	{
		return NotesData->Last().TimeMs / 1000.0f;
	}

	return 0.0f;
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

TArray<FString> UConcertGameInstance::GetAvailableCustomSongs() const
{
	TArray<FString> AvailableSongs;
	FString Path = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("CustomSongs/"));

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

const TArray<FNoteData>& UConcertGameInstance::GetConcertLocation1Data() const
{
	return ConcertLocation1Data.NotesData;
}

const TArray<FNoteData>& UConcertGameInstance::GetConcertLocation2Data() const
{
	return ConcertLocation2Data.NotesData;
}

void UConcertGameInstance::SetSelectedSong(const FString& SongName)
{
	SelectedSong = SongName;
}



void UConcertGameInstance::SetSelectedCharacterMesh(UStaticMesh* Mesh)
{
	SelectedCharacterMesh = Mesh;
}

UStaticMesh* UConcertGameInstance::GetSelectedCharacterMesh() const
{
	return SelectedCharacterMesh;
}



void UConcertGameInstance::SetSelectedCharacter(int32 CharacterIndex)
{
	SelectedCharacterIndex = CharacterIndex;
}

int32 UConcertGameInstance::GetSelectedCharacter() const
{
	return SelectedCharacterIndex;
}



void UConcertGameInstance::SetSelectedInstrument(int32 InstrumentIndex)
{
	SelectedInstrumentIndex = InstrumentIndex;
}

int32 UConcertGameInstance::GetSelectedInstrument() const
{
	return SelectedInstrumentIndex;
}



UUIGameManager* UConcertGameInstance::GetUIGameManager() const
{
	return UIGameManager;
}

void UConcertGameInstance::LoadHighScoreData()
{
	HighScoreSaveGame = Cast<UHighScoreSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));
	if (!HighScoreSaveGame)
	{
		HighScoreSaveGame = Cast<UHighScoreSaveGame>(UGameplayStatics::CreateSaveGameObject(UHighScoreSaveGame::StaticClass()));
	}
}

void UConcertGameInstance::LoadAllSongData()
{
	USongDataParserSubsystem* ParserSubsystem = GetSubsystem<USongDataParserSubsystem>();

	if (ParserSubsystem)
	{
		if (ParserSubsystem->ParseSongData(TEXT("LagosCSV.csv")))
		{
			ConcertLocation1Data.NotesData = ParserSubsystem->GetParsedNotesData();
		}
		ParserSubsystem->NotesData.Empty();

		if (ParserSubsystem->ParseSongData(TEXT("ChankuraCSV.csv")))
		{
			ConcertLocation2Data.NotesData = ParserSubsystem->GetParsedNotesData();
		}
	}
}