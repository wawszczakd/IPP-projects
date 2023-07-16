/** @file
 * Implementacja klasy przechowującej numery telefonów
 *
 * @author Dominik Wawszczak <dw440014@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "phone_forward.h"

/**
 * Wartość maksymalnej cyfry.
 */
static const int MAX_DIGIT = 11;

/** @brief Zwraca cyfrę odpowiadającą znakowi.
 * @param[in] character – znak do zamienienia.
 * @return Cyfra odpowiadająca znakowi lub -1, jeśli @p character nie jest cyfrą.
 */
static int getDigit(char character) {
	if ('0' <= character && character <= '9') {
		return character - '0';
	}
	else if (character == '*') {
		return 10;
	}
	else if (character == '#') {
		return 11;
	}
	else {
		return -1;
	}
}

/** @brief Zwraca znak odpowiadający cyfrze.
 * @param[in] digit – cyfra do zamienienia.
 * @return Znak odpowiadający cyfrze lub '\0', jeśli @p digit nie jest znakiem.
 */
static char getCharacter(int digit) {
	if (0 <= digit && digit <= 9) {
		return '0' + digit;
	}
	else if (digit == 10) {
		return '*';
	}
	else if (digit == 11) {
		return '#';
	}
	else {
		return '\0';
	}
}

/**
 * To jest struktura przechowująca przekierowania numerów telefonów.
 * Jest ona zaimplementowana na drzewie trie.
 */
struct PhoneForward {
	PhoneForward **children; ///< synowie wierzchołka
	PhoneForward *parent;    ///< ojciec wierzchołka
	int digit;               ///< cyfra na krawędzi do ojca
	char *redirection;       ///< przekierowanie
};

/**
 * To jest struktura przechowująca ciąg numerów telefonów.
 * Jest ona zaimplementowana na drzewie trie.
 */
struct PhoneNumbers {
	PhoneNumbers **children; ///< synowie wierzchołka
	PhoneNumbers *parent;    ///< ojciec wierzchołka
	char *number;            ///< numer, któremu odpowiada wierzchołek
	size_t totalNumbers;     ///< liczba numerów w poddrzewie
};

PhoneForward * phfwdNew(void) {
	PhoneForward *result = malloc(sizeof(PhoneForward));
	if (result == NULL) {
		return NULL;
	}
	
	result->children = malloc((MAX_DIGIT + 1) * sizeof(PhoneForward *));
	if (result->children == NULL) {
		free(result);
		return NULL;
	}
	for (int digit = 0; digit <= MAX_DIGIT; digit++) {
		result->children[digit] = NULL;
	}
	
	result->parent = NULL;
	result->digit = -1;
	result->redirection = NULL;
	
	return result;
}

void phfwdDelete(PhoneForward *pf) {
	if (pf == NULL) {
		return;
	}
	
	PhoneForward *currNode = pf;
	while (true) {
		for (int digit = 0; digit <= MAX_DIGIT; digit++) {
			if (currNode->children[digit] != NULL) {
				currNode = currNode->children[digit];
				currNode->parent->children[digit] = NULL;
				digit = -1;
				continue;
			}
		}
		
		if (currNode == pf) {
			break;
		}
		
		PhoneForward *parent = currNode->parent;
		free(currNode->children);
		free(currNode->redirection);
		free(currNode);
		currNode = parent;
	}
	
	free(currNode->children);
	free(currNode->redirection);
	free(currNode);
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2) {
	if (pf == NULL || num1 == NULL || num2 == NULL) {
		return false;
	}
	
	size_t length1 = 0;
	while (getDigit(num1[length1]) != -1) {
		length1++;
	}
	if (num1[length1] != '\0' || length1 == 0) {
		return false;
	}
	
	size_t length2 = 0;
	while (getDigit(num2[length2]) != -1) {
		length2++;
	}
	if (num2[length2] != '\0' || length2 == 0) {
		return false;
	}
	
	bool different = false;
	if (length1 != length2) {
		different = true;
	}
	else {
		for (size_t i = 0; i < length1; i++) {
			if (num1[i] != num2[i]) {
				different = true;
				break;
			}
		}
	}
	if (!different) {
		return false;
	}
	
	PhoneForward *num1Node = pf;
	for (size_t i = 0; i < length1; i++) {
		int digit = getDigit(num1[i]);
		if (num1Node->children[digit] == NULL) {
			num1Node->children[digit] = phfwdNew();
			if (num1Node->children[digit] == NULL) {
				return false;
			}
			num1Node->children[digit]->parent = num1Node;
			num1Node->children[digit]->digit = digit;
		}
		
		num1Node = num1Node->children[digit];
	}
	
	free(num1Node->redirection);
	num1Node->redirection = malloc(sizeof(char) * (length2 + 1));
	if (num1Node->redirection == NULL) {
		return false;
	}
	
	for (size_t i = 0; i < length2; i++) {
		num1Node->redirection[i] = num2[i];
	}
	num1Node->redirection[length2] = '\0';
	
	return true;
}

