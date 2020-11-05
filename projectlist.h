typedef struct node{
	int val;
	int carry;	
	struct node *prev, *next;
}node;

typedef struct list{
	int decimal;
	int sign;
	node *head, *tail;
}list;

void initlist(list *l);
void insert(list *l, int num, int pos);
int length(list *l);
int remov(list *l, int pos);
void printlist(list l);
void append(list *l, int val);
int remov(list *l, int pos);
int getdig(list *l, int pos);
void printlist(list l);
void destroy(list **l);
int show(list *l, int pos);


