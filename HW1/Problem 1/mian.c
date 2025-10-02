#include <stdio.h>
#include <stdlib.h>

/* ====== Ackermann 函數 ====== */

// 遞迴版本
unsigned long long ack_recursive(unsigned long long m, unsigned long long n) {
    if (m == 0ULL) {
        return n + 1ULL;
    } else if (n == 0ULL) {
        return ack_recursive(m - 1ULL, 1ULL);
    } else {
        return ack_recursive(m - 1ULL, ack_recursive(m, n - 1ULL));
    }
}

/* ====== 自製堆疊結構 (用來模擬呼叫堆疊) ====== */
typedef struct {
    unsigned long long *data;
    size_t cap;
    size_t top;
} ULLStack;

void initStack(ULLStack *st) {
    st->data = NULL;
    st->cap = 0;
    st->top = 0;
}

void freeStack(ULLStack *st) {
    if (st->data) free(st->data);
}

void reserve(ULLStack *st, size_t new_cap) {
    if (new_cap <= st->cap) return;
    unsigned long long *nd = (unsigned long long*)malloc(new_cap * sizeof(unsigned long long));
    for (size_t i = 0; i < st->top; ++i) nd[i] = st->data[i];
    free(st->data);
    st->data = nd;
    st->cap = new_cap;
}

void push(ULLStack *st, unsigned long long v) {
    if (st->top >= st->cap) {
        size_t nc = (st->cap == 0 ? 1024 : st->cap * 2);
        reserve(st, nc);
    }
    st->data[st->top++] = v;
}

unsigned long long pop(ULLStack *st) {
    return st->data[--st->top];
}

int empty(ULLStack *st) {
    return st->top == 0;
}

/* ====== 非遞迴 Ackermann (手動堆疊) ====== */
unsigned long long ack_iterative(unsigned long long m, unsigned long long n) {
    ULLStack st;
    initStack(&st);
    push(&st, m);

    while (!empty(&st)) {
        m = pop(&st);
        if (m == 0ULL) {
            n = n + 1ULL;
        } else if (n == 0ULL) {
            push(&st, m - 1ULL);
            n = 1ULL;
        } else {
            push(&st, m - 1ULL);
            push(&st, m);
            n = n - 1ULL;
        }
    }

    freeStack(&st);
    return n;
}

/* ====== 主程式 ====== */
int main() {
    unsigned long long m, n;
    int how;

    printf("輸入 m 與 n：");
    scanf("%llu %llu", &m, &n);

    printf("選擇計算方式：1) 遞迴  2) 非遞迴  3) 兩者比對\n");
    scanf("%d", &how);

    if (how == 1) {
        printf("遞迴結果 = %llu\n", ack_recursive(m, n));
    } else if (how == 2) {
        printf("非遞迴結果 = %llu\n", ack_iterative(m, n));
    } else {
        unsigned long long a1 = ack_recursive(m, n);
        unsigned long long a2 = ack_iterative(m, n);
        printf("遞迴 = %llu, 非遞迴 = %llu\n", a1, a2);
    }

    return 0;
}
