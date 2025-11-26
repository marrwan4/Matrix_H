#include "Matrix.h"

int main() {
    Matrix m1(2, 2);
    initializer_list<double> l = { 1.5, 2, 3, 4 };
    initializer_list<double>{1, 2, 3, 4} >> m1;
    cout << m1;
    cout << (m1.transpose()).transpose();
    Matrix<double> m2(2, 2);
    cout << "Enter the second matrix:" << endl;
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
    Matrix b(3, 3, { 1,2,3,4,55,6,7,8,9 });
    cout << (a == b) << endl;
    cout << (Matrix<int>(6, 6).Identity()) << endl << (Matrix<int>(6, 6).generateIntegers(1, 10));
    cout << 66 * (Matrix<int>(6, 6).Identity());
    Matrix<double> s(4, 4, {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16});
    Matrix<double> q(4, 4, {16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1});
    cout << "Strassen multiplication:" << endl;
    cout << Matrix<double>::strassenMultiply(s, q) << endl;
    cout << "Standard multiplication:" << endl;
    cout << s * q << endl;
    return 0;
}