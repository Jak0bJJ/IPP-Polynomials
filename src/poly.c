/** @file
  Implementacja klasy wielomianów rzadkich wielu zmiennych

  @authors Jakub Jakubowski <jj429267@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "poly.h"

/**
 * Porównuje 2 jednomiany na podstawie wartości ich wykładników
 * @param[in] a : jednomian
 * @param[in] b : jednomian
 */
static int mono_cmp(const void *a, const void *b)
{
    Mono val1 = *(Mono*)a;
    Mono val2 = *(Mono*)b;
    return (val1.exp - val2.exp);
}

/**
 * Sprawdza, czy pamięć została poprawnie zaalockowana,
 * w przeciwnym wypadku kończy działanie programu.
 * @param[in] p : wskaźnik na zaalockowana pamięć
 */
static void CheckMem (const Mono *tab)
{
    if (tab==NULL)
        exit(1);
}

/**
 * Zwraca maksimum z dwóch liczb typu poly_exp_t.
 * @param[in] a : wykładnik
 * @param[in] b : wykładnik
 * @return maksimum z a i b
 */
static poly_exp_t max (poly_exp_t a, poly_exp_t b)
{
    if (a>b)
        return a;
    else
        return b;
}

/**
 * Zwraca liczbe typu poly_coeff_t podniesiona do potęgi n.
 * @param[in] x : liczba całkowita
 * @param[in] n : wykładnik
 * @return liczba całkowita `x^n`
 */
static poly_coeff_t Power (poly_coeff_t a, poly_exp_t n)
{
    poly_coeff_t w = 1;
    while(n>0)
    {
        if (n%2 == 1)
            w *= a;
        a*= a;
        n/=2;
    }
    return w;
}

void PolyDestroy(Poly *p)
{
    if (p->arr!=NULL)
    {
        for (size_t i=0;i<p->size;++i)
        {
            PolyDestroy(&p->arr[i].p);
        }
        free(p->arr);
    }
}

Poly PolyClone(const Poly *p)
{
    if (!PolyIsCoeff(p))
    {
        Poly new_p ;
        new_p.size=p->size;
        Mono *tab = malloc(p->size*(sizeof (Mono)));
        CheckMem(tab);
        for (size_t i = 0; i < p->size; ++i)
        {
            tab[i] = MonoClone(&p->arr[i]);
        }
        new_p.arr=tab;
        return new_p;
    }
    else
    {
        return (Poly) {.coeff = p->coeff, .arr = NULL};
    }
}

Poly PolyAdd(const Poly *p, const Poly *q)
{
    if (p->arr==NULL && q->arr != NULL)
        return PolyAdd (q,p);
    if (p->arr==NULL && q->arr == NULL)
    {
        return PolyFromCoeff(p->coeff+q->coeff);
    }
    if (q->arr==NULL)
    {
        Poly new_p = PolyClone(p);
        new_p.arr = realloc(new_p.arr, (p->size+1)*(sizeof (Mono)) );
        CheckMem(new_p.arr);
        new_p.size++;
        new_p.arr[p->size] = (Mono) {.p = PolyFromCoeff(q->coeff), .exp = 0};
        return new_p;
    }
    Poly new_p = PolyClone(p);
    new_p.arr = realloc(new_p.arr, (p->size+q->size+1)*(sizeof (Mono)) );
    CheckMem(new_p.arr);
    new_p.size+=q->size;
    for (size_t i=0;i<q->size;++i)
    {
        new_p.arr[p->size+i]=MonoClone(&q->arr[i]);
    }
    return new_p;
}

Poly PolyAddMonos(size_t count, const Mono monos[])
{
    Poly p;
    p.size=count;
    p.arr = malloc(count*(sizeof (Mono)));
    CheckMem(p.arr);
    for (size_t i = 0; i < count; ++i)
    {
        p.arr[i] = monos[i];
    }
    return p;
}

Mono MonoMulCoeff (const Poly *p, const Mono *q)
{
    assert(PolyIsCoeff(p));
    Mono result;
    result.exp=q->exp;
    result.p =  PolyMul(&q->p,p);
    return result;
}

Mono* PolyMulMono (const Poly *p, const Mono *q)
{
    Mono* result = malloc(p->size*(sizeof (Mono)));
    CheckMem(result);
    for (size_t i=0; i<p->size; ++i)
    {
        result[i] = (Mono) {.p = PolyMul(&p->arr[i].p, &q->p),.exp = p->arr[i].exp + q->exp};
    }
    return result;
}

