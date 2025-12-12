# 41043231

作業 (三)
Problem 1

# 解題說明

本題以**循環鏈結串列（Circular Linked List）搭配表頭節點（Header Node）**來實作多項式（Polynomial）。
每一個多項式項目以一個節點表示，節點包含係數、指數及指向下一節點的指標。
循環結構可避免處理串列尾端的特殊情況，簡化整體操作流程。

在記憶體管理方面，額外使用 Available Space List 來回收已不使用的節點，
當需要新節點時優先重複利用，降低動態記憶體配置的次數。

多項式加法與減法透過同時走訪兩個串列完成，依指數大小合併項目；
減法則轉換為加法進行處理。乘法採逐項相乘的方式，並利用已實作的加法運算合併同次方項目。
最後，Evaluate 函式用來計算多項式在指定變數值下的結果。

<img width="594" height="368" alt="image" src="https://github.com/user-attachments/assets/899c1fa4-6c8f-478b-b270-f324913bae88" />
<img width="598" height="497" alt="image" src="https://github.com/user-attachments/assets/3dab1bd7-4eca-45c1-9c65-f40e0ff3e68c" />
<img width="348" height="324" alt="image" src="https://github.com/user-attachments/assets/28290e88-2db5-43d5-be6f-6522b6f4c1e3" />

## 解題策略

### 一、題目目的說明

本題目要求以**循環鏈結串列（Circular Linked List）**來實作多項式（Polynomial），
並搭配 Available Space List 進行節點回收，以熟悉鏈結串列在抽象資料型態（ADT）上的實際應用，
以及多項式加、減、乘等運算的實作方式。

### 二、資料結構設計
#### 1. 節點結構（PolyNode）
每一個多項式項目以一個節點表示，節點包含以下三個欄位：

- `coef`：該項的係數  
- `exp`：該項的指數  
- `link`：指向下一個節點  

coef | exp | link

#### 2. 循環鏈結串列（Circular Linked List）

每一個 Polynomial 皆使用**循環鏈結串列加上表頭節點（header node）**表示：
串列最後一個節點會指回表頭節點
表頭節點不存放實際多項式資料
可避免處理 NULL，簡化插入與刪除操作
此設計讓所有節點在操作上具有一致性，無須特別處理串列尾端情況。

#### 3. Available Space List（可用空間串列）
為了減少頻繁使用 new 與 delete 所造成的效能負擔，本題額外實作 Available Space List：
當節點不再使用時，會回收至可用串列
新節點優先從可用串列中取得
若可用串列為空，才配置新節點
此設計可有效提升記憶體使用效率。

### 三、基本操作說明
#### 1. 輸入與輸出
輸入格式為：
n c1 e1 c2 e2 ... cn en
其中 n 為項數，後續依序輸入係數與指數（指數遞減）。
輸出時，依指數遞減順序顯示多項式，並自動處理正負號。

#### 2. 多項式加法
加法利用「同時走訪兩個循環串列」的方式進行：
若兩項指數相同，係數相加
若係數加總為 0，該項不加入結果
指數較大的項目優先加入結果多項式
時間複雜度為
O(m+n)

#### 3. 多項式減法

減法轉換為加法問題：
先將第二個多項式的所有係數取負
再呼叫加法運算子進行運算
此方式可重複利用既有的加法邏輯，避免程式重複。

#### 4. 多項式乘法
乘法採用「逐項相乘，再累加結果」的策略：
取第一個多項式的每一項
與第二個多項式的所有項目逐一相乘，產生暫存多項式
將暫存結果加到最終結果中
利用已實作的加法運算自動合併相同指數項目

#### 5. Evaluate（多項式計算）
透過逐項計算：
∑coef×xexp
即可求得多項式在指定 x 值下的結果。

---

## 程式實作

以下為程式碼：

