// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"  
#include "Components/Image.h" 
#include "InstrumentSelectionWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInstrumentSelected, int32, InstrumentIndex);

UCLASS()
class D3_PROJECT_MUPO_API UInstrumentSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnInstrumentSelected OnInstrumentSelected;

	UFUNCTION(BlueprintCallable)
	void InitializeInstrumentSelectionWidget(UTexture2D* CharacterImage);

	void SetCachedLevelName(const FString& LevelName);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnConcertCharacterButtonClicked();

	UFUNCTION()
	void OnMarimbaCharacterButtonClicked();

private:
	UPROPERTY(meta = (BindWidget))
	UButton* ConcertCharacterButton;

	UPROPERTY(meta = (BindWidget))
	UButton* MarimbaCharacterButton;

	UPROPERTY(meta = (BindWidget))
	UImage* SelectedCharacterImage;


	FString CachedLevelName;
};