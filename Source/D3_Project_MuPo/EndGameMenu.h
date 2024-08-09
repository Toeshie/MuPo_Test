// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndGameMenu.generated.h"

/**
 * 
 */
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

	UPROPERTY(meta = (BindWidget))
	class UButton* PlayAgainButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitToOverworldButton;
};