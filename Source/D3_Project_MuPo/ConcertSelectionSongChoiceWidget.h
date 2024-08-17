// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConcertSelectionWidget.h"
#include "ConcertSelectionSongChoiceWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSongChosenDelegate, const FString&, SelectedSongName, const FString&, SelectedCharacter);

/**
 * 
 */

UCLASS()
class D3_PROJECT_MUPO_API UConcertSelectionSongChoiceWidget : public UConcertSelectionWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitializeWidgetWithSongs(const FString& SongName, const TArray<FString>& CharacterOptions, const TArray<FString>& AvailableSongs);

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnSongChosenDelegate OnSongChosen;

protected:
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* SongComboBox;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnPlayButtonClicked();

private:
	FString GetSelectedSong() const;
	
};