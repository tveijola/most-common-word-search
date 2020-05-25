#include "word_mod.h"

/*
 * Modifies all characters of a word to lower case.
 */
void toLowerCase(char *str)
{
	int i = 0;
	while (str[i] != '\0')
	{
		str[i] = tolower(str[i]);
		i++;
	}
}

/*
 * Modifies input word to contain only alphabetic characters (' is allowed).
 * 
 * First, if any number is found, the word is deemed invalid.
 * Trims the beginning and end of the string from undesired characters.
 * Finally, checks if there are undesired characters left in the middle.
 *
 */
int removeExtraCharacters (char *str)
{
	/* Allowed characters */
	static const char allowedCharacters[] = "abcdefghijklmnopqrstuvwxyz'";
	
	/* Undesired characters */
	static const char undesiredCharacters[] = ".,\"/\\()_[]{}*!";
	
	/* Number characters */
	static const char numbers[] = "0123456789";
	
	/*
	 * If there is a number in the string, 
	 * mark the string as empty and return. Ex. 2nd, 2001
	 */
	if (strpbrk(str, numbers) != NULL)
	{
		str[0] = 0;
		return 0;
	}
	
	/* Trim left side */
	// While the first character is not allowed, remove it
	while (strchr(allowedCharacters, str[0]) == NULL)
	{
		memmove(str, str+1, strlen(str));
	}
	
	/* Trim right side */
	// If the very last char is ', and it is preceded by undesired char, remove it.
	if (str[strlen(str)-1] == '\'' && strchr(allowedCharacters, str[strlen(str)-2]) == NULL)
	{
		str[strlen(str)-1] = 0;
	}
	// While the last character is not allowed, remove it
	while (strchr(allowedCharacters, str[strlen(str)-1]) == NULL)
	{
		str[strlen(str)-1] = 0;
	}
	
	/* If undesired characters left in the middle, invalid. */
	if (strpbrk(str, undesiredCharacters) != NULL)
	{
		str[0] = 0;
	}
		
	return 1;
}