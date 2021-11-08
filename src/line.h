/** @file
  Interfejs wczytywania oraz obsługiwania linii dla stosu wielomianów rzadkich wielu zmiennych

  @authors Jakub Jakubowski <jj429267@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#ifndef POLYNOMIALS_LINE_H
#define POLYNOMIALS_LINE_H

#include "stack.h"

/**
 * Wczytuje linie ze standardowego wejścia
 * @param[in] fin : flaga sprawdzający czy doszliśmy do końca pliku
 * @return Linia
 */
char* get_line (int *fin, int line_num);

/**
 * Wywołuje na stosie zapisane w linii polecenie
 * @param[in] line : linia
 * @param[in] line_num : numer linii
 * @param[in] stack : stos wielomianów
 */
void solve_line (char* line, int line_num, struct Stack *stack);


#endif //POLYNOMIALS_LINE_H
