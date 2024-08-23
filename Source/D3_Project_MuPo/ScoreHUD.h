// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ScoreHUD.generated.h"

/**
 * 
 */
UCLASS()
class D3_PROJECT_MUPO_API AScoreHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	void SetScore(int32 NewScore);
	void SetStreak(int32 NewStreak);
	void SetMultiplier(int32 NewMultiplier);
	void SetGoodHits(int32 NewGoodHits); // New method to set good hits
	void SetPerfectHits(int32 NewPerfectHits); // New method to set perfect hits
	void SetTotalNotes(int32 NewTotalNotes); // New method to set total notes
	float CalculateHitPercentage() const; // New method to calculate the hit percentage
	void SetHitPercentage(float NewHitPercentage);

private:
	int32 Player1Score;
	int32 CurrentStreak;
	int32 ScoreMultiplier;
	int32 GoodHits; // New variable to store good hits
	int32 PerfectHits; // New variable to store perfect hits
	int32 TotalNotes; // New variable to store total notes
	FVector2D TextSize;
	float HitPercentage;

	
};
