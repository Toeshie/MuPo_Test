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

UConcertSelectionWidget::UConcertSelectionWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    static ConstructorHelpers::FObjectFinder<UTexture2D> FilledStarObj(TEXT("/Game/Blueprints/UI/cartoon_style_smley_face_star"));  
    if (FilledStarObj.Succeeded())
    {
        FilledStarTexture = FilledStarObj.Object;
    }

    static ConstructorHelpers::FObjectFinder<UTexture2D> EmptyStarObj(TEXT("/Game/Blueprints/UI/13595"));  
    if (EmptyStarObj.Succeeded())
    {
        EmptyStarTexture = EmptyStarObj.Object;
    }
}

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

void UConcertSelectionWidget::InitializeWidget(const FString& SongName, const FString& ConcertName, int32 BestStars)
{
    UE_LOG(LogTemp, Log, TEXT("InitializeWidget called with SongName: %s, ConcertName: %s"), *SongName, *ConcertName);

    if (TopTextBlock)
    {
        TopTextBlock->SetText(FText::FromString(ConcertName));
        UE_LOG(LogTemp, Log, TEXT("TopTextBlock updated with ConcertName: %s"), *ConcertName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("TopTextBlock is nullptr"));
    }

    if (BottomTextBlock)
    {
        BottomTextBlock->SetText(FText::FromString(SongName));
        UE_LOG(LogTemp, Log, TEXT("BottomTextBlock updated with SongName: %s"), *SongName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("BottomTextBlock is nullptr"));
    }

    UpdateStars(BestStars);  // Ensure this is called
}


void UConcertSelectionWidget::UpdateStars(int32 BestStars)
{
    if (!StarBox)
    {
        UE_LOG(LogTemp, Warning, TEXT("StarBox is null"));
        return;
    }

    StarBox->ClearChildren();

    const int32 MaxStars = 5;
    for (int32 i = 0; i < MaxStars; ++i)
    {
        UImage* StarImage = NewObject<UImage>(this);
        if (i < BestStars)
        {
            // Ensure StarImage is valid before applying the brush
            if (StarImage)
            {
                StarImage->SetBrushFromTexture(FilledStarTexture);
            }
        }
        else
        {
            if (StarImage)
            {
                StarImage->SetBrushFromTexture(EmptyStarTexture);
            }
        }
        StarBox->AddChild(StarImage);
    }
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