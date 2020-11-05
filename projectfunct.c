#include<limits.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"projectfunct.h"
#include<math.h>
#define DECIMAL_PRECISION 10

int greater(list *l1, list *l2) {
	if(l1->sign == 1 && l2->sign == -1)
		return 1;
	if(l2->sign == 1 && l1->sign == -1)
		return -1;
	/*Make no. of digits after decimal equal*/
	if(l1->decimal > l2->decimal) {
		int i;
		int y = (l1->decimal - l2->decimal);
		for(i = 0; i < y; i++) {
			append(l2, 0);
			l2->decimal++;
		}
	}
	else if(l2->decimal > l1->decimal) {
		int i;
		int y = (l2->decimal - l1->decimal);
		for(i = 0; i < y; i++) {
			append(l1, 0);
			l1->decimal++;
		}
	}
	int a = (length(l1) - length(l2));
	if(a > 0) {
		int i = 0;
		while(i < a) {
			insert(l2, 0, 0);
			i++;
		}
	}
	else if(a < 0) {
		int i = 0;
		a = -a;
		while(i < a) {
			insert(l1, 0, 0);
			i++;
		}
	}	
	
		int i;
		for(i = 1; i <= length(l1); i++) {
			if(show(l1, i - 1) > show(l2, i - 1))
				return 1;
			else if(show(l1, i - 1) < show(l2, i - 1))
				return -1;
		}
		return 0;
}


int iszero(list *l){
	int i;
	node *tmp;
	tmp = l->head;
	while(tmp != NULL){
		if(tmp->val != 0)
			return -1; 
		else 
			i = 0;
		tmp = tmp->next;
	}
	return i;
}


list *add(list *l1, list *l2) {
	int carry = 0, num = 0, i, n;
	int d1, d2, sign1, sign2;
	d1 = l1->decimal;
	d2 = l2->decimal; 
	list *result;
	node *node1, *node2;
	result = (list *)malloc(sizeof(list));
	initlist(result);
	if(d1 > d2) {
		n = d1 - d2;
		for(i = 0; i < n; i++) {
			append(l2, 0);
			d2++;
		}
	}
	if(d2 > d1) {
		n = d2 - d1;
		for(i = 0; i < n; i++) {
			append(l1, 0);
			d1++;
		}
	}

	sign1 = l1->sign;
	sign2 = l2->sign;

	if(l1->sign != l2->sign) {
		if(l1->sign == -1) {
			l1->sign = 1;
			result = substract(l2, l1);
			l1->sign = sign1;
			return result;
		}
		else if(l2->sign == -1) {
			l2->sign = 1;
			result = substract(l1, l2);
			l2->sign = sign2;
			return result;
		}
	}
	else if(l1->sign == l2->sign) {
		if(l1->sign == -1)
			result->sign = -1;
		else
			result->sign = 1;

		node1 = l1->tail;
		node2 = l2->tail;

		while(node1 != NULL && node2 != NULL){
			num = num + node1->val + node2->val;
			carry = num / 10;
			num = num % 10;
			insert(result, 0, num);
			node1 = node1->prev;
			node2 = node2->prev;
			num = carry;
		}
		if(node1 == NULL) {
			while(node2 != NULL){
				num = node2->val + carry;
				node2 = node2->prev;
				carry = num / 10;
				num = num % 10;
				insert(result, 0, num);
				num = carry;
			}
		}
		if(node2 == NULL) {
			while(node1 != NULL){
				num = node1->val + num;
				node1 = node1->prev;
				carry = num / 10;
				num = num % 10;
				insert(result, 0, num);
				num = carry;
			}
		}
	}
	result->decimal = d1;
	if(carry != 0)
		insert(result, 0, carry);
	return result;
}

list *substract(list *l1, list *l2) {
	int carry = 0, num = 0, i, n, g;
	int d1 , d2;
	d1 = l1->decimal;
	d2 = l2->decimal; 
	list *result, *tmp;
	node *node1, *node2;
	result = (list *)malloc(sizeof(list));
	initlist(result);


	if (l1 == NULL)
		return l2;

	if (l2 == NULL)
		return l1;

	if(d1 > d2) {
		n = d1 - d2;
		for(i = 0; i < n; i++) {
			append(l2, 0);
			l2->decimal++;
		}
	}
	else if(d2 > d1) {
		n = d2 - d1;
		for(i = 0; i < n; i++) {
			append(l1, 0);
			l1->decimal++;
		}
	}

	if(l1->sign==1 && l2->sign==-1){
		l2->sign = 1;
		result = add(l1,l2);
		l2->sign = -1;
		return result;
	}

	if(l1->sign==-1 && l2->sign==1){
		l1->sign = 1;
		result = add(l1,l2);
		result->sign = -1;
		return result;
	}

	if(l1->sign==-1 && l2->sign==-1){
		l1->sign = 1;
		l2->sign = 1;
		result = substract(l2,l1);
		l1->sign = -1;
		l2->sign = -1;
		return result;
	}

	g = greater(l1, l2);
	if(g == -1){
		result->sign = -1;
		tmp = l1;
		l1 = l2;
		l2 = tmp;
	}

	node1 = l1->tail;
	node2 = l2->tail;
	
	while(node1 != NULL && node2 != NULL) {
		num = node1->val - node2->val + num;
		if(num < 0){
			num = num + 10;
			carry = -1;
		}
		else carry = 0;
		insert(result, 0, num);
		num = carry;
		node1 = node1->prev;
		node2 = node2->prev;
	}
	if(node1 == NULL) {
		while(node2 != NULL){
			num = node2->val + num;
			node2 = node2->prev;
			if(num < 0){
				num = num + 10;
				carry = -1;
			}
			else carry = num / 10;
			insert(result, 0, num);
			num = carry;
		}
	}
	if(node2 == NULL) {
		while(node1 != NULL){
			num = node1->val + num;
			node1 = node1->prev;
			if(num < 0){
				num = num + 10;
				carry = -1;
			}
			else carry = 0;
			insert(result, 0, num);
			num = carry;
		}
	}
	result->decimal = l1->decimal;
	if(carry != 0)
		insert(result, 0, carry);
	return result;
}

