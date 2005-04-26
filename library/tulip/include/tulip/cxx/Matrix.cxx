#include <cmath> 

#define MATRIXTLPGEO tlp::Matrix<Obj,SIZE>


//===================================================================
//Specialisation
namespace tlp {
    template<typename Obj>
    class Matrix<Obj,1>:public Vector<Vector<Obj,1>,1> {
    public:
      Obj determinant() {return (*this)[0][0];}
      //      Matrix<Obj,1>& fill(Obj obj) {return *this;}
      Matrix<Obj,1>& inverse() {(*this)[0][0] = 1.0 / (*this)[0][0]; return *this;}
      Matrix<Obj,1>& transpose() {return *this;}
      Matrix<Obj,1>& operator*=(const Matrix<Obj,1> &mat) {(*this)[0][0] *= mat[0][0]; return *this;}
      //      Matrix<Obj,1>& operator/=(const Obj &obj){return *this;}
      Matrix<Obj,1> cofactor() {return *this;}
    };
}

//===================================================================
template<typename Obj,unsigned int SIZE>
MATRIXTLPGEO & MATRIXTLPGEO::fill(Obj obj) {
  for (unsigned int i=0; i<SIZE; ++i)
    (*this)[i].fill(obj);
  return (*this);
}
//===================================================================
template<typename Obj,unsigned int SIZE>
MATRIXTLPGEO & MATRIXTLPGEO::operator*=(const MATRIXTLPGEO &mat) {
  MATRIXTLPGEO tmpMat(*this);
  fill(0);
  for (unsigned int i=0;i<SIZE;++i)
    for (unsigned int j=0;j<SIZE;++j)
      for (unsigned int k=0;k<SIZE;++k)
	(*this)[i][j]+=tmpMat[i][k]*mat[k][j];
  return (*this);
}
//=====================================================================================
template<typename Obj,unsigned int SIZE>
MATRIXTLPGEO & MATRIXTLPGEO::operator*=(const Obj &obj) {
  for (int i=0;i<SIZE;++i)
    (*this)[i] *= obj;
  return (*this);
}
//=====================================================================================
template<typename Obj,unsigned int SIZE>
MATRIXTLPGEO & MATRIXTLPGEO::operator/=(const MATRIXTLPGEO &mat) {
  MATRIXTLPGEO tmpMat(mat);
  tmpMat.inverse();
  (*this) *= tmpMat;
  return (*this);
}
//=====================================================================================
template<typename Obj,unsigned int SIZE>
MATRIXTLPGEO & MATRIXTLPGEO::operator/=(const Obj &obj) {
  for (int i=0;i<SIZE;++i)
    (*this)[i] /= obj;
  return (*this);
}
//=====================================================================================
template<typename Obj,unsigned int SIZE>
Obj MATRIXTLPGEO::determinant() const {
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
MATRIXTLPGEO MATRIXTLPGEO::cofactor() const{
  MATRIXTLPGEO result;
  switch (SIZE){
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
    for (unsigned int j=0;j<SIZE;++j) {
      for (unsigned int i=0;i<SIZE;++i) {
	i1 = 0;
	for (unsigned int ii=0;ii<SIZE;++ii) {
	  if (ii == i)
	    continue;
	  j1 = 0;
	  for (unsigned int jj=0;jj<SIZE;jj++) {
	    if (jj == j)
	      continue;
	    c[i1][j1] = (*this)[ii][jj];
	    ++j1;
	  }
	    ++i1;
	}
	if ((i+j) & 1)  result[i][j]=c.determinant(); else result[i][j]=-c.determinant();
      }
    }
    break;
  }
  return result;
}
//=====================================================================================
template<typename Obj,unsigned int SIZE>
MATRIXTLPGEO & MATRIXTLPGEO::transpose() {
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
MATRIXTLPGEO & MATRIXTLPGEO::inverse() {
  (*this) = (*this).cofactor().transpose() /= (*this).determinant();
  return (*this);
}
//=====================================================================================
template<typename Obj,unsigned int SIZE>
MATRIXTLPGEO MATRIXTLPGEO::operator*(const MATRIXTLPGEO &mat2) const {
  return  MATRIXTLPGEO(*this)*=mat2;
}
//=====================================================================================
template<typename Obj,unsigned int SIZE>
MATRIXTLPGEO MATRIXTLPGEO::operator/(const MATRIXTLPGEO &mat2) const{
  return  MATRIXTLPGEO(*this)/=mat2;
}
//=====================================================================================
template<typename Obj,unsigned int SIZE>
MATRIXTLPGEO MATRIXTLPGEO::operator*(const Obj &obj) const {
  return  MATRIXTLPGEO(*this) *= obj;
}
//=====================================================================================
template<typename Obj,unsigned int SIZE>
MATRIXTLPGEO MATRIXTLPGEO::operator/(const Obj &obj) const{
  return  MATRIXTLPGEO(*this) /= obj;
}
//=====================================================================================
template<typename Obj,unsigned int SIZE>
tlp::Vector<Obj,SIZE> MATRIXTLPGEO::operator*(const tlp::Vector<Obj,SIZE> &vec) const {
  tlp::Vector<Obj,SIZE> result;
  for (unsigned int row=0; row<SIZE; ++row) {
    result[row] = (*this)[row][0] * vec[0];
  }
  for (unsigned int col=1; col<SIZE; ++col) {
    for (unsigned int row=0; row<SIZE; ++row) {
      result[row] += (*this)[row][col] * vec[col];
    }
  }
  return  result;
}
