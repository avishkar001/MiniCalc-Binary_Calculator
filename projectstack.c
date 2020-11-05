#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "projectstack.h"
//#include"projectlist.h"

void init(stack *s) {
	s->i = 0;
}

void push(stack *s, list *l) {
	s->l[s->i]= l;
	(s->i)++;
}

list *pop(stack *s) {
	list *tmp = (list *)malloc(sizeof(list)); 
	tmp = s->l[s->i - 1];
	(s->i)--;
	return tmp;
}

int isempty(stack *s) {
	return s->i == 0;
}

int isfull(stack *s) {
	return s->i == MAX;
}