list *multiply(list *l1, list *l2) {
	int i, j, k, len1, len2, lenr;	
	list *result;
	node *first, *second;
	int prod = 0;
	len1 = length(l1);
	len2 = length(l2);
	result = (list *)malloc(sizeof(list));
	initlist(result);

	int d1 = 0, d2 = 0;
	d1 = l1->decimal;
	d2 = l2->decimal;


	if(iszero(l1) == 0)
		return l1;
	if(iszero(l2) == 0)
		return l2;

	int sign1, sign2;

	sign1 = l1->sign;
	sign2 = l2->sign;

	if(l1->sign != l2->sign) {
		if(l1->sign == -1) {
			l1->sign = 1;
			result = multiply(l1, l2);
			l1->sign = sign1;
			result->sign = -1;
			return result;
		}
		else if(l2->sign == -1) {
			l2->sign = 1;
			result = multiply(l1, l2);
			l2->sign = sign2;
			result->sign = -1;
			return result;
		}
	}
	else if(l1->sign == l2->sign) {
			result->sign = 1;
			lenr = len1 + len2;
			int a[lenr];
			for( i = 0; i < lenr ; i++)
				a[i] = 0;
			second = l2->tail;
			int carry1 = 0;
			for(i = 0; i < len2; i++){
				first = l1->tail;
				for(j = lenr - 1 - i; first != NULL; j--){
					prod = first->val * second->val + a[j];	
					first = first->prev;
					carry1 = prod / 10;
					prod = prod % 10;
					a[j - 1] = carry1 + a[j - 1];
					a[j] = prod;
				}
				second = second->prev;
			}
			int k = 0;
			while(a[k] == 0){
				k++;	
			}
			for(i = 0; k < lenr; k++, i++) {
				insert(result, i, a[k]);
			}
			result->decimal = d1 + d2;
			return result;
	}
}

list *divide(list *l1, list *l2) {
	if(iszero(l2) == 0) {
		printf("\nanswer cant be defined ....division by zero not possible\n");
		return NULL;
	}
	list *result;
	result = (list *)malloc(sizeof(list));
	list *temp;
	temp = (list *)malloc(sizeof(list));
	initlist(temp);
	initlist(result);

	int m, sign1, sign2;
	sign1 = l1->sign;
	sign2 = l2->sign;
	if(l1->sign == l2->sign) {
		result->sign = 1;
		l1->sign = l2->sign = 1;
	}
	else {
		result->sign = -1;
		l1->sign = l2->sign = 1;
	}
	
	int a, b, i, j, k, num;
	m = l1->decimal - l2->decimal;
	
	a = l1->decimal;
	b = l2->decimal;
	
	l1->decimal = l2->decimal = 0;
	
	for(i = 0; i < length(l1); i++) {
		num = show(l1, i);
		append(temp, num);
		if(greater(temp, l2) < 0) {
		
			append(result, 0);
		}
		else {
			int d = 0;
			while(1) {
				temp = substract(temp, l2);
				d++;
				if(greater(temp, l2) == -1)
					break;
				
			}
			append(result, d);	
		}
	}
	j = 0; k = 0;
	while(j < DECIMAL_PRECISION) {
		append(temp, 0);
		int d = 0;
		while(greater(temp, l2) > 0) {
			temp = substract(temp, l2);
			d++;
		}
		append(result, d);
		k++;j++;
	}
	result->decimal = DECIMAL_PRECISION + m;
	l1->decimal = a;
	l2->decimal = b;
	l1->sign = sign1;
	l2->sign = sign2;
	return result;
}


list *modulu(list *l1, list *l2) {
	
	int d1, d2, n;

	d1 = l1->decimal;
	d2 = l2->decimal;
	
	if(d1 > d2) {
		n = d1 - d2;
		for(int i = 0; i < n; i++) {
			append(l2, 0);
			l2->decimal++;
		}
	}
	else if(d2 > d1) {
		n = d2 - d1;
		for(int i = 0; i < n; i++) {
			append(l1, 0);
			l1->decimal++;
		}
	}

	l1->decimal = 0;
	l2->decimal = 0;
	
	while(greater(l1, l2) > 0){
		l1 = substract(l1, l2);
	}
	l1->decimal = d1;
	l2->decimal = d2;
	
	return l1;
}

