#include <iostream>
#include <vector>
#include <cmath> 
#include <algorithm>


// 定義表示多項式的單項式類別
class SingleTerm {
public:
    SingleTerm(float coefficient = 0, int exponent = 0) : coef(coefficient), exp(exponent) {}
    float coef; // 係數
    int exp;    // 指數
};

// 定義表示多項式的類別
class Polynomial {
public:
    Polynomial() = default;
    Polynomial(const Polynomial&) = default;
    Polynomial& operator=(const Polynomial&) = default;
    ~Polynomial() = default;

    void AppendTerm(float coefficient, int exponent); // 插入單項式
    Polynomial SumWith(const Polynomial& other) const; // 多項式相加
    Polynomial MultiplyWith(const Polynomial& other) const; // 多項式相乘
    float EvaluateAt(float value) const; // 計算多項式在某點的值
    Polynomial Derivative() const; // 計算多項式的一次導數

    friend std::ostream& operator<<(std::ostream& out, const Polynomial& poly); // 重載輸出運算符
    friend std::istream& operator>>(std::istream& in, Polynomial& poly); // 重載輸入運算符

private:
    std::vector<SingleTerm> terms; // 存儲多項式的單項式

    // 比較單項式的指數，用於排序
    static bool CompareTerms(const SingleTerm& a, const SingleTerm& b) {
        return a.exp > b.exp;
    }
};

// 插入單項式到多項式中
void Polynomial::AppendTerm(float coefficient, int exponent) {
    if (coefficient == 0) return; // 忽略係數為零的項

    // 查找是否存在相同指數的單項式
    for (auto& term : terms) {
        if (term.exp == exponent) {
            term.coef += coefficient;
            if (term.coef == 0) {
                // 如果係數相加後為零，則刪除該單項式
                terms.erase(std::remove_if(terms.begin(), terms.end(), [exponent](const SingleTerm& t) { return t.exp == exponent; }), terms.end());
            }
            std::sort(terms.begin(), terms.end(), CompareTerms); // 按指數排序
            return;
        }
    }

    // 如果沒有相同指數的單項式，則添加新單項式
    terms.emplace_back(coefficient, exponent);
    std::sort(terms.begin(), terms.end(), CompareTerms); // 按指數排序
}

// 多項式相加
Polynomial Polynomial::SumWith(const Polynomial& other) const {
    Polynomial result = *this;
    for (const auto& term : other.terms) {
        result.AppendTerm(term.coef, term.exp);
    }
    return result;
}

// 多項式相乘
Polynomial Polynomial::MultiplyWith(const Polynomial& other) const {
    Polynomial result;
    for (const auto& term1 : terms) {
        for (const auto& term2 : other.terms) {
            result.AppendTerm(term1.coef * term2.coef, term1.exp + term2.exp);
        }
    }
    return result;
}

// 計算多項式在某點的值
float Polynomial::EvaluateAt(float value) const {
    float result = 0;
    for (const auto& term : terms) {
        result += term.coef * std::pow(value, term.exp);
    }
    return result;
}

// 計算多項式的一次導數
Polynomial Polynomial::Derivative() const {
    Polynomial deriv;
    for (const auto& term : terms) {
        if (term.exp != 0) {
            deriv.AppendTerm(term.coef * term.exp, term.exp - 1);
        }
    }
    return deriv;
}

// 重載輸出運算符
std::ostream& operator<<(std::ostream& out, const Polynomial& poly) {
    if (poly.terms.empty()) {
        out << "0";
        return out;
    }
    bool first = true;
    for (const auto& term : poly.terms) {
        if (!first && term.coef > 0) out << " + ";
        if (term.coef < 0) out << " - ";
        if (!first && term.coef < 0) out << -term.coef;
        else out << term.coef;

        if (term.exp > 0) out << "x^" << term.exp;
        first = false;
    }
    return out;
}

// 重載輸入運算符
std::istream& operator>>(std::istream& in, Polynomial& poly) {
    int numTerms;
    std::cout << "輸入單項式數量：";
    if (!(in >> numTerms) || numTerms < 0) {
        std::cerr << "無效的單項式數量" << std::endl;
        in.setstate(std::ios::failbit);
        return in;
    }

    for (int i = 0; i < numTerms; ++i) {
        float coefficient;
        int exponent;
        std::cout << "輸入係數和指數：";
        if (!(in >> coefficient >> exponent)) {
            std::cerr << "無效係數或指數" << std::endl;
            in.setstate(std::ios::failbit);
            return in;
        }
        poly.AppendTerm(coefficient, exponent);
    }

    return in;
}

int main() {
    Polynomial poly1, poly2;
    std::cout << "輸入多項式 1：" << std::endl;
    std::cin >> poly1;
    std::cout << "輸入多項式 2：" << std::endl;
    std::cin >> poly2;

    Polynomial sum = poly1.SumWith(poly2);
    Polynomial product = poly1.MultiplyWith(poly2);

    std::cout << "多項式 1: " << poly1 << std::endl;
    std::cout << "多項式 2: " << poly2 << std::endl;
    std::cout << "和: " << sum << std::endl;
    std::cout << "積: " << product << std::endl;

    float x;
    std::cout << "輸入計算這兩個多項式：";
    if (std::cin >> x) {
        std::cout << "poly1(" << x << ") = " << poly1.EvaluateAt(x) << std::endl;
        std::cout << "poly2(" << x << ") = " << poly2.EvaluateAt(x) << std::endl;
    }
    else {
        std::cerr << "無效輸入" << std::endl;
    }

    Polynomial deriv1 = poly1.Derivative();
    Polynomial deriv2 = poly2.Derivative();

    std::cout << "多項式 1 的導數: " << deriv1 << std::endl;
    std::cout << "多項式 2 的導數: " << deriv2 << std::endl;

    return 0;
}
