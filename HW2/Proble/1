# 41043231

作業 (一)
Problem 1

## 解題說明

本題要求實作 Ackermann 函數 (Ackermann function)，並且分別使用 遞迴版本與非遞迴版本來完成。
Ackermann 函數是一個經典的遞迴數學函數，定義如下：

<img width="657" height="135" alt="image" src="https://github.com/user-attachments/assets/58625674-9f03-4c59-b0c3-546bccd3e524" />

### 解題策略

1. 遞迴版本
   直接依照數學公式翻寫成程式。
   當 m=0 時，回傳 n+1；
   當 n=0 且 m>0 時，回傳 A(m-1, 1)；
   其他情況則回傳 A(m-1, A(m, n-1))。

2. 非遞迴版本
   因為 Ackermann 函數遞迴深度可能很大，容易造成 系統呼叫堆疊溢位 (Stack Overflow)。
   改以手動模擬堆疊的方式實作，把遞迴過程展開為迴圈。
   利用一個自製的堆疊結構存放尚未處理的 m 值，逐步計算出結果。

3. 主程式流程
   讓使用者輸入 m 與 n。
   提供選項：
      (1) 使用遞迴版本計算。
      (2) 使用非遞迴版本計算。
      (3) 兩者都算並比對結果是否一致。

## 程式實作

以下為程式碼：

```cpp
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

```

## 效能分析

1. 時間複雜度：程式的時間複雜度約為 $O(2^n)$ 等級，因為 Ackermann 函數的成長速度遠快於一般線性或多項式演算法。
2. 空間複雜度：空間複雜度可視為 $O(m \times n)$ 的規模，其中遞迴版本受到呼叫堆疊深度限制，而非遞迴版本則需要額外的堆疊記憶體來模擬遞迴。

## 測試與驗證
<img width="426" height="123" alt="image" src="https://github.com/user-attachments/assets/a2d8eacc-a1e8-4f27-8186-95742cda29bc" />

<img width="430" height="507" alt="image" src="https://github.com/user-attachments/assets/12457509-ebc9-465b-a71b-998a8afb45c4" />




### 測試案例

| 測試案例 | 輸入參數 $(m,n)$ | 預期輸出 | 實際輸出 |
| ---- | ------------ | ---- | ---- |
| 測試一  | $(0,0)$      | 1    | 1    |
| 測試二  | $(1,2)$      | 4    | 4    |
| 測試三  | $(2,3)$      | 9    | 9    |
| 測試四  | $(3,3)$      | 61   | 61   |
| 測試五  | $(3,4)$      | 125  | 125  |


### 編譯與執行指令

```shell
$ gcc hw1.c -o hw1
$ ./hw1
```

### 結論

程式能正確計算 Ackermann 函數 $A(m,n)$ 的值。
在小範圍輸入（例如 $m \leq 3$）下，遞迴版與非遞迴版輸出結果一致，驗證程式邏輯正確。
測試案例涵蓋了多種情況（$m=0$、$n=0$、$m,n>0$），有效驗證程式的正確性。
需要注意的是，當 $m$ 或 $n$ 較大時，Ackermann 函數的成長極快，可能導致執行時間過長或記憶體不足。

## 申論及開發報告

### 開發過程中的挑戰

1. 遞迴深度限制
   C 語言的系統堆疊有限，當輸入稍微大一點時
   (例如 $A(4,1)$)，遞迴版本可能會因呼叫深度過高而導致程式崩潰。
3. 非遞迴的實作
   必須手動設計一個堆疊結構，來模擬函式呼叫的展開與回溯。
   如何正確地壓入與彈出參數，是確保計算正確性的關鍵。

3. 效能上的限制
   即使是非遞迴版本，當 $m$ 或 $n$ 過大時，計算步驟依然呈現爆炸性成長。
   因此本程式僅適合用於展示程式設計技巧，而不適合作為實務計算工具。
## 心得
透過本次實作，我更加理解了 遞迴的運作機制，以及 如何用迴圈與堆疊結構來取代遞迴。
Ackermann 函數雖然在實務上幾乎沒有應用，但它是一個很好的教材，能幫助我們清楚看到遞迴深度、空間使用，以及不同實作方式的差異。
這題目的重點不在於「計算多大數字」，而在於體會「遞迴與非遞迴的思維轉換」。
