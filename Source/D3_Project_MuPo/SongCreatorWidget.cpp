// Fill out your copyright notice in the Description page of Project Settings.

#include "SongCreatorWidget.h"

#include "ListEntryWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

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

    // Initialize the action combo box with options
    if (ActionComboBox)
    {
        ActionComboBox->AddOption(TEXT("spawn"));
        ActionComboBox->AddOption(TEXT("despawn"));
        ActionComboBox->SetSelectedOption(TEXT("spawn"));
    }
}

void UCSVWidget::OnAddUpdateEntry()
{
    FCSVEntry NewEntry;

    NewEntry.TimeMs = FCString::Atoi(*TimeMsTextBox->GetText().ToString());
    NewEntry.NoteNumber = FCString::Atoi(*NoteNumberTextBox->GetText().ToString());
    NewEntry.Track = FCString::Atoi(*TrackTextBox->GetText().ToString());
    NewEntry.Action = ActionComboBox->GetSelectedOption();

    CSVEntries.Add(NewEntry);

    UpdateVisualization();
    ClearFields();
}

void UCSVWidget::OnExportCSV()
{
    FString FilePath = FPaths::ProjectDir() + "/ExportedCSV.csv";

    FString FileContent = "Time_ms,Note_Number,Track,Action\n";
    for (const FCSVEntry& Entry : CSVEntries)
    {
        FileContent += FString::Printf(TEXT("%d,%d,%d,%s\n"),
            Entry.TimeMs, Entry.NoteNumber, Entry.Track, *Entry.Action);
    }

    FFileHelper::SaveStringToFile(FileContent, *FilePath);
}

void UCSVWidget::UpdateDropdown()
{
    // Implementation to update the dropdown or list view with the new data
}

void UCSVWidget::UpdateVisualization()
{
    if (!EntriesListView) return;

    EntriesListView->ClearListItems();

    for (const FCSVEntry& Entry : CSVEntries)
    {
        UListEntryWidget* EntryWidget = CreateWidget<UListEntryWidget>(this, ListEntryWidgetBPClass);
        if (EntryWidget)
        {
            EntryWidget->TimeMsText->SetText(FText::FromString(FString::FromInt(Entry.TimeMs)));
            EntryWidget->NoteNumberText->SetText(FText::FromString(FString::FromInt(Entry.NoteNumber)));
            EntryWidget->TrackText->SetText(FText::FromString(FString::FromInt(Entry.Track)));
            EntryWidget->ActionText->SetText(FText::FromString(Entry.Action));

            EntriesListView->AddItem(EntryWidget);
        }
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
        ActionComboBox->SetSelectedOption(TEXT("spawn"));
    }
}

void UCSVWidget::OnRowSelect()
{
    // Implementation for selecting and populating data when a row is selected
}