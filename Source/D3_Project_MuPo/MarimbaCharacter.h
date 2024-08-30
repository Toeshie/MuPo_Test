#pragma once

#include "CoreMinimal.h"
#include "ConcertCharacter.h"
#include "MarimbaCharacter.generated.h"

UCLASS()
class D3_PROJECT_MUPO_API AMarimbaCharacter : public AConcertCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMarimbaCharacter();

protected:
	virtual void BeginPlay() override;

	// Initialize input mappings specifically for MarimbaCharacter
	virtual void InitializeInputMappings() override;

	// Override note input handlers and pause toggle to use inherited functionality
	virtual void HandleHighNoteInput(const FInputActionValue& Value) override;
	virtual void HandleLowNoteInput(const FInputActionValue& Value) override;
	virtual void ToggleProxyMenuPause() override;
	virtual void PlaySound(USoundCue* SoundCue) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetCharacterMesh(UStaticMesh* NewMesh);

private:
	// New input actions for MarimbaCharacter
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_MarimbaNoteHigh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_MarimbaNoteLow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> IMC_Marimba;

	// Marimba-specific sound cues
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* MarimbaHighNoteHitSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* MarimbaLowNoteHitSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* MarimbaMissNoteSound;
};