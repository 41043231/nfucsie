# 41043231

作業 (二)
Problem 1

## 解題說明

本題要求設計一個能進行多項式運算的程式，需以**抽象資料型態（ADT）**的概念實作 Polynomial 類別，
並支援以下三種主要功能：

1. **多項式加法（Add）**  
   將兩個多項式中指數相同的項合併，指數不同的項則依照指數大小排列，  
   最終輸出一個新的多項式作為加法結果。

2. **多項式乘法（Mult）**  
   將兩個多項式進行逐項相乘，並對結果中指數相同的項進行合併與排序，  
   輸出乘法展開後的多項式。

3. **多項式值的計算（Eval）**  
   能代入一個指定的實數 \( x \) 值，計算該多項式在該點的函數值。

<img width="1156" height="828" alt="image" src="https://github.com/user-attachments/assets/11066922-d28d-4d63-95dd-108d53e962d8" />
<img width="1140" height="765" alt="image" src="https://github.com/user-attachments/assets/7053ef67-5691-4a76-af69-3363e0a5ef41" />
<img width="1148" height="741" alt="image" src="https://github.com/user-attachments/assets/0ec521b7-b4fa-4474-b1fa-663cf646cbd3" />

### 解題策略

# 解題策略（Solution Strategy）

本題採用 **物件導向程式設計（Object-Oriented Programming, OOP）** 的思維，  
以類別（Class）封裝多項式資料與相關運算，達成模組化與可維護性高的設計。  
整體策略分為三個階段：**輸入 → 運算 → 輸出**。

---

## 一、資料結構設計

1. **Term 類別**  
   - 負責儲存多項式中單一項的資料，包括：
     - `coef`：係數（float）
     - `exp`：指數（int）  
   - 設為 `private`，確保資料封裝性，並以 `friend class Polynomial` 讓 `Polynomial` 可直接存取。

2. **Polynomial 類別**  
   - 以動態陣列 `termArray` 儲存所有非零項。  
   - 包含三個主要屬性：
     - `terms`：實際項數
     - `capacity`：陣列容量
     - `termArray`：項目儲存空間  
   - 提供三個主要運算函式：
     - `Add()`：多項式加法  
     - `Mult()`：多項式乘法  
     - `Eval()`：多項式值計算  

---

## 二、運算邏輯設計

1. **多項式加法（Add）**  
   - 同時遍歷兩個多項式陣列。  
   - 若指數相同，則係數相加；若不同，將較大指數者直接加入結果。  
   - 結束後若仍有剩餘項，直接附加至結果中。  

2. **多項式乘法（Mult）**  
   - 以雙層迴圈進行項與項相乘。  
   - 對於結果中相同指數的項，將係數累加。  
   - 完成後依指數由大至小排序，並移除係數為 0 的項。  

3. **多項式值計算（Eval）**  
   - 逐項代入輸入的 \( x \) 值。  
   - 使用 `pow(x, exp)` 計算每項的冪次，並乘以係數後累加。

---

## 三、運算子多載與使用者介面

1. **輸入運算子多載（`>>`）**  
   - 讓使用者可直接使用 `cin >> p1` 的方式輸入多項式。  
   - 讀入後自動依指數排序。

2. **輸出運算子多載（`<<`）**  
   - 以數學形式輸出多項式，例如 `3x^2 - 2x + 1`。  
   - 自動處理正負號與常數項的格式化。

---

## 程式實作

以下為程式碼：

