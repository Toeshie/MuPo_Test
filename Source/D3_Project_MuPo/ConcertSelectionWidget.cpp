// Fill out your copyright notice in the Description page of Project Settings.


#include "ConcertSelectionWidget.h"
#include "Components/TextBlock.h"
#include "Components/ComboBoxString.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Blueprint/WidgetTree.h"

void UConcertSelectionWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ConfirmButton)
    {
        ConfirmButton->OnClicked.AddDynamic(this, &UConcertSelectionWidget::OnConfirmButtonClicked);
    }
}

void UConcertSelectionWidget::InitializeWidget(const FString& SongName, const TArray<FString>& CharacterOptions, const FString& LevelName, int32 BestStars)
{
    if (SongNameTextBlock)
    {
        SongNameTextBlock->SetText(FText::FromString(SongName));
    }

    if (LevelNameTextBlock)
    {
        LevelNameTextBlock->SetText(FText::FromString(LevelName));
    }

    UpdateStars(BestStars);
}

void UConcertSelectionWidget::UpdateStars(int32 BestStars)
{
    if (!StarBox) return;

    StarBox->ClearChildren();

    const int32 MaxStars = 5;
    for (int32 i = 0; i < MaxStars; ++i)
    {
        UImage* StarImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
        if (i < BestStars)
        {
            UTexture2D* FilledStarTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Blueprints/UI/cartoon_style_smley_face_star.cartoon_style_smley_face_star"));
            StarImage->SetBrushFromTexture(FilledStarTexture);
        }
        else
        {
            UTexture2D* EmptyStarTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Blueprints/UI/13595.13595"));
            StarImage->SetBrushFromTexture(EmptyStarTexture);
        }
        StarImage->SetDesiredSizeOverride(FVector2D(64, 64));
        StarBox->AddChild(StarImage);
    }

    UE_LOG(LogTemp, Warning, TEXT("Updated Stars in Widget: %d"), BestStars);
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