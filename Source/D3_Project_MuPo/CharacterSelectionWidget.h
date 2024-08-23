// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "CharacterSelectionWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterSelected, int32, CharacterIndex);

UCLASS()
class D3_PROJECT_MUPO_API UCharacterSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCharacterSelected OnCharacterSelected;

protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnCharacter1ButtonClicked();
	UFUNCTION()
	void OnCharacter2ButtonClicked();
	UFUNCTION()
	void OnCharacter3ButtonClicked();
	UFUNCTION()
	void OnCharacter4ButtonClicked();

	UFUNCTION()
	void OnCharacterButtonClicked(int32 CharacterIndex);

private:
	void SetCharacterMesh(int32 CharacterIndex);

	UPROPERTY(meta = (BindWidget))
	UButton* CharacterButton1;

	UPROPERTY(meta = (BindWidget))
	UButton* CharacterButton2;

	UPROPERTY(meta = (BindWidget))
	UButton* CharacterButton3;

	UPROPERTY(meta = (BindWidget))
	UButton* CharacterButton4;

	UPROPERTY(meta = (BindWidget))
	UImage* CharacterImage1;

	UPROPERTY(meta = (BindWidget))
	UImage* CharacterImage2;

	UPROPERTY(meta = (BindWidget))
	UImage* CharacterImage3;

	UPROPERTY(meta = (BindWidget))
	UImage* CharacterImage4;

	UStaticMesh* CharacterMeshes[4]; 
};