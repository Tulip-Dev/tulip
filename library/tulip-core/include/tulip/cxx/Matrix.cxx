/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include <cmath>

//===================================================================
//Specialisation
namespace tlp {
template<typename Obj>
class Matrix<Obj,1>:public Array<Vector<Obj,1>,1> {
public:
  Obj determinant() {
    return (*this)[0][0];
  }
  //      Matrix<Obj,1>& fill(Obj obj) {return *this;}
  Matrix<Obj,1>& inverse() {
    (*this)[0][0] = 1.0 / (*this)[0][0];
    return *this;
  }
  Matrix<Obj,1>& transpose() {
    return *this;
  }
  Matrix<Obj,1>& operator*=(const Matrix<Obj,1> &mat) {
    (*this)[0][0] *= mat[0][0];
    return *this;
  }
  //      Matrix<Obj,1>& operator/=(const Obj &obj){return *this;}
  Matrix<Obj,1> cofactor() {
    return *this;
  }
};
}


//===================================================================
template<typename Obj,unsigned int SIZE>
MATRIX::Matrix(const std::vector<std::vector<Obj> > &covarianceMatrix) {
  for(unsigned int i=0; i < SIZE; i++)
    for(unsigned int j=0; j < SIZE; j++)
      (*this)[i][j] = covarianceMatrix[i][j] / (sqrt(covarianceMatrix[i][i] * covarianceMatrix[j][j]));
}

