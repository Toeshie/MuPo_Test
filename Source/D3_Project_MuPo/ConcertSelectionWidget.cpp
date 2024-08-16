// Fill out your copyright notice in the Description page of Project Settings.


#include "ConcertSelectionWidget.h"
#include "Components/TextBlock.h"
#include "Components/ComboBoxString.h"
#include "Components/Button.h"

void UConcertSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.AddDynamic(this, &UConcertSelectionWidget::OnConfirmButtonClicked);
	}
}

void UConcertSelectionWidget::InitializeWidget(const FString& SongName, const TArray<FString>& CharacterOptions)
{
	if (SongNameTextBlock)
	{
		SongNameTextBlock->SetText(FText::FromString(SongName));
	}

	if (CharacterComboBox)
	{
		CharacterComboBox->ClearOptions();
		for (const FString& Option : CharacterOptions)
		{
			CharacterComboBox->AddOption(Option);
		}

		// Select the first option by default
		if (CharacterOptions.Num() > 0)
		{
			CharacterComboBox->SetSelectedOption(CharacterOptions[0]);
		}
	}
}

FString UConcertSelectionWidget::GetSelectedCharacter() const
{
	if (CharacterComboBox)
	{
		return CharacterComboBox->GetSelectedOption();
	}

	return FString();
}

void UConcertSelectionWidget::OnConfirmButtonClicked()
{
	OnConfirm.Broadcast();
}