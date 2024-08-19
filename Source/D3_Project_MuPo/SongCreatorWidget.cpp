// Fill out your copyright notice in the Description page of Project Settings.

#include "SongCreatorWidget.h"
#include "ListEntryWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/ListView.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "SongDataEntry.h"

void UCSVWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    if (AddUpdateButton)
    {
        AddUpdateButton->OnClicked.AddDynamic(this, &UCSVWidget::OnAddUpdateEntry);
    }

    if (ExportButton)
    {
        ExportButton->OnClicked.AddDynamic(this, &UCSVWidget::OnExportCSV);
    }
    
    if (ActionComboBox)
    {
        ActionComboBox->AddOption(TEXT("spawn"));
        ActionComboBox->AddOption(TEXT("despawn"));
        ActionComboBox->SetSelectedOption(TEXT("spawn"));
    }

    if (EntriesListView)
    {
        EntriesListView->OnItemClicked().AddUObject(this, &UCSVWidget::OnRowSelect);
    }
    
}

void UCSVWidget::TestListView()
{
    if (EntriesListView && ListEntryWidgetBPClass)
    {
        for (int32 i = 0; i < 3; ++i)
        {
            USongDataEntry* DataEntry = NewObject<USongDataEntry>(this);
            DataEntry->TimeMs = i * 1000;
            DataEntry->NoteNumber = i + 1;
            DataEntry->Track = 1;
            DataEntry->Action = "spawn";

            EntriesListView->AddItem(DataEntry);
        }
    }
}

void UCSVWidget::AddItemToList(const FString& TimeMs, const FString& NoteNumber, const FString& Track, const FString& Action)
{
    if (!EntriesListView || !ListEntryWidgetBPClass) return;

    // Create a new data entry object
    USongDataEntry* DataEntry = NewObject<USongDataEntry>(this);
    DataEntry->TimeMs = FCString::Atoi(*TimeMs);
    DataEntry->NoteNumber = FCString::Atoi(*NoteNumber);
    DataEntry->Track = FCString::Atoi(*Track);
    DataEntry->Action = Action;

    // Add the data entry to the ListView
    EntriesListView->AddItem(DataEntry);
}

void UCSVWidget::OnAddUpdateEntry()
{
    // Create a new data entry object
    USongDataEntry* DataEntry = NewObject<USongDataEntry>(this);
    DataEntry->TimeMs = FCString::Atoi(*TimeMsTextBox->GetText().ToString());
    DataEntry->NoteNumber = FCString::Atoi(*NoteNumberTextBox->GetText().ToString());
    DataEntry->Track = FCString::Atoi(*TrackTextBox->GetText().ToString());
    DataEntry->Action = ActionComboBox->GetSelectedOption();

    if (SelectedIndex >= 0)
    {
        // Update existing entry
        CSVEntries[SelectedIndex] = ConvertToFcsvEntry(DataEntry);

        // Update the corresponding row in the list view
        EntriesListView->RequestRefresh();
    }
    else
    {
        // Add new entry
        FCSVEntry NewEntry = ConvertToFcsvEntry(DataEntry);
        CSVEntries.Add(NewEntry);

        // Add to ListView
        EntriesListView->AddItem(DataEntry);
    }
    
    ClearFields();
    SelectedIndex = -1;
}

void UCSVWidget::OnExportCSV()
{
    
    FString FileName = FileNameTextBox ? FileNameTextBox->GetText().ToString() : TEXT("DefaultSongName");
    
   
    if (!FileName.EndsWith(TEXT(".csv")))
    {
        FileName += TEXT(".csv");
    }
    
    FString FolderPath = FPaths::ProjectContentDir() + "CustomSongs/";
    
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.DirectoryExists(*FolderPath))
    {
        PlatformFile.CreateDirectory(*FolderPath);
    }

    FString FilePath = FolderPath + FileName;
    
    FString FileContent = "Time_ms,Note_Number,Track,Action\n";
    for (const FCSVEntry& Entry : CSVEntries)
    {
        FileContent += FString::Printf(TEXT("%d,%d,%d,%s\n"),
            Entry.TimeMs, Entry.NoteNumber, Entry.Track, *Entry.Action);
    }
    
    if (FFileHelper::SaveStringToFile(FileContent, *FilePath))
    {
        UE_LOG(LogTemp, Log, TEXT("CSV file successfully saved to: %s"), *FilePath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save CSV file to: %s"), *FilePath);
    }
}

void UCSVWidget::UpdateVisualization()
{
    if (!EntriesListView) return;
    
    EntriesListView->ClearListItems();
    
    for (const FCSVEntry& Entry : CSVEntries)
    {
        AddItemToList(
            FString::FromInt(Entry.TimeMs),
            FString::FromInt(Entry.NoteNumber),
            FString::FromInt(Entry.Track),
            Entry.Action
        );
    }
}

void UCSVWidget::ClearFields()
{
    if (TimeMsTextBox)
    {
        TimeMsTextBox->SetText(FText::GetEmpty());
    }

    if (NoteNumberTextBox)
    {
        NoteNumberTextBox->SetText(FText::GetEmpty());
    }

    if (TrackTextBox)
    {
        TrackTextBox->SetText(FText::GetEmpty());
    }

    if (ActionComboBox)
    {
        ActionComboBox->SetSelectedOption(TEXT("spawn")); // or a default action
    }
}

void UCSVWidget::OnRowSelect(UObject* SelectedItem)
{
    USongDataEntry* DataEntry = Cast<USongDataEntry>(SelectedItem);
    if (DataEntry)
    {
        // Populate input fields with the selected row's data
        TimeMsTextBox->SetText(FText::AsNumber(DataEntry->TimeMs));
        NoteNumberTextBox->SetText(FText::AsNumber(DataEntry->NoteNumber));
        TrackTextBox->SetText(FText::AsNumber(DataEntry->Track));
        ActionComboBox->SetSelectedOption(DataEntry->Action);

        // Find the corresponding FCSVEntry index
        SelectedIndex = FindCSVEntryIndex(DataEntry);
    }
}

FCSVEntry UCSVWidget::ConvertToFcsvEntry(USongDataEntry* DataEntry)
{
    FCSVEntry Entry;
    Entry.TimeMs = DataEntry->TimeMs;
    Entry.NoteNumber = DataEntry->NoteNumber;
    Entry.Track = DataEntry->Track;
    Entry.Action = DataEntry->Action;
    return Entry;
}

int32 UCSVWidget::FindCSVEntryIndex(USongDataEntry* DataEntry)
{
    if (!DataEntry) return INDEX_NONE;

    for (int32 i = 0; i < CSVEntries.Num(); i++)
    {
        if (CSVEntries[i].TimeMs == DataEntry->TimeMs &&
            CSVEntries[i].NoteNumber == DataEntry->NoteNumber &&
            CSVEntries[i].Track == DataEntry->Track &&
            CSVEntries[i].Action == DataEntry->Action)
        {
            return i;
        }
    }
    return INDEX_NONE; // Not found
}

