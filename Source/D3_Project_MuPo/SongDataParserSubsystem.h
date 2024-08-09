// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SongDataParserSubsystem.generated.h"


USTRUCT(BlueprintType)
struct FNoteData
{
	GENERATED_BODY()

	
	UPROPERTY(BlueprintReadOnly)
	int32 TimeMs = 0; // Time in milliseconds when the note action occurs, initialized to 0

	UPROPERTY(BlueprintReadOnly)
	int32 NoteNumber = 0; // Identifies the note, initialized to 0

	UPROPERTY(BlueprintReadOnly)
	int32 Track = 0; // Track number, if applicable, initialized to 0

	UPROPERTY(BlueprintReadOnly)
	FString Action = TEXT(""); // Action to be taken (e.g., "spawn", "despawn"), initialized to an empty string

	// Method to check if the struct has valid data
	bool IsValid() const
	{
		// Define valid ranges for NoteNumber and Track based on your application
		const int32 ValidNoteNumberMin = 0;
		const int32 ValidNoteNumberMax = 127; // MIDI standard range for note numbers
		const int32 ValidTrackMin = 0;
		const int32 ValidTrackMax = 15; // Example range for MIDI tracks

		return TimeMs >= 0 &&
			   NoteNumber >= ValidNoteNumberMin && NoteNumber <= ValidNoteNumberMax &&
			   Track >= ValidTrackMin && Track <= ValidTrackMax &&
			   !Action.IsEmpty();
	}

	FString ToString() const
	{
		return FString::Printf(TEXT("TimeMs: %d, NoteNumber: %d, Track: %d, Action: %s"), TimeMs, NoteNumber, Track, *Action);
	}
};

/**
 * 
 */

UCLASS()
class D3_PROJECT_MUPO_API USongDataParserSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:
	
	bool IsValid() const;
	
public:
	
	TArray<FNoteData> NotesData;
	
	const TArray<FNoteData>& GetParsedNotesData() const { return NotesData; }
	
	bool ParseSongData(const FString& FilePath);
};
