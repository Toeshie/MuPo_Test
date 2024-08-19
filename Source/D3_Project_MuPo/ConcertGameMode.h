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
	int32 CurrentStreak; 
	int32 ScoreMultiplier; 
	const int32 MaxMultiplier = 4; 
	const int32 StreakToIncreaseMultiplier = 10; 

	int32 TotalNotes; 
	int32 GoodHits; 
	int32 PerfectHits; 

	int32 TotalNotesInSong;

	
	void ScheduleEndGameMenu(float Delay);
	FTimerHandle TimerHandle;

public:
	AConcertGameMode();

	UFUNCTION(BlueprintCallable, Category = "Score")
	void UpdatePlayer1Score(int32 ScoreDelta);

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetPlayer1Score() const;

	void DisplayScore();

	void NoteHit(bool bIsCorrect, bool bIsPerfect);
	
	int32 GetFinalScore() const;
	float GetCorrectNotePercentage() const;

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
	TSubclassOf<UUserWidget> EndGameMenuClass;

	UFUNCTION()
	void HandleNoteSpawned();

	void LoadSongData();

	void ShowEndGameMenu(); 
	
	
};