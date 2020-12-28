#ifndef NUMBER_H_INCLUDED
#define NUMBER_H_INCLUDED

#include<math.h>
#define PLUS 1
#define MINUS -1
#define PI 3.1415926535
#include "list.h"

void copy(list *l1, list *l2);
int checkzero(list *l);
int compare(list *l1, list *l2);
double radian(list *l1);
list *addnum(list *l1, list *l2);
list *subnum(list *l1, list *l2);
list *mulnum(list *l1, list *l2);
list *division(list *l1, list *l2);
list *modulus(list *l1, list *l2);
list *power(list *l1, list *l2);
list *sine(list *l);
list *cosine(list *l);
list *tangent(list *l);

#endif // NUMBER_H_INCLUDED