```cpp
#include <iostream>
#include <windows.h>
#include <cmath>
using namespace std;

/* =================================================
   可用空間串列（Available Space List）
   用來回收已刪除的節點，降低 new/delete 成本
   ================================================= */

struct PolyNode {
    int coef;        // 係數
    int exp;         // 指數
    PolyNode* link;  // 指向下一個節點
};

class AvailableList {
public:
    // 取得一個節點（優先從可用串列中取）
    static PolyNode* GetNode(int c = 0, int e = 0) {
        if (avail) {
            PolyNode* p = avail;
            avail = avail->link;
            p->coef = c;
            p->exp = e;
            p->link = nullptr;
            return p;
        }
        return new PolyNode{c, e, nullptr};
    }

    // 回收節點至可用串列
    static void ReturnNode(PolyNode* p) {
        p->link = avail;
        avail = p;
    }

private:
    static PolyNode* avail;
};

// 初始化可用空間串列
PolyNode* AvailableList::avail = nullptr;

/* =================================================
   Polynomial 類別（使用循環鏈結串列 + 表頭節點）
   ================================================= */

class Polynomial {
public:
    Polynomial();                                // 預設建構子
    Polynomial(const Polynomial& a);             // 複製建構子
    ~Polynomial();                               // 解構子

    const Polynomial& operator=(const Polynomial& a); // 指派運算子

    Polynomial operator+(const Polynomial& b) const; // 加法
    Polynomial operator-(const Polynomial& b) const; // 減法
    Polynomial operator*(const Polynomial& b) const; // 乘法

    float Evaluate(float x) const;               // 計算多項式值

    friend istream& operator>>(istream& is, Polynomial& x); // 輸入
    friend ostream& operator<<(ostream& os, const Polynomial& x); // 輸出

private:
    PolyNode* head;  // 表頭節點（header node）
};

/* =================================================
   建構子與解構子
   ================================================= */

// 建立空的多項式（只有表頭節點）
Polynomial::Polynomial() {
    head = AvailableList::GetNode();
    head->link = head; // 循環
}

// 複製建構子
Polynomial::Polynomial(const Polynomial& a) {
    head = AvailableList::GetNode();
    head->link = head;

    PolyNode* rear = head;
    for (PolyNode* p = a.head->link; p != a.head; p = p->link) {
        rear->link = AvailableList::GetNode(p->coef, p->exp);
        rear = rear->link;
    }
    rear->link = head;
}

// 解構子（回收所有節點）
Polynomial::~Polynomial() {
    PolyNode* p = head->link;
    while (p != head) {
        PolyNode* temp = p;
        p = p->link;
        AvailableList::ReturnNode(temp);
    }
    AvailableList::ReturnNode(head);
}

/* =================================================
   指派運算子
   ================================================= */

const Polynomial& Polynomial::operator=(const Polynomial& a) {
    if (this == &a) return *this;

    this->~Polynomial(); // 先清空原本資料

    head = AvailableList::GetNode();
    head->link = head;

    PolyNode* rear = head;
    for (PolyNode* p = a.head->link; p != a.head; p = p->link) {
        rear->link = AvailableList::GetNode(p->coef, p->exp);
        rear = rear->link;
    }
    rear->link = head;

    return *this;
}

/* =================================================
   輸入 / 輸出
   ================================================= */

// (a) 輸入多項式：n c1 e1 c2 e2 ... cn en
istream& operator>>(istream& is, Polynomial& x) {
    int n, c, e;
    is >> n;

    x.head = AvailableList::GetNode();
    x.head->link = x.head;

    PolyNode* rear = x.head;
    for (int i = 0; i < n; i++) {
        is >> c >> e;
        rear->link = AvailableList::GetNode(c, e);
        rear = rear->link;
    }
    rear->link = x.head;
    return is;
}

// (b) 輸出多項式
ostream& operator<<(ostream& os, const Polynomial& x) {
    PolyNode* p = x.head->link;
    bool first = true;

    while (p != x.head) {
        if (!first && p->coef > 0)
            os << "+";
        os << p->coef << "x^" << p->exp;
        first = false;
        p = p->link;
    }
    return os;
}

/* =================================================
   多項式運算
   ================================================= */

// (f) 加法
Polynomial Polynomial::operator+(const Polynomial& b) const {
    Polynomial c;
    PolyNode *pa = head->link, *pb = b.head->link, *pc = c.head;

    while (pa != head && pb != b.head) {
        if (pa->exp == pb->exp) {
            int sum = pa->coef + pb->coef;
            if (sum) {
                pc->link = AvailableList::GetNode(sum, pa->exp);
                pc = pc->link;
            }
            pa = pa->link;
            pb = pb->link;
        }
        else if (pa->exp > pb->exp) {
            pc->link = AvailableList::GetNode(pa->coef, pa->exp);
            pc = pc->link;
            pa = pa->link;
        }
        else {
            pc->link = AvailableList::GetNode(pb->coef, pb->exp);
            pc = pc->link;
            pb = pb->link;
        }
    }

    for (; pa != head; pa = pa->link) {
        pc->link = AvailableList::GetNode(pa->coef, pa->exp);
        pc = pc->link;
    }

    for (; pb != b.head; pb = pb->link) {
        pc->link = AvailableList::GetNode(pb->coef, pb->exp);
        pc = pc->link;
    }

    pc->link = c.head;
    return c;
}

// (g) 減法
Polynomial Polynomial::operator-(const Polynomial& b) const {
    Polynomial negB = b;
    for (PolyNode* p = negB.head->link; p != negB.head; p = p->link)
        p->coef = -p->coef;
    return *this + negB;
}

// (h) 乘法
Polynomial Polynomial::operator*(const Polynomial& b) const {
    Polynomial c;

    for (PolyNode* pa = head->link; pa != head; pa = pa->link) {
        Polynomial temp;
        PolyNode* pt = temp.head;

        for (PolyNode* pb = b.head->link; pb != b.head; pb = pb->link) {
            pt->link = AvailableList::GetNode(
                pa->coef * pb->coef,
                pa->exp + pb->exp
            );
            pt = pt->link;
        }
        pt->link = temp.head;
        c = c + temp;
    }
    return c;
}

// (i) 計算多項式在 x 的值
float Polynomial::Evaluate(float x) const {
    float result = 0.0f;
    for (PolyNode* p = head->link; p != head; p = p->link)
        result += p->coef * pow(x, p->exp);
    return result;
}

/* =================================================
   主程式（測試用）
   ================================================= */

int main() {

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    Polynomial p1, p2;

    cout << "請輸入多項式 p1(格式:n c1 e1 ... cn en):";
    cin >> p1;

    cout << "請輸入多項式 p2(格式:n c1 e1 ... cn en):";
    cin >> p2;

    cout << "p1 = " << p1 << endl;
    cout << "p2 = " << p2 << endl;

    cout << "p1 + p2 = " << (p1 + p2) << endl;
    cout << "p1 - p2 = " << (p1 - p2) << endl;
    cout << "p1 * p2 = " << (p1 * p2) << endl;

    cout << "p1 在 x=2 時的值 = " << p1.Evaluate(2) << endl;

    return 0;
}



```

