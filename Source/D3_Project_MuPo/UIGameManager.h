// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UIGameManager.generated.h"

/**
 * 
 */
UCLASS()
class D3_PROJECT_MUPO_API UUIGameManager : public UObject
{
	GENERATED_BODY()
	
public:

	UUIGameManager();
	
	// Function to handle level loading
	UFUNCTION(BlueprintCallable)
	void LoadLevel(const FName& LevelName);

	// Function to set selected character
	UFUNCTION(BlueprintCallable)
	void SetSelectedCharacter(int32 CharacterIndex);

	// Function to set selected instrument
	UFUNCTION(BlueprintCallable)
	void SetSelectedInstrument(int32 InstrumentIndex);

	UFUNCTION(BlueprintCallable)
	void LoadCharacterSelectionWidget();
	
private:
	
	TSubclassOf<class UCharacterSelectionWidget> CharacterSelectionWidgetClass;
};
