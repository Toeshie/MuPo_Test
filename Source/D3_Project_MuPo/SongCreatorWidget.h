// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SongCreatorWidget.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCSVEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 TimeMs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 NoteNumber;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Track;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Action;
};

UCLASS()
class D3_PROJECT_MUPO_API UCSVWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* TimeMsTextBox;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* NoteNumberTextBox;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* TrackTextBox;

	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* ActionComboBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* AddUpdateButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExportButton;

	UPROPERTY(meta = (BindWidget))
	class UListView* EntriesListView;

private:
	TArray<FCSVEntry> CSVEntries;

	UFUNCTION()
	void OnAddUpdateEntry();

	UFUNCTION()
	void OnExportCSV();

	void UpdateDropdown();
	void UpdateVisualization();
	void ClearFields();
	void OnRowSelect();
};