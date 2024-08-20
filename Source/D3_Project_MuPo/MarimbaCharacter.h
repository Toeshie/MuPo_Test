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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Initialize input mappings specifically for MarimbaCharacter
	virtual void InitializeInputMappings() override;
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

private:
	// New input actions for MarimbaCharacter
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_MarimbaNoteHigh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_MarimbaNoteLow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_Pause;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> IMC_Marimba;
};