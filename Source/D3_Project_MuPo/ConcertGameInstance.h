// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NoteData.h" 
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
	void LoadAllSongData();
	float GetSongDuration(FName LevelName) const;

	UFUNCTION(BlueprintCallable, Category = "Songs")
	const TArray<FNoteData>& GetSongDataForLevel(FName LevelName) const;

	const TArray<FNoteData>& GetConcertLocation1Data() const;
	const TArray<FNoteData>& GetConcertLocation2Data() const;
	const TArray<FNoteData>& GetConcertLocation3Data() const;

private:
	FLevelSongData ConcertLocation1Data;
	FLevelSongData ConcertLocation2Data;
	FLevelSongData ConcertLocation3Data;
};