// Fill out your copyright notice in the Description page of Project Settings.


#include "EndGameMenu.h"

#include "ConcertCharacter.h"
#include "OverworldCarCharacter.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


void UEndGameMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (PlayAgainButton)
	{
		PlayAgainButton->SetKeyboardFocus();
		PlayAgainButton->OnClicked.AddDynamic(this, &UEndGameMenu::OnPlayAgainClicked);
	}

	if (QuitToOverworldButton)
	{
		QuitToOverworldButton->OnClicked.AddDynamic(this, &UEndGameMenu::OnQuitToOverworldClicked);
	}
}

void UEndGameMenu::OnPlayAgainClicked()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(nullptr);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetName()), false);
}


void UEndGameMenu::OnQuitToOverworldClicked()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(nullptr);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("Overworld"), false);
}

void UEndGameMenu::SetupInputContexts()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AConcertCharacter* ConcertCharacter = Cast<AConcertCharacter>(PlayerController->GetPawn()))
		{
			ConcertCharacter->InitializeInputMappings();
		}
		else if (AOverworldCarCharacter* OverworldCarCharacter = Cast<AOverworldCarCharacter>(PlayerController->GetPawn()))
		{
			OverworldCarCharacter->InitializeInputMappings();
		}
	}
}

void UEndGameMenu::SetFinalScore(int32 Score)
{
	if (FinalScoreText)
	{
		FinalScoreText->SetText(FText::AsNumber(Score));
	}
}

void UEndGameMenu::SetStarsBasedOnPercentage(float Percentage)
{
	if (StarBox)
	{
		StarBox->ClearChildren();

		int32 StarCount = FMath::Clamp(FMath::FloorToInt(Percentage / 20.0f), 0, 5);

		for (int32 i = 0; i < StarCount; i++)
		{
			UImage* StarImage = NewObject<UImage>(StarBox);

			// Load your star texture
			UTexture2D* StarTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Blueprints/UI/cartoon_style_smley_face_star.cartoon_style_smley_face_star")); 

			if (StarTexture)
			{
				FSlateBrush Brush;
				Brush.SetResourceObject(StarTexture);
				StarImage->SetDesiredSizeOverride(FVector2D(64.0f, 64.0f)); 
				StarImage->SetBrush(Brush);
				
			}

			StarBox->AddChild(StarImage);
		}
	}
}
