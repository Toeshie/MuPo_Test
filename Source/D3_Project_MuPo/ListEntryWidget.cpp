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
		NoteNumberText->SetText(FText::AsNumber(DataEntry->NoteNumber));
		TrackText->SetText(FText::AsNumber(DataEntry->Track));
		ActionText->SetText(FText::FromString(DataEntry->Action));
	}
}