#include"projectstack.h"
#define DECIMAL_PRECISION 10

int iszero(list *l);
int greater(list *l1, list *l2);
list *add(list *l1, list *l2);
list *substract(list *l1, list *l2);
list *multiply(list *l1, list *l2);
list *divide(list *l1, list *l2);
list *modulus(list *l1, list *l2);
list *factorial(list *l1);

double list_to_double(list *l);
list *power(list *l1, list *l2);
list *get_2pi();
list *get_360();

double sine(list *l);
double cosine(list *l);
double tangent(list *l);
double cot(list *l);
double sec(list *l);
double cosec(list *l);
double sine_d(list *l);
double cosine_d(list *l);
double tangent_d(list *l);
double cot_d(list *l);
double sec_d(list *l);
double cosec_d(list *l);
