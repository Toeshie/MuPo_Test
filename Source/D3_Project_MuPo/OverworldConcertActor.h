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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Level")
	FName LevelToLoad;
    
protected:

	UPROPERTY(BlueprintReadOnly)
	UStaticMeshComponent* BuildingMesh;
	
	UPROPERTY(BlueprintReadWrite, Category = "Collider")
	UBoxComponent* ConcertCollider;

	UPROPERTY(BlueprintReadWrite, Category = "Root")
	USceneComponent* RootRoot;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UConcertSelectionWidget> WidgetClass;

	UPROPERTY()
	UConcertSelectionWidget* WidgetInstance;
	

	virtual void BeginPlay() override;

	void ShowWidget();
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	
	void OnSongChosen(const FString& SelectedSongName, const FString& SelectedCharacter);
	void ShowStandardWidget(const FString& SongName,const FString& LevelName, int32 BestStars);
	void ShowCustomSongSelectionWidget(UConcertGameInstance* GameInstance, const FString& SongName);
	void EnablePlayerInteraction();
};