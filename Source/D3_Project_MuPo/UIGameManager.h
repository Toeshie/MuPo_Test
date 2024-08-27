// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UIGameManager.generated.h"

class AOverworldConcertActor;  // Forward declaration
class UConcertSelectionWidget;

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

	// New method to update the widget based on the cached actor
	UFUNCTION(BlueprintCallable)
	void UpdateWidgetFromCachedActor();

	UFUNCTION()
	void OnCharacterSelected(int32 CharacterIndex, UTexture2D* SelectedCharacterImage);

	UFUNCTION()
	void OnInstrumentSelected(int32 InstrumentIndex);

	UFUNCTION()
	void CacheOverworldConcertActor(AOverworldConcertActor* OverworldConcertActor);

	void RemoveConcertSelectionWidget();

private:
	TSubclassOf<class UCharacterSelectionWidget> CharacterSelectionWidgetClass;
	TSubclassOf<class UInstrumentSelectionWidget> InstrumentSelectionWidgetClass;

	// Cached reference to the concert actor
	AOverworldConcertActor* CachedOverworldConcertActor;

	// Cached reference to the concert selection widget
	UConcertSelectionWidget* CachedConcertSelectionWidget;
};