#pragma once

#include "CoreMinimal.h"
#include "NoteData.generated.h"

USTRUCT(BlueprintType)
struct D3_PROJECT_MUPO_API FNoteData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 TimeMs = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 NoteNumber = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 Track = 0;

	UPROPERTY(BlueprintReadOnly)
	FString Action = TEXT("");

	bool IsValid() const
	{
		const int32 ValidNoteNumberMin = 0;
		const int32 ValidNoteNumberMax = 127;
		const int32 ValidTrackMin = 0;
		const int32 ValidTrackMax = 15;

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