void phfwdRemove(PhoneForward *pf, char const *num) {
	if (pf == NULL || num == NULL) {
		return;
	}
	
	size_t length = 0;
	while (getDigit(num[length]) != -1) {
		length++;
	}
	if (num[length] != '\0' || length == 0) {
		return;
	}
	
	PhoneForward *numNode = pf;
	for (size_t i = 0; i < length; i++) {
		int digit = getDigit(num[i]);
		if (numNode->children[digit] == NULL) {
			return;
		}
		
		numNode = numNode->children[digit];
	}
	
	numNode->parent->children[getDigit(num[length - 1])] = NULL;
	phfwdDelete(numNode);
}

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę niezawierającą żadnych numerów.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
PhoneNumbers * phnumNew(void) {
	PhoneNumbers *result = malloc(sizeof(PhoneNumbers));
	if (result == NULL) {
		return NULL;
	}
	
	result->children = malloc((MAX_DIGIT + 1) * sizeof(PhoneNumbers *));
	if (result->children == NULL) {
		free(result);
		return NULL;
	}
	for (int digit = 0; digit <= MAX_DIGIT; digit++) {
		result->children[digit] = NULL;
	}
	
	result->parent = NULL;
	
	result->number = NULL;
	
	result->totalNumbers = 0;
	
	return result;
}

/** @brief Dodaje numer telefonu.
 * @param[in,out] pnum – wskaźnik na strukturę przechowującą ciąg numerów;
 * @param[in] num      – numer do dodania.
 */
void phnumAdd(PhoneNumbers *pnum, char const *num) {
	if (pnum == NULL || num == NULL) {
		return;
	}
	
	size_t length = 0;
	while (getDigit(num[length]) != -1) {
		length++;
	}
	if (num[length] != '\0' || length == 0) {
		return;
	}
	
	PhoneNumbers *numNode = pnum;
	for (size_t i = 0; i < length; i++) {
		int digit = getDigit(num[i]);
		if (numNode->children[digit] == NULL) {
			numNode->children[digit] = phnumNew();
			if (numNode->children[digit] == NULL) {
				return;
			}
			numNode->children[digit]->parent = numNode;
		}
		
		numNode = numNode->children[digit];
	}
	
	if (numNode->number == NULL) {
		numNode->number = malloc(sizeof(char) * (length + 1));
		if (numNode->number == NULL) {
			return;
		}
		for (size_t i = 0; i < length; i++) {
			numNode->number[i] = num[i];
		}
		numNode->number[length] = '\0';
		
		while (true) {
			numNode->totalNumbers++;
			if (numNode->parent == NULL) {
				break;
			}
			numNode = numNode->parent;
		}
	}
}

void phnumDelete(PhoneNumbers *pnum) {
	if (pnum == NULL) {
		return;
	}
	
	PhoneNumbers *currNode = pnum;
	while (true) {
		for (int digit = 0; digit <= MAX_DIGIT; digit++) {
			if (currNode->children[digit] != NULL) {
				currNode = currNode->children[digit];
				currNode->parent->children[digit] = NULL;
				digit = -1;
				continue;
			}
		}
		
		if (currNode == pnum) {
			break;
		}
		
		PhoneNumbers *parent = currNode->parent;
		free(currNode->children);
		free(currNode->number);
		free(currNode);
		currNode = parent;
	}
	
	free(currNode->children);
	free(currNode->number);
	free(currNode);
}

PhoneNumbers * phfwdGet(PhoneForward const *pf, char const *num) {
	if (pf == NULL) {
		return NULL;
	}
	
	bool incorrect = false;
	size_t length = 0;
	if (num == NULL) {
		incorrect = true;
	}
	else {
		while (getDigit(num[length]) != -1) {
			length++;
		}
		if (num[length] != '\0' || length == 0) {
			incorrect = true;
		}
	}
	if (incorrect) {
		return phnumNew();
	}
	
	PhoneForward const *maxPrefNode = pf;
	size_t matchedDigits = 0;
	for (; matchedDigits < length; matchedDigits++) {
		int digit = getDigit(num[matchedDigits]);
		if (maxPrefNode->children[digit] == NULL) {
			break;
		}
		
		maxPrefNode = maxPrefNode->children[digit];
	}
	
	while (maxPrefNode->parent != NULL && maxPrefNode->redirection == NULL) {
		maxPrefNode = maxPrefNode->parent;
		matchedDigits--;
	}
	
	PhoneNumbers *result = phnumNew();
	if (result == NULL) {
		return NULL;
	}
	
	if (maxPrefNode->parent == NULL) {
		phnumAdd(result, num);
	}
	else {
		size_t redirectionLength = 0;
		while (getDigit(maxPrefNode->redirection[redirectionLength]) != -1) {
			redirectionLength++;
		}
		
		size_t resultLength = redirectionLength + length - matchedDigits;
		
		char *tmp = malloc(sizeof(char) * (resultLength + 1));
		if (tmp == NULL) {
			phnumDelete(result);
			return NULL;
		}
		
		for (size_t i = 0; i < redirectionLength; i++) {
			tmp[i] = maxPrefNode->redirection[i];
		}
		for (size_t i = matchedDigits; i < length; i++) {
			tmp[i - matchedDigits + redirectionLength] = num[i];
		}
		tmp[resultLength] = '\0';
		
		phnumAdd(result, tmp);
		
		free(tmp);
	}
	
	return result;
}