```cpp
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;

//==============================
// Term 類別：代表多項式的一個項
//==============================
class Term {
    // 讓 Polynomial 類別以及輸入/輸出運算子可以存取私有成員
    friend class Polynomial;
    friend istream& operator>>(istream&, class Polynomial&);
    friend ostream& operator<<(ostream&, const class Polynomial&);
private:
    float coef; // 係數
    int exp;    // 指數
};

//==============================
// Polynomial 類別：多項式的主要結構
//==============================
class Polynomial {
private:
    Term *termArray; // 非零項陣列
    int capacity;    // 陣列容量
    int terms;       // 實際項數

public:
    // 建構函式
    Polynomial(int cap = 10) {
        capacity = cap;
        terms = 0;
        termArray = new Term[capacity];
    }

    // 解構函式
    ~Polynomial() {
        delete[] termArray;
    }

    //==============================
    // 輸入運算子 >>
    //==============================
    friend istream& operator>>(istream& in, Polynomial& poly) {
        cout << "請輸入多項式的項數：";
        in >> poly.terms;

        if (poly.terms > poly.capacity) {
            delete[] poly.termArray;
            poly.capacity = poly.terms;
            poly.termArray = new Term[poly.capacity];
        }

        cout << "請輸入每項 (係數 指數)，例如：3 2 表示 3x^2：" << endl;
        for (int i = 0; i < poly.terms; i++) {
            cout << "第 " << i + 1 << " 項：";
            in >> poly.termArray[i].coef >> poly.termArray[i].exp;
        }

        // 按指數由大到小排序
        sort(poly.termArray, poly.termArray + poly.terms,
             [](const Term& a, const Term& b) { return a.exp > b.exp; });

        return in;
    }

    //==============================
    // 輸出運算子 <<
    //==============================
    friend ostream& operator<<(ostream& out, const Polynomial& poly) {
        if (poly.terms == 0) {
            out << "0";
            return out;
        }

        for (int i = 0; i < poly.terms; i++) {
            float c = poly.termArray[i].coef;
            int e = poly.termArray[i].exp;

            if (i > 0 && c > 0)
                out << " + ";
            else if (c < 0) {
                out << " - ";
                c = -c;
            }

            if (e == 0)
                out << c;
            else if (e == 1)
                out << c << "x";
            else
                out << c << "x^" << e;
        }
        return out;
    }

    //==============================
    // 加法函式 Add
    //==============================
    Polynomial Add(const Polynomial& poly) const {
        Polynomial result(capacity + poly.capacity);
        int i = 0, j = 0, k = 0;

        while (i < terms && j < poly.terms) {
            if (termArray[i].exp == poly.termArray[j].exp) {
                float sum = termArray[i].coef + poly.termArray[j].coef;
                if (fabs(sum) > 1e-6) {
                    result.termArray[k].coef = sum;
                    result.termArray[k].exp = termArray[i].exp;
                    k++;
                }
                i++; j++;
            } else if (termArray[i].exp > poly.termArray[j].exp)
                result.termArray[k++] = termArray[i++];
            else
                result.termArray[k++] = poly.termArray[j++];
        }

        while (i < terms) result.termArray[k++] = termArray[i++];
        while (j < poly.terms) result.termArray[k++] = poly.termArray[j++];

        result.terms = k;
        return result;
    }

    //==============================
    // 乘法函式 Mult
    //==============================
    Polynomial Mult(const Polynomial& poly) const {
        Polynomial result(terms * poly.terms + 1);

        for (int i = 0; i < terms; i++) {
            for (int j = 0; j < poly.terms; j++) {
                float c = termArray[i].coef * poly.termArray[j].coef;
                int e = termArray[i].exp + poly.termArray[j].exp;

                bool found = false;
                for (int k = 0; k < result.terms; k++) {
                    if (result.termArray[k].exp == e) {
                        result.termArray[k].coef += c;
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    result.termArray[result.terms].coef = c;
                    result.termArray[result.terms].exp = e;
                    result.terms++;
                }
            }
        }

        // 去除係數為 0 的項
        int valid = 0;
        for (int i = 0; i < result.terms; i++)
            if (fabs(result.termArray[i].coef) > 1e-6)
                result.termArray[valid++] = result.termArray[i];
        result.terms = valid;

        // 依照指數排序
        sort(result.termArray, result.termArray + result.terms,
             [](const Term& a, const Term& b) { return a.exp > b.exp; });

        return result;
    }

    //==============================
    // Eval：代入 x 計算值
    //==============================
    float Eval(float x) const {
        float sum = 0;
        for (int i = 0; i < terms; i++)
            sum += termArray[i].coef * pow(x, termArray[i].exp);
        return sum;
    }
};

//==============================
// 主程式區
//==============================
int main() {
    Polynomial p1, p2;

    cout << "=== 輸入第一個多項式 ===" << endl;
    cin >> p1;
    cout << "p1(x) = " << p1 << endl;

    cout << "\n=== 輸入第二個多項式 ===" << endl;
    cin >> p2;
    cout << "p2(x) = " << p2 << endl;

    Polynomial sum = p1.Add(p2);
    Polynomial product = p1.Mult(p2);

    cout << "\n--- 加法結果 ---" << endl;
    cout << "p1 + p2 = " << sum << endl;

    cout << "\n--- 乘法結果 ---" << endl;
    cout << "p1 * p2 = " << product << endl;

    float x;
    cout << "\n請輸入要代入的 x 值：";
    cin >> x;
    cout << "p1(" << x << ") = " << p1.Eval(x) << endl;
    cout << "p2(" << x << ") = " << p2.Eval(x) << endl;

    return 0;
}


```

