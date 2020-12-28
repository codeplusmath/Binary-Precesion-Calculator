#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stack.h"
#include "list.h"
#include "number.h"

#define OPERAND 100
#define OPERATOR 200
#define VARIABLE 300
#define	END	400
#define ERROR	500
#define PREINC	600
#define POSINC	700
#define PREDEC	800
#define POSDEC	900
#define COUNT 2048

typedef struct token{
	char op;
	int dec;
	int type; /* type takes values OPERAND/OPERATOR/END/*/
	list *l;
	int flag;
	char var;

}token;

token *getnext(char *, int *);

// Enum is used to define datatype and it makes a program easy to read and maintain.
enum states { SPC, DIG, OPR, STOP, ERR };

token *getnext(char *arr, int *reset){
	static int currstate;
	int nextstate;
	static int i;
	static list *l;
	static int dec = 0;
	static int flag = 0;
	static int m = 0, num = 0, sign = 1;
	l = (list *)malloc(sizeof(list));
	init(l);
	if(m == 1){
		append(l, num);
		m = 0;
	}
	if(*reset == 1){
		i = 0;
		currstate = SPC;
		*reset = 0;
	}
	token *t = (token *)malloc(sizeof(token));
	while(1){
		switch(arr[i]){
			case '0': case '1': case '2': case '3':
			case '4': case '5': case '6': case '7':
			case '8': case '9': case '.':
				nextstate = DIG;
				break;
			case '+': case '-': case '*': case '/':
			case '%': case '(': case ')': case '^':
			case 'S': case 'C': case 'T':
				nextstate = OPR;
				break;


			case '\0':
				nextstate = STOP;
				break;
			case ' ':
				nextstate = SPC;
				break;
			default:
				nextstate = ERR;
				break;
		}
		switch(currstate){
			case SPC:
				if(nextstate == DIG){
					if(arr[i] == '.'){
						flag = 1;
					}
					else{
						append(l,(arr[i] - '0'));
						if(flag == 1)
							dec++;
					}
				}
				break;
			case DIG:
				if(nextstate == DIG){
					if(arr[i] == '.'){
						flag = 1;
					}
					else{
						append(l, (arr[i] - '0'));
						if(flag == 1)
							dec++;
					}
				}
				else {
					t->type = OPERAND;
					t->l = l;
					t->l->dec = dec;
					t->l->sign = sign;
					sign = 1;
					dec = 0;
					flag = 0;
					currstate = nextstate;
					i++;
					return t;
				}
				break;
			case OPR:
				if(arr[i - 2] == '('){
					if(arr[i - 1] == '-')
						sign = -1;
					else
						sign = 1;
				}
				if(arr[i - 1] == 'I' && arr[i] == 'p' && arr[i + 1] == '-' && arr[i + 2] == '>')
					t->var = 'i';
				if(sign != -1){
					t->type = OPERATOR;
					t->op = arr[i - 1];
				}
				if(arr[i-1] == '+' && arr[i] == '+' && arr[i + 1] <= 'z' && arr[i + 1] >= 'a')
					t->type = PREINC;
				if(arr[i-1] == '+' && arr[i] == '+' && arr[i - 2] <= 'z' && arr[i - 2] >= 'a')
					t->type = POSINC;
				if(arr[i-1] == '-' && arr[i] == '-' && arr[i + 1] <= 'z' && arr[i + 1] >= 'a')
					t->type = PREDEC;
				if(arr[i-1] == '-' && arr[i] == '-' && arr[i - 2] <= 'z' && arr[i - 2] >= 'a')
					t->type = POSDEC;
				currstate = nextstate;
				if(arr[i - 1] == '/' && arr[i] == '*'){
					while(1){
						i++;
						if(arr[i] == '*' && arr[i + 1] == '/'){
							break;
						}
					}
					nextstate = SPC;
					i = i + 1;
					break;
				}
				if(nextstate == DIG){
					if(arr[i] == '.'){
						flag = 1;
						dec = 0;
					}
					else{
						m = 1;
						num = arr[i] - '0';
						if(flag == 1)
							dec++;
					}
				}
				i++;
				return t;
				break;

			case STOP:
				t->type = END;
				currstate = nextstate;
				i++;
				return t;
				break;
			case ERR:
				t->type = ERROR;
				currstate = nextstate;
				i++;
				return t;
				break;
		}
		currstate = nextstate;
		i++;
	}
}