list *modulus(list *l1, list *l2) {
	if(iszero(l2) == 0) {
		printf("ERROR:modulo by zero");
		return NULL;
	}
	list *result;
	result = (list *)malloc(sizeof(list));
	list *temp;
	temp = (list *)malloc(sizeof(list));
	initlist(temp);
	initlist(result);
	list *l3;
	l3 = (list *)malloc(sizeof(list));
	initlist(l3);
	copy(l2, l3);
	int m, sign1, sign2;
	sign1 = l1->sign;
	sign2 = l2->sign;
	if(l1->sign == l2->sign) {
		result->sign = 1;
		l1->sign = l2->sign = 1;
	}
	else {
		result->sign = -1;
		l1->sign = l2->sign = 1;
	}
	
	int a, b;
	m = length(l1);
	int i, j, num;
	
	a = l1->decimal;
	b = l2->decimal;
	/*if(l1->decimal != 0) {
		length(l1) = length(l1) - l1->decimal;
	}*/
	if(l3->decimal != 0) {
		i = l3->decimal;
		for(j = 0; j < i; j++) {
			remov(l3, length(l3));
		}
	}
	for(i = 0; i < length(l1) - l1->decimal; i++) {
		num = show(l1, i);
		append(temp, num);
		if(greater(temp, l3) < 0) {
			append(result, 0);
		}
		else {
			int d = 0;
			while(1) {
				temp = substract(temp, l3);
				d++;
				if(greater(temp, l3) < 0)
					break;
					
			}
			append(result, d);
		}
	}

	l1->sign = sign1;
	l2->sign = sign2;
	l1->decimal = a;
	l2->decimal = b;
	return temp;
}

list *factorial(list *l1) {
	list *result, *count;
	if(l1->decimal == 0){
		result = (list *)malloc(sizeof(list));
		initlist(result);
		insert(result, 0, 1);

		count = (list *)malloc(sizeof(list));
		initlist(count);
		insert(count, 0, 1);

		while(iszero(l1)) {
			result = multiply(l1, result);
				l1 = substract(l1, count);
		}
		return result;
	}

	else printf("factorial of decimal number cant be calculated");
	return NULL;		 
	
}



list *power(list *l1, list *l2){
	int i, d;
	d = l1->decimal;

	list *result, *count, *pow;	
	result = (list *)malloc(sizeof(list));
	initlist(result);
	insert(result, 0, 1);

	pow = (list *)malloc(sizeof(list));
	initlist(pow);
	insert(pow, 0, 0);
	pow = add(l2, pow);

	count = (list *)malloc(sizeof(list));
	initlist(count);
	insert(count, 0, 1);

	for(i = 0; iszero(pow) != 0; i++){
		result = multiply(result, l1);
		pow = substract(pow, count);
		if(!iszero(pow))
			break;			
	}
	result->decimal = 2 * d;
	return result;
}


list* get_pi(){

	list* pi = (list*)malloc(sizeof(list));

	pi->sign = 1;
	pi->decimal = 5;

	char a[] = {6, 2, 8, 3, 1, 9};

	for(int i=0; i<6; i++){
		insert(pi, i, a[i]);
	}
	
	return pi;
}

double list_to_double(list *l){

	double ans = 0;
	int i;
	double power = ceil(pow(10 , ((length(l) - l->decimal) - 1)));

	for(i=0; i<(length(l) ); i++){

		ans += show(l,i) * power;
		printf("\n%d %d\n", show(l,i), power);
		power = power / 10;
	}
	ans *= l->sign;
	return ans;
}

double sine(list *l){

	list *result;	
	result = (list *)malloc(sizeof(list));
	initlist(result);

	result = modulus(l, get_pi());
	return sin(list_to_double(result));
}

double cosine(list *l){

	list *result;	
	result = (list *)malloc(sizeof(list));
	initlist(result);

	result = modulus(l, get_pi());
	return cos(list_to_double(result));
}
double tangent(list *l){

	list *result;	
	result = (list *)malloc(sizeof(list));
	initlist(result);

	result = modulus(l, get_pi());
	return tan(list_to_double(result));
}
double cot(list *l){

	list *result;	
	result = (list *)malloc(sizeof(list));
	initlist(result);

	result = modulus(l, get_pi());
	return cos(list_to_double(result))/sin(list_to_double(result));
}
double sec(list *l){

	list *result;	
	result = (list *)malloc(sizeof(list));
	initlist(result);

	result = modulus(l, get_pi());
	return 1/cos(list_to_double(result));
}
double cosec(list *l){

	list *result;	
	result = (list *)malloc(sizeof(list));
	initlist(result);

	result = modulus(l, get_pi());
	return 1/sin(list_to_double(result));
}