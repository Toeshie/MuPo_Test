// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ComboBoxString.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "ConcertSelectionWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConfirmDelegate);

UCLASS()
class D3_PROJECT_MUPO_API UConcertSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitializeWidget(const FString& SongName, const TArray<FString>& CharacterOptions, const FString& LevelName, int32 BestStars);

	UFUNCTION(BlueprintCallable)
	FString GetSelectedCharacter() const;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnConfirmDelegate OnConfirm;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SongNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LevelNameTextBlock;  // For displaying "Alentejo" or "Porto"

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CharacterComboBox;

	UPROPERTY(meta = (BindWidget))
	UButton* ConfirmButton;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* StarBox;  // Horizontal box to hold the stars

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnConfirmButtonClicked();

private:
	void UpdateStars(int32 BestStars);
};