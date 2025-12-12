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


