// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "OverworldCarCharacter.generated.h"

UCLASS()
class D3_PROJECT_MUPO_API AOverworldCarCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AOverworldCarCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void InitializeInputMappings();

protected:
	void Move(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);

	

	FVector CurrentVelocity;
	float CurrentTurnRate;

	UPROPERTY(EditAnywhere)
	float MoveSpeed;

	UPROPERTY(EditAnywhere)
	float TurnSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* CarMesh;

	// Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_MoveForward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_Turn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> IMC_Car;
	
};