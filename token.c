#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include"projectfunct.h"
#include"itop.h"


#define OPERATOR	1
#define	OPERAND		2
#define	END		3
#define	ERROR		4
#define FACTORIAL	5


typedef struct token{
	int type;
	int num;
	char op;
	list *l;
	int decflag;
}token;



void addtolist(token t, char ch){
	int i;
	i = ch - '0';
	if(i >=0 && i<= 9){
		append(t.l, i);
	}	
	if(t.decflag == 1)
		t.l->decimal++;
}

int readline(char *arr, int n) {
	char ch;
	int i = 0;
	while((ch = getchar()) != '\n' && i < n){
		arr[i++] = ch;
	}
	arr[i] = '\0';
	return i;
}


enum states {START, DIG, OP, STOP, ERR, SPC};
                  
token getnext(char *str) {
	static int currstate = START;
	int nextstate;
	static int i = 0;
	token t;
	t.l = (list*)malloc(sizeof(list));
	int sum = 0, setdecimal = 0;
	char currchar, currop;
	while(1) {
		currchar = str[i];
		switch(currstate) {
			case START:
				switch(currchar) {
					case '0': case '1': case '2': case '3': case '4': 
					case '5': case '6': case '7': case '8': case '9':
						nextstate = DIG;
						initlist(t.l);
						addtolist(t, currchar);
						break;

					case '+': case '-': case '*': case '/': case '%':case '^':
						nextstate = OP;
						currop = currchar;
						break;

					case ' ': case '\t':
						nextstate = SPC;
						break;

					case '\0':case '\n':
						nextstate = STOP;
			            		break;

					case '.':
						setdecimal = 1;
						nextstate = DIG;
						break;

					default:
						break;
				}
				break;
			case DIG:
				switch(currchar) {
					case '0': case '1': case '2': case '3': case '4': 
					case '5': case '6': case '7': case '8': case '9':
						nextstate = DIG;
						addtolist(t, currchar);
						if(setdecimal == 1)
							t.l->decimal++;
						break;

					case '+': case '-': case '*': case '/': case '%': case '^':
						t.type = OPERATOR;
						nextstate = OP;
						currop = currchar;
						break;

					case ' ': case '\t':
						nextstate = SPC;
						t.type = OPERAND;
						currstate = nextstate;
						return t;	
						break;

					case '\0':case '\n':
						nextstate = END;
						t.type = OPERAND;
						i++;
						currstate = nextstate;
						return t;	
						break;

					case '.':
						setdecimal = 1;
						nextstate = DIG;
						break;
					case '!':
						nextstate = SPC;
						t.type = FACTORIAL;
						i++;
						currstate = nextstate;
						return t;
						break;
					default:
						nextstate = ERR;
						t.type = OPERAND;
						i++;
						currstate = nextstate;
						return t;	
						break;
				}
				break;
			case SPC:
				switch(currchar) {
					case '0': case '1': case '2': case '3': case '4':
					case '5': case '6': case '7': case '8': case '9':
						nextstate = DIG;
						initlist(t.l);
						addtolist(t, currchar);
						if(setdecimal == 1)
							t.l->decimal++;
						break;

					case '+': case '-': case '*': case '/': case '%': case '^':case '!':
						t.type = OPERATOR;
						nextstate = OP;
						currop = currchar;
						break;

					case ' ': case '\t':
						nextstate = SPC;
						break;
					case '\0':
						nextstate = STOP;
						break;
					case '.':
						setdecimal = 1;
						nextstate = DIG;
						break;
					default:
						nextstate = ERR;
						break;
				}
				break;
			case OP:
				switch(currchar) {
					case '0': case '1': case '2': case '3': case '4': 
					case '5': case '6': case '7': case '8': case '9':
						nextstate = DIG;
						initlist(t.l);
						if(currop == '-')
							t.l->sign = -1;
						addtolist(t, currchar);
						if(setdecimal == 1)
							t.l->decimal++;
						t.type = OPERAND;
						currstate = nextstate;
						break;
					case '+': case '-': case '*': case '/': case '%': case '!':case '^':
						nextstate = OP;
						t.type = OPERATOR;
						t.op = currop;
						currop = currchar;
						i++;
						currstate = nextstate;
						return t;
						break;

					case ' ': case '\t':
						nextstate = SPC;
						t.type = OPERATOR;
						t.op = currop;
						i++;
						currstate = nextstate;
						return t;
						break;
					case '\0':case '\n':
						nextstate = STOP;
						t.type = OPERATOR;
						t.op = currop;
						i++;
						currstate = nextstate;
						return t;
						break;
					case '.':
						setdecimal = 1;
						nextstate = DIG;
						break;
					default:
						nextstate = ERR;
						t.type = OPERATOR;
						t.op = currop;
						i++;
						currstate = nextstate;
						return t;
						break;
				}
				break;
			case STOP:
				t.type = END;
				return t;
				break;
			case ERR:
				t.type = ERROR;
				return t;
				break;
		}
		currstate = nextstate;
		i++;
	}
	return t;
}

