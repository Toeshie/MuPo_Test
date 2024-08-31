// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SoundSettingsWidget.generated.h"

/**
 * 
 */
UCLASS()
class D3_PROJECT_MUPO_API USoundSettingsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class USlider* MusicVolumeSlider;

	UPROPERTY(meta = (BindWidget))
	class USlider* FXVolumeSlider;

	UPROPERTY(meta = (BindWidget))
	class USlider* AmbientVolumeSlider;

	UPROPERTY(meta = (BindWidget))
	class UCheckBox* MuteCheckBox;

	UFUNCTION()
	void OnMusicVolumeChanged(float Value);

	UFUNCTION()
	void OnFXVolumeChanged(float Value);

	UFUNCTION()
	void OnAmbientVolumeChanged(float Value);

	UFUNCTION()
	void OnMuteStateChanged(bool bIsChecked);

	virtual void NativeConstruct() override;

private:
	class UUIGameManager* UIGameManagerInstance;
};
