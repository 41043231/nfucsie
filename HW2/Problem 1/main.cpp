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