Poly PolyMul(const Poly *p, const Poly *q)
{
    if (PolyIsCoeff(p) && PolyIsCoeff(q))
    {
        return (Poly) {.coeff = p->coeff * q->coeff, .arr = NULL};
    }
    if (PolyIsCoeff(p) && !PolyIsCoeff(q))
    {
        return PolyMul(q,p);
    }
    if (PolyIsCoeff(q))
    {
        if (PolyIsZero(q))
        {
            return PolyZero();
        }
        Poly result;
        result.size = p->size;
        result.arr = malloc(result.size*(sizeof (Mono)));
        CheckMem(result.arr);
        for (size_t i=0;i<p->size;++i)
        {
            result.arr[i] = MonoMulCoeff(q,&p->arr[i]);
        }
        return result;
    }
    Poly result;
    result.size = p->size * q->size;
    result.arr = malloc(result.size*(sizeof (Mono)));
    CheckMem(result.arr);
    size_t iterator = 0;
    for (size_t i=0; i<q->size; ++i)
    {
        Mono* tab = PolyMulMono(p, &q->arr[i]);
        for (size_t j=0;j<p->size;++j)
        {
            result.arr[iterator]=tab[j];
            iterator++;
        }
        free(tab);
    }
    return result;
}

Poly PolyNeg(const Poly *p)
{
    return PolyMul (p, &((Poly) {.coeff = (-1), .arr = NULL}) );
}

Poly PolySub(const Poly *p, const Poly *q)
{
    Poly result, new_q = PolyNeg(q);
    result=PolyAdd(p, &new_q );
    PolyDestroy(&new_q);
    return result;
}

poly_exp_t PolyDegBy(const Poly *p, size_t var_idx)
{

    if (var_idx==0)
    {
        if (PolyIsZero(p))
        {
            return -1;
        }
        if (PolyIsCoeff(p))
        {
            return 0;
        }
        poly_exp_t max_val=0;
        for (size_t i=0;i<p->size;++i)
        {
            //  if (p->arr[i].p.arr == NULL)
            max_val = max (max_val, p->arr[i].exp);
        }
        return max_val;
    }
    else
    {
        if (PolyIsZero(p))
        {
            return -1;
        }
        if (PolyIsCoeff(p))
        {
            return 0;
        }
        poly_exp_t max_val=0;
        for (size_t i=0;i<p->size;++i)
        {
            if (p->arr[i].p.arr != NULL)
                max_val = max (max_val, PolyDegBy(&p->arr[i].p, var_idx-1));
        }
        return max_val;
    }
}

 poly_exp_t PolyDeg(const Poly *p)
{
    if (PolyIsZero(p))
    {
        return -1;
    }
    if (PolyIsCoeff(p))
    {
        return 0;
    }
    poly_exp_t max_val=0;
    for (size_t i=0;i<p->size;++i)
    {
        max_val = max (max_val, PolyDeg(&p->arr[i].p) + p->arr[i].exp);
    }
    return max_val;
}


