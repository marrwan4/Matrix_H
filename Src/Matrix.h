#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

// Make Matrix<double> * Matrix<int> possible #Future#
template <typename T = double, typename = enable_if_t<is_arithmetic<T>::value>>
class Matrix {
private:
    int rows, cols;
    int longestNumber = 0;
    vector<vector<T>> data;
    void getLongestNumberLength() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int length = to_string(data[i][j]).length();
                if (length > longestNumber) {
                    longestNumber = length;
                }
            }
        }
    }
public:
    // Constructors
    Matrix(int const& rows, int const& cols) {
        this->rows = rows;
        this->cols = cols;
        data = vector<vector<T>>(rows, vector<T>(cols, 0));
        getLongestNumberLength();
    };
    Matrix(int const& rows, int const& cols, T const& value) {
        this->rows = rows;
        this->cols = cols;
        data = vector<vector<T>>(rows, vector<T>(cols, value));
        getLongestNumberLength();
    };
    Matrix(int const& rows, int const& cols, initializer_list<T> const& values) {
        this->rows = rows;
        this->cols = cols;
        if (values.size() != rows * cols) {
            throw invalid_argument("Incorrect number of values to initialize the matrix.");
        }
        auto it = values.begin();
        data = vector<vector<T>>(rows, vector<T>(cols, 0));
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                data[i][j] = *it++;
            }
        }
        getLongestNumberLength();
    };
    Matrix(Matrix const& obj) {
        this->rows = obj.rows;
        this->cols = obj.cols;
        this->data = vector<vector<T>>(rows, vector<T>(cols, 0));
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                this->data[i][j] = obj.data[i][j];
            }
        }
        longestNumber = obj.longestNumber;
    }
    // checkers
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
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (data[i][j] != obj.data[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
    bool operator!=(Matrix const& obj) const {
        return !(*this == obj);
    }
    bool isEmpty() const {
        return rows == 0 && cols == 0;
    }
    // setters & getters
    friend istream& operator>>(istream& in, Matrix& obj) {
        for (int i = 0; i < obj.rows; i++) {
            for (int j = 0; j < obj.cols; j++) {
                in >> obj.data[i][j];
            }
        }
        obj.getLongestNumberLength();
        return in;
    }
    friend Matrix& operator>>(initializer_list<T> const& values, Matrix& obj) { //add elements to matrix by 
        if (values.size() != obj.rows * obj.cols) {
            throw invalid_argument("Incorrect number of values to populate the matrix.");
        }
        auto it = values.begin();
        for (int i = 0; i < obj.rows; ++i) {
            for (int j = 0; j < obj.cols; ++j) {
                obj.data[i][j] = *it++;
            }
        }
        return obj;
    }
    friend ostream& operator<< (ostream& out, Matrix const& obj) {//incomplete
        out << " __" << setw((obj.longestNumber) * (obj.cols)) << "__" << endl;
        for (int i = 0; i < obj.rows; i++) {
            out << "|";
            for (int j = 0; j < obj.cols; j++) {
                out << setw(((obj.longestNumber) / 2)) << right << obj.data[i][j] << setw((obj.longestNumber) / 2) << " ";
            }
            out << "|" << endl;
        }
        out << " __" << setw((obj.longestNumber) * (obj.cols)) << "__" << endl;
        return out;
    }
    void setCell(int const& row, int const& col, T const& value) {
        data[row][col] = value;
    }
    Matrix generateIntegers(int const& minimum, int const& maximum) { //May add generation of doubles, symmetric, scotch matrices
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                data[i][j] = (rand() % (maximum - minimum + 1)) + minimum;
            }
        }
        return *this;
    }
    Matrix Identity() { //Makes a fast identity matrix use like: Matrix<int>(2,2).Identity()
        if (!isSquare()) {
            throw invalid_argument("Matrix is not square.");
        }
        for (int i = 0; i < rows; i++) {
            data[i][i] = 1;
        }
        return *this;
    }
    T getCell(int const& row, int const& col) const {
        return data[row][col];
    }
    int getRows() const {
        return rows;
    }
    int getCols() const {
        return cols;
    }
    Matrix getRow(int const& row) const {
        Matrix res(1, cols);
        for (int i = 0; i < cols; i++) {
            res.data[0][i] = data[row][i];
        }
        return res;
    }
    Matrix getCol(int const& col) const {
        Matrix res(rows, 1);
        for (int i = 0; i < rows; i++) {
            res.data[i][0] = data[i][col];
        }
        return res;
    }
    // Matrix & Matrix operations
    Matrix operator+(Matrix const& obj) const {
        if (!canOperate(obj)) {
            throw invalid_argument("Matrix dimensions do not match.");
        }
        Matrix res(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                res.data[i][j] = data[i][j] + obj.data[i][j];
            }
        }
        return res;
    }
    Matrix operator+=(Matrix const& obj) {
        if (!canOperate(obj)) {
            throw invalid_argument("Matrix dimensions do not match.");
        }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                data[i][j] += obj.data[i][j];
            }
        }
        return *this;
    }
    Matrix operator-(Matrix const& obj) const {
        if (!canOperate(obj)) {
            throw invalid_argument("Matrix dimensions do not match.");
        }
        Matrix res(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                res.data[i][j] = data[i][j] - obj.data[i][j];
            }
        }
        return res;
    }
    Matrix operator-=(Matrix const& obj) {
        if (!canOperate(obj)) {
            throw invalid_argument("Matrix dimensions do not match.");
        }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                data[i][j] -= obj.data[i][j];
            }
        }
        return *this;
    }
    Matrix operator*(Matrix const& obj) const {
        if (cols != obj.rows) {
            throw invalid_argument("Matrix dimensions do not match.");
        }
        Matrix res(rows, obj.cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < obj.cols; j++) {
                for (int k = 0; k < cols; k++) {
                    res.data[i][j] += data[i][k] * obj.data[k][j];
                }
            }
        }
        return res;
    }
    Matrix operator*=(Matrix const& obj) {
        if (cols != obj.rows) {
            throw invalid_argument("Matrix dimensions do not match.");
        }
        Matrix res(rows, obj.cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < obj.cols; j++) {
                for (int k = 0; k < cols; k++) {
                    res.data[i][j] += data[i][k] * obj.data[k][j];
                }
            }
        }
        *this = res;
        return *this;
    }
    double innerProduct(Matrix const& obj) const { //didn't test
        if (!canOperate(obj)) {
            throw invalid_argument("Matrix dimensions do not match.");
        }
        int res = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                res += data[i][j] * obj.data[i][j];
            }
        }
        return res;
    }
    // Matrix & Scalar operations
    Matrix operator*(T const& number) const {
        Matrix res(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                res.data[i][j] = data[i][j] * number;
            }
        }
        return res;
    }
    Matrix operator*=(T const& number) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                data[i][j] *= number;
            }
        }
        return *this;
    }
    Matrix operator/(T const& number) const {
        if (number == 0) {
            throw invalid_argument("Division by zero.");
        }
        Matrix res(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                res.data[i][j] = data[i][j] / number;
            }
        }
        return res;
    }
    Matrix operator/=(T const& number) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                data[i][j] /= number;
            }
        }
        return *this;
    }
    friend Matrix operator*(T const& number, Matrix const& obj) {
        return obj * number;
    }
    // Matrix Operations
        // Matrix submatrix (){}
        // Matrix coFactor(){}
        // Matrix adjoint() { return (*this.coFactor()).transpose() }
        // Matrix inverse() { return (1 / (*this.determinant())) * (*this.adjoint()); }
    Matrix transpose() const {
        Matrix res(cols, rows);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                res.data[j][i] = data[i][j];
            }
        }
        return res;
    }
    double determinant() const { //incomplete, didn't test
        if (rows != cols) {
            cout << "Dimension error: Matrix is not square" << endl;
            return 0;
        }
        if (rows == 1) {
            return data[0][0];
        }
        if (rows == 2) {
            return data[0][0] * data[1][1] - data[0][1] * data[1][0];
        }
        double det = 0;
        for (int i = 0; i < rows; i++) {
            Matrix submatrix(rows - 1, cols - 1);
            for (int j = 1; j < rows; j++) {
                for (int k = 0; k < cols; k++) {
                    if (k < i) {
                        submatrix.data[j - 1][k] = data[j][k];
                    } else if (k > i) {
                        submatrix.data[j - 1][k - 1] = data[j][k];
                    }
                }
            }
            det += pow(-1, i) * data[0][i] * submatrix.determinant();
        }
    }
    double norm() const { //didn't test
        int res = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                res += data[i][j] * data[i][j];
            }
        }
        return sqrt(res);
    }
    double trace() const {
        if (!isSquare()) {
            throw invalid_argument("Matrix is not square.");
        }
        int res = 0;
        for (int i = 0; i < rows; i++) {
            res += data[i][i];
        }
        return res;
    }
    // Matrix Rank #Future#
    // Matrix Simple row operations and simpleefiaction #Future#
    // Matrix Decomposition #Future#
    // Triangular Matrix #Future#
    // Matrix Eigenvalues and Eigenvectors #Future#
    // Tall and Horizontal Matrix #Future#
};

// MatrixType #Future#

#endif