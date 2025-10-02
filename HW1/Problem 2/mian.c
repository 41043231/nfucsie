#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ====== 輔助函數：輸出一個子集合 ====== */
void print_subset(char **elems, int *pick, int n) {
    printf("(");
    int first = 1;
    for (int i = 0; i < n; ++i) {
        if (pick[i]) {
            if (!first) printf(", ");
            printf("%s", elems[i]);
            first = 0;
        }
    }
    printf(")\n");
}

/* ====== 遞迴函數：逐個決定是否選取元素 ====== */
void powerset_dfs(int idx, int n, char **elems, int *pick) {
    if (idx == n) {
        print_subset(elems, pick, n);
        return;
    }
    // 不選 elems[idx]
    pick[idx] = 0;
    powerset_dfs(idx + 1, n, elems, pick);
    // 選 elems[idx]
    pick[idx] = 1;
    powerset_dfs(idx + 1, n, elems, pick);
}

/* ====== 主程式 ====== */
int main() {
    int n;
    printf("輸入元素數量：");
    scanf("%d", &n);

    // 配置存放元素字串的陣列
    char **elems = (char**)malloc(sizeof(char*) * (n > 0 ? n : 1));
    for (int i = 0; i < n; ++i) {
        elems[i] = (char*)malloc(64);  // 每個元素最多 63 字元
    }

    printf("請輸入 %d 個元素（以空白分隔）：\n", n);
    for (int i = 0; i < n; ++i) {
        scanf("%s", elems[i]);
    }

    int *pick = (int*)malloc(sizeof(int) * n);
    for (int i = 0; i < n; ++i) pick[i] = 0;

    printf("\n所有子集合 (Power set)：\n");
    powerset_dfs(0, n, elems, pick);

    // 釋放記憶體
    for (int i = 0; i < n; ++i) free(elems[i]);
    free(elems);
    free(pick);

    return 0;
}

