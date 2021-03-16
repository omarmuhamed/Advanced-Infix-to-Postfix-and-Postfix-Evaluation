#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
typedef struct stack {
    int top;
    unsigned max;
    int *data;
} Stack;

typedef struct stack2{
    int top;
    unsigned max;
    double *data;
} Stack2;
int isEmpty(Stack *stack) {
    if (stack->top == -1) {
        return 1;
    }
    return 0;
}
int isFull(Stack *stack) {
    return stack->top >= stack->max -1;
}
void printStack(Stack *stack) {

    for (int i = stack->top; i >= 0; i--) {
        printf("%c", stack->data[i]);
    }

    printf("%*s| ", stack->max / 2  - stack->top + 4, "");
}
void push(Stack *stack, char data) {
    stack->data[++stack->top] = (int)data;
    if (!isFull(stack)) {
        stack->data[stack->top + 1] = '\0';
    }
}

char pop(Stack *stack) {
    if (!isEmpty(stack)) {
        return (char)stack->data[stack->top--];
    }
    return '.';
}

char peek(Stack *stack) {
    return (char)stack->data[stack->top];
}

int isOperand(char a) {
    return (a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || (a >= '0' && a <= '9' || a == '.');
}

int precedence(char a) {

    switch (a) {
        case '+':
        case '-':
            return 1;

        case '*':
        case '/':
        case '%':
            return 2;

        case '^':
            return 3;
        default:
            return -1;
    }

}

int isDigit(char c) {
    return (c >= '0' && c <= '9');
}
void build_row(Stack *stack, char *result, char *data, int i){
    printf("| %*s| ", -stack->max - 5, (data + i));
    printStack(stack);
    printf("%*s|\n", -(int)(stack->max * 1.5), result);
    for (int x = 0; x < (int)(stack->max * 1.5) + stack->max + (int)(stack->max / 2) + 17; x++) {
        printf("-");
    }
    printf("\n");
}
int isEmpty2(Stack2 *stack) {
    if (stack->top == -1) {
        return 1;
    }
    return 0;
}

void push2(Stack2 *stack, double data) {
    stack->data[++stack->top] = data;
}

double pop2(Stack2 *stack) {
    if (!isEmpty2(stack)) {
        return stack->data[stack->top--];
    }
}
double calculate(double op1, double op2, char operator) {
    switch (operator) {
        case '+':
            return op2 + op1;
        case '-':
            return op2 - op1;
        case '*':
            return op2 * op1;
        case '/':
            return op2 / op1;
        case '^':
            return pow(op2, op1);
        case '%':
            return fmod(op2, op1);
        default:
            return -1;
    }
}
double evaluate(char data[]) {
    Stack2 *stack = (Stack2 *)malloc(sizeof(Stack2));
    stack->top = -1;
    stack->max = strlen(data);
    stack->data = (double *)malloc(sizeof(double) * stack->max);

    double op1, op2, temp, temp2;
    int perc, neg = 0;
    for (int i = 0; i < stack->max; i++) {
        if (data[i] == ' '){
            continue;
        }
        if (isDigit(data[i])) {

            temp = 0;
            temp2 = 0;
            while (isDigit(data[i])) {
                temp = temp * 10 + (int)(data[i++] - '0');
            }
            if (data[i] == '.') {
                i++;
                perc = 10;
                while(isDigit(data[i])) {
                    temp2 += ((double)((int)(data[i++] - '0'))/(perc));
                    perc *= 10;
                }
            } else {
                i--;
            }
            if(neg){
                push2(stack, -(temp + temp2));
                neg = 0;
            } else {
                push2(stack, temp + temp2);
            }
        } else {
            if (data[i] == '-' && (data[i+1] != ' ' && data[i+1] != '\0')){
                neg = 1;
                continue;
            }
            op1 = pop2(stack);
            op2 = pop2(stack);
            push2(stack, calculate(op1, op2, data[i]));
        }
    }
    return pop2(stack);
}
int isNegative(char *data, int i){
    for (int j = i - 1; i >= 0; i--) {
        if (data[j] != ' ') {
            if (isOperand(data[j])) {
                return 0;
            }
            return 1;
        }
    }
    return 1;
}
char *convert(char *data) {

    Stack *stack = (Stack *) malloc(sizeof(Stack));
    stack->max = strlen(data);
    stack->data = (int *) malloc(stack->max * sizeof(int));
    stack->top = -1;
    int i = 0, j = 0;
    char *result = (char *)malloc(stack->max * 2);
    result[0] = '\0';
    for (int x = 0; x < (int)(stack->max * 1.5) + stack->max + (int)(stack->max / 2) + 17; x++) {
        printf("-");
    }
    printf("\n");
    for (; data[i]; i++) {
        if (data[i] == ' ') {
            continue;
        }
        result[j+1] = '\0';
        build_row(stack, result, data, i);
        if (isOperand(data[i])) {
            result[j++] = data[i];
        } else {
            switch (data[i]) {
                case '(':
                    push(stack, data[i]);
                    break;
                case ')':
                    while (!isEmpty(stack) && peek(stack) != '(') {
                        result[j++] = ' ';
                        result[j++] = pop(stack);
                        build_row(stack, result, data, i);
                    }
                    pop(stack);
                    break;
                default:
                    if ((data[i] == '-' && data[i-1] == '(') || (data[i] == '-' && isNegative(data, i)) || (data[i] == '-' && i == 0)) {
                        result[j++] = data[i];
                    }else{

                        result[j++] = ' ';
                        while (!isEmpty(stack) && precedence(data[i]) <= precedence(peek(stack))) {
                            result[j++] = pop(stack);
                            result[j++] = ' ';
                            build_row(stack, result, data, i);
                        }
                        push(stack, data[i]);
                    }
            }
        }

    }
    build_row(stack, result, data, i);
    while (!isEmpty(stack)) {
        result[j++] = ' ';
        result[j++] = pop(stack);
        result[j] = '\0';
        build_row(stack, result, data, i);
    }
    return result;
}

int main() {

    char exp[300];
    char *res;
    printf("Enter Infix expression: ");
    scanf("%[^\n]", exp);
    printf("\n");
    res = convert(exp);
    printf("Postfix: %s\n",res);
    printf("Value: %lf", evaluate(res));
    fflush(stdin);
    getchar();
}