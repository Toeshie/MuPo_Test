// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UIGameManager.generated.h"

UCLASS()
class D3_PROJECT_MUPO_API UUIGameManager : public UObject
{
	GENERATED_BODY()
    
public:
	UUIGameManager();
    
	UFUNCTION(BlueprintCallable)
	void LoadLevel(const FName& LevelName);

	UFUNCTION(BlueprintCallable)
	void SetSelectedCharacter(int32 CharacterIndex);

	UFUNCTION(BlueprintCallable)
	void SetSelectedInstrument(int32 InstrumentIndex);

	UFUNCTION(BlueprintCallable)
	void LoadCharacterSelectionWidget();

	UFUNCTION(BlueprintCallable)
	void LoadInstrumentSelectionWidget(UTexture2D* CharacterImage, class AOverworldConcertActor* OverworldConcertActor);

	// New function that handles instrument selection and level loading
	UFUNCTION()
	void OnInstrumentSelected(int32 InstrumentIndex);

private:
	TSubclassOf<class UCharacterSelectionWidget> CharacterSelectionWidgetClass;
	TSubclassOf<class UInstrumentSelectionWidget> InstrumentSelectionWidgetClass;

	AOverworldConcertActor* CachedOverworldConcertActor;  // Cache the actor for level loading
};