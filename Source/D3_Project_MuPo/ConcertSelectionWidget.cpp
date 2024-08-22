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
    LoadCharacterSelectionWidgetClass();
}

void UConcertSelectionWidget::InitializeWidget(const FString& SongName, const FString& LevelName, int32 BestStars)
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

void UConcertSelectionWidget::OnConfirmButtonClicked()
{
    if (UIManager)
    {
        // Use the UIManager to load the character selection widget
        UIManager->LoadCharacterSelectionWidget();
    }
}

void UConcertSelectionWidget::ShowCharacterSelectionWidget()
{
    if (CharacterSelectionWidgetClass)
    {
        UCharacterSelectionWidget* CharacterSelectionWidget = CreateWidget<UCharacterSelectionWidget>(GetWorld(), CharacterSelectionWidgetClass);
        if (CharacterSelectionWidget)
        {
            CharacterSelectionWidget->AddToViewport();
            this->RemoveFromParent();  // Remove the ConcertSelectionWidget from the viewport

            // Optionally, bind the delegate if you want to handle character selection
            CharacterSelectionWidget->OnCharacterSelected.AddDynamic(this, &UConcertSelectionWidget::HandleCharacterSelected);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to create CharacterSelectionWidget instance"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("CharacterSelectionWidgetClass is not set"));
    }
}

void UConcertSelectionWidget::HandleCharacterSelected(int32 CharacterIndex)
{
    UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (GameInstance)
    {
        UStaticMesh* SelectedCharacterMesh = GameInstance->GetSelectedCharacterMesh();

        // This is where you can trigger the level loading after the character is selected
        AOverworldConcertActor* OverworldConcertActor = Cast<AOverworldConcertActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AOverworldConcertActor::StaticClass()));
        if (OverworldConcertActor)
        {
            OverworldConcertActor->LoadLevel();  // Trigger the level loading
        }
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