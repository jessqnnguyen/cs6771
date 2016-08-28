#include "EuclideanVector.h"
#include <iostream>
#include <cmath> // Calculate square root library
#include <string.h>

namespace evec {

  // CONSTRUCTORS.
  EuclideanVector::EuclideanVector(): EuclideanVector(1){}

  // Default constructor (delegate constructor), sets magnitude to default 1 in each dimension.
  EuclideanVector::EuclideanVector(unsigned int d): EuclideanVector(d, 0.0){}

  EuclideanVector::EuclideanVector(unsigned int d, double m) :
                dimension{d}, magnitude{m}, euclideanNorm{-1} {
    vector = new double[dimension];
    for (unsigned int i = 0; i < dimension; ++i) {
      vector[i] = magnitude;
    }
  }

  EuclideanVector::EuclideanVector(std::initializer_list<double> il) {
    vector = new double[il.size()];
    int i = 0;
    for (auto it = il.begin(); it != il.end(); ++it) {
      vector[i] = *it;
      i++;
    }
  }

  // Copy constructor.
  EuclideanVector::EuclideanVector(const EuclideanVector& v) :
              dimension{v.dimension}, magnitude{v.magnitude} {
    vector = new double[dimension];
    for (unsigned int i = 0; i < v.dimension; ++i) {
      vector[i] = v.vector[i];
    }
  }

  // Move constructor.
  EuclideanVector::EuclideanVector(EuclideanVector&& v) :
    dimension{v.dimension}, magnitude{v.magnitude} {
    // dimension = std::move(v.dimension);
    // magnitude = std::move(v.magnitude);
    vector = std::move(v.vector);
    // [TODO: Clean up this function and also fix move functionality.]
    // [NOTE: Currently, move doesn't clear the original vector, how to?] 
    // double *temp = new double[1];
    // temp[0] = 0;
    // delete [] v.vector;
    // v.vector = temp;
    // vector = new double[dimension];
    // for (unsigned int i = 0; i < v.dimension; ++i) {
    //   vector[i] = std::move(v.vector[i]);
    // }
  }

  double EuclideanVector::getEuclideanNorm() {
    // If Euclidean norm hasn't been calculated and cached.
    if (euclideanNorm != -1) {
      int sumSquares = 0;
      for (unsigned int i = 0; i < dimension; ++i) {
        sumSquares += vector[i] * vector[i];
      }
      euclideanNorm = sqrt(sumSquares);
    }
    return euclideanNorm;
  }

  EuclideanVector EuclideanVector::createUnitVector() {
    EuclideanVector unitVector(dimension, magnitude);
    if (euclideanNorm != -1) {
      unitVector.euclideanNorm = euclideanNorm;
    }
    // Loop through the new unit vector's vector and divide all original magnitudes
    // by the Euclidean norm
    double currMagnitude;
    for (unsigned int i = 0; i < dimension; ++i) {
      currMagnitude = vector[i];
      unitVector.vector[i] = currMagnitude/getEuclideanNorm();
    }
    return unitVector;

  }

  double& EuclideanVector::operator[](const unsigned int dimension) {
    return vector[dimension];
  }

  void EuclideanVector::operator+=(const EuclideanVector &b) {
    // Check both vectors are of the same dimension.
    if (dimension == b.dimension) {
      for (unsigned int i = 0; i < dimension; ++i) {
        vector[i] += b.vector[i];
      }
    }
  }

  void EuclideanVector::operator-=(const EuclideanVector &b) {
    // Check both vectors are of the same dimension.
    if (dimension == b.dimension) {
      for (unsigned int i = 0; i < dimension; ++i) {
        vector[i] -= b.vector[i];
      }
    }
  }

  void EuclideanVector::operator*=(const EuclideanVector &b) {
    // Check both vectors are of the same dimension.
    if (dimension == b.dimension) {
      for (unsigned int i = 0; i < dimension; ++i) {
        vector[i] *= b.vector[i];
      }
    }
  }

  void EuclideanVector::operator/=(const EuclideanVector &b) {
    // Check both vectors are of the same dimension.
    if (dimension == b.dimension) {
      for (unsigned int i = 0; i < dimension; ++i) {
        vector[i] /= b.vector[i];
      }
    }
  }

  bool operator==(const EuclideanVector &a, const EuclideanVector &b) {
    bool result = true;
    if (a.dimension != b.dimension) result = false;
    if (a.magnitude != b.magnitude) result = false;
    for (unsigned int i = 0; i < a.dimension; ++i) {
      if (a.vector[i] != b.vector[i]) result = false;
    }
    return result;
  }

  bool operator!=(const EuclideanVector &a, const EuclideanVector &b) {
    bool result = true;
    if (a.dimension == b.dimension) result = false;
    if (a.magnitude == b.magnitude) result = false;
    for (unsigned int i = 0; i < a.dimension; ++i) {
      if (a.vector[i] == b.vector[i]) result = false;
    }
    return result;
  }

  EuclideanVector operator+(const EuclideanVector &a, const EuclideanVector &b) {
    EuclideanVector outputVector(a.dimension);
    // Check both vectors are of the same dimension.
    if (a.dimension == b.dimension) {
      for (unsigned int i = 0; i < a.dimension; ++i) {
         outputVector.vector[i] = a.vector[i] + b.vector[i];
      }
    }
    return outputVector;
  }

  EuclideanVector operator-(const EuclideanVector &a, const EuclideanVector &b) {
    EuclideanVector outputVector(a.dimension);
    // Check both vectors are of the same dimension.
    if (a.dimension == b.dimension) {
      for (unsigned int i = 0; i < a.dimension; ++i) {
         outputVector.vector[i] = a.vector[i] - b.vector[i];
      }
    }
    return outputVector;
  }

  EuclideanVector operator/(const EuclideanVector &a, const EuclideanVector &b) {
    EuclideanVector outputVector(a.dimension);
    // Check both vectors are of the same dimension.
    if (a.dimension == b.dimension) {
      for (unsigned int i = 0; i < a.dimension; ++i) {
         outputVector.vector[i] = a.vector[i] / b.vector[i];
      }
    }
    return outputVector;
  }


  std::ostream& operator<<(std::ostream& out, const EuclideanVector& v) {
    out << "[";
    for (unsigned int i = 0; i < v.dimension; ++i) {
      out << v.vector[i];
      if (i != v.dimension - 1) {
        out << " ";
      }
    }
    out << "]";
    return out;
  }


}
