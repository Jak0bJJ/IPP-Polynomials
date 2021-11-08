/** @file
  Interfejs klasy stosu wielomianów rzadkich wielu zmiennych

  @authors Jakub Jakubowski <jj429267@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#ifndef POLYNOMIALS_STACK_H
#define POLYNOMIALS_STACK_H

#include "stack.h"
#include "poly.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * To jest struktura przechowująca stos wielomianów.
 */
typedef struct Stack {
    int top; ///< indeks do wierzchołka stosu
    long long cap; ///< ilość zaalokowanej przez stos pamięci
    struct Poly *arr; ///< tablica przechowująca wielomiany znajdujące się na stosie
} Stack;

/**
 * Tworzy nowy Stos.
 * @return stos wielomianów
 */
struct Stack StackCreate();

/**
 * Usuwa stos.
 * @param[in] stack : stos wielomianów
 */
void StackDestroy (struct Stack *stack);

/**
 * Wstawia wielomian na góre stosu.
 * @param[in] stack : stos wielomianów
 * @param[in] p : wielomian
 */
void StackPush (struct Stack *stack, Poly p);

/**
 * Usuwa wielomian z wierzchołka stosu.
 * @param[in] stack : stos wielomianów
 * @param[in] err : bezpiecznik sprawdzający czy da się wykonać działanie
 */
void StackPop (struct Stack *stack, int *err);

/**
 * Wstawia wielomian tożsamościowo równy zeru na góre stosu.
 * @param[in] stack : stos wielomianów
 */
void StackZero (struct Stack *stack);

/**
 * Sprawdza, czy wielomian na wierzchołku stosu jest współczynnikiem
 * – wypisuje na standardowe wyjście 0 lub 1.
 * @param[in] stack : stos wielomianów
 * @param[in] err : bezpiecznik
 * @return Czy wielomian na wierzchołku stosu @p stack jest współczynnikiem?
 */
bool StackIsCoeff (struct Stack *stack, int *err);

/**
 * Sprawdza, czy wielomian na wierzchołku stosu jest tożsamościowo równy zeru
 * – wypisuje na standardowe wyjście 0 lub 1.
 * @param[in] stack : stos wielomianów
 * @param[in] err : bezpiecznik
 * @return Czy wielomian na wierzchołku stosu @p stack jest tożsamościowo równy zeru?
 */
bool StackIsZero (struct Stack *stack, int *err);

/**
 * Wstawia na stos kopię wielomianu z wierzchołka.
 * @param[in] stack : stos wielomianów
 * @param[in] err : bezpiecznik
 */
void StackClone (struct Stack *stack, int *err);

/**
 * Dodaje dwa wielomiany z wierzchu stosu, usuwa je
 * i wstawia na wierzchołek stosu ich sumę.
 * @param[in] stack : stos wielomianów
 * @param[in] err : bezpiecznik
 */
void StackAdd (struct Stack *stack, int *err);

/**
 * Mnoży dwa wielomiany z wierzchu stosu, usuwa je
 * i wstawia na wierzchołek stosu ich iloczyn.
 * @param[in] stack : stos wielomianów
 * @param[in] err : bezpiecznik
 */
void StackMul (struct Stack *stack, int *err);

/**
 * Neguje wielomian na wierzchołku stosu.
 * @param[in] stack : stos wielomianów
 * @param[in] err : bezpiecznik
 */
void StackNeg (struct Stack *stack, int *err);

/**
 * Odejmuje od wielomianu z wierzchołka wielomian pod wierzchołkiem,
 * usuwa je i wstawia na wierzchołek stosu różnicę.
 * @param[in] stack : stos wielomianów
 * @param[in] err : bezpiecznik
 */
void StackSub (struct Stack *stack, int *err);

/**
 * Sprawdza, czy dwa wielomiany na wierzchu stosu są równe –
 * wypisuje na standardowe wyjście 0 lub 1.
 * @param[in] stack : stos wielomianów
 * @param[in] err : bezpiecznik
 * @return Czy dwa wielomiany na wierzchu stosu @p stack są równe?
 */
bool StackIsEq (struct Stack *stack, int *err);

/**
 * Wypisuje na standardowe wyjście stopień wielomianu
 * (−1 dla wielomianu tożsamościowo równego zeru).
 * @param[in] stack : stos wielomianów
 * @param[in] err : bezpiecznik
 * @return Stopień wielomianu na wierzchołku stosu @p stack
 */
poly_exp_t StackDeg (struct Stack *stack, int *err);

/**
 * Wypisuje na standardowe wyjście stopień wielomianu ze względu na zmienną
 * o numerze ind (−1 dla wielomianu tożsamościowo równego zeru).
 * @param[in] stack : stos wielomianów
 * @param[in] err : bezpiecznik
 * @param[in] ind : indeks zmiennej
 * @return Stopień wielomianu na wierzchołku stostu @p stack z względu na zmienną o indeksie @p ind
 */
poly_exp_t StackDegBy (struct Stack *stack, int *err, size_t ind);

/**
 * Wylicza wartość wielomianu w punkcie x, usuwa wielomian
 * z wierzchołka i wstawia na stos wynik operacji.
 * @param[in] stack : stos wielomianów
 * @param[in] err : bezpiecznik
 * @param[in] ind : indeks zmiennej
 */
void StackAt (struct Stack *stack, int *err, poly_coeff_t ind);

/**
 * Wypisuje na standardowe wyjście wielomian z wierzchołka stosu.
 * @param[in] stack : stos wielomianów
 * @param[in] err : bezpiecznik
 * @return Wielomian znajdujący się na wierzchu stosu @p stack
 */
void StackPrint (struct Stack *stack, int *err);


/**
 * Zdejmuje z wierzchołka stosu najpierw wielomian p, a potem kolejno
 * wielomiany q[k - 1], q[k - 2], …, q[0] i umieszcza na stosie wynik
 * operacji złożenia.
 * @param[in] stack : stos wielomianów
 * @param[in] err : bezpiecznik
 * @param[in] k : ilość wielomianów składowych
 */
void StackCompose (struct Stack *stack, int *err, unsigned long long k);


#endif //POLYNOMIALS_STACK_H
