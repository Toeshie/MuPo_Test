// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ListEntryWidget.generated.h"

UCLASS()
class D3_PROJECT_MUPO_API UListEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TimeMsText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NoteNumberText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TrackText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ActionText;
};