// Fill out your copyright notice in the Description page of Project Settings.


#include "SongDataParserSubsystem.h"



bool USongDataParserSubsystem::ParseSongData(const FString& FilePath)

{
	TArray<FString> Lines;
	if (FFileHelper::LoadFileToStringArray(Lines, *FilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("File read successfully from: %s"), *FilePath);
		for (int32 i = 1; i < Lines.Num(); ++i) // Start at 1 to skip header
		{
			TArray<FString> Columns;
			Lines[i].ParseIntoArray(Columns, TEXT(","), true);

			UE_LOG(LogTemp, Warning, TEXT("Line %d: Found %d columns."), i, Columns.Num());

			if (Columns.Num() == 4) // Ensure there are exactly 4 columns
			{
				FNoteData NoteData;
				NoteData.TimeMs = FCString::Atoi(*Columns[0]);
				NoteData.NoteNumber = FCString::Atoi(*Columns[1]);
				NoteData.Track = FCString::Atoi(*Columns[2]);
				NoteData.Action = Columns[3];

				NotesData.Add(NoteData);
				UE_LOG(LogTemp, Warning, TEXT("Line %d: Note data added: %s"), i, *NoteData.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Line %d: Incorrect number of columns."), i);
			}
		}

		if (NotesData.Num() > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Success parsing data. Total notes added: %d"), NotesData.Num());
			return true; // Parsing successful
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No valid note data found in file: %s"), *FilePath);
			return false; // No valid data found
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Failed to read file: %s"), *FilePath);
	return false; // File could not be loaded or parsed
}
