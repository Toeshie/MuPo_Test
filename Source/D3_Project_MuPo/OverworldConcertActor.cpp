// Fill out your copyright notice in the Description page of Project Settings.

#include "OverworldConcertActor.h"
#include "ConcertSelectionWidget.h"
#include "D3_Project_MuPoCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ConcertGameInstance.h"

// Sets default values
AOverworldConcertActor::AOverworldConcertActor()
{
    PrimaryActorTick.bCanEverTick = true;

    // Initialize the root component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    // Initialize the collider
    ConcertCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Concert Collider"));
    ConcertCollider->SetupAttachment(RootComponent);

    // Set default values for the collider, such as its extent
    ConcertCollider->InitBoxExtent(FVector(50.f, 50.f, 50.f));  // Example size
    ConcertCollider->SetCollisionProfileName(TEXT("Trigger"));  // Set the collision profile to Trigger (adjust as needed)

    // Attach overlap events (if necessary)
    ConcertCollider->OnComponentBeginOverlap.AddDynamic(this, &AOverworldConcertActor::OnBeginOverlap);
    ConcertCollider->OnComponentEndOverlap.AddDynamic(this, &AOverworldConcertActor::OnEndOverlap);
}

void AOverworldConcertActor::BeginPlay()
{
    Super::BeginPlay();
}

void AOverworldConcertActor::ShowWidget()
{
    if (WidgetClass && !WidgetInstance)
    {
        WidgetInstance = CreateWidget<UConcertSelectionWidget>(GetWorld(), WidgetClass);
        if (WidgetInstance)
        {
            UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
            FString SongName = GameInstance ? GameInstance->GetSongNameForLevel(LevelToLoad) : TEXT("Unknown Song");

            WidgetInstance->InitializeWidget(SongName, TArray<FString>({TEXT("Concert Character")}));
            WidgetInstance->OnConfirm.AddDynamic(this, &AOverworldConcertActor::LoadLevel);

            WidgetInstance->AddToViewport();

            if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
            {
                PlayerController->bShowMouseCursor = true;
                PlayerController->bEnableClickEvents = true;
                PlayerController->bEnableMouseOverEvents = true;
            }
        }
    }
}

void AOverworldConcertActor::LoadLevel()
{
    if (WidgetInstance)
    {
        FString SelectedCharacter = WidgetInstance->GetSelectedCharacter();
        UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
        if (GameInstance)
        {
            GameInstance->SetSelectedCharacter(SelectedCharacter);
        }

        // Remove the widget
        WidgetInstance->RemoveFromParent();
        WidgetInstance = nullptr;

        // Load the level
        if (LevelToLoad != NAME_None)
        {
            UGameplayStatics::OpenLevel(this, LevelToLoad);
        }
    }
}

void AOverworldConcertActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && OtherComp)
    {
        if (AD3_Project_MuPoCharacter* PlayerCharacter = Cast<AD3_Project_MuPoCharacter>(OtherActor))
        {
            ShowWidget();
        }
    }
}

void AOverworldConcertActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor != this && OtherComp)
    {
        if (AD3_Project_MuPoCharacter* PlayerCharacter = Cast<AD3_Project_MuPoCharacter>(OtherActor))
        {
            if (WidgetInstance)
            {
                WidgetInstance->RemoveFromParent();
                WidgetInstance = nullptr;

                APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
                if (PlayerController)
                {
                    PlayerController->bShowMouseCursor = false;
                    PlayerController->bEnableClickEvents = false;
                    PlayerController->bEnableMouseOverEvents = false;
                }
            }
        }
    }
}
