#include <iostream>
#ifndef EUCLIDEANVECTOR_H
#define EUCLIDEANVECTOR_H

namespace evec {
  class EuclideanVector {
  public:

    // CONSTRUCTORS.
    EuclideanVector();
    // Default constructor (delegate constructor), sets magnitude to default 1.
    EuclideanVector(unsigned int d);
    EuclideanVector(unsigned int d, double m);

    template <typename Iterator> EuclideanVector(Iterator begin, Iterator end) {
      // Determine dimension of vector.
      // Store dimension.
      int d = 0;
      for (auto it = begin; it != end; ++it) {
        d++;
      }
      dimension = d;
      vector = new double[d];
      unsigned int i = 0;
      // Populate vector with magnitudes.
      for (auto it = begin; it != end; ++it) {
        vector[i] = *it;
        i++;
      }
    }

    EuclideanVector(std::initializer_list<double> il);

    // Copy constructor.
    EuclideanVector(const EuclideanVector& v); // Make a copy, don't modify original object copying.

    // Move constructor.
    EuclideanVector(EuclideanVector&& v); // && refers to an rvalue reference

    // Destructor.
    ~EuclideanVector(){ delete[] vector; }

    // Getters.
    unsigned int getNumDimensions() { return dimension; }
    // Returns a double, the value of the magnitude in the dimension
    // given as the function parameter
    double get(unsigned int dimension) const { return vector[dimension]; }

    // Returns the Euclidean norm of the vector as a double.
    // The Euclidean norm is the square root of the sum of
    // the squares of the magnitudes in each dimension.
    double getEuclideanNorm();
    // Returns an Euclidean vector that is the unit vector of this vector.
    // The magnitude for each dimension in the unit vector is the original
    // vector's magnitude divided by the Euclidean norm.
    EuclideanVector createUnitVector();

    // Get and set the value in a given dimension of the Euclidean Vector.
    double& operator[](const unsigned int dimension);
    void operator+=(const EuclideanVector &b);
    void operator-=(const EuclideanVector &b);
    void operator*=(const EuclideanVector &b);
    void operator/=(const EuclideanVector &b);

    // True if the two vectors are equal in the number of dimensions and the magnitude in each dimension is equal.
    friend bool operator==(const EuclideanVector &a, const EuclideanVector &b);
    friend bool operator!=(const EuclideanVector &a, const EuclideanVector &b);
    friend EuclideanVector operator+(const EuclideanVector &a, const EuclideanVector &b);
    friend EuclideanVector operator-(const EuclideanVector &a, const EuclideanVector &b);
    friend EuclideanVector operator/(const EuclideanVector &a, const EuclideanVector &b);

    // Define << operator.
    friend std::ostream& operator<<(std::ostream& out, const EuclideanVector& v);

  private:
    double *vector;
    unsigned int dimension;
    double magnitude;
    // Store cached Euclidean norm, default -1 until first call of getEuclideanNorm().
    double euclideanNorm;
  };
}
#endif
