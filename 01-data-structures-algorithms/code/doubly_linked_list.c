#include <stdio.h>
#include <stdlib.h>

/* 이중 연결 리스트: prev 포인터가 추가됨 */
typedef struct DNode {
    int data;
    struct DNode *prev;
    struct DNode *next;
} DNode;

/* head, tail을 함께 관리하면 양끝 삽입/삭제가 모두 O(1) */
typedef struct {
    DNode *head;
    DNode *tail;
    int size;
} DList;

void dlist_init(DList *l) {
    l->head = l->tail = NULL;
    l->size = 0;
}

static DNode *new_node(int value) {
    DNode *n = malloc(sizeof(DNode));
    n->data = value;
    n->prev = n->next = NULL;
    return n;
}

/* 맨 앞 삽입: O(1) */
void push_front(DList *l, int value) {
    DNode *n = new_node(value);
    n->next = l->head;
    if (l->head != NULL) l->head->prev = n;
    else l->tail = n;              /* 빈 리스트였다면 tail도 n */
    l->head = n;
    l->size++;
}

/* 맨 뒤 삽입: tail 덕분에 O(1) (단일 리스트는 O(n)) */
void push_back(DList *l, int value) {
    DNode *n = new_node(value);
    n->prev = l->tail;
    if (l->tail != NULL) l->tail->next = n;
    else l->head = n;              /* 빈 리스트였다면 head도 n */
    l->tail = n;
    l->size++;
}

/* 노드 포인터를 이미 알면 삭제는 O(1): prev 덕분에 이전 노드를 찾을 필요 없음 */
void remove_node(DList *l, DNode *n) {
    if (n->prev != NULL) n->prev->next = n->next;
    else l->head = n->next;        /* head 삭제 */
    if (n->next != NULL) n->next->prev = n->prev;
    else l->tail = n->prev;        /* tail 삭제 */
    free(n);
    l->size--;
}

/* 값으로 찾아 삭제: 탐색 O(n) */
int remove_value(DList *l, int value) {
    for (DNode *c = l->head; c != NULL; c = c->next) {
        if (c->data == value) {
            remove_node(l, c);
            return 1;
        }
    }
    return 0;
}

/* 특정 노드 뒤에 삽입: O(1) */
void insert_after(DList *l, DNode *pos, int value) {
    DNode *n = new_node(value);
    n->prev = pos;
    n->next = pos->next;
    if (pos->next != NULL) pos->next->prev = n;
    else l->tail = n;
    pos->next = n;
    l->size++;
}

void print_forward(DList *l) {
    for (DNode *c = l->head; c != NULL; c = c->next) printf("%d <-> ", c->data);
    printf("NULL\n");
}

/* 단일 리스트로는 불가능한 역방향 순회 */
void print_backward(DList *l) {
    for (DNode *c = l->tail; c != NULL; c = c->prev) printf("%d <-> ", c->data);
    printf("NULL\n");
}

void dlist_free(DList *l) {
    DNode *c = l->head;
    while (c != NULL) {
        DNode *next = c->next;
        free(c);
        c = next;
    }
    dlist_init(l);
}

int main(void) {
    DList list;
    dlist_init(&list);

    push_back(&list, 1);
    push_back(&list, 2);
    push_back(&list, 3);
    push_front(&list, 0);
    printf("forward : "); print_forward(&list);   /* 0 1 2 3 */
    printf("backward: "); print_backward(&list);  /* 3 2 1 0 */

    insert_after(&list, list.head->next, 15);     /* 1 뒤에 15 */
    printf("insert  : "); print_forward(&list);   /* 0 1 15 2 3 */

    remove_value(&list, 2);
    printf("remove 2: "); print_forward(&list);   /* 0 1 15 3 */

    remove_node(&list, list.head);                /* head 삭제 */
    remove_node(&list, list.tail);                /* tail 삭제 */
    printf("rm ends : "); print_forward(&list);   /* 1 15 */
    printf("backward: "); print_backward(&list);
    printf("size = %d\n", list.size);

    dlist_free(&list);
    return 0;
}
