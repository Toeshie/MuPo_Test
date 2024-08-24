// Fill out your copyright notice in the Description page of Project Settings.

#include "ConcertSelectionWidget.h"
#include "CharacterSelectionWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "ConcertGameInstance.h"
#include "OverworldConcertActor.h"

void UConcertSelectionWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UIManager = NewObject<UUIGameManager>(this);
    
    if (ConfirmButton)
    {
        ConfirmButton->OnClicked.AddDynamic(this, &UConcertSelectionWidget::OnConfirmButtonClicked);
    }
    ConfirmButton->SetKeyboardFocus();
    LoadCharacterSelectionWidgetClass();

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (PlayerController)
    {
        PlayerController->bShowMouseCursor = true;
        PlayerController->bEnableClickEvents = true;
        PlayerController->bEnableMouseOverEvents = true;
        PlayerController->SetInputMode(FInputModeGameAndUI());
    }
}

void UConcertSelectionWidget::InitializeWidget(const FString& SongName, const FString& LevelName, int32 BestStars)
{
    UE_LOG(LogTemp, Warning, TEXT("InitializeWidget called with SongName: %s, LevelName: %s, BestStars: %d"), *SongName, *LevelName, BestStars);
    
        if (SongNameTextBlock)
        {
            SongNameTextBlock->SetText(FText::FromString(SongName));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("SongNameTextBlock is nullptr"));
        }
    
        if (LevelNameTextBlock)
        {
            LevelNameTextBlock->SetText(FText::FromString(LevelName));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("LevelNameTextBlock is nullptr"));
        }
    
        UpdateStars(BestStars);  // Ensure this is called
}

void UConcertSelectionWidget::UpdateStars(int32 BestStars)
{
    if (!StarBox)
    {
        UE_LOG(LogTemp, Warning, TEXT("StarBox is nullptr!"));
        return;
    }

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

void UConcertSelectionWidget::OnConfirmButtonClicked()
{
    if (UIManager)
    {
        // Show the character selection widget when the confirm button is clicked
        UIManager->LoadCharacterSelectionWidget();

        // Re-enable input handling for the new widget
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
        if (PlayerController)
        {
            PlayerController->bShowMouseCursor = true;
            PlayerController->bEnableClickEvents = true;
            PlayerController->bEnableMouseOverEvents = true;
            PlayerController->SetInputMode(FInputModeGameAndUI());
        }

        // Remove the current widget from the viewport
        this->RemoveFromParent();
    }
}

void UConcertSelectionWidget::LoadCharacterSelectionWidgetClass()
{
    FString WidgetPath = TEXT("/Game/Blueprints/UI/CharacterSelectionWDG.CharacterSelectionWDG_C");

    // Load the widget class at runtime
    CharacterSelectionWidgetClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr, *WidgetPath);

    if (!CharacterSelectionWidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load CharacterSelectionWidget class at path: %s"), *WidgetPath);
    }
}