int precedence(char op){
	if(op == '*' || op == '/' || op == '%')
		return 3;
	if(op == '+' || op == '-')
		return 2;
	if(op == '(')
		return 1;
	return 0;
}

//Readline reads the string inputed and stores it in an array.
int readline(char *arr, int len){
	int k = 0;
	int ch;
	while((ch = getchar()) != '\n' && k < len - 1){
		arr[k] = ch;
		k++;
	}
	arr[k] = '\0';
	return k;
}


//Static creates variable of type list which is used in infix function and will remain till whole program ends.
static list *L[26][10];

/* > Infix function solves the infix equation.
   > It takes Input in form of tokens from getnext
   > and stores operators in cstack and operands in stack
   > '(' and ')' are considered to be as operands.
   > When a closing bracket comes it pops the operands out and do the calculations. At a time it pops two operators out.
*/
list *infix(char *str){
	list *x, *y, *z, *w;
	x = (list *)malloc(sizeof(list));
	y = (list *)malloc(sizeof(list));
	z = (list *)malloc(sizeof(list));
	w = (list *)malloc(sizeof(list));
	static int r[26];
	static int f[26];
	init(x);
	init(y);
	init(z);
	init(w);
	int reset = 1, c, b;
	char op1, op2, ch;
	token *t;
	t = (token *)malloc(sizeof(token));
	cstack cs;
	stack a;
	stack1 e;
	sinit(&a);
	cinit(&cs);
	sinit1(&e);
	int flag = 0, setpow = 0, settrigo = 0;
	int i = 0, k, varflag = 0, m, prepos, postfix = 0;
	i = 0;
	while(1){
		t = getnext(str, &reset);
		if(t->type == OPERAND){
			if(setpow == 1){
				x = pop(&a);
				if(dec(t->l) != 0)
					return NULL;
				y = power(x, t->l);
				setpow = 0;
				push(&a, y);
			}
			else{
				push(&a, t->l);
			}
		}
		else if(t->type == OPERATOR){
			switch(t->op){
				case '+': case '/': case '*': case '-': case '%':
					cpush(&cs, t->op);
					break;
				case '(':
					cpush(&cs, t->op);
					flag++;
					break;
				case '^':
					setpow = 1;
					break;
				case 'S':
					settrigo = 1;
					break;
				case 'C':
					settrigo = 2;
					break;
				case 'T':
					settrigo = 3;
					break;
				case ')':
					if(settrigo != 0){
						switch(settrigo) {
							case 1:
								x = pop(&a);
								y = sine(x);
								push(&a, y);
								break;
							case 2:
								x = pop(&a);
								y = cosine(x);
								push(&a, y);
								break;
							case 3:
								x = pop(&a);
								y = tangent(x);
								push(&a, y);
								break;
						}
						settrigo = 0;
					}
					flag--;
					if(flag < 0)
						return NULL;
					while(1){
						if(!cempty(&cs)){
							op1 = cpop(&cs);
							if(op1 == '('){
								break;
							}
						}
						else
							break;
						if(!cempty(&cs))
							op2 = cpop(&cs);
						c = precedence(op1);
						b = precedence(op2);
						if(c > b){
							if(!empty(&a))
								z = pop(&a);
							else
								return NULL;
							if(!empty(&a))
								y = pop(&a);
							else
								return NULL;
							switch(op1){
								case '+':
									push(&a, addnum(y, z));
									break;
								case '-':
									push(&a, subnum(y, z));
									break;
								case '/':
									push(&a, division(y, z));
									break;
								case '*':
									push(&a, mulnum(y, z));
									break;
								case '%':
									push(&a, modulus(y,z));
									break;
							}
							if(op2 != '('){
								if(!empty(&a))
									z = pop(&a);
								else
									return NULL;
								if(!empty(&a))
									y = pop(&a);
								else
									return NULL;
								switch(op2){
									case '+':
										push(&a, addnum(y, z));
										break;
									case '-':
										push(&a, subnum(y, z));
										break;
									case '/':
										push(&a, division(y, z));
										break;
									case '*':
										push(&a, mulnum(y, z));
										break;
									case '%':
										push(&a, modulus(y,z));
										break;
								}
							}
						}
						else if(c <= b){
							if(!empty(&a))
								z = pop(&a);
							else
								return NULL;
							if(!empty(&a))
								y = pop(&a);
							else
								return NULL;
							if(!empty(&a))
								x = pop(&a);
							else
								return NULL;
							switch(op2){
								case '+':
									x = addnum(x, y);
									break;
								case '-':
									x = subnum(x, y);
									break;
								case '/':
									x = division(x, y);
									break;
								case '*':
									x = mulnum(x, y);
									break;
								case '%':
									x = modulus(x, y);
									break;
							}
							switch(op1){
								case '+':
									push(&a, addnum(x, z));
									break;
								case '-':
									push(&a, subnum(x, z));
									break;
								case '/':
									push(&a, division(x, z));
									break;
								case '*':
									push(&a, mulnum(x, z));
									break;
								case '%':
									push(&a, modulus(x,z));
									break;
							}
						}
					}
					break;
				default:

					return NULL;
					break;
			}
		}
		else if(t->type == END){
			while(!empty(&a)){
				x = pop(&a);
				push1(&e, x);

			}
			char op[100];
			i = 0;
			while(!cempty(&cs)){
				op[i] = cpop(&cs);
				i++;
			}
			int j = 0;
			while(j < i){
				cpush(&cs, op[j]);
				j++;
			}

			while(1){
				if(flag != 0)
					return NULL;
				if(!cempty(&cs)){
					op1 = cpop(&cs);
				}
				else{
					break;
				}
				if(!cempty(&cs))
					op2 = cpop(&cs);
				else{
					if(!empty1(&e)){
						z = pop1(&e);
					}
					else
						return NULL;
					if(!empty1(&e)){
						y = pop1(&e);
					}
					else
						return NULL;
					switch(op1){
						case '+':
							push1(&e, addnum(z, y));
							break;
						case '-':
							push1(&e, subnum(z, y));
							break;
						case '/':
							push1(&e, division(z, y));
							break;
						case '*':
							push1(&e, mulnum(z, y));
							break;
						case '%':
							push1(&e, modulus(z,y));
							break;
					}
					break;
				}
				c = precedence(op1);
				b = precedence(op2);
				if(c >= b) {
					if(!empty1(&e)){
						z = pop1(&e);
					}
					else
						return NULL;
					if(!empty1(&e)){
						y = pop1(&e);
					}
					else
						return NULL;
					switch(op1){
						case '+':
							push1(&e, addnum(z, y));
							break;
						case '-':
							push1(&e, subnum(z, y));
							break;
						case '/':
							push1(&e, division(z, y));
							break;
						case '*':
							push1(&e, mulnum(z, y));
							break;
						case '%':
							push1(&e, modulus(z, y));
							break;
					}
					cpush(&cs, op2);
				}
				else if(c < b){
					if(!empty1(&e)){
						z = pop1(&e);
					}
					else
						return NULL;
					if(!empty1(&e)){
						y = pop1(&e);
					}
					else
						return NULL;
					if(!empty1(&e))
						x = pop1(&e);
					else
						return NULL;
					switch(op2){
						case '+':
							push1(&e, addnum(y, x));
							break;
						case '-':
							push1(&e, subnum(y, x));
							break;
						case '/':
							push1(&e, division(y, x));
							break;
						case '*':
							push1(&e, mulnum(y, x));
							break;
						case '%':
							push1(&e, modulus(y, x));
							break;
					}
					push1(&e, z);
					cpush(&cs, op1);
				}
			}
			if(!empty1(&e) && cempty(&cs)){

				y = (pop1(&e));
				if(varflag == 1){
					L[k][f[k]] = y;
					if(postfix == 1){
						list *unity;
						unity = (list *)malloc(sizeof(list));
						init(unity);
						append(unity, 1);
						if(prepos == 2){
							L[m][f[k]] = addnum(L[m][f[k]], unity);
							r[k] = r[k] + 1;
							prepos = 0;
						}
						else if(prepos == 4){
							L[m][f[k]] = subnum(L[m][f[k]], unity);
							r[k] = r[k] + 1;
							prepos = 0;
						}
						postfix = 0;
					}
					varflag = 0;
					f[k] = f[k] + 1;
					x = NULL;
					free(x);
					z = NULL;
					free(z);
					w = NULL;
					free(w);
					return y;
				}



				if(empty1(&e) && cempty(&cs)){
					x = NULL;
					z = NULL;
					w = NULL;
					free(x);
					free(z);
					free(w);
					return (y);

				}
				else if(setpow == 1){
					if(!empty1(&e))
						x = pop1(&e);
					else
						return NULL;
					if(x->dec != 0)
						return NULL;
					z = power(y, x);
					setpow = 0;
					x = NULL;
					y = NULL;
					w = NULL;
					free(x);
					free(y);
					free(w);
					return z;
				}
				else{
					x = NULL;
					y = NULL;
					w = NULL;
					z = NULL;
					free(x);
					free(y);
					free(w);
					free(z);
					return NULL;
				}
			}
			else{

				x = NULL;
				y = NULL;
				w = NULL;
				z = NULL;
				free(x);
				free(y);
				free(w);
				free(z);
				return NULL;
			}

		}
		else if(t->type == ERROR){
			printf("error");
			x = NULL;
			y = NULL;
			z = NULL;
			w = NULL;
			free(x);
			free(y);
			free(z);
			free(w);
			return NULL;
		}
		else if(t->type == PREINC){
			prepos = 1;
			getnext(str, &reset);
		}
		else if(t->type == POSINC){
			postfix = 1;
			getnext(str, &reset);
			varflag = 1;
			prepos = 2;
		}
		else if(t->type == PREDEC){
			prepos = 3;
			getnext(str, &reset);
		}
		else if(t->type == POSDEC){
			postfix = 1;
			getnext(str, &reset);
			prepos = 4;
			varflag = 1;
		}
		else if(t->type == VARIABLE){
			if(t->flag == 1){
				ch = t->var;
				k = ch - 'a';
				if(r[k] != 0){

					L[k][r[k]] = (list*)malloc(sizeof(list));
					init(L[k][r[k]]);
					r[k] = r[k] + 1;
				}
				else{
					L[k][0] = (list*)malloc(sizeof(list));
					init(L[k][0]);
					r[k] = 1;
					f[k] = 0;
				}
				varflag = 1;
				t = getnext(str, &reset);
			}
			else{
				ch = t->var;
				m = ch - 'a';
				if(prepos == 1 || prepos == 3){
					list *unity;
					unity = (list *)malloc(sizeof(list));
					init(unity);
					append(unity, 1);
					L[m][r[m]] = (list*)malloc(sizeof(list));
					init(L[m][r[m]]);
					r[m] = r[m] + 1;
					varflag = 1;
					switch(prepos){
						case 1:
							L[m][f[m] - 1] = addnum(L[m][f[m] - 1], unity);
							push(&a, L[m][f[m] - 1]);
							break;
						case 3:
							L[m][f[m] - 1] = subnum(L[m][f[m] - 1], unity);
							push(&a, L[m][f[m] - 1]);
							break;
					}
					prepos = 0;
				}
				else {
					push(&a, L[m][f[m] - 1]);
				}
			}

		}
	}
	if(!empty1(&e))
		return (pop1(&e));
	printf("last");
	return NULL;
}

// Main function of program.
int main(int argc, char *argv[]){
	int arg = 0;
	printf("=================================================================\n");
    printf("                    00000            00000\n");
    printf("                    0    0         0      \n");
    printf("                    0    0        0       \n");
    printf("                    00000         0       \n");
    printf("                    0    0        0       \n");
    printf("                    0    0         0      \n");
    printf("                    00000            00000\n");
    printf("====================== BC with version:1.o ======================\n");
    printf("Note :- For exit press Enter\n");
    printf("Rajkumar.V.Sawant - 111903066 - sawantrv19.comp@coep.ac.in\n");
	char str[COUNT];
	int x;
	list *ans;
	ans = (list *)malloc(sizeof(list));
	init(ans);
	while((x = readline(str, COUNT))){
		ans = infix(str);
		if(ans == NULL) {
            //Standard error message
			fprintf(stderr, "Erorr in expression\n");
		}
		else {
			if(arg != 5) {
				traverse(ans, arg);
			}
			else
				traverse(ans, arg);
			printf("\n");
		}
	}

	//After giving output we need to free up the static variable.
	int i, j;
	for(i = 0; i < 26; i++)
		for(j = 0; j < 10; j++)
			free(L[i][j]);
	return 0;
}

