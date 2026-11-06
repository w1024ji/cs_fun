#include <stdio.h>
#include <stdlib.h>

/* 이진 탐색 트리(BST): 왼쪽 서브트리 < 노드 < 오른쪽 서브트리
 * 출력 문구는 터미널 인코딩 문제를 피하려고 영어로 작성 */
typedef struct TNode {
    int key;
    struct TNode *left, *right;
} TNode;

static TNode *new_node(int key) {
    TNode *n = malloc(sizeof(TNode));
    n->key = key;
    n->left = n->right = NULL;
    return n;
}

/* 삽입: 평균 O(log n), 한쪽으로 치우치면 최악 O(n) */
TNode *insert(TNode *root, int key) {
    if (root == NULL) return new_node(key);
    if (key < root->key) root->left = insert(root->left, key);
    else if (key > root->key) root->right = insert(root->right, key);
    return root;                       /* 중복 키는 무시 */
}

/* 탐색: 평균 O(log n) */
TNode *search(TNode *root, int key) {
    while (root != NULL && root->key != key)
        root = (key < root->key) ? root->left : root->right;
    return root;
}

static TNode *min_node(TNode *n) {
    while (n->left != NULL) n = n->left;
    return n;
}

/* 삭제: 자식 수에 따라 3가지 경우 */
TNode *delete_key(TNode *root, int key) {
    if (root == NULL) return NULL;
    if (key < root->key) root->left = delete_key(root->left, key);
    else if (key > root->key) root->right = delete_key(root->right, key);
    else {
        if (root->left == NULL) {            /* 자식 0개 또는 오른쪽만 */
            TNode *r = root->right;
            free(root);
            return r;
        }
        if (root->right == NULL) {           /* 왼쪽 자식만 */
            TNode *l = root->left;
            free(root);
            return l;
        }
        /* 자식 2개: 오른쪽 서브트리의 최솟값(후속자)으로 대체 */
        TNode *succ = min_node(root->right);
        root->key = succ->key;
        root->right = delete_key(root->right, succ->key);
    }
    return root;
}

/* 깊이 우선 순회 3가지 */
void preorder(TNode *r)  { if (r) { printf("%d ", r->key); preorder(r->left);  preorder(r->right); } }
void inorder(TNode *r)   { if (r) { inorder(r->left);  printf("%d ", r->key); inorder(r->right); } }
void postorder(TNode *r) { if (r) { postorder(r->left); postorder(r->right); printf("%d ", r->key); } }

/* 레벨 순회(너비 우선): 큐 사용 */
void levelorder(TNode *root) {
    if (root == NULL) return;
    TNode *queue[100];
    int front = 0, rear = 0;
    queue[rear++] = root;
    while (front < rear) {
        TNode *cur = queue[front++];
        printf("%d ", cur->key);
        if (cur->left)  queue[rear++] = cur->left;
        if (cur->right) queue[rear++] = cur->right;
    }
}

int height(TNode *r) {
    if (r == NULL) return -1;          /* 빈 트리 -1, 노드 하나 0 */
    int hl = height(r->left), hr = height(r->right);
    return 1 + (hl > hr ? hl : hr);
}

void free_tree(TNode *r) {
    if (r) { free_tree(r->left); free_tree(r->right); free(r); }
}

int main(void) {
    /*        50
     *       /  \
     *     30    70
     *    /  \   / \
     *  20   40 60  80          */
    TNode *root = NULL;
    int keys[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < 7; i++) root = insert(root, keys[i]);

    printf("preorder  : "); preorder(root);   printf("\n");   /* 50 30 20 40 70 60 80 */
    printf("inorder   : "); inorder(root);    printf("\n");   /* 20 30 40 50 60 70 80 (정렬됨!) */
    printf("postorder : "); postorder(root);  printf("\n");   /* 20 40 30 60 80 70 50 */
    printf("levelorder: "); levelorder(root); printf("\n");   /* 50 30 70 20 40 60 80 */
    printf("height = %d\n\n", height(root));

    printf("search 60: %s\n", search(root, 60) ? "found" : "not found");
    printf("search 65: %s\n\n", search(root, 65) ? "found" : "not found");

    root = delete_key(root, 20);   /* 잎 노드 */
    printf("delete 20 (leaf)     : "); inorder(root); printf("\n");
    root = delete_key(root, 30);   /* 자식 1개 (40만 남음) */
    printf("delete 30 (1 child)  : "); inorder(root); printf("\n");
    root = delete_key(root, 50);   /* 루트, 자식 2개 */
    printf("delete 50 (2 child)  : "); inorder(root); printf("\n");
    printf("new root = %d\n", root->key);

    /* 정렬된 순서로 넣으면 한쪽으로 치우침: 최악 O(n) */
    TNode *skew = NULL;
    for (int i = 1; i <= 7; i++) skew = insert(skew, i);
    printf("\nskewed tree height = %d (balanced would be 2)\n", height(skew));

    free_tree(root);
    free_tree(skew);
    return 0;
}
