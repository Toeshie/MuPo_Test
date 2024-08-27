// Fill out your copyright notice in the Description page of Project Settings.


#include "SongDataParserSubsystem.h"



bool USongDataParserSubsystem::ParseSongData(const FString& FileName)
{
	NotesData.Empty();

	FString FullPath = FPaths::ProjectContentDir() + TEXT("SongData/") + FileName;

	TArray<FString> Lines;
	if (FFileHelper::LoadFileToStringArray(Lines, *FullPath))
	{
		UE_LOG(LogTemp, Warning, TEXT("File read successfully from: %s"), *FullPath);
		for (int32 i = 1; i < Lines.Num(); ++i)
		{
			TArray<FString> Columns;
			Lines[i].ParseIntoArray(Columns, TEXT(","), true);

			if (Columns.Num() == 4)
			{
				FNoteData NoteData;
				NoteData.TimeMs = FCString::Atoi(*Columns[0]);
				NoteData.NoteNumber = FCString::Atoi(*Columns[1]);
				NoteData.Track = FCString::Atoi(*Columns[2]);
				NoteData.Action = Columns[3];

				if (NoteData.IsValid())
				{
					NotesData.Add(NoteData);
					UE_LOG(LogTemp, Warning, TEXT("Note added: %s"), *NoteData.ToString());
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Invalid note data at line %d: %s"), i, *Lines[i]);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Incorrect number of columns at line %d"), i);
			}
		}

		if (NotesData.Num() > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Successfully parsed %d notes"), NotesData.Num());
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No valid notes found in file: %s"), *FullPath);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to read file: %s"), *FullPath);
	}

	return false;
}
