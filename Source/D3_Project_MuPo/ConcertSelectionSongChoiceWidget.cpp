// Fill out your copyright notice in the Description page of Project Settings.


#include "ConcertSelectionSongChoiceWidget.h"
#include "ConcertGameInstance.h"
#include "Components/ComboBoxString.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UConcertSelectionSongChoiceWidget::NativeConstruct()
{
    Super::NativeConstruct();

    SongNameTextBlock->SetText(FText::FromString(TEXT("Concert of the Custom Songs!")));

    if (ConfirmButton)
    {
        ConfirmButton->OnClicked.AddDynamic(this, &UConcertSelectionSongChoiceWidget::OnPlayButtonClicked);
    }
}

void UConcertSelectionSongChoiceWidget::InitializeWidgetWithSongs(const FString& SongName, const TArray<FString>& CharacterOptions, const TArray<FString>& AvailableSongs, const FString& LevelName, int32 BestStars)
{
    // Call the base class InitializeWidget method with all necessary parameters
    InitializeWidget(SongName, CharacterOptions, LevelName, BestStars);

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

    UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (GameInstance)
    {
        // Log to verify the selected song
        UE_LOG(LogTemp, Log, TEXT("Selected song: %s"), *SelectedSong);
        GameInstance->SetSelectedSong(SelectedSong);
        GameInstance->SetSelectedCharacter(SelectedCharacter);
    }

    OnSongChosen.Broadcast(SelectedSong, SelectedCharacter);
}
