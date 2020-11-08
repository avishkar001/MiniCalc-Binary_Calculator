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
#include <ctype.h>
#include "projectfunct.h"
#include "itop.h"


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
}token;



void addtolist(token t, char ch){
	int i;
	i = ch - '0';
	if(i >=0 && i<= 9){
		append(t.l, i);
	}	
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

					case '+': case '-': case '*': case '/': case '%': case '^':
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

double trigno(char arr[]){

	char str[10];
	int op = 0, i;

	strncpy(str, arr, 7);

	if(strncmp(str, "sin", 3)==0){
		op = 1;
		i = 4;
		if(str[i-1] == 'd'){
			i++;
			op += 6;
		}
	}
	else if(strncmp(str, "cosec", 5)==0){
		op = 6;
		i = 6;
		if(str[i-1] == 'd'){
			i++;
			op += 6;
		}
	}
	else if(strncmp(str, "cos", 3)==0){
		op = 2;
		i = 4;
		if(str[i-1] == 'd'){
			i++;
			op += 6;
		}
	}
	else if(strncmp(str, "tan", 3)==0){
		op = 3;
		i = 4;
		if(str[i-1] == 'd'){
			i++;
			op += 6;
		}
	}
	else if(strncmp(str, "cot", 3)==0){
		op = 4;
		i = 4;
		if(str[i-1] == 'd'){
			i++;
			op += 6;
		}
	}
	else if(strncmp(str, "sec", 3)==0){
		op = 5;
		i = 4;
		if(str[i-1] == 'd'){
			i++;
			op += 6;
		}
	}
	
	
	list* l = (list *)malloc(sizeof(list));
	initlist(l);

	int dec_flag = 0;
	while(arr[i] != '\0'){
		if(arr[i] == '-')
			l->sign = -1;

		if(arr[i] == '.')
			dec_flag = 1;

		if(isdigit(arr[i])){
			append(l, arr[i] - '0');

			if(dec_flag)
				l->decimal++;
		}

		i++;
	}
	
	switch(op){
		case 1:
			return sine(l);
			break;
		case 2:
			return cosine(l);
			break;
		case 3:
			return tangent(l);
			break;
		case 4:
			return cot(l);
			break;
		case 5:
			return sec(l);
			break;
		case 6:
			return cosec(l);
			break;
		case 7:
			return sine_d(l);
			break;
		case 8:
			return cosine_d(l);
			break;
		case 9:
			return tangent_d(l);
			break;
		case 10:
			return cot_d(l);
			break;
		case 11:
			return sec_d(l);
			break;
		case 12:
			return cosec_d(l);
			break;
		
		default:
			printf("invalid expression");
			exit(1);
	}
		
	return 0;

}

int main(int argc, char *argv[]) {
	char line[1000];
	int x;
	list *l;
	l = (list *)malloc(sizeof(list));
	FILE *fp;
	fp = fopen("steps.txt", "a");
	fprintf(fp, "\n");
	
	while(x = readline(line, 1000)) {

		if(isalpha(line[0])){
			printf("%lf", trigno(line));
			return 0;
		}

		if(!(argc == 2 && !strcmp(argv[1], "-p")))
			infix_to_postfix(line);
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




