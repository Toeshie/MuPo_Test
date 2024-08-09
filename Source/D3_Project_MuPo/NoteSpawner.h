// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "NoteSpawner.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoteSpawnedDelegate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class D3_PROJECT_MUPO_API UNoteSpawner : public USceneComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LowNoteValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HighNoteValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UClass* BP_DrumNoteHigh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UClass* BP_DrumNoteLow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings")
	FVector LocalOffsetHigh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings")
	FVector LocalOffsetLow;

	

public:
	UNoteSpawner();
	void ClearScheduledNotes();

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	UFUNCTION()
	void SpawnNoteBasedOnNoteData(const FNoteData& Note);

	void ScheduleNotes(const TArray<FNoteData>& InNotesData);

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FNoteSpawnedDelegate OnNoteSpawned;

	void SetNotesData(const TArray<FNoteData>& NewNotesData);

private:
	
	TArray<FTimerHandle> NoteTimerHandles;
	TArray<FNoteData> ScheduledNotes;
};
