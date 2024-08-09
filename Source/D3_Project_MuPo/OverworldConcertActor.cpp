// Fill out your copyright notice in the Description page of Project Settings.

#include "OverworldConcertActor.h"
#include "D3_Project_MuPoCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "ConcertGameInstance.h"

// Sets default values
AOverworldConcertActor::AOverworldConcertActor()
{
    PrimaryActorTick.bCanEverTick = true;

    RootRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = RootRoot;

    ConcertLocationMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Concert Mesh"));
    ConcertLocationMesh->SetupAttachment(RootRoot);

    ConcertCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Concert Collider"));
    ConcertCollider->SetupAttachment(RootComponent);
    ConcertCollider->InitBoxExtent(FVector(50.f, 50.f, 50.f));
    
    ConcertCollider->OnComponentBeginOverlap.AddDynamic(this, &AOverworldConcertActor::OnBeginOverlap);
    ConcertCollider->OnComponentEndOverlap.AddDynamic(this, &AOverworldConcertActor::OnEndOverlap);
}

// Called when the game starts or when spawned
void AOverworldConcertActor::BeginPlay()
{
    Super::BeginPlay();
}

void AOverworldConcertActor::LoadLevel()
{
    if (LevelToLoad != NAME_None)
    {
        UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
        if (GameInstance)
        {
            const TArray<FNoteData>& NotesData = GameInstance->GetSongDataForLevel(LevelToLoad);
            // You can now use NotesData in your level to set up the song

            // Proceed to load the level
            UGameplayStatics::OpenLevel(this, LevelToLoad);
        }
    }
}

void AOverworldConcertActor::ShowWidget()
{
    if (WidgetClass && !WidgetInstance)
    {
        WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
        if (WidgetInstance)
        {
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

void AOverworldConcertActor::DismissWidget()
{
    if (WidgetInstance)
    {
        WidgetInstance->RemoveFromParent();
        WidgetInstance = nullptr;
        
        if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
        {
            PlayerController->bShowMouseCursor = false;
            PlayerController->bEnableClickEvents = false;
            PlayerController->bEnableMouseOverEvents = false;
        }
    }
}

// Called every frame
void AOverworldConcertActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AOverworldConcertActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && OtherComp)
    {
        if (AD3_Project_MuPoCharacter* PlayerCharacter = Cast<AD3_Project_MuPoCharacter>(OtherActor))
        {
            //UE_LOG(LogTemp, Warning, TEXT("IN"));
            LoadLevel();
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
            //UE_LOG(LogTemp, Warning, TEXT("OUT"));
            //DismissWidget();
        }
    }
}

