/** @file
  Implementacja wczytywania oraz obsługiwania linii dla stosu wielomianów rzadkich wielu zmiennych

  @authors Jakub Jakubowski <jj429267@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#include "stack.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* get_line (int *fin, int line_num)
{
    int ch;
    char* line = malloc(sizeof (char));
    bool comment = false, cmd = false, err = false;
    int size = 0;
    int cap = 1;
    while (((ch = getchar()) != '\n') && (ch != EOF))
    {
        if (size==0 && ((ch>64 && ch<91) || (ch>96 && ch<123)) )
        {
            cmd=true;
        }
        //jeśli linia jest komentarzem
        if (size==0 && ch=='#')
        {
            comment=true;
        }
        if (ch=='\0')
        {
            err=true;
        }
        //zwiększamy przydział pamięci
        if (size+1>=cap)
        {
            cap*=2;
            if( (line = realloc(line, cap)) == NULL )
            {
                exit(1);
            }
        }
        line[size] = (char) ch;
        size++;
    }
    if (comment)
    {
        free(line);
        return NULL;
    }
    if (err)
    {
        if (cmd)
        {
            fprintf(stderr, "ERROR %d WRONG COMMAND\n", line_num);
        }
        else
        {
            fprintf (stderr, "ERROR %d WRONG POLY\n", line_num);
        }
        free(line);
        return NULL;
    }
    if (ch==EOF)
    {
        *fin = 1;
    }
    if (size==0)
    {
        free(line);
        return NULL;
    }

    line[size] = '\0';
    return line;
}

void solve_line (char* line, int line_num, struct Stack *stack)
{
    //  printf("line: %s\n", line);
    char* store = NULL;
    int space_count=0;
    for (size_t i=0;i<strlen(line);++i)
    {
        if (line[i]==' ')
            space_count++;
    }
    if (line[0]==' ')
    {
        fprintf(stderr, "ERROR %d WRONG POLY\n", line_num);
        return;
    }

    store = strtok(line," ");
    int err = 0;
    if ((line[0]>64 && line[0]<91) || (line[0]>96 && line[0]<123)) {
        if (strcmp(store, "ZERO") == 0)
        {
            store = strtok(NULL," ");
            if (store!=NULL || space_count!=0)
                fprintf(stderr, "ERROR %d WRONG COMMAND\n", line_num);
            else
                StackZero(stack);
        }
        else if (strcmp(store, "IS_COEFF") == 0)
        {
            store = strtok(NULL," ");
            if (store!=NULL || space_count!=0)
                fprintf(stderr, "ERROR %d WRONG COMMAND\n", line_num);
            else
            {
                int res = StackIsCoeff(stack, &err);
                if (!err)
                    printf("%d\n",res);
            }
        }
        else if (strcmp(store, "IS_ZERO") == 0)
        {
            store = strtok(NULL," ");
            if (store!=NULL || space_count!=0)
                fprintf(stderr, "ERROR %d WRONG COMMAND\n", line_num);
            else
            {
                int res = StackIsZero(stack, &err);
                if (!err)
                    printf("%d\n",res);
            }
        }
        else if (strcmp(store, "CLONE") == 0)
        {
            store = strtok(NULL," ");
            if (store!=NULL || space_count!=0)
                fprintf(stderr, "ERROR %d WRONG COMMAND\n", line_num);
            else
                StackClone(stack, &err);
        }
        else if (strcmp(store, "ADD") == 0)
        {
            store = strtok(NULL," ");
            if (store!=NULL || space_count!=0)
                fprintf(stderr, "ERROR %d WRONG COMMAND\n", line_num);
            else
                StackAdd(stack, &err);
        }
        else if (strcmp(store, "MUL") == 0)
        {
            store = strtok(NULL," ");
            if (store!=NULL || space_count!=0)
                fprintf(stderr, "ERROR %d WRONG COMMAND\n", line_num);
            else
                StackMul(stack, &err);
        }
        else if (strcmp(store, "NEG") == 0)
        {
            store = strtok(NULL," ");
            if (store!=NULL || space_count!=0)
                fprintf(stderr, "ERROR %d WRONG COMMAND\n", line_num);
            else
                StackNeg(stack, &err);
        }
        else if (strcmp(store, "SUB") == 0)
        {
            store = strtok(NULL," ");
            if (store!=NULL || space_count!=0)
                fprintf(stderr, "ERROR %d WRONG COMMAND\n", line_num);
            else
                StackSub(stack, &err);
        }
        else if (strcmp(store, "IS_EQ") == 0)
        {
            store = strtok(NULL," ");
            if (store!=NULL || space_count!=0)
                fprintf(stderr, "ERROR %d WRONG COMMAND\n", line_num);
            else
            {
                int res = StackIsEq(stack, &err);
                if (!err)
                    printf("%d\n", res);
            }
        }
        else if (strcmp(store, "DEG") == 0)
        {
            store = strtok(NULL," ");
            if (store!=NULL || space_count!=0)
                fprintf(stderr, "ERROR %d WRONG COMMAND\n", line_num);
            else
            {
                poly_exp_t res = StackDeg(stack, &err);
                if (!err)
                    printf("%d\n", res);
            }
        }
        else if (strcmp(store, "DEG_BY") == 0)
        {
            store = strtok(NULL, " ");
            if (store==NULL)
            {
                fprintf(stderr, "ERROR %d DEG BY WRONG VARIABLE\n", line_num);
                return;
            }
            char *pEnd;
            unsigned long long ll = strtoull(store, &pEnd, 10);
            char *new_num = malloc(30);
            if (new_num == NULL)
                exit(1);
            sprintf(new_num, "%llu", ll);
            if (strcmp(store, new_num) != 0)
            {
                fprintf(stderr, "ERROR %d DEG BY WRONG VARIABLE\n", line_num);
                free(new_num);
                return;
            }
            free(new_num);
            store = strtok(NULL, " ");
            if (store!=NULL || space_count!=1)
                fprintf(stderr, "ERROR %d DEG BY WRONG VARIABLE\n", line_num);
            else
            {
                poly_exp_t res = StackDegBy(stack, &err, ll);
                if (!err)
                    printf("%d\n",res);
            }
        }
        else if (strcmp(store, "AT") == 0)
        {
            store = strtok(NULL, " ");
            if (store==NULL)
            {
                fprintf(stderr, "ERROR %d AT WRONG VALUE\n", line_num);
                return;
            }
            char *pEnd;
            unsigned long long ll = strtoull(store, &pEnd, 10);
            char *new_num = malloc(30);
            if (new_num == NULL)
                exit(1);
            sprintf(new_num, "%llu", ll);
            if (strcmp(store, new_num) != 0)
            {
                free(new_num);
                fprintf(stderr, "ERROR %d AT WRONG VALUE\n", line_num);
                return;
            }
            free(new_num);
            store = strtok(NULL, " ");
            if (store!=NULL || space_count!=1)
                fprintf(stderr, "ERROR %d AT WRONG VALUE\n", line_num);
            else
            {
                StackAt(stack, &err, ll);
            }
        }
        else if (strcmp(store, "COMPOSE") == 0)
        {
            store = strtok(NULL, " ");
            if (store==NULL)
            {
                fprintf(stderr, "ERROR %d COMPOSE WRONG PARAMETER\n", line_num);
                return;
            }
            char *pEnd;
            unsigned long long ll = strtoull(store, &pEnd, 10);
            char *new_num = malloc(30);
            if (new_num == NULL)
                exit(1);

            sprintf(new_num, "%llu", ll);
            if (strcmp(store, new_num) != 0)
            {
                free(new_num);
                fprintf(stderr, "ERROR %d COMPOSE WRONG PARAMETER\n", line_num);
                return;
            }
            free(new_num);
            store = strtok(NULL, " ");
            if (store!=NULL || space_count!=1)
                fprintf(stderr, "ERROR %d COMPOSE WRONG PARAMETER\n", line_num);
            else
            {   //printf("Przeszlo line\n");
                StackCompose(stack, &err, ll);
            }
        }
        else if (strcmp(store, "PRINT") == 0)
        {
            store = strtok(NULL," ");
            if (store!=NULL || space_count!=0)
                fprintf(stderr, "ERROR %d WRONG COMMAND\n", line_num);
            else
                StackPrint(stack, &err);
        }
        else if (strcmp(store, "POP") == 0)
        {
            store = strtok(NULL," ");
            if (store!=NULL || space_count!=0)
                fprintf(stderr, "ERROR %d WRONG COMMAND\n", line_num);
            else
                StackPop(stack, &err);
        }
        else
        {
            fprintf(stderr, "ERROR %d WRONG COMMAND\n", line_num);
        }
    }
    else
    {
        char* store_ = store;
        store=strtok(NULL," ");
        if (store!=NULL||space_count!=0)
        {
            fprintf (stderr, "ERROR %d WRONG POLY\n", line_num);
            return;
        }
        Poly p = PolyFromStr(store_);
        if (PolyIsErr(&p))
        {
            PolyDestroy(&p);
            fprintf (stderr, "ERROR %d WRONG POLY\n", line_num);
        }
        else
        {
            StackPush(stack,p);
        }
    }
    if (err)
        fprintf (stderr, "ERROR %d STACK UNDERFLOW\n", line_num);
}

#include "line.h"
