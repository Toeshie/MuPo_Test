// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "NoteData.h" 
#include "NoteSpawner.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoteSpawnedDelegate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class D3_PROJECT_MUPO_API UNoteSpawner : public USceneComponent
{
	GENERATED_BODY()

public:
	UNoteSpawner();

	TArray<FNoteData> NotesData;
	void SetNotesData(const TArray<FNoteData>& NewNotesData);
	void ClearScheduledNotes();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FNoteSpawnedDelegate OnNoteSpawned;

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

private:
	void HandleNoteSpawning();
	void SpawnNoteBasedOnNoteData(const FNoteData& Note);

	// Timer handle for spawning notes
	FTimerHandle NoteSpawnTimerHandle;

	// Current list of notes to be spawned
	TArray<FNoteData> CurrentNotesData;

	// Index of the next note to be spawned
	int32 CurrentNoteIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notes", meta = (AllowPrivateAccess = "true"))
	int32 LowNoteValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notes", meta = (AllowPrivateAccess = "true"))
	int32 HighNoteValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notes", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> BP_DrumNoteHigh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notes", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> BP_DrumNoteLow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings", meta = (AllowPrivateAccess = "true"))
	FVector LocalOffsetHigh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings", meta = (AllowPrivateAccess = "true"))
	FVector LocalOffsetLow;
};