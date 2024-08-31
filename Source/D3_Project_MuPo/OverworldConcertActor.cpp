// Fill out your copyright notice in the Description page of Project Settings.

#include "OverworldConcertActor.h"
#include "HighScoreSaveGame.h"
#include "D3_Project_MuPoCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ConcertGameInstance.h"

AOverworldConcertActor::AOverworldConcertActor()
{
    PrimaryActorTick.bCanEverTick = true;
    
    ConcertCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ConcertCollider"));
    RootComponent = ConcertCollider;

    
    ConcertCollider->SetBoxExtent(FVector(1000.0f, 1000.0f, 500.0f)); 

    
    ConcertCollider->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

    ConcertCollider->OnComponentBeginOverlap.AddDynamic(this, &AOverworldConcertActor::OnBeginOverlap);
    ConcertCollider->OnComponentEndOverlap.AddDynamic(this, &AOverworldConcertActor::OnEndOverlap);
    
}

void AOverworldConcertActor::BeginPlay()
{
    Super::BeginPlay();
    
    FString ActorName = GetName();
    UE_LOG(LogTemp, Log, TEXT("BeginPlay - ActorName: %s"), *ActorName);
    
    if (ActorName.Contains("BP_OverworldConcertLocation_C_2"))
    {
        SongName = "Lagos";
        ConcertName = "Alentejo";
    }
    else if (ActorName.Contains("BP_OverworldConcertLocation_C_0"))
    {
        SongName = "Chankura";
        ConcertName = "Porto";
    }
    else
    {
        SongName = "Default Song";
        ConcertName = "Default Concert";
    }

    UE_LOG(LogTemp, Log, TEXT("BeginPlay - SongName: %s, ConcertName: %s"), *SongName, *ConcertName);
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

FString AOverworldConcertActor::GetSongName()
{
    UE_LOG(LogTemp, Log, TEXT("GetSongName called: %s"), *SongName);
    return SongName;
}

FString AOverworldConcertActor::GetConcertName()
{
    UE_LOG(LogTemp, Log, TEXT("GetConcertName called: %s"), *ConcertName);
    return ConcertName;
}

int32 AOverworldConcertActor::GetBestStars() const
{
    UHighScoreSaveGame* LoadGameInstance = Cast<UHighScoreSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("HighScoreSaveSlot"), 0));
    
    if (!LoadGameInstance) // If the save game doesn't exist, create a new one
    {
        LoadGameInstance = Cast<UHighScoreSaveGame>(UGameplayStatics::CreateSaveGameObject(UHighScoreSaveGame::StaticClass()));
        UGameplayStatics::SaveGameToSlot(LoadGameInstance, TEXT("HighScoreSaveSlot"), 0);
    }

    if (LoadGameInstance)
    {
        const FLevelScoreData* ScoreData = LoadGameInstance->LevelScores.Find(ConcertName);
        if (ScoreData)
        {
            return ScoreData->GetStarRating();
        }
    }
    return 0; // Return 0 if no data is found
}

void AOverworldConcertActor::LoadLevel()
{
    if (LevelToLoad != NAME_None)
    {
        UGameplayStatics::OpenLevel(this, LevelToLoad);
    }
}

void AOverworldConcertActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (AD3_Project_MuPoCharacter* PlayerCharacter = Cast<AD3_Project_MuPoCharacter>(OtherActor))
    {
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            PlayerController->bShowMouseCursor = true;
            PlayerController->bEnableClickEvents = true;
            PlayerController->bEnableMouseOverEvents = true;
            PlayerController->SetInputMode(FInputModeGameAndUI());
        }

        UUIGameManager* GameManager = PlayerCharacter->GetUIGameManager();
        if (GameManager)
        {
            GameManager->CacheOverworldConcertActor(this);  // Cache the actor
            UE_LOG(LogTemp, Log, TEXT("OnBeginOverlap - Cached Actor: %s"), *this->GetName());
        }
    }
}

void AOverworldConcertActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (AD3_Project_MuPoCharacter* PlayerCharacter = Cast<AD3_Project_MuPoCharacter>(OtherActor))
    {
        UUIGameManager* GameManager = PlayerCharacter->GetUIGameManager();
        if (GameManager)
        {
            GameManager->RemoveConcertSelectionWidget();
            UE_LOG(LogTemp, Log, TEXT("OnEndOverlap - Removed ConcertSelectionWidget from viewport"));
        }
        
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            PlayerController->bShowMouseCursor = false;
            PlayerController->SetInputMode(FInputModeGameOnly());
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
        //GameInstance->SetSelectedCharacter(SelectedCharacter);
    }

    UGameplayStatics::OpenLevel(this, FName("ConcertLocation_CustomSongs"));
}