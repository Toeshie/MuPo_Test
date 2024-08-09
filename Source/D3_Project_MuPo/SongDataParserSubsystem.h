// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NoteData.h" 
#include "SongDataParserSubsystem.generated.h"

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