#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <stdexcept>
#include <type_traits>
#include <initializer_list>

#ifdef USE_MPI
    #include <mpi.h>
#endif

using namespace std;

// Make Matrix<double> * Matrix<int> possible #Future#
template <typename T = double, typename = enable_if_t<is_arithmetic<T>::value>>
class Matrix {
private:
    int rows, cols;
    int longestNumber = 0;
    vector<T> data;

    void getLongestNumberLength() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int length = to_string((*this)(i, j)).length();
                if (length > longestNumber) {
                    longestNumber = length;
                }
            }
        }
    }

    static void devideTheMatrixIntoQuadrants(Matrix const& source, Matrix& A11, Matrix& A12, Matrix& A21, Matrix& A22) {
        int newSize = source.rows / 2;
        for (int i = 0; i < newSize; i++) {
            for (int j = 0; j < newSize; j++) {
                A11(i, j) = source(i, j);
                A12(i, j) = source(i, j + newSize);
                A21(i, j) = source(i + newSize, j);
                A22(i, j) = source(i + newSize, j + newSize);
            }
        }
    }

    static Matrix combineQuadrants(Matrix const& C11, Matrix const& C12, Matrix const& C21, Matrix const& C22) {
        int newSize = C11.rows;
        Matrix res(newSize * 2, newSize * 2);
        for (int i = 0; i < newSize; i++) {
            for (int j = 0; j < newSize; j++) {
                res(i, j) = C11(i, j);
                res(i, j + newSize) = C12(i, j);
                res(i + newSize, j) = C21(i, j);
                res(i + newSize, j + newSize) = C22(i, j);
            }
        }
        return res;
    }