list *postfix(char *str, FILE *fp) {
	list *l[20];
	int i = 0;
	list *l1, *l2, *result, *tmp;
	token *t;
	stack s;

	result = (list *)malloc(sizeof(list));
	l1 = (list *)malloc(sizeof(list));
	l2 = (list *)malloc(sizeof(list));
	tmp = (list *)malloc(sizeof(list));
	t = (token*)malloc(sizeof(token));
	initlist(l1);
	initlist(l2);
	initlist(result);

	init(&s);

	while(1) {
		*t = getnext(str);
		if(t->type == OPERAND) {
			push(&s, t->l);
			i++;
		}
		else if(t->type == OPERATOR) {
				l2 = pop(&s); i--;
				l1 = pop(&s); i--;
				switch(t->op) {
					case '+':						
						fprintlist(*l1, fp);
						fprintf(fp, " + ");
						fprintlist(*l2, fp);
						result = add(l1, l2);
						fprintf(fp, " = ");
						fprintlist(*result, fp);
						fprintf(fp, "\n");
						break;
					case '-':
						fprintlist(*l1, fp);
						fprintf(fp, " - ");
						fprintlist(*l2, fp);
						result = substract(l1, l2);
						fprintf(fp, " = ");
						fprintlist(*result, fp);
						fprintf(fp, "\n");
						break;
					case '*':
						fprintlist(*l1, fp);
						fprintf(fp, " * ");
						fprintlist(*l2, fp);
						result = multiply(l1, l2);
						fprintf(fp, " = ");
						fprintlist(*result, fp);
						fprintf(fp, "\n");
						break;
					case '/':
						fprintlist(*l1, fp);
						fprintf(fp, " / ");
						fprintlist(*l2, fp);
						result = divide(l1, l2);
						fprintf(fp, " = ");
						fprintlist(*result, fp);
						fprintf(fp, "\n");
						break;
					case '%':
						fprintlist(*l1, fp);
						fprintf(fp, " %% ");
						fprintlist(*l2, fp);
						result = modulus(l1, l2);
						fprintf(fp, " = ");
						fprintlist(*result, fp);
						fprintf(fp, "\n");
						break;
					case '^':
						fprintlist(*l1, fp);
						fprintf(fp, " ^ ");
						fprintlist(*l2, fp);						
						result = power(l1, l2);
						fprintf(fp, " = ");
						fprintlist(*result, fp);
						fprintf(fp, "\n");
						break;
				}
		push(&s, result);
		destroy(&l1);
		destroy(&l2);
		i++;
		} 
		else if(t->type == FACTORIAL){
			result = factorial(t->l);
			push(&s, result);
			i++;
		}
		else if(t->type == END){
			return pop(&s);
			i--; 
			break;
		}
		else if(t->type == ERROR) 
			return NULL;
		
	}
	if(i == 1)
		return pop(&s);
	else
		return NULL;
}


int main(int argc, char *argv[]) {
	char line[1000];
	int x;
	list *l;
	l = (list *)malloc(sizeof(list));
	FILE *fp;
	fp = fopen("steps.txt", "w");

	while(x = readline(line, 1000)) {
		itop(line);
		l = postfix(line, fp);
		if(l->head != NULL){
			printlist(*l);
			
			printf("\n");
			break;
		}
		else
			fprintf(stderr, "Error in expression\n");
	}
	
	return 0;
}




