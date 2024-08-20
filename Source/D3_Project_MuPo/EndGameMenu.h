// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"          // Include for UButton
#include "Components/TextBlock.h"       // Include for UTextBlock
#include "Components/HorizontalBox.h"   // Include for UHorizontalBox
#include "EndGameMenu.generated.h"

UCLASS()
class D3_PROJECT_MUPO_API UEndGameMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnPlayAgainClicked();

	UFUNCTION()
	void OnQuitToOverworldClicked();
	void SetupInputContexts();

	// Properly declare the components with UPROPERTY and correct types
	UPROPERTY(meta = (BindWidget))
	UButton* PlayAgainButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitToOverworldButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* FinalScoreText;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* StarBox;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HighStarBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HighScoreText;


public:
	void SetFinalScore(int32 Score);
	void SetStarsBasedOnPercentage(float Percentage);
	void SetHighScore(int32 HighScore);
	void SetStarsBasedOnHighestPercentage(float Percentage);
};