/** @brief Rekurencyjna funkcja do znajdowania przekierowań.
 * @param[in] pf         – wskaźnik na strukturę przechowującą przekierowania
 *                         numerów;
 * @param[in] num        – wskaźnik na napis reprezentujący szukany numer;
 * @param[in] length     – długość numeru;
 * @param[in] depth      – aktualna głębokość w drzewie.
 * @param[in,out] result – wskaźnik na strukturę przechowującą numery telefonów.
 */
void dfs(PhoneForward const *pf, char const *num, const size_t length,
         size_t depth, PhoneNumbers *result) {
	if (pf->redirection != NULL) {
		size_t redirectionLength = 0;
		while (getDigit(pf->redirection[redirectionLength]) != -1) {
			redirectionLength++;
		}
		
		if (redirectionLength <= length) {
			bool ok = true;
			for (size_t i = 0; i < redirectionLength; i++) {
				if (pf->redirection[i] != num[i]) {
					ok = false;
					break;
				}
			}
			
			if (ok) {
				char *tmp = malloc(sizeof(char) * (depth + (length -
				                   redirectionLength) + 1));
				
				if (tmp != NULL) {
					size_t currDepth = depth;
					PhoneForward const *currNode = pf;
					while (true) {
						if (currNode->parent == NULL) {
							 break;
						}
						
						tmp[--currDepth] = getCharacter(currNode->digit);
						
						currNode = currNode->parent;
					}
					
					for (size_t i = redirectionLength; i < length; i++) {
						tmp[depth + (i -redirectionLength)] = num[i];
					}
					
					tmp[depth + (length - redirectionLength)] = '\0';
					
					phnumAdd(result, tmp);
					
					free(tmp);
				}
			}
		}
	}
	
	for (int digit = 0; digit <= MAX_DIGIT; digit++) {
		if (pf->children[digit] != NULL) {
			dfs(pf->children[digit], num, length, depth + 1, result);
		}
	}
}

PhoneNumbers * phfwdReverse(PhoneForward const *pf, char const *num) {
	if (pf == NULL) {
		return NULL;
	}
	
	bool incorrect = false;
	size_t length = 0;
	if (num == NULL) {
		incorrect = true;
	}
	else {
		while (getDigit(num[length]) != -1) {
			length++;
		}
		if (num[length] != '\0' || length == 0) {
			incorrect = true;
		}
	}
	if (incorrect) {
		return phnumNew();
	}
	
	PhoneNumbers *result = phnumNew();
	phnumAdd(result, num);
	dfs(pf, num, length, 0, result);
	
	return result;
}

PhoneNumbers * phfwdGetReverse(PhoneForward const *pf, char const *num) {
	PhoneNumbers *tmp = phfwdReverse(pf, num);
	if (tmp == NULL) {
		return NULL;
	}
	
	PhoneNumbers *result = phnumNew();
	for (size_t i = 0; true; i++) {
		char const *maybe = phnumGet(tmp, i);
		if (maybe == NULL) {
			break;
		}
		
		PhoneNumbers *redirection = phfwdGet(pf, maybe);
		if (strcmp(phnumGet(redirection, 0), num) == 0) {
			phnumAdd(result, maybe);
		}
		phnumDelete(redirection);
	}
	
	phnumDelete(tmp);
	
	return result;
}

char const * phnumGet(PhoneNumbers const *pnum, size_t idx) {
	if (pnum == NULL) {
		return NULL;
	}
	else if (pnum->totalNumbers <= idx) {
		return NULL;
	}
	
	PhoneNumbers const *currNode = pnum;
	while (true) {
		if (idx == 0 && currNode->number != NULL) {
			return currNode->number;
		}
		
		if (currNode->number != NULL) {
			idx--;
		}
		
		for (int digit = 0; digit <= MAX_DIGIT; digit++) {
			if (currNode->children[digit] != NULL) {
				if (currNode->children[digit]->totalNumbers <= idx) {
					idx -= currNode->children[digit]->totalNumbers;
				}
				else {
					currNode = currNode->children[digit];
					break;
				}
			}
		}
	}
}