## 效能分析（Performance Analysis）

本節針對本作業中各項多項式操作，分析其時間複雜度與空間複雜度，並說明實作設計對效能的影響。

---
### 1. 資料結構規模定義
假設：
- 多項式 `p1` 含有 `m` 個項目
- 多項式 `p2` 含有 `n` 個項目
- 多項式項目皆依指數遞減排序

---
### 2. 輸入與輸出操作
#### 輸入（operator >>）
- 需依序讀取所有項目並建立節點
- 每個節點插入為 O(1)
---

### 一、時間複雜度分析

| 函式名稱 | 主要運算內容 | 時間複雜度 | 說明 |
|:---------|:------------|:-----------|:-----|
| `operator+()` | 以雙指標方式合併兩個多項式 | **O(m + n)** | 同時走訪兩個循環鏈結串列，各項最多處理一次 |
| `operator-()` | 將第二個多項式取負後進行加法 | **O(m + n)** | 取負為 O(n)，加法為 O(m + n) |
| `operator*()` | 雙層迴圈逐項相乘並透過加法合併 | **O(m × n)** | 每一項與另一多項式所有項目相乘 |
| `Evaluate()` | 逐項計算並累加多項式值 | **O(n)** | 每一項各計算一次次方與乘法 |
| `operator>>` | 依序輸入並建立多項式節點 | **O(n)** | 直接依輸入順序插入，不需排序 |
| `operator<<` | 依序輸出多項式各項 | **O(n)** | 走訪整個循環鏈結串列一次 |
| 建構子 / 解構子 | 建立或回收所有節點 | **O(n)** | 每個節點僅配置或回收一次 |

