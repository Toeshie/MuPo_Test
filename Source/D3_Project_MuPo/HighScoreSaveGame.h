// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HighScoreSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FLevelScoreData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	int32 HighScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	float SuccessPercentage;
	
	int32 GetStarRating() const;
};


UCLASS()
class D3_PROJECT_MUPO_API UHighScoreSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Scores")
	TMap<FString, FLevelScoreData> LevelScores;

	UHighScoreSaveGame();

	// Sound settings
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	float MusicVolume;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	float FXVolume;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	float AmbientVolume;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	bool bIsMuted;
};