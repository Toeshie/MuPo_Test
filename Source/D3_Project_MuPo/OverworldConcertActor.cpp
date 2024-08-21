// Fill out your copyright notice in the Description page of Project Settings.

#include "OverworldConcertActor.h"
#include "ConcertSelectionWidget.h"
#include "D3_Project_MuPoCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ConcertGameInstance.h"
#include "ConcertSelectionSongChoiceWidget.h"

AOverworldConcertActor::AOverworldConcertActor()
{
    PrimaryActorTick.bCanEverTick = true;

    BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
    RootComponent = BuildingMesh;  // Set the mesh as the root

    ConcertCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ConcertCollider"));
    ConcertCollider->SetupAttachment(BuildingMesh);
    ConcertCollider->InitBoxExtent(FVector(50.f, 50.f, 50.f));
    ConcertCollider->SetCollisionProfileName(TEXT("Trigger"));

    ConcertCollider->OnComponentBeginOverlap.AddDynamic(this, &AOverworldConcertActor::OnBeginOverlap);
    ConcertCollider->OnComponentEndOverlap.AddDynamic(this, &AOverworldConcertActor::OnEndOverlap);
}

void AOverworldConcertActor::BeginPlay()
{
    Super::BeginPlay();
}

void AOverworldConcertActor::ShowWidget()
{
    if (!WidgetClass || WidgetInstance) return;

    UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (!GameInstance) return;

    FString SongName = GameInstance->GetSongNameForLevel(LevelToLoad);
    FString LevelName;
    int32 BestStars = GameInstance->GetBestStarsForLevel(LevelToLoad.ToString());

    if (LevelToLoad == "ConcertLocation_CustomSongs")
    {
        ShowCustomSongSelectionWidget(GameInstance, SongName);
    }
    else
    {
        LevelName = (LevelToLoad == "ConcertLocation_1") ? TEXT("Alentejo") : TEXT("Porto");
        ShowStandardWidget(SongName, LevelName, BestStars);
    }
}

void AOverworldConcertActor::ShowStandardWidget(const FString& SongName, const FString& LevelName, int32 BestStars)
{
    WidgetInstance = CreateWidget<UConcertSelectionWidget>(GetWorld(), WidgetClass);
    if (!WidgetInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create widget instance."));
        return;
    }

    WidgetInstance->InitializeWidget(SongName, TArray<FString>({ TEXT("Concert Character") }), LevelName, BestStars);
    WidgetInstance->OnConfirm.AddDynamic(this, &AOverworldConcertActor::LoadLevel);
    WidgetInstance->AddToViewport();

    EnablePlayerInteraction();
}

void AOverworldConcertActor::ShowCustomSongSelectionWidget(UConcertGameInstance* GameInstance, const FString& SongName)
{
    WidgetInstance = CreateWidget<UConcertSelectionSongChoiceWidget>(GetWorld(), WidgetClass);
    if (!WidgetInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create custom song selection widget instance."));
        return;
    }

    TArray<FString> AvailableSongs = GameInstance->GetAvailableCustomSongs();
    FString LevelName = TEXT("CustomSongs");
    int32 BestStars = GameInstance->GetBestStarsForLevel(LevelName);

    Cast<UConcertSelectionSongChoiceWidget>(WidgetInstance)->InitializeWidgetWithSongs(SongName, { TEXT("Concert Character") }, AvailableSongs, LevelName, BestStars);
    WidgetInstance->AddToViewport();

    EnablePlayerInteraction();
}

void AOverworldConcertActor::EnablePlayerInteraction()
{
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        PlayerController->bShowMouseCursor = true;
        PlayerController->bEnableClickEvents = true;
        PlayerController->bEnableMouseOverEvents = true;
    }
}

void AOverworldConcertActor::LoadLevel()
{
    if (WidgetInstance)
    {
        UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
        if (GameInstance)
        {
            GameInstance->SetSelectedCharacter(WidgetInstance->GetSelectedCharacter());
        }

        WidgetInstance->RemoveFromParent();
        WidgetInstance = nullptr;

        if (LevelToLoad != NAME_None)
        {
            UGameplayStatics::OpenLevel(this, LevelToLoad);
        }
    }
}

void AOverworldConcertActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (Cast<AD3_Project_MuPoCharacter>(OtherActor))
    {
        ShowWidget();
    }
}

void AOverworldConcertActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (Cast<AD3_Project_MuPoCharacter>(OtherActor) && WidgetInstance)
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

void AOverworldConcertActor::OnSongChosen(const FString& SelectedSongName, const FString& SelectedCharacter)
{
    if (SelectedSongName.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("No song selected."));
        return;
    }

    UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (GameInstance)
    {
        GameInstance->SetSelectedSong(SelectedSongName);
        GameInstance->SetSelectedCharacter(SelectedCharacter);
    }

    UGameplayStatics::OpenLevel(this, FName("ConcertLocation_CustomSongs"));
}