/*
 ============================================================================
 Name        : hangman.c
 Author      : Demetri Karras
 Version     :
 Copyright   : Your copyright notice
 Description : Exercise 1
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

int main() {

	setvbuf(stdout, NULL, _IONBF, 0);

	printf("Welcome to Hangman! Please provide the name of your input file:\n");

	char inputFileName[256];
	//scanf("%s", inputFileName);

	int MAX_WORDS = 20000;
	int MAX_LENGTH = 100;
	char words[MAX_WORDS][MAX_LENGTH];
	int wordCount = 0;

	FILE  *inputFile;
	inputFile = fopen("dictionary.txt", "r");
	if (inputFile == NULL) {
		printf ("Input file could not be opened.\n");
		return 1;
	}
	else {
		while (fgets(words[wordCount], MAX_LENGTH - 1, inputFile) != NULL && wordCount < MAX_WORDS) {
			int indexOfNewLine = strcspn(words[wordCount], "\n");
			words[wordCount][indexOfNewLine] = '\0';
			wordCount++;
		}
	}
	fclose(inputFile);

	srand(time(NULL));
	int randomIndex = rand() % wordCount;

	// ************************************************************

	char goalWord[MAX_LENGTH];
	strcpy(goalWord, words[randomIndex]);
	int goalWordLength = strlen(goalWord);

	char goalWordDisplay[goalWordLength + 1];
	goalWordDisplay[goalWordLength] = '\0';
	for (int i = 0; i < goalWordLength; i++) {
		goalWordDisplay[i] = '-';
	}
	int remainingBadGuesses = 6;
	char lettersUsed[7] = {'\0'};
	bool gameActive = true;
	bool won = false;
	bool lost = false;

	while (gameActive) {

		if (strcmp(goalWordDisplay, goalWord) == 0) {
			won = true;
		}
		else if (remainingBadGuesses == 0) {
			lost = true;
		}

		if (won || lost) {
			gameActive = false;
		}

		if (lost) {
			printf("You have no incorrect guesses remaining.\n");
		}
		else {
			printf("You have %i incorrect guesses remaining.\n", remainingBadGuesses);
		}

		printf("Word: ");
		printf("%s", goalWordDisplay);
		printf("\n");

		printf("Incorrect guesses: ");
		for (int i = 0; lettersUsed[i] != '\0'; i++) {
			if (lettersUsed[i] != NULL) {
				printf("%c ", lettersUsed[i]);
			}
		}
		printf("\n");

		if (gameActive) { // only prompts for an attempt if game is ongoing
			char guessedChar;
			printf("Your guess: ");
			scanf(" %c", &guessedChar);

			printf("\n");

			bool correctGuess = false;
			for (int i = 0; i < goalWordLength; i++) {
				if (guessedChar == goalWord[i]) {
					goalWordDisplay[i] = guessedChar;
					correctGuess = true;
				}
			}
			if (correctGuess) {
				printf("The word DOES contain %c.\n", guessedChar);
			}
			else {
				printf("The word DOES NOT contain %c.\n", guessedChar);
				lettersUsed[6 - remainingBadGuesses] = guessedChar;
				remainingBadGuesses--;
			}
		}

		if (won) {
			printf("\nCongrats! The word was %s!", goalWord);
		}
		else if (lost) {
			printf("\nYou lost! The word was %s.", goalWord);
		}
	}

	return 0;
}

