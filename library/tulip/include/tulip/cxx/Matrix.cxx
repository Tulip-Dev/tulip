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
MATRIXTLPGEO::Matrix(const std::vector<std::vector<Obj> > &covarianceMatrix)
{
  for(unsigned int i=0; i < SIZE; i++)
      for(unsigned int j=0; j < SIZE; j++)
	  (*this)[i][j] = covarianceMatrix[i][j] / (sqrt(covarianceMatrix[i][i] * covarianceMatrix[j][j]));
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
  for (unsigned int i=0;i<SIZE;++i)
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
  for (unsigned int i=0;i<SIZE;++i)
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

//=====================================================================================
template<typename Obj, unsigned int SIZE>
tlp::Vector<Obj, SIZE> MATRIXTLPGEO::powerIteration(const int nIterations) const
{
  tlp::Vector<Obj, SIZE> iteration;

  for(unsigned int i=0; i < SIZE; i++)
    iteration[i] = 1;
  
  for(unsigned int i=0; i < nIterations; i++)
    {
      iteration = (*this) * iteration;
      
      iteration /= iteration.norm();
    }

  return iteration;
}

//=====================================================================================
template<typename Obj, unsigned int SIZE>
void MATRIXTLPGEO::caracteristicPolynome(Polynome &result) const
{

  // This function only works for 3x3 matrices :
  if (SIZE != 3)
    {
      std::cerr << "Computation allowed only for 3x3 Matrices. Yours sizes : " << SIZE << "x" << SIZE << std::endl;
      return;
    }
  
  // This is quite an ugly function but ... well ... it works :)
  // If P is the caracteristic Polynome of the matrix M
  // with :
  //     ( a-Lambda  b         c       )
  // M = ( d         e-Lambda  f       )
  //     ( g         h         i-Lambda)
  //
  // Then we deduce that P = A * Lambda³ + B * Lambda² + C * Lambda + D
  // Where :
  //  
  //  A = -1
  //  B = a + e + i
  //  C = -ae -ai -ei + hf + bd + cg
  //  D = aei - ahf - bdi + bgf + cdh - cge

  float a, b, c, d, e, f, g, h, i;

  a = (*this)[0][0];
  b = (*this)[0][1];
  c = (*this)[0][2];
  d = (*this)[1][0];
  e = (*this)[1][1];
  f = (*this)[1][2];
  g = (*this)[2][0];
  h = (*this)[2][1];
  i = (*this)[2][2];

  // A = -1
  result.a = -1;

  // B = a + e + i
  result.b = a + e + i;

  // C = -ae -ai -ei + hf + bd + cg
  result.c = -(a * e) - (a * i) - (e * i) + (h * f) + (b * d) + (c * g);

  // D = aei - ahf - bdi + bgf + cdh - cge
  result.d = (a * e * i) - (a * h * f) - (b * d * i) + (b * g * f) + (c * d * h) - (c * g * e);
}

//=====================================================================================

template<typename Obj, unsigned int SIZE>
bool MATRIXTLPGEO::simplify(tlp::Matrix<Obj, 2> &simplifiedMatrix) const
{
   if (SIZE != 3)
    {
      std::cerr << "Computation allowed only for 3x3 Matrices. Yours sizes : " << SIZE << "x" << SIZE << std::endl;

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
bool MATRIXTLPGEO::computeEigenVector(const float x, tlp::Vector<Obj, 3> &eigenVector) const
{
  if (SIZE != 2)
    {
      std::cerr << "Computation allowed only for 2x2 Matrices. Yours sizes : " << SIZE << "x" << SIZE << std::endl;

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

//=====================================================================================

template<typename Obj, unsigned int SIZE>
bool MATRIXTLPGEO::computeEigenVectors(tlp::Matrix<Obj, 3> &eigenVectors) const
{
  if (SIZE != 3)
    {
      std::cerr << "Computation allowed only for 3x3 Matrices. Yours sizes : " << SIZE << "x" << SIZE << std::endl;

      return false;
    }

  // To compute the EigenVectors of the matrix, we first need to find the EigenValues
  
  // We compute, firstly, the Caracteristic Polynome of the Matrix :
  Polynome cara;

  caracteristicPolynome(cara);

  // EigenValues are given by the roots of the Polynome :
  float EigenValues[3];
  int nRes;

  cara.resolv(EigenValues, nRes);

  if (nRes != 3)
    {
      std::cerr << "Non Symmetric Matrix !!!" << std::endl;

      return false;
    }

  /* ==== DEBUG ====
     std:: cout << "EigenValues = ";
  for(int i=0; i < 3; i++) 
    std::cout << EigenValues[i] << "; "; 
    std::cout << std::endl; */

  for(int i=0; i < 3; i++)
    {
      // We compute the equation system :
      Matrix<float, 3> eQSys(*this);

      for(int j=0; j < 3; j++)
	eQSys[j][j] -= EigenValues[i];

      /*  ==== DEBUG ====
      std::cout << "\nEQSYS =\n"; 
      for(int j=0; j < 3; j++)
      std::cout << eQSys[j] << std::endl;*/

      // And we simplify it :
      Matrix<float, 2> eVSys;

      if (!eQSys.simplify(eVSys))
	{
	  std::cerr << "Couldn't Simplify matrix for " << i << "th EigenValue\n";

	  return false;
	}

      /* ==== DEBUG ====
      std::cout << "\nEVQYQ =\n";
      for(int j=0; j < 2; j++)
      std::cout << eVSys[j] << std::endl; */

      // And finally, we find the matching EigenVector
      if (!eVSys.computeEigenVector(1, eigenVectors[i]))
	{
	  std::cerr << "Couldn't compute EigenVector for " << i << "th EigenValue\n";

	  return false;
	}
      
    }

  return true;
}