void PolyStandarize (Poly *p)
{
    if (!PolyIsCoeff(p))
    {
        /** Sortujemy jednomiany po ich wykładnikach */
        qsort(p->arr,p->size, sizeof (Mono), mono_cmp);
        /** Pętla dodająca do siebie sąsiednie jednomiany,
         * jeśli mają te same wykładniki */
        for (size_t i=1;i<p->size;++i)
        {
            if (p->arr[i-1].exp==p->arr[i].exp)
            {
                Poly temp =  PolyAdd( &p->arr[i].p, &p->arr[i-1].p);
                PolyDestroy(&p->arr[i].p);
                PolyDestroy(&p->arr[i-1].p);
                p->arr[i].p = temp;
                p->arr[i-1].exp=-1;
            }
        }
        /** Pętla usuwający z listy jednomianów te, które są zerowe
         * lub zostały dodane do innych jednomianów w poprzedniej pętli*/
        for (size_t i=0;i<p->size;++i)
        {
            if (p->arr[p->size-1].exp==-1 || PolyIsZero(&p->arr[p->size-1].p) )
            {
                p->size--;
                i--;
                continue;
            }
            else if (p->arr[i].exp==-1 || PolyIsZero(&p->arr[i].p ) )
            {
                p->arr[i] = p->arr[p->size-1];
                p->size--;
            }
        }
        /** Zmiana zaalokowanej przez wielomian pamięci */
        p->arr = realloc(p->arr, p->size * sizeof(Mono));
        /** Rekurencyjne wywołanie standaryzacji dla współczynników
         * jednomianów znajdujących się na liście */
        for (size_t i=0; i<p->size;++i)
        {
            if (p->arr[i].p.arr!= NULL)
                PolyStandarize(&p->arr[i].p);
        }
        /** Pętla usuwający z listy jednomianów te, które
         * zostały wyzerowane w skutek rekurencyjnych wywołan */
        for (size_t i=0;i < p->size; ++i)
        {
            if ( PolyIsZero(&p->arr[p->size-1].p) )
            {
                p->size--;
                i--;
                continue;
            }
            if ( PolyIsZero(&p->arr[i].p ) )
            {
                p->arr[i] = p->arr[p->size-1];
                p->size--;
            }
        }
        /** Jeśli w liście jednomianów nie pozostał żaden jednomian
          zmieniamy w wielomian, w taki który jest tożsamościowo równy zero */
        if (p->size==0)
        {
            PolyDestroy(p);
            p->arr=NULL;
            p->coeff=0;
            return;
        }
        /** Jeżeli w liście jednomianów pozostał jeden jednomian, którego
         * współczynnik jest stała, a wykładnik jest równy zero, to zmieniamy
         * wielomian w stałą */
        if (p->size==1 && p->arr[0].exp==0 && p->arr[0].p.arr == NULL)
        {
            poly_coeff_t temp = p->arr[0].p.coeff;
            PolyDestroy(p);
            p->arr=NULL;
            p->coeff=temp;
            return;
        }
        /** Pętla usuwający z listy jednomianów te, które
         * zostały wyzerowane w skutek rekurencyjnych wywołan */
        /** Zmiana zaalokowanej przez wielomian pamięci */
        p-> arr = realloc(p->arr, p->size * sizeof(Mono));
        /** Posortowanie jednomianów w liście po ich wykładnikach */
        qsort(p->arr,p->size, sizeof (Mono), mono_cmp);
    }
}

bool PolyIsEq(const Poly *p, const Poly *q)
{
    Poly new_p = PolyClone(p);
    Poly new_q = PolyClone(q);
    PolyStandarize(&new_p);
    PolyStandarize(&new_q);
    if (new_q.arr == NULL && new_p.arr != NULL)
    {
        PolyDestroy(&new_p);
        PolyDestroy(&new_q);
        return false;
    }
    if (new_p.arr==NULL)
    {
        if (new_q.arr != NULL || new_p.coeff != new_q.coeff)
        {
            PolyDestroy(&new_p);
            PolyDestroy(&new_q);
            return false;
        }
        else
        {
            PolyDestroy(&new_p);
            PolyDestroy(&new_q);
            return true;
        }
    }
    if (new_q.arr==NULL)
    {
        PolyDestroy(&new_p);
        PolyDestroy(&new_q);
        return false;
    }
    if (new_p.size!=new_q.size)
    {
        PolyDestroy(&new_p);
        PolyDestroy(&new_q);
        return false;
    }
    for (size_t i=0;i<new_p.size;++i)
    {
        if (new_p.arr[i].exp!=new_q.arr[i].exp)
        {
            PolyDestroy(&new_p);
            PolyDestroy(&new_q);
            return false;
        }
        if (PolyIsEq(&(new_p.arr[i].p),&(new_q.arr[i].p))==false)
        {
            PolyDestroy(&new_p);
            PolyDestroy(&new_q);
            return false;
        }
    }
    PolyDestroy(&new_p);
    PolyDestroy(&new_q);
    return true;
}

Poly PolyAt(const Poly *p, poly_coeff_t x)
{
    if (PolyIsCoeff(p))
    {
        return PolyFromCoeff(p->coeff);
    }
    Poly new_p = {.size = 0, .arr = malloc( (sizeof (Mono)) )};
    CheckMem(p->arr);
    size_t j=0;
    for (size_t i=0; i<p->size;++i)
    {
        Poly temp = PolyMul(&p->arr[i].p, &((Poly) {.coeff = (Power(x,p->arr[i].exp)), .arr = NULL}));
        if (PolyIsCoeff(&temp))
        {
            new_p.arr = realloc(new_p.arr, (new_p.size + 1) * (sizeof(Mono)));
            CheckMem(new_p.arr);
            CheckMem(new_p.arr);
            new_p.size++;
            new_p.arr[j]= (Mono) {.p = temp, .exp=0};
            ++j;
        }
        else
        {
            new_p.arr = realloc(new_p.arr, (new_p.size + temp.size) * (sizeof(Mono)));
            CheckMem(new_p.arr);
            new_p.size = new_p.size + temp.size;
            for (size_t k = 0; k < temp.size; ++k) {
                new_p.arr[j] = MonoClone(&temp.arr[k]);
                ++j;
            }
            PolyDestroy(&temp);
        }
    }
    return new_p;
}

