/** @file
  Kalkulatora czytający oraz wykonujący dane wierszami ze standardowego wejścia.

  @authors Jakub Jakubowski <jj429267@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#ifdef NDEBUG
#undef NDEBUG
#endif

#include "stack.h"
#include <stdlib.h>
#include "line.h"


int main() {

    char* line;
    int end=0, line_num=1;
    Stack stack = StackCreate();
    while (!end)
    {
        line = get_line(&end, line_num);
        if (line!=NULL)
        {
            solve_line(line, line_num, &stack);
        }
        line_num++;
        free(line);
    }
    StackDestroy(&stack);
    return 0;
}
