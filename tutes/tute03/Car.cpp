#include "Car.h"

unsigned int Car::numObjects = 0;

// Copy constructor
Car::Car(const Car& c) {
  manufacturer = c.manufacturer; // dot notation access for private member vars if same type of class!
  numSeats = c.numSeats;
  numObjects++;
}

// Move constructor
Car::Car( Car&& c) {
  manufacturer = std::move(c.manufacturer);
  numSeats = std::move(numSeats);
  numObjects++;
}

// Copy operator
Car& Car::operator=(const Car& c) {
  manufacturer = c.manufacturer;
  numSeats = c.numSeats;
  // numObjects++; don't need to increment because already incremented in constructor
  return *this;
}

// Move operator
Car& Car::operator=(Car&& c) {
  manufacturer = std::move(c.manufacturer);
  numSeats = std::move(c.numSeats);
  return *this;
}

// TIP
// Question - is this valid?
// a = std::move(a);
// Generally, don't do this, put in move guards.
// Since you don't want to operate on yourself!
// So you check you're not moving on yourself in the move guards.

// Assignment 2
// unsigned int EuclideanVector::EuclideanVector() {...}
