// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class D3_PROJECT_MUPO_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void TogglePauseMenu();

	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void RestartLevel();

private:
	bool bIsPaused;
};
