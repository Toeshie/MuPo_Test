// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ConcertGameMode.generated.h"

/**
 * 
 */
UCLASS()
class D3_PROJECT_MUPO_API AConcertGameMode : public AGameModeBase
{
	GENERATED_BODY()
    
	int32 Player1Score;
	int32 CurrentStreak; // To keep track of the current streak of correct notes
	int32 ScoreMultiplier; // To keep track of the current score multiplier
	const int32 MaxMultiplier = 4; // Maximum multiplier
	const int32 StreakToIncreaseMultiplier = 10; // Number of correct notes needed to increase the multiplier

	int32 TotalNotes; // Total number of notes spawned
	int32 GoodHits; // Number of good hits
	int32 PerfectHits; // Number of perfect hits

	int32 TotalNotesInSong; // Total number of notes in the current song

	
	void ScheduleEndGameMenu(float Delay);
	FTimerHandle TimerHandle;

public:
	AConcertGameMode();

	UFUNCTION(BlueprintCallable, Category = "Score")
	void UpdatePlayer1Score(int32 ScoreDelta);

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetPlayer1Score() const;

	void DisplayScore();

	void NoteHit(bool bIsCorrect, bool bIsPerfect); // New function to handle note hits with type

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Music")
	USoundWave* MusicWave;

	float SongDuration = 0.0f;

private:
	UPROPERTY()
	class UNoteSpawner* NoteSpawner;

	UPROPERTY()
	class UConcertGameInstance* GameInstance;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> EndGameMenuClass; // Widget class for the end game menu

	UFUNCTION()
	void HandleNoteSpawned();

	void LoadSongData();

	void ShowEndGameMenu(); // Function to display the end game menu
	
	
};