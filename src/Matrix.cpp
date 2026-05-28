#include "Matrix.h"
#include<iostream>
#include<algorithm>
#include<cmath>
using namespace std;
Matrix::Matrix( int height, int width)
{
    this->height = height;
    this->width = width;
    insides = new double* [height];
    for (int i = 0; i < height; i++)
    {
        insides[i] = new double[width]();
    }
}

Matrix::Matrix(const Matrix& other)
{
    height = other.height;
    width = other.width;
    insides = new double* [height];
    for (int i = 0; i < other.height; i++)
    {
        insides[i] = new double[other.width];
    }
    for (int i = 0; i < other.height; i++)
    {
        for (int j = 0; j < other.width; j++)
        {
            insides[i][j] = other[i][j];
        }
    }
}

Matrix::Matrix(std::initializer_list<std::initializer_list<double>> init)
{
    height = init.size();
    width = init.begin()->size();
    insides = new double* [height];
    int i = 0;
    for (const auto& row : init) {
        insides[i] = new double[width];
        int j = 0;
        for (double value : row) {
            insides[i][j++] = value;
        }
        i++;
    }
}

Matrix & Matrix::operator=(Matrix other)
{
    swap(*this,other);
    return *this;
}

double*& Matrix::operator [] (int index) const
{
    return insides[index];
}

Matrix & Matrix::operator *=(const Matrix &matrix) {
    if (height != matrix.width)
    {
        std::cerr << "couldn't multiply matrices";
        return *this;
    }
    else
    {
        Matrix temp(height, matrix.width);
        for (int i = 0; i < height; i++)
            for (int j = 0; j < matrix.width; j++)
                for (int k = 0; k < width; k++)
                {
                    temp[i][j] += insides[i][k] * matrix[k][j];
                }
        *this = temp;
        return *this;
    }
};

const Matrix operator*(const Matrix& lhs,const Matrix& rhs) {
    return (Matrix(lhs) *= rhs);
}

const int& Matrix::getHeight()const {
    return height;
}

const int& Matrix::getWidth()const {
    return width;
}

Matrix Matrix::inverse()const {
    //GaussJ
    if (height != width) { std::cerr << "badmatrix!!"; }
    Matrix identity(identityMatrix(height));
    Matrix temp(*this);
    for (int i = 0; i < height; i++)
    {
        for (int j = width-1; j > i-1; j--)
        {
            if(j!=i)temp[i][j] = temp[i][j] / temp[i][i];
            identity[i][j] = identity[i][j] / temp[i][i];
        }
        for (int j = 0; j < height; j++)
        {
            if (j != i) {
                for (int k = i; k < width; k++)
                {
                    temp[j][k] = temp[j][k] - temp[j][i] * temp[i][k];
                    identity[j][k] = identity[j][k] - identity[j][i] * temp[i][k];
                }
            }
        }
    }
    return identity;
}

Matrix Matrix::transpose() const
{
    Matrix temp(width,height);
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            temp[i][j] = insides[j][i];
        }
    }
    return temp;
}

void Matrix::comout() const
{
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            std::cout << insides[i][j] << "   ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

Matrix::~Matrix()
{
    for (int i = 0; i < height; i++)
        delete[] insides[i];
    delete[] insides;

}

void swap(Matrix& first, Matrix& second) {
    using std::swap;
    swap(first.height,second.height);
    swap(first.width,second.width);
    swap(first.insides,second.insides);
};

Matrix Matrix::identityMatrix( int size) {
    Matrix returnMatrix(size, size);
    for (int i = 0; i < size; i++)
    {
        returnMatrix[i][i] = 1;
    }
    return returnMatrix;
}