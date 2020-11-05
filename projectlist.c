#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "projectlist.h"
void initlist(list *l){
	l->decimal = 0;
	l->sign = 1;
	l->head = NULL;
	l->tail = NULL;
}
void insert(list *l, int pos, int val){
	node *x, *tmp;
	int i;

	if(pos < 0 || pos > length(l))
		return;	

	tmp = (node *)malloc(sizeof(node));	
	tmp->val = val;

	/*seperate condition for first node*/
	if(l->head == NULL) {
		tmp->next = NULL; 
		tmp->prev = NULL;
		l->head = l->tail = tmp;
		return;		
	}

	x = l->head;
	for(i = 0; i < pos - 1; i++)
		x = x->next;
	/*if node is to be inserted at front*/
	if(pos == 0) {
		l->head->prev = tmp;
		tmp->next = l->head;
		tmp->prev = NULL;
		l->head = tmp;
		return;
	}
	/*if node is to be inserted at end*/
	else if(pos == length(l)) {
		tmp->prev = l->tail;
		tmp->next = NULL;
		l->tail->next = tmp;
		l->tail = tmp;
		return;
	}/*for all other conditions*/
	else{
		tmp->next = x->next;
		tmp->prev = x;
		tmp->next->prev = tmp;
		x->next = tmp;
		return;
	}

}

void append(list *l, int val) {
	insert(l, length(l), val);
}

int remov(list *l, int pos){
	int i;
	int val;
	node *x, *tmp;
	/* if given position is wrong */	
	if(pos < 0 || pos >= length(l))
		return 0;	
	
	/*for empty list*/
	if(l->head == NULL) {
		return 0;
	}
	
	x = l->head;
	/*for removing last node*/
	if(length(l) == 1) {	
		val = x->val;
		l->head = l->tail = NULL;
		x= NULL;
		return val;
	}

	/*removing node from front*/
	if(pos == 0) {
		val = x->val;
		x->next->prev = NULL;
		l->head = x->next;
		free(x);
		return val;
	}

	/*removing node from back*/
	else if(pos == length(l) - 1) {	
		val = l->tail->val;
		tmp = l->tail;
		l->tail->prev->next = NULL;
		l->tail = l->tail->prev;
		free(tmp);
		return val;
	}
	for(i = 0; i < pos - 1; i++)
		x = x->next;
	tmp = x->next;
	x->next = tmp->next;
	tmp->next->prev = x;	
	val = tmp->val;
	free(tmp);
	return val;
}
int length(list *l) {
	node *tmp;
	int len = 0;
	tmp = l->head;
	while(tmp){
		tmp = tmp->next;
		len++;
	}
	return len;
}

void fprintlist(list l, FILE *fp){
	node *tmp;
	tmp = l.head;
	int d = l.decimal;
	int len, i = 0;
	len = length(&l);
	if(l.sign < 0)
		fprintf(fp, "-");
	while(tmp != NULL){
		if(i == len - d){
			if(len == d)
				fprintf(fp, "0");
			fprintf(fp, ".");
		}
		fprintf(fp, "%d", tmp->val);
		tmp = tmp->next;
		i++;
	}
}

void printlist(list l){
	node *tmp;
	tmp = l.head;
	int d = l.decimal;
	int len, i = 0;
	len = length(&l);
	if(l.sign < 0)
		printf("-");
	while(tmp != NULL){
		if(i == len - d){
			if(len == d)
				printf("0");
			printf(".");
		}
		printf("%d", tmp->val);
		tmp = tmp->next;
		i++;
	}
}

int sign(list l){
	node *tmp;
	tmp = l.head;
	if(tmp->val < 0)
		return -1;
	else
		return 1;
}


void copy(list *l1, list *l2) {
	int dec, i, num, sign;
	initlist(l2);
	for(i = 0; i < length(l1); i++) {
		num = show(l1, i);
		append(l2, num);
	}
	dec = l1->decimal;
	l2->decimal = dec;
	sign = l1->sign;
	l2->sign = sign;
	return;
}

void destroy(list **l){

	list* temp = *l;


    node *p = (*l)->head;
	node *q = NULL;

    while(p){
        q=p;
        p=p->next;
        free(q);
    }
    
	free(*l);
	*l = NULL;

    return;
}

int show(list *l, int pos) {
	node *p;
	if(pos <0 || pos > length(l))
		return 0;
	p = l->head;
	int i;
	for(i = 0; i < pos; i++)
		p = p->next;
	return p->val;
}