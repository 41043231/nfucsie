#include <stdio.h>
#include <stdlib.h>

/* ====== Ackermann 函數 ====== */

// 遞迴版本
// 根據數學定義：
// A(0, n) = n + 1
// A(m, 0) = A(m-1, 1)
// A(m, n) = A(m-1, A(m, n-1))
unsigned long long ack_recursive(unsigned long long m, unsigned long long n) {
    if (m == 0ULL) {
        return n + 1ULL;  // 規則 1：當 m=0 時，直接回傳 n+1
    } else if (n == 0ULL) {
        return ack_recursive(m - 1ULL, 1ULL);  // 規則 2：當 n=0 且 m>0，則呼叫 A(m-1, 1)
    } else {
        // 規則 3：一般情況，呼叫 A(m-1, A(m, n-1))
        return ack_recursive(m - 1ULL, ack_recursive(m, n - 1ULL));
    }
}

/* ====== 自製堆疊結構 (用來模擬呼叫堆疊) ====== */
// 在非遞迴版本中，我們需要用一個手動建立的堆疊來模擬遞迴過程
typedef struct {
    unsigned long long *data; // 存放堆疊中的元素 (這裡存 m 的值)
    size_t cap;               // 堆疊容量 (目前配置的大小)
    size_t top;               // 堆疊頂端位置 (指向下一個可存放的位置)
} ULLStack;

// 初始化堆疊
void initStack(ULLStack *st) {
    st->data = NULL;
    st->cap = 0;
    st->top = 0;
}

// 釋放堆疊記憶體
void freeStack(ULLStack *st) {
    if (st->data) free(st->data);
}

// 動態擴充堆疊容量
void reserve(ULLStack *st, size_t new_cap) {
    if (new_cap <= st->cap) return;  // 如果新容量小於等於現有容量就不用擴充
    unsigned long long *nd = (unsigned long long*)malloc(new_cap * sizeof(unsigned long long));
    for (size_t i = 0; i < st->top; ++i) nd[i] = st->data[i];  // 把舊資料複製過來
    free(st->data);
    st->data = nd;
    st->cap = new_cap;
}

// 壓入元素到堆疊
void push(ULLStack *st, unsigned long long v) {
    if (st->top >= st->cap) {
        size_t nc = (st->cap == 0 ? 1024 : st->cap * 2);  // 若容量不足就擴充一倍
        reserve(st, nc);
    }
    st->data[st->top++] = v;  // 放入新元素並移動堆疊頂端
}

// 彈出堆疊頂端元素
unsigned long long pop(ULLStack *st) {
    return st->data[--st->top];
}

// 檢查堆疊是否為空
int empty(ULLStack *st) {
    return st->top == 0;
}

/* ====== 非遞迴 Ackermann (手動堆疊) ====== */
// 使用 while 迴圈 + 手動堆疊來模擬遞迴的呼叫過程
unsigned long long ack_iterative(unsigned long long m, unsigned long long n) {
    ULLStack st;
    initStack(&st);
    push(&st, m);  // 初始把 m 放入堆疊

    while (!empty(&st)) {
        m = pop(&st);  // 取出堆疊頂端的 m
        if (m == 0ULL) {
            n = n + 1ULL;  // 規則 1：A(0, n) = n+1
        } else if (n == 0ULL) {
            push(&st, m - 1ULL);  // 規則 2：A(m, 0) = A(m-1, 1)
            n = 1ULL;
        } else {
            // 規則 3：A(m, n) = A(m-1, A(m, n-1))
            // 先把外層的 m-1 壓入堆疊
            push(&st, m - 1ULL);
            // 再把 m 自己壓回去，模擬稍後還要再處理
            push(&st, m);
            // 將 n 減 1，表示去計算 A(m, n-1)
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
        // 單純使用遞迴版本
        printf("遞迴結果 = %llu\n", ack_recursive(m, n));
    } else if (how == 2) {
        // 單純使用非遞迴版本
        printf("非遞迴結果 = %llu\n", ack_iterative(m, n));
    } else {
        // 兩者都計算，並比對是否一致
        unsigned long long a1 = ack_recursive(m, n);
        unsigned long long a2 = ack_iterative(m, n);
        printf("遞迴 = %llu, 非遞迴 = %llu\n", a1, a2);
    }

    return 0;
}
/*
#include <stdio.h>
#include <stdlib.h>

/* ====== Ackermann 函數 ====== 

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

/* ====== 自製堆疊結構 (用來模擬呼叫堆疊) ====== 
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

/* ====== 非遞迴 Ackermann (手動堆疊) ====== 
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

/* ====== 主程式 ====== 
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
*/
