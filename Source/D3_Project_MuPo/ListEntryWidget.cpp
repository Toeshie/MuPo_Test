// Fill out your copyright notice in the Description page of Project Settings.


#include "ListEntryWidget.h"
#include "SongDataEntry.h"
#include "Components/TextBlock.h"

void UListEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	USongDataEntry* DataEntry = Cast<USongDataEntry>(ListItemObject);
	if (DataEntry)
	{
		TimeMsText->SetText(FText::AsNumber(DataEntry->TimeMs));

		
		FString NoteDisplay = (DataEntry->NoteNumber == 35) ? TEXT("HighNote") : TEXT("LowNote");
		NoteNumberText->SetText(FText::FromString(NoteDisplay));

		
		FString TrackDisplay = (DataEntry->Track == 0) ? TEXT("Drum") : TEXT("Marimba");
		TrackText->SetText(FText::FromString(TrackDisplay));

		ActionText->SetText(FText::FromString(DataEntry->Action));
		
	}
}