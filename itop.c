#include<limits.h>
#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include"itop.h"

typedef struct itop_stack{
    char *a;
    int size;
    int top;
}itop_stack;

void itop_init(itop_stack *s, int len);
void itop_push(itop_stack *s, char d);
int itop_isFull(itop_stack s) ;
int itop_isEmpty(itop_stack s);
char itop_pop(itop_stack *s);
char itop_peek(itop_stack s);

void itop_init(itop_stack *s, int len){
    s->a = (char*)malloc(sizeof(char)*len);
    s->size = len;
    s->top = -1;
}
int itop_isFull(itop_stack s) {
    if(s.top == s.size-1)
        return 1;
    else
        return 0;
}

int itop_isEmpty(itop_stack s) {
    if(s.top == -1)
        return 1;
    else
        return 0;
}
void itop_push(itop_stack *s, char d){
    if(itop_isFull(*s))
        return ;
    else {
        s->top++;
        s->a[s->top] = d;
        return;
    }
}

char itop_pop(itop_stack *s){
    char e = '#';
    if(!itop_isEmpty(*s)){
        e = s->a[s->top];
        s->top--;
    }
    return e;

}

char itop_peek(itop_stack s){

        return s.a[s.top];

}

int precedence(char c){
    if(c == '+')
        return 10;
    else if(c == '-')
        return 10;
    else if(c == '*')
        return 20;
    else if(c == '/')
        return 20;
    else if(c == '%')
        return 20;
    else if(c == '^')
        return 30;
    else if(c == '(')
        return 5;
    return 0;
}

void itop(char* E){


    itop_stack s;
    itop_init(&s,1000);

    char postfix[1000];
    int j = 0;
    char c;
    int i=0;
    while(E[i]!= '\0'){
            c = E[i];
            if(isdigit(c) || c=='.'|| c=='!' || (c=='-' && isdigit(E[i+1]))){
               postfix[j++] = c ;
               if(!(isdigit(E[i+1]) || E[i+1]=='.'|| E[i+1]=='!'))
                    postfix[j++] = ' ';
            }
            else if( c == '(')
                itop_push(&s,'(');
            else if(c == ')'){
                    while(!itop_isEmpty(s) && itop_peek(s)!= '('){
                          postfix[j++] = itop_pop(&s);
                          postfix[j++] = ' ';
                    }
                    if(itop_peek(s) == '(')
                        itop_pop(&s);
            }
            else {
                    if( c == '+' || c =='-' || c == '*'|| c == '/' || c == '^' || c == '%') {
                        if(itop_isEmpty(s))
                            itop_push(&s, c);
                        else {
                            while(!itop_isEmpty(s) && precedence(itop_peek(s)) >= precedence(c)){
                                  postfix[j++] = itop_pop(&s);
                                  postfix[j++] = ' ';
                            }
                            itop_push(&s, c);
                        }
                    }
            }
        i++;
    }

    while(!itop_isEmpty(s)){
        postfix[j++] = itop_pop(&s);
        postfix[j++] = ' ';
    }

    postfix[j] = '\0';
    
    strcpy(E, postfix);
    return ;


}