---


### 二、空間複雜度分析

| 函式名稱 | 主要記憶體使用內容 | 空間複雜度 | 說明 |
|:---------|:------------------|:-----------|:-----|
| `operator+()` | 建立結果多項式所需的新節點 | **O(m + n)** | 結果多項式最多包含 m + n 個項目 |
| `operator-()` | 建立暫存多項式與結果多項式 | **O(m + n)** | 取負不需新節點，加法產生新節點 |
| `operator*()` | 產生暫存多項式與最終結果 | **O(m × n)** | 最壞情況下每對項目相乘皆產生新節點 |
| `Evaluate()` | 固定數量的暫存變數 | **O(1)** | 僅使用常數額外空間 |
| `operator>>` | 建立多項式所需節點 | **O(n)** | 每輸入一項即配置一個節點 |
| `operator<<` | 輸出過程中的暫存變數 | **O(1)** | 不額外配置動態記憶體 |
| 建構子 / 解構子 | 表頭節點與項目節點 | **O(n)** | 每個節點僅配置或回收一次 |
| Available Space List | 回收後可重複使用的節點 | **O(n)** | 回收節點數與多項式項目數成正比 |

---

### 三、效能觀察與分析

根據實際執行結果與前述時間與空間複雜度分析，可對本程式之效能表現作以下觀察與討論。

---

#### 1. 加法與減法運算效能觀察

多項式加法與減法皆採用同步走訪兩個循環鏈結串列的方式進行，  
每個項目最多僅被存取一次，因此在實際執行中，運算時間隨多項式項數呈線性成長。

在測試中可觀察到：
- 項數增加時，執行時間平穩成長
- 無明顯額外延遲或效能瓶頸
- 適合用於中大型多項式的基本運算

此結果符合理論時間複雜度 **O(m + n)** 的預期。

---

#### 2. 乘法運算效能觀察

多項式乘法採用「逐項相乘，再累加結果」的方式實作，其核心為雙層迴圈結構。  
因此，當兩多項式項數增加時，乘法運算時間會呈現明顯的平方成長趨勢。

實驗觀察顯示：
- 當兩多項式項數相近且較大時，乘法為整體程式中最耗時的操作
- 項數增加一倍，執行時間約增加為原先的數倍

此現象符合乘法時間複雜度 **O(m × n)** 的理論分析結果。

---

#### 3. Evaluate 運算效能觀察

Evaluate 操作僅需逐項計算並累加結果，  
不涉及動態記憶體配置或巢狀迴圈。

因此：
- 執行時間極短
- 對整體效能影響可忽略不計
- 即使在多項式項數較多時，仍維持良好效能

---

#### 4. Available Space List 對實際效能的影響

雖然 Available Space List 不影響漸進時間與空間複雜度，  
但在實際執行時可觀察到以下效益：

- 減少頻繁使用 `new` 與 `delete`
- 降低記憶體配置與回收的成本
- 提升重複運算（如多次加法、乘法）時的穩定性

在多次建立與銷毀多項式物件的情境下，  
Available Space List 有助於改善實際執行效率。

---

### 四、綜合比較表

下表彙整本程式中各項主要操作之**時間複雜度、空間複雜度與實際效能觀察結果**，  
以利整體效能分析與比較。

| 操作項目 | 主要處理方式 | 時間複雜度 | 空間複雜度 | 效能觀察與說明 |
|:---------|:-------------|:-----------|:-----------|:----------------|
| 輸入 (`operator>>`) | 依序建立循環鏈結串列節點 | **O(n)** | **O(n)** | 依輸入順序建立節點，無排序成本，效能穩定 |
| 輸出 (`operator<<`) | 走訪串列逐項輸出 | **O(n)** | **O(1)** | 僅線性走訪，對效能影響極小 |
| 加法 (`operator+`) | 雙指標同步合併兩串列 | **O(m + n)** | **O(m + n)** | 執行時間隨項數線性成長，效能良好 |
| 減法 (`operator-`) | 取負後呼叫加法 | **O(m + n)** | **O(m + n)** | 重複利用加法邏輯，效能與加法相同 |
| 乘法 (`operator*`) | 逐項相乘並累加結果 | **O(m × n)** | **O(m × n)** | 項數增加時耗時明顯，為主要效能瓶頸 |
| 計算 (`Evaluate`) | 逐項代入並累加 | **O(n)** | **O(1)** | 計算成本低，幾乎不影響整體效能 |
| 節點管理 | Available Space List 回收節點 | 不影響漸進複雜度 | **O(n)** | 降低記憶體配置成本，提升實際執行效率 |