public:
    // Constructors
    Matrix(int const& rows, int const& cols) {
        this->rows = rows;
        this->cols = cols;
        data = vector<T>(rows * cols, 0);
        getLongestNumberLength();
    }

    Matrix(int const& rows, int const& cols, T const& value) {
        this->rows = rows;
        this->cols = cols;
        data = vector<T>(rows * cols, value);
        getLongestNumberLength();
    }

    Matrix(int const& rows, int const& cols, initializer_list<T> const& values) {
        this->rows = rows;
        this->cols = cols;
        if (values.size() != static_cast<size_t>(rows * cols)) {
            throw invalid_argument("Incorrect number of values to initialize the matrix.");
        }
        data = vector<T>(values);
        getLongestNumberLength();
    }

    Matrix(Matrix const& obj) {
        this->rows = obj.rows;
        this->cols = obj.cols;
        this->data = obj.data; 
        this->longestNumber = obj.longestNumber;
    }

    // Checkers
    bool isSquare() const {
        return rows == cols;
    }

    bool canOperate(Matrix const& obj) const {
        return rows == obj.rows && cols == obj.cols;
    }

    bool operator==(Matrix const& obj) const {
        if (!canOperate(obj)) {
            return false;
        }
        return this->data == obj.data;
    }

    bool operator!=(Matrix const& obj) const {
        return !(*this == obj);
    }

    bool isEmpty() const {
        return rows == 0 && cols == 0;
    }

    // Setters & Getters (Parentheses operators handle mapping automatically)
    T& operator()(int r, int c) {
        return data[r * cols + c];
    }

    T const& operator()(int r, int c) const {
        return data[r * cols + c];
    }

    friend istream& operator>>(istream& in, Matrix& obj) {
        for (int i = 0; i < obj.rows; i++) {
            for (int j = 0; j < obj.cols; j++) {
                in >> obj(i, j);
            }
        }
        obj.getLongestNumberLength();
        return in;
    }

    friend Matrix& operator>>(initializer_list<T> const& values, Matrix& obj) { 
        if (values.size() != static_cast<size_t>(obj.rows * obj.cols)) {
            throw invalid_argument("Incorrect number of values to populate the matrix.");
        }
        auto it = values.begin();
        for (int i = 0; i < obj.rows; ++i) {
            for (int j = 0; j < obj.cols; ++j) {
                obj(i, j) = *it++;
            }
        }
        return obj;
    }

    friend ostream& operator<< (ostream& out, Matrix const& obj) {
        // not yet well implemented.
        out << " __" << setw((obj.longestNumber) * (obj.cols)) << "__" << endl;
        for (int i = 0; i < obj.rows; i++) {
            out << "|";
            for (int j = 0; j < obj.cols; j++) {
                out << setw(((obj.longestNumber) / 2)) << right << obj(i, j) << setw((obj.longestNumber) / 2) << " ";
            }
            out << "|" << endl;
        }
        out << " __" << setw((obj.longestNumber) * (obj.cols)) << "__" << endl;
        return out;
    }

    void setCell(int const& row, int const& col, T const& value) {
        (*this)(row, col) = value;
    }

    Matrix generateIntegers(int const& minimum, int const& maximum) {
        // to be removed
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                (*this)(i, j) = (rand() % (maximum - minimum + 1)) + minimum;
            }
        }
        return *this;
    }

    Matrix Identity() { 
        if (!isSquare()) {
            throw invalid_argument("Matrix is not square.");
        }
        fill(data.begin(), data.end(), 0);
        for (int i = 0; i < rows; i++) {
            (*this)(i, i) = 1;
        }
        return *this;
    }

    T getCell(int const& row, int const& col) const {
        return (*this)(row, col);
    }

    int getRows() const { return rows; }
    int getCols() const { return cols; }

    Matrix getRow(int const& row) const {
        Matrix res(1, cols);
        for (int i = 0; i < cols; i++) {
            res(0, i) = (*this)(row, i);
        }
        return res;
    }

    Matrix getCol(int const& col) const {
        Matrix res(rows, 1);
        for (int i = 0; i < rows; i++) {
            res(i, 0) = (*this)(i, col);
        }
        return res;
    }

    // Matrix & Matrix operations
    Matrix operator+(Matrix const& obj) const {
        if (!canOperate(obj)) {
            throw invalid_argument("Matrix dimensions do not match.");
        }
        Matrix res(rows, cols);
        for (size_t i = 0; i < data.size(); i++) {
            res.data[i] = data[i] + obj.data[i];
        }
        return res;
    }

    Matrix operator+=(Matrix const& obj) {
        if (!canOperate(obj)) {
            throw invalid_argument("Matrix dimensions do not match.");
        }
        for (size_t i = 0; i < data.size(); i++) {
            data[i] += obj.data[i];
        }
        return *this;
    }

    Matrix operator-(Matrix const& obj) const {
        if (!canOperate(obj)) {
            throw invalid_argument("Matrix dimensions do not match.");
        }
        Matrix res(rows, cols);
        for (size_t i = 0; i < data.size(); i++) {
            res.data[i] = data[i] - obj.data[i];
        }
        return res;
    }

    Matrix operator-=(Matrix const& obj) {
        if (!canOperate(obj)) {
            throw invalid_argument("Matrix dimensions do not match.");
        }
        for (size_t i = 0; i < data.size(); i++) {
            data[i] -= obj.data[i];
        }
        return *this;
    }

    // Multi-threaded optimized loop via row-major format order (i, k, j)
    Matrix operator*(Matrix const& obj) const {
        if (cols != obj.rows) {
            throw invalid_argument("Matrix dimensions do not match.");
        }
        Matrix res(rows, obj.cols, 0);
        
        #ifdef USE_OPENMP
        #pragma omp parallel for collapse(2) schedule(static)
        #endif
        for (int i = 0; i < rows; i++) {
            for (int k = 0; k < cols; k++) {
                for (int j = 0; j < obj.cols; j++) {
                    res(i, j) += (*this)(i, k) * obj(k, j);
                }
            }
        }
        return res;
    }

    // Strassen Multiplication
    static Matrix strassenMultiply(Matrix const& first, Matrix const& second) {
        if (!first.isSquare() || !second.isSquare() || first.canOperate(second) == false) {
            throw invalid_argument("Strassen requires square matrices of same size.");
        }
        if (first.rows == 1) {
            Matrix res(1, 1);
            res(0, 0) = first(0, 0) * second(0, 0);
            return res;
        }
        int k = first.rows / 2;
        Matrix A11(k, k), A12(k, k), A21(k, k), A22(k, k);
        devideTheMatrixIntoQuadrants(first, A11, A12, A21, A22);
        
        Matrix B11(k, k), B12(k, k), B21(k, k), B22(k, k);
        devideTheMatrixIntoQuadrants(second, B11, B12, B21, B22);
        
        Matrix P = strassenMultiply(A11 + A22, B11 + B22);
        Matrix Q = strassenMultiply(A21 + A22, B11);
        Matrix R = strassenMultiply(A11, B12 - B22);
        Matrix S = strassenMultiply(A22, B21 - B11);
        Matrix Tm = strassenMultiply(A11 + A12, B22);
        Matrix U = strassenMultiply(A21 - A11, B11 + B12);
        Matrix V = strassenMultiply(A12 - A22, B21 + B22);

        Matrix C11 = P + S - Tm + V;
        Matrix C12 = R + Tm;
        Matrix C21 = Q + S;
        Matrix C22 = P + R - Q + U;
        return combineQuadrants(C11, C12, C21, C22);
    }

    Matrix operator*=(Matrix const& obj) {
        *this = *this * obj;
        return *this;
    }

    double innerProduct(Matrix const& obj) const { 
        if (!canOperate(obj)) {
            throw invalid_argument("Matrix dimensions do not match.");
        }
        double res = 0;
        for (size_t i = 0; i < data.size(); i++) {
            res += data[i] * obj.data[i];
        }
        return res;
    }

    // Matrix & Scalar operations
    Matrix operator*(T const& number) const {
        Matrix res(rows, cols);
        for (size_t i = 0; i < data.size(); i++) {
            res.data[i] = data[i] * number;
        }
        return res;
    }

    Matrix operator*=(T const& number) {
        for (size_t i = 0; i < data.size(); i++) {
            data[i] *= number;
        }
        return *this;
    }

    Matrix operator/(T const& number) const {
        if (number == 0) {
            throw invalid_argument("Division by zero.");
        }
        Matrix res(rows, cols);
        for (size_t i = 0; i < data.size(); i++) {
            res.data[i] = data[i] / number;
        }
        return res;
    }

    Matrix operator/=(T const& number) {
        if (number == 0) {
            throw invalid_argument("Division by zero.");
        }
        for (size_t i = 0; i < data.size(); i++) {
            data[i] /= number;
        }
        return *this;
    }

    friend Matrix operator*(T const& number, Matrix const& obj) {
        return obj * number;
    }

    Matrix transpose() const {
        Matrix res(cols, rows);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                res(j, i) = (*this)(i, j);
            }
        }
        return res;
    }

    double determinant() const { 
        if (rows != cols) {
            cout << "Dimension error: Matrix is not square" << endl;
            return 0;
        }
        if (rows == 1) {
            return (*this)(0, 0);
        }
        if (rows == 2) {
            return (*this)(0, 0) * (*this)(1, 1) - (*this)(0, 1) * (*this)(1, 0);
        }
        double det = 0;
        for (int i = 0; i < rows; i++) {
            Matrix submatrix(rows - 1, cols - 1);
            for (int j = 1; j < rows; j++) {
                for (int k = 0; k < cols; k++) {
                    if (k < i) {
                        submatrix(j - 1, k) = (*this)(j, k);
                    } else if (k > i) {
                        submatrix(j - 1, k - 1) = (*this)(j, k);
                    }
                }
            }
            det += pow(-1, i) * (*this)(0, i) * submatrix.determinant();
        }
        return det;
    }

    double norm() const { 
        double res = 0;
        for (size_t i = 0; i < data.size(); i++) {
            res += data[i] * data[i];
        }
        return sqrt(res);
    }

    double trace() const {
        if (!isSquare()) {
            throw invalid_argument("Matrix is not square.");
        }
        double res = 0;
        for (int i = 0; i < rows; i++) {
            res += (*this)(i, i);
        }
        return res;
    }
};

#endif