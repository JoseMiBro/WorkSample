// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/Conversation/ConversationAsset.h"

void UConversationAsset::ReverseLines()
{
  for (int i = 0; i < Lines.Num() / 2; ++i)
  {
    Lines.Swap(i, Lines.Num() - i - 1);
  }
}

bool UConversationAsset::IsConversationImportant()
{
  DialogueImportantCount = 0;
  IsDialogueImportant = false;

  for (int i = 0; i < Lines.Num(); ++i) 
  {
    if (Lines[i].IsLineImportant) 
    {
      IsDialogueImportant = true;
      ++DialogueImportantCount;
    }
  }
  return IsDialogueImportant;
}