// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SongDataEntry.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ListEntryWidget.generated.h"

UCLASS()
class D3_PROJECT_MUPO_API UListEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	// Implement this interface function to populate the widget based on the data object
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TimeMsText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NoteNumberText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TrackText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ActionText;
};