//===================================================================
template<typename Obj,unsigned int SIZE>
MATRIX & MATRIX::fill(Obj obj) {
  for (unsigned int i=0; i<SIZE; ++i)
    (*this)[i].fill(obj);

  return (*this);
}
//======================================================
template<typename Obj,unsigned int SIZE>
MATRIX&  MATRIX::operator+=(const MATRIX &mat) {
  for (unsigned int i=0; i<SIZE; ++i)
    (*this)[i] += mat[i];

  return (*this);
}
//======================================================
template<typename Obj,unsigned int SIZE>
MATRIX&  MATRIX::operator-=(const MATRIX &mat) {
  for (unsigned int i=0; i<SIZE; ++i)
    (*this)[i] -= mat[i];

  return (*this);
}
//======================================================
template<typename Obj,unsigned int SIZE>
bool MATRIX::operator==(const MATRIX &mat) const {
  for (unsigned int i=0; i<SIZE; ++i) {
    if (((*this)[i] != mat[i]))
      return false;
  }

  return true;
}
//======================================================
template<typename Obj,unsigned int SIZE>
bool MATRIX::operator!=(const MATRIX &mat) const {
  for (unsigned int i=0; i<SIZE; ++i) {
    if (((*this)[i] != mat[i]))
      return true;
  }

  return false;
}
//===================================================================
template<typename Obj,unsigned int SIZE>
MATRIX & MATRIX::operator*=(const MATRIX &mat) {
  (*this) = (*this) * mat;
  return (*this);
}
//=====================================================================================
template<typename Obj,unsigned int SIZE>
MATRIX & MATRIX::operator*=(const Obj &obj) {
  for (unsigned int i=0; i<SIZE; ++i)
    (*this)[i] *= obj;

  return (*this);
}
//=====================================================================================
template<typename Obj,unsigned int SIZE>
MATRIX & MATRIX::operator/=(const MATRIX &mat) {
  MATRIX tmpMat(mat);
  tmpMat.inverse();
  (*this) *= tmpMat;
  return (*this);
}
//=====================================================================================
template<typename Obj,unsigned int SIZE>
MATRIX & MATRIX::operator/=(const Obj &obj) {
  for (unsigned int i=0; i<SIZE; ++i)
    (*this)[i] /= obj;

  return (*this);
}
//=====================================================================================
template<typename Obj,unsigned int SIZE>
Obj MATRIX::determinant() const {
  switch (SIZE) {
  case 2:
    return (*this)[0][0] * (*this)[1][1] - (*this)[1][0] * (*this)[0][1];
    break;

  case 3:
    return (*this)[0][0] * ((*this)[1][1]*(*this)[2][2] - (*this)[1][2] * (*this)[2][1])
           - (*this)[0][1] * ((*this)[1][0]*(*this)[2][2] - (*this)[1][2] * (*this)[2][0])
           + (*this)[0][2] * ((*this)[1][0]*(*this)[2][1] - (*this)[1][1] * (*this)[2][0]) ;
    break;

  default:
    int j2;
    Obj det = 0;

    for (unsigned int j1=0; j1<SIZE; ++j1) {
      tlp::Matrix<Obj, SIZE - 1> m;

      for (unsigned int i=1; i<SIZE; i++) {
        j2 = 0;

        for (unsigned int j=0; j<SIZE; ++j) {
          if (j == j1)
            continue;

          m[i-1][j2] = (*this)[i][j];
          ++j2;
        }
      }

      if (j1 & 1)
        det += (*this)[0][j1] * m.determinant();
      else
        det -= (*this)[0][j1] * m.determinant();
    }

    return(det);
  }
}
//=====================================================================================
template<typename Obj,unsigned int SIZE>
MATRIX MATRIX::cofactor() const {
  MATRIX result;

  switch (SIZE) {
  case 2:
    (result)[0][0] = (*this)[1][1];
    (result)[0][1] = - (*this)[1][0];
    (result)[1][0] = - (*this)[0][1];
    (result)[1][1] = (*this)[0][0];
    break;

  case 3:
    (result)[0][0] = (*this)[1][1]*(*this)[2][2] - (*this)[1][2]*(*this)[2][1];
    (result)[0][1] = - ((*this)[1][0]*(*this)[2][2] - (*this)[2][0]*(*this)[1][2]);
    (result)[0][2] = (*this)[1][0]*(*this)[2][1] - (*this)[1][1]*(*this)[2][0];
    (result)[1][0] = - ((*this)[0][1]*(*this)[2][2] - (*this)[0][2]*(*this)[2][1]);
    (result)[1][1] = (*this)[0][0]*(*this)[2][2] - (*this)[0][2]*(*this)[2][0];
    (result)[1][2] = - ((*this)[0][0]*(*this)[2][1] - (*this)[0][1]*(*this)[2][0]);
    (result)[2][0] = (*this)[0][1]*(*this)[1][2] - (*this)[0][2]*(*this)[1][1];
    (result)[2][1] = - ((*this)[0][0]*(*this)[1][2] - (*this)[0][2]*(*this)[1][0]);
    (result)[2][2] = (*this)[0][0]*(*this)[1][1] - (*this)[0][1]*(*this)[1][0];
    break;

  default :
    int i1,j1;
    tlp::Matrix<Obj,SIZE - 1> c;

    for (unsigned int j=0; j<SIZE; ++j) {
      for (unsigned int i=0; i<SIZE; ++i) {
        i1 = 0;

        for (unsigned int ii=0; ii<SIZE; ++ii) {
          if (ii == i)
            continue;

          j1 = 0;

          for (unsigned int jj=0; jj<SIZE; jj++) {
            if (jj == j)
              continue;

            c[i1][j1] = (*this)[ii][jj];
            ++j1;
          }

          ++i1;
        }

        if ((i+j) & 1)  result[i][j]=c.determinant();
        else result[i][j]=-c.determinant();
      }
    }

    break;
  }

  return result;
}
//=====================================================================================
template<typename Obj,unsigned int SIZE>
MATRIX & MATRIX::transpose() {
  register Obj tmp;

  for (unsigned int i=1; i<SIZE; ++i) {
    for (unsigned int j=0; j<i; ++j) {
      tmp = (*this)[i][j];
      (*this)[i][j] = (*this)[j][i];
      (*this)[j][i] = tmp;
    }
  }

  return (*this);
}
//=====================================================================================
template<typename Obj,unsigned int SIZE>
MATRIX & MATRIX::inverse() {
  (*this) = (*this).cofactor().transpose() /= (*this).determinant();
  return (*this);
}
//=====================================================================================
template<typename Obj,unsigned int SIZE>
MATRIX tlp::operator+(const MATRIX &mat1 ,const MATRIX &mat2) {
  return MATRIX(mat1)+=mat2;
}
//=====================================================================================
template<typename Obj,unsigned int SIZE>
MATRIX tlp::operator-(const MATRIX &mat1 ,const MATRIX &mat2) {
  return MATRIX(mat1)-=mat2;
}
//=====================================================================================
template<typename Obj,unsigned int SIZE>
MATRIX tlp::operator*(const MATRIX &mat1 ,const MATRIX &mat2) {
  MATRIX result;

  for (unsigned int i=0; i<SIZE; ++i)
    for (unsigned int j=0; j<SIZE; ++j) {
      result[i][j] = mat1[i][0] * mat2[0][j];

      for (unsigned int k=1; k<SIZE; ++k)
        result[i][j] += mat1[i][k] * mat2[k][j];
    }

  return result;
}
//=====================================================================================
template<typename Obj,unsigned int SIZE>
MATRIX MATRIX::operator/(const MATRIX &mat2) const {
  return  MATRIX(*this)/=mat2;
}
//=====================================================================================
template<typename Obj,unsigned int SIZE>
MATRIX MATRIX::operator/(const Obj &obj) const {
  return  MATRIX(*this) /= obj;
}
//=====================================================================================
template<typename Obj,unsigned int SIZE>
MATRIX tlp::operator*(const MATRIX &mat ,const Obj &obj) {
  return  MATRIX(mat) *= obj;
}
//=====================================================================================
template<typename Obj, unsigned int SIZE>
tlp::Vector<Obj, SIZE> tlp::operator*( const MATRIX &mat , const tlp::Vector<Obj, SIZE> &vec) {
  tlp::Vector<Obj,SIZE> result;

  for (unsigned int row=0; row<SIZE; ++row) {
    result[row] = mat[row][0] * vec[0];
  }

  for (unsigned int col=1; col<SIZE; ++col) {
    for (unsigned int row=0; row<SIZE; ++row) {
      result[row] += mat[row][col] * vec[col];
    }
  }

  return  result;
}
//=====================================================================================
template<typename Obj,unsigned int SIZE>
tlp::Vector<Obj,SIZE> tlp::operator*( const tlp::Vector<Obj,SIZE> &vec, const MATRIX &mat) {
  tlp::Vector<Obj,SIZE> result;

  for (unsigned int row=0; row<SIZE; ++row) {
    result[row] = mat[0][row] * vec[0];
  }

  for (unsigned int col=1; col<SIZE; ++col) {
    for (unsigned int row=0; row<SIZE; ++row) {
      result[row] += mat[col][row] * vec[col];
    }
  }

  return  result;
}

