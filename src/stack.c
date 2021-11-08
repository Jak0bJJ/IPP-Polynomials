/** @file
  Implementacja klasy stosu wielomianów rzadkich wielu zmiennych

  @authors Jakub Jakubowski <jj429267@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#include "stack.h"
#include "poly.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

struct Stack StackCreate()
{
    struct Stack stack;
    stack.top=-1;
    stack.cap=1;
    stack.arr = malloc(sizeof (Poly));
    if (stack.arr==NULL)
        exit(1);
    return stack;
}

void StackDestroy (struct Stack *stack)
{
    while (stack->top!=-1)
    {
        PolyDestroy(&stack->arr[stack->top]);
        stack->top--;
    }
    free(stack->arr);
}

void StackPush (struct Stack *stack, Poly p)
{
    if (stack->top+1>=stack->cap)
    {
        stack->cap*=2;
        stack->arr = realloc(stack->arr, stack->cap*(sizeof (Poly)));
        if (stack->arr == NULL)
            exit(1);
    }
    stack->top++;
    PolyStandarize(&p);
    stack->arr[stack->top] = p;
}

void StackPop (struct Stack *stack, int *err)
{
    if (stack->top== -1)
    {
        *err = 1;
        return;
    }
    PolyDestroy(&stack->arr[stack->top]);
    stack->top--;
}

void StackZero (struct Stack *stack)
{
    StackPush(stack, PolyZero());
}

bool StackIsCoeff (struct Stack *stack, int *err)
{
    if (stack->top== -1)
    {
        *err = 1;
        return false;
    }
    return PolyIsCoeff(&stack->arr[stack->top]);
}

bool StackIsZero (struct Stack *stack, int *err)
{
    if (stack->top== -1)
    {
        *err = 1;
        return false;
    }
    return PolyIsZero(&stack->arr[stack->top]);
}

void StackClone (struct Stack *stack, int *err)
{
    if (stack->top== -1)
    {
        *err = 1;
        return;
    }
    StackPush(stack, PolyClone(&stack->arr[stack->top]));
}

void StackAdd (struct Stack *stack, int *err)
{
    if (stack->top < 1)
    {
        *err = 1;
        return;
    }
    Poly new_poly = PolyAdd(&stack->arr[stack->top], &stack->arr[stack->top-1]);
    StackPop(stack, err);
    StackPop(stack, err);
    StackPush(stack, new_poly);
}

void StackMul (struct Stack *stack, int *err)
{
    if (stack->top < 1)
    {
        *err = 1;
        return;
    }
    Poly new_poly = PolyMul(&stack->arr[stack->top], &stack->arr[stack->top-1]);
    StackPop(stack, err);
    StackPop(stack, err);
    StackPush(stack, new_poly);
}

void StackNeg (struct Stack *stack, int *err)
{
    if (stack->top== -1)
    {
        *err = 1;
        return;
    }
    Poly new_poly = PolyNeg(&stack->arr[stack->top]);
    StackPop(stack, err);
    StackPush(stack, new_poly);
}

void StackSub (struct Stack *stack, int *err)
{
    if (stack->top < 1)
    {
        *err = 1;
        return;
    }
    Poly new_poly = PolySub(&stack->arr[stack->top], &stack->arr[stack->top-1]);
    StackPop(stack, err);
    StackPop(stack, err);
    StackPush(stack, new_poly);
}

bool StackIsEq (struct Stack *stack, int *err)
{
    if (stack->top < 1)
    {
        *err = 1;
        return false;
    }
    return PolyIsEq(&stack->arr[stack->top], &stack->arr[stack->top-1]);
}

poly_exp_t StackDeg (struct Stack *stack, int *err)
{
    if (stack->top== -1)
    {
        *err = 1;
        return 0;
    }
    return PolyDeg(&stack->arr[stack->top]);
}

poly_exp_t StackDegBy (struct Stack *stack, int *err, size_t ind)
{
    if (stack->top== -1)
    {
        *err = 1;
        return 0;
    }
    return PolyDegBy(&stack->arr[stack->top], ind);
}

void StackAt (struct Stack *stack, int *err, poly_coeff_t ind)
{
    if (stack->top== -1)
    {
        *err = 1;
        return;
    }
    Poly new_poly = PolyAt(&stack->arr[stack->top], ind);
    StackPop(stack, err);
    StackPush(stack, new_poly);
}

void StackPrint (struct Stack *stack, int *err)
{
    if (stack->top== -1)
    {
        *err = 1;
        return;
    }
    PolyStandarize(&stack->arr[stack->top]);
    PrintPoly(&stack->arr[stack->top]);
    printf("\n");
}

void StackCompose (struct Stack *stack, int *err, unsigned long long k)
{
    unsigned long long ull_stack = stack->top;
    if (ull_stack < k)
    {
        *err = 1;
        return;
    }
    Poly p = stack->arr[stack->top];
    Poly q[k];
    for (unsigned long long i=0; i<k; ++i)
    {
        q[k-1-i]=stack->arr[stack->top-1-i];
    }
    Poly res = PolyCompose(&p, k, q);
    for (unsigned long long i=0; i<=k; ++i)
        StackPop(stack,err);
    StackPush(stack, res);
}