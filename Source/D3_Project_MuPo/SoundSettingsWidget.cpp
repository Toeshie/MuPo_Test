// Fill out your copyright notice in the Description page of Project Settings.

#include "SoundSettingsWidget.h"

#include "ConcertGameInstance.h"
#include "Components/Slider.h"
#include "Components/CheckBox.h"
#include "UIGameManager.h"
#include "Kismet/GameplayStatics.h"

void USoundSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GameInstance)
	{
		UIGameManagerInstance = GameInstance->GetUIGameManager();
	}

	if (MusicVolumeSlider)
	{
		MusicVolumeSlider->OnValueChanged.AddDynamic(this, &USoundSettingsWidget::OnMusicVolumeChanged);
	}

	if (FXVolumeSlider)
	{
		FXVolumeSlider->OnValueChanged.AddDynamic(this, &USoundSettingsWidget::OnFXVolumeChanged);
	}

	if (AmbientVolumeSlider)
	{
		AmbientVolumeSlider->OnValueChanged.AddDynamic(this, &USoundSettingsWidget::OnAmbientVolumeChanged);
	}

	if (MuteCheckBox)
	{
		MuteCheckBox->OnCheckStateChanged.AddDynamic(this, &USoundSettingsWidget::OnMuteStateChanged);
	}
}

void USoundSettingsWidget::OnMusicVolumeChanged(float Value)
{
	if (UIGameManagerInstance)
	{
		UIGameManagerInstance->SetMusicVolume(Value);
		UIGameManagerInstance->SaveSoundSettings();
	}
}

void USoundSettingsWidget::OnFXVolumeChanged(float Value)
{
	if (UIGameManagerInstance)
	{
		UIGameManagerInstance->SetFXVolume(Value);
		UIGameManagerInstance->SaveSoundSettings();
	}
}

void USoundSettingsWidget::OnAmbientVolumeChanged(float Value)
{
	if (UIGameManagerInstance)
	{
		UIGameManagerInstance->SetAmbientVolume(Value);
		UIGameManagerInstance->SaveSoundSettings();
	}
}

void USoundSettingsWidget::OnMuteStateChanged(bool bIsChecked)
{
	if (UIGameManagerInstance)
	{
		UIGameManagerInstance->SetMuteState(bIsChecked);
		UIGameManagerInstance->SaveSoundSettings();
	}
}