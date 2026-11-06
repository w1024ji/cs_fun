#include <stdio.h>
#include <string.h>

/* 배열 기반 스택: LIFO (Last In, First Out) */
#define MAX 100

typedef struct {
    int data[MAX];
    int top;            /* 가장 위 원소의 인덱스, 비어 있으면 -1 */
} Stack;

void stack_init(Stack *s) { s->top = -1; }
int  is_empty(Stack *s)   { return s->top == -1; }
int  is_full(Stack *s)    { return s->top == MAX - 1; }

/* push: O(1) */
int push(Stack *s, int v) {
    if (is_full(s)) return 0;      /* overflow */
    s->data[++s->top] = v;
    return 1;
}

/* pop: O(1) */
int pop(Stack *s, int *out) {
    if (is_empty(s)) return 0;     /* underflow */
    *out = s->data[s->top--];
    return 1;
}

/* 응용: 괄호 짝 검사 */
int is_balanced(const char *str) {
    Stack s;
    stack_init(&s);
    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        if (c == '(' || c == '[' || c == '{') {
            push(&s, c);
        } else if (c == ')' || c == ']' || c == '}') {
            int open;
            if (!pop(&s, &open)) return 0;
            if ((c == ')' && open != '(') ||
                (c == ']' && open != '[') ||
                (c == '}' && open != '{')) return 0;
        }
    }
    return is_empty(&s);
}

/* 응용: 후위 표기식 계산 (한 자리 숫자, 예: "23*54*+9-" = 17) */
int eval_postfix(const char *expr) {
    Stack s;
    stack_init(&s);
    for (int i = 0; expr[i] != '\0'; i++) {
        char c = expr[i];
        if (c >= '0' && c <= '9') {
            push(&s, c - '0');
        } else {
            int b, a;
            pop(&s, &b);
            pop(&s, &a);
            switch (c) {
                case '+': push(&s, a + b); break;
                case '-': push(&s, a - b); break;
                case '*': push(&s, a * b); break;
                case '/': push(&s, a / b); break;
            }
        }
    }
    int result;
    pop(&s, &result);
    return result;
}

int main(void) {
    Stack s;
    stack_init(&s);
    push(&s, 1);
    push(&s, 2);
    push(&s, 3);
    int v;
    while (pop(&s, &v)) printf("%d ", v);   /* 3 2 1 : 넣은 순서의 반대 */
    printf("\n\n");

    const char *tests[] = {"{[()]}", "([)]", "((", "a(b)c"};
    for (int i = 0; i < 4; i++)
        printf("%-8s -> %s\n", tests[i], is_balanced(tests[i]) ? "OK" : "NG");

    printf("\n23*54*+9- = %d\n", eval_postfix("23*54*+9-"));
    return 0;
}
