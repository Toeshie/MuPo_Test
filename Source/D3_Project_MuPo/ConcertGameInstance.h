// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NoteData.h"
#include "HighScoreSaveGame.h"
#include "UIGameManager.h"
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
	virtual void Init() override;

	// Score Management
	UFUNCTION(BlueprintCallable, Category = "Scores")
	void SaveLevelScore(const FString& LevelName, int32 Score, float SuccessPercentage);

	UFUNCTION(BlueprintCallable, Category = "Scores")
	int32 GetBestStarsForLevel(const FString& LevelName) const;

	void ResetAllHighScores();

	// Song Data Management
	UFUNCTION(BlueprintCallable, Category = "Songs")
	const TArray<FNoteData>& GetSongDataForLevel(FName LevelName) const;

	UFUNCTION(BlueprintCallable, Category = "Songs")
	float GetSongDuration(FName LevelName) const;

	UFUNCTION(BlueprintCallable, Category = "Songs")
	FString GetSongNameForLevel(FName LevelName) const;

	UFUNCTION(BlueprintCallable, Category = "Songs")
	TArray<FString> GetAvailableCustomSongs() const;

	UFUNCTION(BlueprintCallable, Category = "Songs")
	TArray<FString> GetAvailableHiddenSongs() const;
	
	FString GetSelectedSong() const;
	

	const TArray<FNoteData>& GetConcertLocation1Data() const;
	const TArray<FNoteData>& GetConcertLocation2Data() const;

	void SetSelectedSong(const FString& SongName);

	// Character and Instrument Selection
	void SetSelectedCharacterMesh(UStaticMesh* Mesh);
	UStaticMesh* GetSelectedCharacterMesh() const;

	void SetSelectedCharacter(int32 CharacterIndex);
	void SetSelectedInstrument(int32 InstrumentIndex);

	// Accessor for the UI Manager
	UUIGameManager* GetUIGameManager() const;

private:
	// High Score Data
	UPROPERTY()
	UHighScoreSaveGame* HighScoreSaveGame;

	void LoadHighScoreData();

	// Song Data
	void LoadAllSongData();
	FLevelSongData ConcertLocation1Data;
	FLevelSongData ConcertLocation2Data;
	

	// Selected States
	UPROPERTY()
	UStaticMesh* SelectedCharacterMesh;
	FString SelectedSong;
	int32 SelectedCharacterIndex;
	int32 SelectedInstrumentIndex;

	// UI Manager
	UPROPERTY()
	UUIGameManager* UIGameManager;

	// Save Game Management
	FString SaveSlotName = TEXT("HighScoreSlot");
	uint32 UserIndex = 0;
};