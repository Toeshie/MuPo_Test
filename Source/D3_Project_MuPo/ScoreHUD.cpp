// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"

void AScoreHUD::DrawHUD()
{
    Super::DrawHUD();
    
    FString ScoreText = FString::Printf(TEXT("Player 1 Score: %d"), Player1Score);
    FString StreakText = FString::Printf(TEXT("Streak: %d"), CurrentStreak);
    FString MultiplierText = FString::Printf(TEXT("Multiplier: x%d"), ScoreMultiplier);
    FString GoodHitsText = FString::Printf(TEXT("Good Hits: %d"), GoodHits);
    FString PerfectHitsText = FString::Printf(TEXT("Perfect Hits: %d"), PerfectHits);
    FString HitPercentageText = FString::Printf(TEXT("Hit Percentage: %.2f%%"), CalculateHitPercentage());

    float TextWidth = 10.0f;
    float TextHeight = 10.0f;
    if (GEngine && GEngine->GetLargeFont())
    {
        Canvas->StrLen(GEngine->GetLargeFont(), ScoreText, TextWidth, TextHeight);
    }

    FVector2D ScreenPosition(50.0f, 50.0f);
    FLinearColor TextColor = FLinearColor::White;

    DrawText(ScoreText, TextColor, ScreenPosition.X, ScreenPosition.Y, GEngine->GetLargeFont(), 1.5f, false);
    DrawText(StreakText, TextColor, ScreenPosition.X, ScreenPosition.Y + TextHeight + 10.0f, GEngine->GetLargeFont(), 1.5f, false);
    DrawText(MultiplierText, TextColor, ScreenPosition.X, ScreenPosition.Y + 2 * (TextHeight + 10.0f), GEngine->GetLargeFont(), 1.5f, false);
    DrawText(GoodHitsText, TextColor, ScreenPosition.X, ScreenPosition.Y + 3 * (TextHeight + 10.0f), GEngine->GetLargeFont(), 1.5f, false);
    DrawText(PerfectHitsText, TextColor, ScreenPosition.X, ScreenPosition.Y + 4 * (TextHeight + 10.0f), GEngine->GetLargeFont(), 1.5f, false);
    DrawText(HitPercentageText, TextColor, ScreenPosition.X, ScreenPosition.Y + 5 * (TextHeight + 10.0f), GEngine->GetLargeFont(), 1.5f, false);
}

void AScoreHUD::SetScore(int32 NewScore)
{
    Player1Score = NewScore;
}

void AScoreHUD::SetStreak(int32 NewStreak)
{
    CurrentStreak = NewStreak;
}

void AScoreHUD::SetMultiplier(int32 NewMultiplier)
{
    ScoreMultiplier = NewMultiplier;
}

void AScoreHUD::SetGoodHits(int32 NewGoodHits)
{
    GoodHits = NewGoodHits;
}

void AScoreHUD::SetPerfectHits(int32 NewPerfectHits)
{
    PerfectHits = NewPerfectHits;
}

void AScoreHUD::SetTotalNotes(int32 NewTotalNotes)
{
    TotalNotes = NewTotalNotes;
}

float AScoreHUD::CalculateHitPercentage() const
{
    if (TotalNotes == 0)
    {
        return 0.0f;
    }

    int32 TotalHits = GoodHits + PerfectHits;
    return (static_cast<float>(TotalHits) / TotalNotes) * 100.0f;
}