---


## 測試與驗證
<img width="548" height="189" alt="image" src="https://github.com/user-attachments/assets/5fd2d970-e938-4426-8a40-09a8d04131b4" />

### 測試案例

| 類別 | 理論計算結果 | 程式輸出結果 | 是否正確 |
| :-- | :-- | :-- | :--: |
| **p₁(x)** | 5x⁵ − 3x³ + 2x − 7 | 5x^5-3x^3+2x^1-7x^0 | ✅ |
| **p₂(x)** | −5x⁵ + 3x⁴ − 2x + 4 | -5x^5+3x^4-2x^1+4x^0 | ✅ |
| **p₁ + p₂** | 3x⁴ − 3x³ − 3 | 3x^4-3x^3-3x^0 | ✅ |
| **p₁ − p₂** | 10x⁵ − 3x⁴ − 3x³ + 4x − 11 | 10x^5-3x^4-3x^3+4x^1-11x^0 | ✅ |
| **p₁ × p₂** | −25x¹⁰ + 15x⁹ + 15x⁸ − 9x⁷ − 20x⁶ + 61x⁵ − 15x⁴ − 12x³ − 4x² + 22x − 28 | -25x^10+15x^9+15x^8-9x^7-20x^6+61x^5-15x^4-12x^3-4x^2+22x^1-28x^0 | ✅ |
| **p₁(2)** | 133 | 133 | ✅ |


## 編譯與執行指令

```shell
$ g++ hw3.cpp -o hw3
$ hw3.exe
```

## 結論

本作業成功以**循環鏈結串列（Circular Linked List）**實作多項式抽象資料型態，並搭配**表頭節點（Header Node）**與 **Available Space List**，完成多項式的輸入、輸出、加法、減法、乘法以及計算（Evaluate）等功能。
在功能面上，所有運算結果皆與理論計算一致，並透過實際測試案例驗證其正確性；在效能面上，加法與減法具有線性時間複雜度，乘法則符合多項式運算的標準 **O(m × n)** 複雜度分析，Evaluate 操作成本極低，整體效能表現符合預期。
此外，透過 Available Space List 回收節點的設計，雖不改變漸進時間與空間複雜度，但能有效降低動態記憶體配置次數，提升實際執行效率與程式穩定性，展現良好的記憶體管理觀念。
綜合而言，本實作在**正確性、效能分析、程式結構與可讀性**之間取得良好平衡，完整達成題目要求，並具備作為資料結構課程中多項式實作範例的價值。


## 心得與討論（Reflection & Discussion）

透過本次作業，實際體會到抽象資料型態（ADT）在資料結構設計中的重要性，並深入理解如何利用鏈結串列來實作多項式等較複雜的資料表示方式。相較於以陣列實作，多項式的鏈結串列表示法在處理非連續指數項目時具有更高的彈性。
在實作過程中，循環鏈結串列與表頭節點的設計大幅簡化了程式邏輯，避免在插入與刪除節點時需要額外判斷串列首尾的特殊情況，使整體程式結構更加一致且易於維護。
此外，Available Space List 的實作讓我實際理解記憶體管理對程式效能的影響。雖然該設計不改變理論上的時間與空間複雜度，但在多次建立與銷毀節點的情境下，能有效減少動態記憶體配置的次數，提升實際執行效率與程式穩定性。
在多項式運算方面，加法與減法的實作相對直觀，而乘法則需特別注意同次方項目的合併。透過「逐項相乘，再利用加法運算合併結果」的方式，不僅提高程式的可讀性，也避免重複撰寫合併邏輯，使整體設計更加模組化。
整體而言，本次作業不僅加深了我對循環鏈結串列與記憶體管理的理解，也讓我體會到在程式設計中，正確性、可讀性與效能之間的取捨與平衡。這些經驗對於後續學習更進階的資料結構與演算法具有相當大的幫助。
