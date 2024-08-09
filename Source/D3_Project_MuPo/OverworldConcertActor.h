// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
    
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* ConcertLocationMesh;

	UPROPERTY(BlueprintReadWrite, Category = "Collider")
	UBoxComponent* ConcertCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Root")
	USceneComponent* RootRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Level")
	FName LevelToLoad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY()
	UUserWidget* WidgetInstance;

	virtual void BeginPlay() override;

	void ShowWidget();
	void DismissWidget();
	void Tick(float DeltaTime);

	UFUNCTION()
	void LoadLevel();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};