void PrintMono (Mono *m)
{
    printf("(");
    PrintPoly(&m->p);
    printf(",%d)", m->exp);
}

void PrintPoly (Poly *p)
{
    if (PolyIsCoeff(p))
    {
        printf("%ld", p->coeff);
    }
    else
    {
        PrintMono(&p->arr[0]);
        for (size_t i=1;i<p->size;++i)
        {
            printf("+");
            PrintMono(&p->arr[i]);
        }
    }
}

Mono MonoFromStr (char *line)
{
    if (strlen(line)<3)
        return MonoErr();
    if (line[0]!='(' || line[strlen(line)-1] != ')')
        return MonoErr();
    /** Usuwanie zewnętrznych nawiasów */
    Mono res;
    line++;
    line[strlen(line)-1]=0;
    size_t i=0;
    int counter=0;
    /** Pętla szukająca przecinka nalężącego do procesowanego jednomianu */
    while (i<strlen(line))
    {
        if (line[i]=='(')
            counter++;
        else if (line[i]==')')
            counter--;
        else if (line[i]==',' && counter==0)
            break;
        else if (line[i]!='-' && line[i]!='+' && line[i]!=',' && (line[i]<'0' || line[i]>'9'))
            return MonoErr();
        ++i;
    }
    if (i==strlen(line))
        return MonoErr();
    line[i]='\31';
    /** Dzielenie tekstu na ten odpowiedzialny za wykładnik,
     * oraz ten odpowiedzialny za współczynnik */
    char* store = NULL;
    store = strtok(line,"\31");
    char* polystore = store;
    if (polystore==NULL)
    {
        return MonoErr();
    }
    store = strtok(NULL, "\31");
    if (store==NULL)
    {
        return MonoErr();
    }
    /** Wyciąganie wykładnika z tekstu */
    char* pEnd;
    long long ll = strtoll(store, &pEnd, 10);
    char* new_num = malloc(30);
    if (new_num == NULL)
        exit(1);
    sprintf(new_num, "%lld", ll);
    if (strcmp(store,new_num)!=0)
    {
        free(new_num);
        return MonoErr();
    }
    free(new_num);
    if (ll>=2147483648 || ll <0)
    {
        return MonoErr();
    }
    res.exp=ll;
    /** Wyciąganie współczynnika z tekstu */
    Poly p = PolyFromStr(polystore);
    if (PolyIsErr(&p))
    {
        PolyDestroy(&p);
        return MonoErr();
    }
    res.p=p;
    return res;
}

Poly PolyFromStr (char *line)
{
    /** Przypadek, gdy wielomian jest stałą */
    if (line[0]!='(')
    {
        char* pEnd;
        if (strcmp(line, "-0")==0)
        {
            return PolyZero();
        }
        long long ll = strtoll(line, &pEnd, 10);
        char* new_num = malloc(30);
        if (new_num == NULL)
            exit(1);
        sprintf(new_num, "%lld", ll);
        if (strcmp(line,new_num)!=0)
        {
            free(new_num);
            return PolyErr();
        }
        free(new_num);
        return PolyFromCoeff(ll);
    }
    /** Przypadek ze skrajnymi plusami */
    if (line[0]=='+'||line[strlen(line)-1]=='+')
    {
        return PolyErr();
    }
    size_t i=0;
    int counter=0;
    /** Znajdowanie wszystkich plusów procesowanego wielomianu */
    while (i<strlen(line))
    {
        if (line[i]=='(')
            counter++;
        else if (line[i]==')')
            counter--;
        else if (line[i]=='+' && counter==0)
        {
            line[i]='\31';
        }
        else if (line[i]!='-' && line[i]!='+' && line[i]!=',' && (line[i]<'0' || line[i]>'9'))
            return PolyErr();
        ++i;
    }
    char* store = NULL;
    char** t = NULL;
    int size = 0;
    store = strtok(line,"\31");
    /** Dzielenie tekstu na słowa definiujące jednomiany składające się na wielomian */
    while (store!=NULL)
    {
        t = realloc(t, (size+1) * (sizeof (char *)));
        if (t == NULL)
            exit(1);
        t[size]=malloc(strlen(store)+1);
        if (t[size]==NULL)
            exit(1);
        strcpy(t[size],store);
        size++;
        store = strtok(NULL,"\31");
    }
    Poly p;
    p.size=size;
    p.arr=malloc(sizeof (Mono)* size);
    if (p.arr==NULL)
        exit(1);
    int err=0;
    /** Zmienianie słów na jednomiany, a następnie łączenie je w wielomian */
    for (int j=0;j<size;++j)
    {
        p.arr[j]=MonoFromStr(t[j]);
        if (MonoIsErr(&p.arr[j]))
            err=1;
        free(t[j]);
    }
    free(t);
    if (err)
    {
        PolyDestroy(&p);
        return PolyErr();
    }
    else
        return p;
}

