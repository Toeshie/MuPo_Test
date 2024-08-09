// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "NoteBaseClass.generated.h"

UCLASS()
class D3_PROJECT_MUPO_API ANoteBaseClass : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notes")
	UStaticMeshComponent* NoteMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notes")
	USplineComponent* NoteSpline;

	UPROPERTY()
	UMaterialInstanceDynamic* MaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notes")
	float MoveSpeed;
	
	float TotalDistanceMoved = 0.0f;
	
public:	
	// Sets default values for this actor's properties
	ANoteBaseClass();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	UBoxComponent* PerfectNoteCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	UBoxComponent* GoodNoteCollider1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	UBoxComponent* GoodNoteCollider2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void MoveAlongSpline();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