## 效能分析（Performance Analysis）

本程式的主要運算為 **多項式加法（Add）**、**多項式乘法（Mult）**、與 **代入運算（Eval）**。  
以下針對各個函式的時間與空間複雜度進行分析。

---

## 一、時間複雜度分析

| 函式名稱      | 主要運算內容               | 時間複雜度       | 說明 |
|:-------------|:--------------------------|:----------------|:------|
| `Add()`      | 以雙指標依序合併兩多項式項目 | **O(m + n)**   | 各多項式各遍歷一次，其中 m、n 為項數 |
| `Mult()`     | 雙層迴圈進行逐項相乘與合併   | **O(m × n)**   | 每個項與另一多項式所有項相乘 |
| `Eval()`     | 單層迴圈代入計算            | **O(n)**       | 每項各計算一次冪次與乘法 |
| `operator>>` | 輸入並排序多項式項目        | **O(n log n)** | 排序使用 `std::sort()` |
| `operator<<` | 輸出每項至螢幕              | **O(n)**       | 每項依序輸出一次 |

---

## 二、空間複雜度分析

| 函式名稱      | 額外使用記憶體 | 空間複雜度    | 說明 |
|:-------------|:--------------|:------------|:------|
| `Add()`      | 暫存結果多項式 | **O(m + n)** | 新建一個存放相加結果的物件 |
| `Mult()`     | 暫存乘積項目   | **O(m × n)** | 需儲存所有乘積項與中間合併結果 |
| `Eval()`     | 常數級         | **O(1)**    | 僅使用累加變數 |
| `operator>>` | 暫存輸入陣列   | **O(n)**    | 輸入階段暫存多項式項目 |

---

## 三、效能觀察與分析

1. **加法運算**  
   - 採用**雙指標合併法（Two-pointer Merge）**，效率與排序合併相同。  
   - 對於稀疏多項式（項數少），運算速度非常快。

2. **乘法運算**  
   - 雙層迴圈為主要耗時部分，時間隨項數平方成長。  
   - 若需處理大量項，可進一步改進為使用 **雜湊表（Hash Map）** 以提升合併效率。

3. **代入運算**  
   - 為線性時間，且運算次數與項數成正比。  
   - 可透過 **Horner’s Rule（霍納法則）** 進一步優化。

4. **記憶體使用**  
   - 加法與乘法會建立新物件，不會改動原多項式資料。  
   - 雖增加暫時性記憶體消耗，但維持資料安全與可讀性。

---

## 四、綜合比較表

| 運算項目   | 時間複雜度 | 空間複雜度 | 
|:----------|:----------|:----------|
| 多項式加法 | O(m + n) | O(m + n) | 
| 多項式乘法 | O(m × n) | O(m × n) | 
| 代入運算   | O(n)     | O(1) | 

---


## 測試與驗證
<img width="559" height="523" alt="image" src="https://github.com/user-attachments/assets/3b1db42d-6aff-4218-b0f7-bde47a98c329" />

### 測試案例

| 類別         | 理論計算結果                                             | 程式輸出結果                                                     |  是否正確   |
| **p₁(x)**   | 3x⁴ − 2x³ + 5x − 7                                      | 3x^4 - 2x^3 + 5x - 7                                            |     ✅     |
| **p₂(x)**   | −1x⁴ + 2x³ − 5x + 10                                    | - 1x^4 + 2x^3 - 5x + 10                                         |     ✅     |
| **p₁ + p₂** | 2x⁴ + 3                                                 | 2x^4 + 3                                                        |     ✅     |
| **p₁ × p₂** | −3x⁸ + 8x⁷ − 4x⁶ − 20x⁵ + 57x⁴ − 34x³ − 25x² + 85x − 70 | - 3x^8 + 8x^7 - 4x^6 - 20x^5 + 57x^4 - 34x^3 - 25x^2 + 85x - 70 |     ✅     |
| **p₁(2)**   | 35                                                      | 35                                                              |     ✅     |
| **p₂(2)**   | 0                                                       | 0                                                               |     ✅     |



### 編譯與執行指令

```shell
$ g++ hw1.cpp -o hw1
$ hw1
```

### 結論

本次作業完成了多項式的加法、乘法與代入運算實作，  
透過類別封裝與運算子多載，使程式結構更清晰、可讀性更高。  

整體而言，程式能：
- 正確處理多項式運算與輸出格式。  
- 具備良好的擴充性與維護性。  
- 展現物件導向設計在數學運算中的實際應用價值。  