Poly PolyCloneMonos(size_t count, const Mono monos[])
{
    if (count==0 || monos==NULL)
    {
        return PolyZero();
    }
    else
    {
        Poly p;
        p.size=count;
        p.arr = malloc(count*(sizeof (Mono)));
        CheckMem(p.arr);
        for (size_t i = 0; i < count; ++i)
        {
            p.arr[i] = MonoClone(&monos[i]);
        }
        return p;
    }
}

Poly PolyOwnMonos(size_t count, Mono *monos)
{
    if (count==0 || monos==NULL)
    {
        return PolyZero();
    }
    else
    {
        Poly p;
        p.size = count;
        p.arr = monos;
        return p;
    }
}

Poly PolyPower (const Poly *p, poly_exp_t n)
{
    if (n==0)
    {
        return PolyFromCoeff(1);
    }
    else if (n==1)
    {
        return PolyClone(p);
    }
    Poly res = PolyMul(p,p);
    for (poly_exp_t i=2; i<n; ++i)
    {
        Poly new_res = PolyMul(&res, p);
        PolyDestroy(&res);
        res = new_res;
    }
    return res;
}


Poly PolyComp (const Poly *p, size_t ind, size_t k, const Poly q[]);

/**
 * Funkcja pomocznicza do Poly Compose składająca jednomian
 * @param[in] m : jednomian
 * @param[in] ind : indeks aktualnie składanego wielomianu z tablicy q
 * @param[in] k : długość listy wielomianów składanych
 * @param[in] q : tablica wielomianów składanych
 * @return Złożony wielomian
 */
Poly MonoComp (const Mono *m, size_t ind, size_t k, const Poly q[])
{
    if (ind >= k)
    {
        if (m->exp!=0)
        {
            return PolyZero();
        }
        else
        {
            return PolyComp(&m->p, ind+1, k, q);
        }
    }
    Poly new_p = PolyComp(&m->p, ind+1, k, q);
    Poly pow = PolyPower(&q[ind], m->exp);
    Poly res = PolyMul(&new_p, &pow);
    PolyDestroy(&pow);
    PolyDestroy(&new_p);
    return res;
}

/**
 * Funkcja pomocznicza do Poly Compose składająca wielomian
 * z ind-ym wielomianem z tablicy
 * @param[in] p : wielomian
 * @param[in] ind : indeks aktualnie składanego wielomianu z tablicy q
 * @param[in] k : długość listy wielomianów składanych
 * @param[in] q : tablica wielomianów składanych
 * @return Złożony wielomian
 */
Poly PolyComp (const Poly *p, size_t ind, size_t k, const Poly q[])
{
    if (PolyIsCoeff(p))
    {
        return PolyClone(p);
    }
    else
    {
        Poly res = PolyZero();
        for (size_t i=0 ; i<p->size; ++i)
        {
            Poly new_monos = MonoComp(&p->arr[i], ind, k,q);
            Poly new_res = PolyAdd(&res, &new_monos);
            PolyDestroy(&res);
            PolyDestroy(&new_monos);
            res = new_res;
        }
        return res;
    }

}

Poly PolyCompose(const Poly *p, size_t k, const Poly q[])
{
    size_t ind = 0;
    return PolyComp (p, ind , k, q);
}