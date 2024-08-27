// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundCue.h"
#include "MainConcertCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class D3_PROJECT_MUPO_API AMainConcertCameraActor : public ACameraActor
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Movement")
	float MovementAmplitude = 100.0f; // Adjust this value to control the width of the movement

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Movement")
	float MovementFrequency = 1.0f; // Adjust this value to control the speed of the movement

	float TimeSinceBegin = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music")
	float MusicStartTimer;
	
public:
	AMainConcertCameraActor();

	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY()
	AActor* OverlappingNote;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* NoteChecker;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMeshComponent* TestPlane;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnNoteBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnNoteEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Timer handle for delayed music start
	FTimerHandle MusicStartTimerHandle;

	// Function to play music
	UFUNCTION()
	void PlayMusic();

	// SoundCue to hold the reference to the music
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Music")
	USoundCue* MusicCue;
	
};
