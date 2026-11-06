#include <stdio.h>
#include <stdlib.h>

/* 단일 연결 리스트 노드 */
typedef struct Node {
    int data;
    struct Node *next;
} Node;

/* 맨 앞 삽입: O(1) */
Node *insert_front(Node *head, int value) {
    Node *n = malloc(sizeof(Node));
    n->data = value;
    n->next = head;
    return n; /* 새 head */
}

/* 맨 뒤 삽입: tail 포인터가 없으면 O(n) */
Node *insert_back(Node *head, int value) {
    Node *n = malloc(sizeof(Node));
    n->data = value;
    n->next = NULL;
    if (head == NULL) return n;
    Node *cur = head;
    while (cur->next != NULL) cur = cur->next;
    cur->next = n;
    return head;
}

/* 값으로 삭제: 탐색 O(n), 삭제 자체는 O(1) */
Node *delete_value(Node *head, int value) {
    Node *prev = NULL, *cur = head;
    while (cur != NULL && cur->data != value) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == NULL) return head;          /* 없음 */
    if (prev == NULL) head = cur->next;    /* head 삭제 */
    else prev->next = cur->next;
    free(cur);
    return head;
}

/* 리스트 뒤집기: O(n), 포인터 3개 */
Node *reverse(Node *head) {
    Node *prev = NULL, *cur = head;
    while (cur != NULL) {
        Node *next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    return prev;
}

void print_list(Node *head) {
    for (Node *c = head; c != NULL; c = c->next) printf("%d -> ", c->data);
    printf("NULL\n");
}

void free_list(Node *head) {
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

int main(void) {
    /* 배열: 연속 메모리, 인덱스 접근 O(1) */
    int arr[5] = {10, 20, 30, 40, 50};
    printf("arr[2] = %d, *(arr+2) = %d\n", arr[2], *(arr + 2));

    /* 배열 중간 삽입: 뒤 원소를 한 칸씩 밀어야 하므로 O(n) */
    int big[6] = {10, 20, 30, 40, 50};
    for (int i = 5; i > 2; i--) big[i] = big[i - 1];
    big[2] = 25;
    printf("array after insert: ");
    for (int i = 0; i < 6; i++) printf("%d ", big[i]);
    printf("\n\n");

    /* 연결 리스트 */
    Node *head = NULL;
    head = insert_back(head, 1);
    head = insert_back(head, 2);
    head = insert_back(head, 3);
    head = insert_front(head, 0);
    print_list(head);          /* 0 -> 1 -> 2 -> 3 */
    head = delete_value(head, 2);
    print_list(head);          /* 0 -> 1 -> 3 */
    head = reverse(head);
    print_list(head);          /* 3 -> 1 -> 0 */
    free_list(head);
    return 0;
}
