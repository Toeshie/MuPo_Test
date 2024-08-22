// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ConcertGameInstance.h"
#include "ConcertGameMode.generated.h"

UCLASS()
class D3_PROJECT_MUPO_API AConcertGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AConcertGameMode();

	// Score Management
	UFUNCTION(BlueprintCallable, Category = "Score")
	void UpdatePlayer1Score(int32 ScoreDelta);

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetPlayer1Score() const;

	void NoteHit(bool bIsCorrect, bool bIsPerfect);

	int32 GetFinalScore() const;
	float GetCorrectNotePercentage() const;

protected:
	virtual void BeginPlay() override;

private:
	// Player Score Data
	int32 Player1Score;
	int32 CurrentStreak;
	int32 ScoreMultiplier;
	const int32 MaxMultiplier = 4;
	const int32 StreakToIncreaseMultiplier = 10;

	int32 TotalNotes;
	int32 GoodHits;
	int32 PerfectHits;

	// Song Data
	UPROPERTY()
	class UNoteSpawner* NoteSpawner;

	UPROPERTY()
	class UConcertGameInstance* GameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music", meta = (AllowPrivateAccess = "true"))
	USoundWave* MusicWave;

	float SongDuration = 0.0f;
	int32 TotalNotesInSong;

	// End Game Menu
	FTimerHandle TimerHandle;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> EndGameMenuClass;

	void ScheduleEndGameMenu(float Delay);
	void ShowEndGameMenu();

	void DisplayScore();
	void LoadSongData();
	void HandleNoteSpawned();
};