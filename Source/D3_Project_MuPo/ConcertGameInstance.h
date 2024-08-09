// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ConcertGameInstance.generated.h"

struct FNoteData;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FLevelSongData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName LevelName;

	UPROPERTY(BlueprintReadOnly)
	TArray<FNoteData> NotesData;
};

UCLASS()
class D3_PROJECT_MUPO_API UConcertGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	void SetDefaultSoundMix();

	UFUNCTION(BlueprintCallable, Category = "Songs")
	const TArray<FNoteData>& GetSongDataForLevel(FName LevelName) const;

	float GetSongDuration(FName LevelName) const;

private:
	void LoadAllSongData();
	TMap<FName, FLevelSongData> AllSongsData;
};
