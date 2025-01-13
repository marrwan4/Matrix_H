#include "Matrix.h"

int main() {
    Matrix m1(2, 2);
    initializer_list<double> l = { 1.5, 2, 3, 4 };
    initializer_list<double>{1, 2, 3, 4} >> m1;
    cout << m1;
    cout << (m1.transpose()).transpose();
    Matrix<double> m2(2, 2);
    cin >> m2;
    cout << m2;
    cout << m1 + m2;
    cout << m1 * 2;
    cout << m1 * m2;
    cout << m1 - m2;
    cout << m1 / 2;
    cout << m1.transpose();
    bool eq;
    eq = m1 == m2;
    cout << eq << (m1 == m2);
    Matrix a(2, 2, { 1,2,3,4 });
    Matrix b(3, 3, { 1,2,3,4,55 ,6,7,8,9 });
    cout << (a == b) << endl;
    cout << (Matrix<int>(6, 6).Identity()) << endl << (Matrix<int>(6, 6).generateIntegers(1, 10));
    cout << 66 * (Matrix<int>(6, 6).Identity());
    return 0;
}