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

void UConcertSelectionSongChoiceWidget::InitializeWidgetWithSongs(const FString& SongName, const TArray<FString>& AvailableSongs, const FString& LevelName, int32 BestStars)
{
    InitializeWidget(SongName, LevelName, BestStars);

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

    UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (GameInstance)
    {
        UE_LOG(LogTemp, Log, TEXT("Selected song: %s"), *SelectedSong);
        GameInstance->SetSelectedSong(SelectedSong);
    }

    OnSongChosen.Broadcast(SelectedSong);
}
