// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HighScoreSaveGame.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FLevelScoreData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Scores")
	int32 HighScore;

	UPROPERTY(VisibleAnywhere, Category = "Scores")
	float SuccessPercentage;

	FLevelScoreData()
		: HighScore(0), SuccessPercentage(0.0f) {}
};

UCLASS()
class D3_PROJECT_MUPO_API UHighScoreSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// A map to store score data for each level by level name
	UPROPERTY(VisibleAnywhere, Category = "Scores")
	TMap<FString, FLevelScoreData> LevelScores;

	UHighScoreSaveGame();
};