#include"projectlist.h"

#define MAX 100

typedef struct stack{
	int i;
	list *l[MAX];
}stack;

void init(stack *s);
void push(stack *s, list *l);
list *pop(stack *s);
int isempty(stack *s);
int isfull(stack *s);


