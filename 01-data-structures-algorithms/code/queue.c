#include <stdio.h>

/* 원형 큐(circular queue): FIFO (First In, First Out)
 * 일반 배열 큐는 dequeue 후 앞쪽 공간을 못 써서 낭비된다.
 * 인덱스를 % SIZE로 순환시키면 공간을 재사용할 수 있다. */
#define SIZE 3

typedef struct {
    int data[SIZE];
    int front;      /* 다음에 꺼낼 원소 위치 */
    int rear;       /* 다음에 넣을 위치 */
    int count;      /* 원소 개수: front==rear일 때 가득/빈 상태를 구분하기 위해 사용 */
} Queue;

void queue_init(Queue *q) { q->front = q->rear = q->count = 0; }
int  q_empty(Queue *q)    { return q->count == 0; }
int  q_full(Queue *q)     { return q->count == SIZE; }

/* enqueue: O(1) */
int enqueue(Queue *q, int v) {
    if (q_full(q)) return 0;
    q->data[q->rear] = v;
    q->rear = (q->rear + 1) % SIZE;
    q->count++;
    return 1;
}

/* dequeue: O(1) */
int dequeue(Queue *q, int *out) {
    if (q_empty(q)) return 0;
    *out = q->data[q->front];
    q->front = (q->front + 1) % SIZE;
    q->count--;
    return 1;
}

int main(void) {
    Queue q;
    queue_init(&q);

    for (int i = 1; i <= 5; i++) enqueue(&q, i * 10);
    printf("full 상태에서 enqueue: %s\n", enqueue(&q, 60) ? "성공" : "실패(overflow)");

    int v;
    dequeue(&q, &v); printf("dequeue: %d\n", v);   /* 10 : 먼저 넣은 것이 먼저 나옴 */
    dequeue(&q, &v); printf("dequeue: %d\n", v);   /* 20 */

    enqueue(&q, 60);                               /* 앞쪽 빈 공간 재사용 */
    enqueue(&q, 70);
    printf("front=%d rear=%d count=%d\n", q.front, q.rear, q.count);

    printf("남은 순서: ");
    while (dequeue(&q, &v)) printf("%d ", v);       /* 30 40 50 60 70 */
    printf("\n");
    return 0;
}