//=====================================================================================
template<typename Obj, unsigned int SIZE>
tlp::Vector<Obj, SIZE> MATRIX::powerIteration(const unsigned int nIterations) const {
  tlp::Vector<Obj, SIZE> iteration;

  for(unsigned int i=0; i < SIZE; i++)
    iteration[i] = 1;

  for(unsigned int i=0; i < nIterations; i++) {
    iteration = (*this) * iteration;

    iteration /= iteration.norm();
  }

  return iteration;
}

//=====================================================================================

template<typename Obj, unsigned int SIZE>
bool MATRIX::simplify(tlp::Matrix<Obj, 2> &simplifiedMatrix) const {
  if (SIZE != 3) {
    tlp::warning() << "Computation allowed only for 3x3 Matrices. Yours sizes : " << SIZE << "x" << SIZE << std::endl;

    return false;
  }

  // We start with a matrix representing an equation system under the following form :
  //
  // ax + by + cz = 0
  // dx + ey + fz = 0
  // gx + hy + iz = 0
  //
  // So M looks like :
  //
  //     ( ax by cz )  *(e1)*
  // M = ( dx ey fz )  *(e2)*
  //     ( gx hy iz )  *(e3)*
  //
  // What we want is something like that :
  //
  // jx + ky = 0
  // lx + mz = 0
  //
  // So to reduce the matrix, we will use the Gaussian Elimination.
  // For the first line we will apply a Gaussian Elimination between (e1) and (e2)
  // For the second line we will apply a Gaussian Elimination between (e1) and (e3)

  float coeff;

  // First Gaussian Elimination :
  // The pivot is z

  coeff = (*this)[1][2] / (*this)[0][2]; // fz / cz

  // After that:
  // jx = dx - (coeff * ax)
  // ky = ey - (coeff * by)
  simplifiedMatrix[0][0] = (*this)[1][0] - (coeff * (*this)[0][0]);
  simplifiedMatrix[0][1] = (*this)[1][1] - (coeff * (*this)[0][1]);

  // Second Gaussian Elimination :
  // The pivot is y

  coeff = (*this)[2][1] / (*this)[0][1]; // hy / by

  // Idem :
  // lx = gx - (coeff * ax)
  // mz = iz - (coeff * cz)
  simplifiedMatrix[1][0] = (*this)[2][0] - (coeff * (*this)[0][0]);
  simplifiedMatrix[1][1] = (*this)[2][2] - (coeff * (*this)[0][2]);

  return true;
}

//=====================================================================================

template<typename Obj, unsigned int SIZE>
bool MATRIX::computeEigenVector(const float x, tlp::Vector<Obj, 3> &eigenVector) const {
  if (SIZE != 2) {
    tlp::warning() << "Computation allowed only for 2x2 Matrices. Yours sizes : " << SIZE << "x" << SIZE << std::endl;

    return false;
  }

  eigenVector[0] = x; // Fixed by user

  // We know that the matrix we are using is under that form :
  //
  //     ( ax   by )
  // M = (         )
  //     ( cx   dz )
  //
  // Since we have a fixed x, we can compute y and z :
  //
  // y = -a / b
  // z = -c / d

  float a, b, c, d;

  a = (*this)[0][0];
  b = (*this)[0][1];
  c = (*this)[1][0];
  d = (*this)[1][1];

  eigenVector[1] = (-a * x) / b;
  eigenVector[2] = (-c * x) / d;

  return true;
}
