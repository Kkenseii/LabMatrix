#pragma once
#include <cmath>
#include <iostream>
#include <stdexcept>
#include "tmathvector.h"

template <class T>
class TMatrix : public TMathVector<TMathVector<T>> {

};
