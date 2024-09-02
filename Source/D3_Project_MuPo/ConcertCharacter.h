// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "MainConcertCameraActor.h"
#include "NiagaraComponent.h"
#include "Sound/SoundCue.h"
#include "ConcertCharacter.generated.h"

UCLASS()
class D3_PROJECT_MUPO_API AConcertCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AConcertCharacter();

	virtual void InitializeInputMappings();

	UPROPERTY()
	UStaticMeshComponent* CharacterMesh;
	UPROPERTY()
	UStaticMeshComponent* InstrumentMesh;

	virtual void SetCharacterMesh(UStaticMesh* NewMesh);
	virtual void SetInstrumentMesh(UStaticMesh* NewMesh);

protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void HandleHighNoteInput(const FInputActionValue& Value);
	virtual void HandleLowNoteInput(const FInputActionValue& Value);
	virtual void ToggleProxyMenuPause();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UPauseMenuWidget> PauseMenuClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UI")
	class UPauseMenuWidget* PauseMenuWidgetInstance;

	UPROPERTY()
	TObjectPtr<AMainConcertCameraActor> ConcertCameraActor;

	
	virtual void ValidateNoteHit(const FInputActionValue& Value, bool bIsHighNote);
	virtual void PlaySound(USoundCue* SoundCue);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	UNiagaraComponent* Hit_NGS;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	UNiagaraComponent* Miss_NGS;


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

	
	
};

