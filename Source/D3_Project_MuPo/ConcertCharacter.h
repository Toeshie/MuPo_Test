// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "MainConcertCameraActor.h"
#include "Sound/SoundCue.h"
#include "ConcertCharacter.generated.h"

UCLASS()
class D3_PROJECT_MUPO_API AConcertCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AConcertCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ToggleProxyMenuPause();
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void InitializeInputMappings();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UPauseMenuWidget> PauseMenuClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UI")
	class UPauseMenuWidget* PauseMenuWidgetInstance;

private:
	void HandleHighNoteInput(const FInputActionValue& Value);
	void HandleLowNoteInput(const FInputActionValue& Value);
	void ValidateNoteHit(const FInputActionValue& Value, bool bIsHighNote);
	void PlaySound(USoundCue* SoundCue);

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* HighNoteHitSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* LowNoteHitSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* MissNoteSound;
	
	// Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_NoteHigh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_NoteLow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_Pause;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> IMC_Drums;

	UPROPERTY()
	TObjectPtr<AMainConcertCameraActor> ConcertCameraActor;
	
};

