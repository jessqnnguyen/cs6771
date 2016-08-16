/**
 * KEY QUESTIONS:
 * What does it mean for a class or function to be const correct?
 * What is a static function/variable?
 * NOTES:
 * public variable - dot notation access
 */

#include <string>

#ifndef CAR_H
#define CAR_H

class Car {
  public:

    // Standard constructor.
    // Initialise car with a member initialiser list and uniform initialiser.
    Car(std::string m, unsigned int s): manufacturer{m}, numSeats{s} {numObjects++;}

    // Delegate constructor.
    Car(): Car("unknown", 4){}

    // Copy constructor.
    Car(const Car& c); // Make a copy, don't modify original object copying.

    // Move constructor.
    Car(Car&& c);

    // Copy operator - operator overloading.
    Car& operator=(const Car& c);

    // Move operator - operator overloading.
    Car& operator=(Car&& c);

    // Destructors!
    // Not car is the destructor for car.
    ~Car() {numObjects--;}

    // Return member vars.
    // This is a const function which returns a string - this means that
    // nothing outside of this function will be modified by this function.
    // Hence if a function is not going to change anything make it const
    inline std::string getManufacturer(void) const { return manufacturer}

    inline unsigned int getNumSeats(void) const { return numSeats; }

    // How do we access the static var numObjects?
    // Note: We can't make static functions const!
    inline static unsigned int getNumObjects() {return numObjects;}


  private:
  	std::string manufacturer;
  	unsigned int numSeats;

    // This can only be initialised in Car.cpp.
    static unsigned int numObjects;

};

#endif
