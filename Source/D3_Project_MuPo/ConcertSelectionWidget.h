// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIGameManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "ConcertSelectionWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConfirmDelegate);

UCLASS()
class D3_PROJECT_MUPO_API UConcertSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UConcertSelectionWidget(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable)
	void InitializeWidget(const FString& SongName, const FString& LevelName, int32 BestStars);

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnConfirmDelegate OnConfirm;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TopTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BottomTextBlock;

	UPROPERTY(meta = (BindWidget))
	UButton* ConfirmButton;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* StarBox;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnConfirmButtonClicked();

private:
	void UpdateStars(int32 BestStars);
	
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> CharacterSelectionWidgetClass;

	void LoadCharacterSelectionWidgetClass();

	UUIGameManager* UIManager;

	UTexture2D* FilledStarTexture;
	UTexture2D* EmptyStarTexture;
};