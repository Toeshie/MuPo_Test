// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NoteData.h"
#include "HighScoreSaveGame.h"
#include "ConcertGameInstance.generated.h"

USTRUCT()
struct FLevelSongData
{
	GENERATED_BODY()

	TArray<FNoteData> NotesData;
};

UCLASS()
class D3_PROJECT_MUPO_API UConcertGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Scores")
	void SaveLevelScore(const FString& LevelName, int32 Score, float SuccessPercentage);

	UFUNCTION(BlueprintCallable, Category = "Scores")
	int32 GetBestStarsForLevel(const FString& LevelName) const;

	virtual void Init() override;
	void LoadAllSongData();
	float GetSongDuration(FName LevelName) const;
	void ResetAllHighScores();

	UFUNCTION(BlueprintCallable, Category = "Songs")
	const TArray<FNoteData>& GetSongDataForLevel(FName LevelName) const;

	const TArray<FNoteData>& GetConcertLocation1Data() const;
	const TArray<FNoteData>& GetConcertLocation2Data() const;
	const TArray<FNoteData>& GetConcertLocation3Data() const;

	UFUNCTION(BlueprintCallable, Category = "Songs")
	FString GetSongNameForLevel(FName LevelName) const;

	void SetSelectedSong(const FString& SongName);
	FString GetSelectedSong() const;
	TArray<FString> GetAvailableCustomSongs() const;
	TArray<FString> GetAvailableHiddenSongs() const;

private:
	FLevelSongData ConcertLocation1Data;
	FLevelSongData ConcertLocation2Data;
	FLevelSongData ConcertLocation3Data;
	FString SelectedSong;

	UPROPERTY()
	UHighScoreSaveGame* HighScoreSaveGame;

	FString SaveSlotName;
	uint32 UserIndex;

	void LoadHighScoreData();
};