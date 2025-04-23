/*
	Author: Demetri Karras
	Assignment Number: 1
	Date of Submission: April 22nd, 2025

	Name of this file: hangman.c
	Description:
	This program allows the user to play hangman in the terminal. The user is
	prompted to enter a text file that contains the words to be used for
	the game. A random word is selected from this file as the goal word. It is
	displayed to the user as a series of dashes representing each letter. The
	user guesses letters one at a time in an attempt to replace all the dashes
	with letters, effectively guessing the word. On each turn, the user sees
	their progress in guessing the goal word, the letters they have incorrectly
	guessed, and the number of partial matches in the word set they provided.
	If the user makes 6 incorrect guesses, they lose. If they reveal
	the entire word, they win.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define MAX_WORDS 60000 // maximum number of words obtained from the input file
#define MAX_LENGTH 30 // maximum size of a word from the input file
#define MAX_INCORRECT_GUESSES 6

int getPartialMatches(char words[MAX_WORDS][MAX_LENGTH], int wordCount, char goalWordDisplay[]);

int main() {

	setvbuf(stdout, NULL, _IONBF, 0);

	/*
	Print welcome message and obtain input file name.
	*/
	char inputFileName[256];

	printf("Welcome to Hangman! Please provide the name of your input file:\n");
	scanf("%s", inputFileName);

	/*
	The specified file is opened. Words are read from the file and stored in an
	array. A variable is incremented to keep count of all words.
	*/
	char words[MAX_WORDS][MAX_LENGTH];
	int wordCount = 0;

	FILE  *inputFile;
	inputFile = fopen(inputFileName, "r");
	if (inputFile == NULL) {
		printf ("Input file could not be opened.\n");
		return 1;
	}
	else {
		while (fgets(words[wordCount], MAX_LENGTH - 1, inputFile) != NULL && wordCount < MAX_WORDS) {
			int indexOfNewline = strcspn(words[wordCount], "\n");
			words[wordCount][indexOfNewline] = '\0';
			wordCount++;
		}
	}
	fclose(inputFile);

	/*
	A random number is generated and used to obtain a random word from the array
	containing words from the input file. This word is copied into the goalWord
	variable.
	*/
	srand(time(NULL));
	int randomIndex = rand() % wordCount;

	char goalWord[MAX_LENGTH];
	strcpy(goalWord, words[randomIndex]);
	int goalWordLength = strlen(goalWord);

	/*
	Initializes variables used during the game.
	*/
	char goalWordDisplay[goalWordLength + 1]; // stores dashes for each letter of the goal word - dashes are replaced with correctly guessed letters during the game
	goalWordDisplay[goalWordLength] = '\0';
	for (int i = 0; i < goalWordLength; i++) {
		goalWordDisplay[i] = '-';
	}
	int remainingBadGuesses = MAX_INCORRECT_GUESSES;
	char incorrectLetters[MAX_INCORRECT_GUESSES];
	bool gameActive = true;
	bool won = false;
	bool lost = false;

	/*
	This block contains the main logic of the game. The status of the game is
	monitored using variables. The number of remaining guesses, display of the
	goal word, incorrectly guessed letters, and partial matches are displayed to
	the user on each turn. Based on the status of the game, certain messages
	will be displayed to show a win or a loss.
	*/
	while (gameActive) {

		/*
		The goal word is compared to the display, and the number of remaining
		guesses is examined to evaluate the status of the game.
		*/
		if (strcmp(goalWordDisplay, goalWord) == 0) {
			won = true;
		}
		else if (remainingBadGuesses == 0) {
			lost = true;
		}

		if (won || lost) {
			gameActive = false;
		}

		/*
		Checks if the game has been lost and prints messages accordingly.
		*/
		if (lost) {
			printf("You have no incorrect guesses remaining.\n");
		}
		else {
			printf("You have %i incorrect guesses remaining.\n", remainingBadGuesses);
		}

		/*
		Prints the goal word display.
		*/
		printf("Word: ");
		printf("%s", goalWordDisplay);
		printf("\n");

		/*
		Prints the letters that have been incorrectly guessed.
		*/
		printf("Incorrect guesses: ");
		for (int i = 0; i < MAX_INCORRECT_GUESSES; i++) {
			printf("%c ", incorrectLetters[i]);
		}
		printf("\n");

		/*
		Calls a function to update partial matches and prints the count.
		*/
		printf("Partial Matches: %d\n", getPartialMatches(words, wordCount, goalWordDisplay));

		/*
		If the game is ongoing, the user is prompted to guess a character. Their
		 guess is compared against the goal word and handled accordingly.
		*/
		if (gameActive) {

			/*
			Prompts the user to guess a character and stores it.
			*/
			char guessedChar;

			printf("Your guess: ");
			scanf(" %c", &guessedChar);
			printf("\n");

			/*
			Iterates through the characters of the goal word to see if it
			contains the guessed character. If it does, the display of the goal
			word is updated accordingly. If not, the character is added to the
			array of incorrectly guessed letters.
			*/
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

				/*
				If the letter has already been incorrectly guessed, an
				appropriate message is printed. If it is a new incorrect guess,
				the character is added to the array of incorrect guesses.
				*/
				bool alreadyIncorrectlyGuessed = false;

				for (int i = 0; i < strlen(incorrectLetters) && alreadyIncorrectlyGuessed == false; i++) {
					if (guessedChar == incorrectLetters[i]) {
						alreadyIncorrectlyGuessed = true;
					}
				}

				if (alreadyIncorrectlyGuessed) {
					printf("%c was already incorrectly guessed. Try again.\n", guessedChar);
				}
				else {
					printf("The word DOES NOT contain %c.\n", guessedChar);
					incorrectLetters[6 - remainingBadGuesses] = guessedChar;
					remainingBadGuesses--;
				}
			}
		}

		/*
		Evaluates the win/loss status of the game and displays a message
		accordingly.
		*/
		if (won) {
			printf("\nCongrats! The word was %s!", goalWord);
		}
		else if (lost) {
			printf("\nYou lost! The word was %s.", goalWord);
		}
	}

	return 0;
}

/*
Function Name: getPartialMatches

Input: Param1(words) - array of all words from the input file, Param2(wordCount)
- the number of words obtained from the input file, Param3(goalWordDisplay) -
the dynamic display of the goal word, with dashes representing unguessed letters

Output: Returns the number of partial matches between the words from the input
file and the current goal word display.

Description: Iterates through all words from the input file to find partial
matches with the current display of the goal word. The length of the display and
the index of correctly guessed characters is used to determine a partial match.
The number of partial matches is returned.
*/
int getPartialMatches(char words[MAX_WORDS][MAX_LENGTH], int wordCount, char goalWordDisplay[]) {

	bool isPartialMatch = true;
	int newPartialMatches = 0;

	/*
	Checks the length and character locations of each word from the input file
	against the current display of the goal word. If the length differs or a
	non-dash character in the goal word display is in the wrong location, there
	is no partial match. Otherwise, there is a partial match, and a variable is
	incremented to keep track of the count.
	*/
	for (int i = 0; i < wordCount; i++) {

		if (strlen(words[i]) != strlen(goalWordDisplay)) {
			isPartialMatch = false;
		}
		else {
			for (int c = 0; c < strlen(goalWordDisplay); c++) {
				if (goalWordDisplay[c] != '-') {
					if (words[i][c] != goalWordDisplay[c]) {
						isPartialMatch = false;
					}
				}
			}
		}

		if (isPartialMatch) {
			newPartialMatches++;
		}

		isPartialMatch = true; // reset to true for next iteration
	}

	return newPartialMatches;
}
