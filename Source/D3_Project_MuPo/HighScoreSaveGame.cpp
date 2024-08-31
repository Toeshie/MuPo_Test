// Fill out your copyright notice in the Description page of Project Settings.


#include "HighScoreSaveGame.h"

UHighScoreSaveGame::UHighScoreSaveGame()
{
	MusicVolume = 1.0f;
	FXVolume = 1.0f;
	AmbientVolume = 1.0f;
	bIsMuted = false;
}

int32 FLevelScoreData::GetStarRating() const
{
	if (SuccessPercentage >= 80.0f) return 5;
	if (SuccessPercentage >= 60.0f) return 4;
	if (SuccessPercentage >= 40.0f) return 3;
	if (SuccessPercentage >= 20.0f) return 2;
	if (SuccessPercentage > 0.0f) return 1;
	return 0;
}

