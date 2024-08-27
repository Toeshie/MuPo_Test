// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SongDataEntry.generated.h"

/**
 * 
 */
UCLASS()
class D3_PROJECT_MUPO_API USongDataEntry : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 TimeMs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 NoteNumber;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Track;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Action;
};
