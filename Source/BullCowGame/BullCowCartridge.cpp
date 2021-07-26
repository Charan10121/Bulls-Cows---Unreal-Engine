// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"
//#include "Math/UnrealMathUtility.h" //this is included within "CoreMinimal.h"

//    FString HiddenWord = TEXT("lmao"); (can declare as global or declare within header file)

void UBullCowCartridge::BeginPlay() // When the game starts
{

    Super::BeginPlay(); //code must go underneath this

    InitGame(); //setting up lives and hiddenword
}

void UBullCowCartridge::OnInput(const FString &Input) // When the player hits enter
{
    if (GameOver) //restart the game
    {
        ClearScreen();

        InitGame();
    }
    else
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::ProcessGuess(const FString &Guess)
{

    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("You have Won!"));
        EndGame();
        return;
    }

    if (HiddenWord.Len() != Guess.Len()) //check no. of char
    {
        PrintLine(TEXT("Should have %i letters, try again."), HiddenWord.Len());
    }
    else if (!isIsogram(Guess)) //check for unique char
    {
        PrintLine(TEXT("Should have all unique letters, try again."));
    }

    else
    {
        --Lives;
        if (Lives > 0)
        {
            int32 BullCount = 0, CowCount = 0;
            GetBullCows(Guess, BullCount, CowCount);
            PrintLine(TEXT("Bulls: %i, Cows: %i"), BullCount, CowCount);
            PrintLine(TEXT("Remaining lives: %i"), Lives);
        }
        else
        {
            ClearScreen();
            PrintLine(TEXT("You have no lives left."));
            PrintLine(TEXT("The Hidden Word was %s"), *HiddenWord);
            EndGame();
        }
    }
}

bool UBullCowCartridge::isIsogram(const FString &Guess) const
{
    for (int32 i = 0; i < Guess.Len() - 1; i++)
    {
        for (int32 j = i + 1; j < Guess.Len(); j++)
        {
            if (Guess[j] == Guess[i])
            {
                return false;
            }
        }
    }
    return true;
}

void UBullCowCartridge::InitGame()
{
    newwords = GetValidWords(Words); //word list that are isograms and 4-8 letters long

    GameOver = false;
    HiddenWord = newwords[FMath::RandRange(0, newwords.Num() - 1)]; //getting a random word from valid word list
    Lives = HiddenWord.Len() * 2;

    PrintLine(TEXT("Welcome to Bulls and Cows!")); //use TEXT macro
    PrintLine(TEXT("Guess the %i letter word."), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives."), Lives);
    PrintLine(TEXT("(Tab to enter Console)"));
    PrintLine(TEXT("Press Enter to Guess"));
    PrintLine(TEXT("Hidden Word is %s"),*HiddenWord);    //debug line
    // PrintLine(TEXT("There are %i total words"),Words.Num());    //debug line //from HiddenWordsList.h
}

void UBullCowCartridge::EndGame()
{
    GameOver = true;
    PrintLine(TEXT("\nPress Enter to Play Again."));
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString> &WordList) const
{
    TArray<FString> newwords;
    for (int32 i = 0; i < WordList.Num(); i++)
    {
        if (WordList[i].Len() >= 4 && WordList[i].Len() <= 8 && isIsogram(WordList[i]))
        {
            newwords.Emplace(WordList[i]);
        }
    }
    return newwords;
}

void UBullCowCartridge::GetBullCows(const FString &Guess, int32 &BullCount, int32 &CowCount) const
{
    for (int32 i = 0; i < Guess.Len(); i++)
    {
        if (Guess[i] == HiddenWord[i])
        {
            BullCount++;
            continue;
        }

        int32 indi;
        if (HiddenWord.FindChar(Guess[i],indi))
        {
            CowCount++;
        }
    }
}
