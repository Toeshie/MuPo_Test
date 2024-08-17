// Fill out your copyright notice in the Description page of Project Settings.


#include "ConcertSelectionSongChoiceWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/Button.h"

void UConcertSelectionSongChoiceWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.AddDynamic(this, &UConcertSelectionSongChoiceWidget::OnPlayButtonClicked);
	}
}

void UConcertSelectionSongChoiceWidget::InitializeWidgetWithSongs(const FString& SongName, const TArray<FString>& CharacterOptions, const TArray<FString>& AvailableSongs)
{
	InitializeWidget(SongName, CharacterOptions);

	if (SongComboBox)
	{
		SongComboBox->ClearOptions();
		for (const FString& Song : AvailableSongs)
		{
			UE_LOG(LogTemp, Log, TEXT("Adding song to ComboBox: %s"), *Song);
			SongComboBox->AddOption(Song);
		}

		if (AvailableSongs.Num() > 0)
		{
			SongComboBox->SetSelectedOption(AvailableSongs[0]);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SongComboBox is null. Make sure it is correctly bound in the widget blueprint."));
	}
}

FString UConcertSelectionSongChoiceWidget::GetSelectedSong() const
{
	if (SongComboBox)
	{
		return SongComboBox->GetSelectedOption();
	}

	return FString();
}

void UConcertSelectionSongChoiceWidget::OnPlayButtonClicked()
{
	FString SelectedSong = GetSelectedSong();
	FString SelectedCharacter = GetSelectedCharacter();

	OnSongChosen.Broadcast(SelectedSong, SelectedCharacter);
}
