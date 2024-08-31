// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConcertGameInstance.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "OverworldConcertActor.generated.h"

UCLASS()
class D3_PROJECT_MUPO_API AOverworldConcertActor : public AActor
{
	GENERATED_BODY()
    
public:    
	// Sets default values for this actor's properties
	AOverworldConcertActor();
	
	void LoadLevel();  // This method will load the level using LevelToLoad
    
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* ConcertCollider;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Level")
	FName LevelToLoad;


	
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	
	void OnSongChosen(const FString& SelectedSongName, const FString& SelectedCharacter);
	void EnablePlayerInteraction();
public:
	FString GetSongName();
	FString GetConcertName();
	int32 GetBestStars() const; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Level")
	FString ConcertName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Level")
	FString SongName;
};