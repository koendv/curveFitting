/**
 * @file curveFitting.h
 * @brief Lightweight curve fitting library for linear, exponential, logarithmic, and power regressions.
 * @details Designed for minimal RAM usage and efficient use on microcontrollers (e.g., Arduino).
 * @url http://github.com/koendv/curveFitting
 */

#ifndef _CURVE_FITTING_H_
#define _CURVE_FITTING_H_

#include <cmath>

/**
 * @class linFit
 * @brief Performs simple linear regression on streamed (x, y) data.
 * @details Computes slope, intercept, correlation, mean, and standard deviation.
 */

template<typename Scalar = float>
class linFit {
public:
  linFit() {
    clear();
  }

  /*! @brief clear statistics */
  void clear() {
    _sum_x = 0.0;
    _sum_x2 = 0.0;
    _sum_y = 0.0;
    _sum_y2 = 0.0;
    _sum_xy = 0.0;
    _n = 0;
  }

  /*! @brief add a sample */
  void add(Scalar x, Scalar y) {
    _sum_x += x;
    _sum_x2 += x * x;
    _sum_y += y;
    _sum_y2 += y * y;
    _sum_xy += x * y;
    _n++;
  }

  /*! @brief remove previously added sample */
  void remove(Scalar x, Scalar y) {
    _sum_x -= x;
    _sum_x2 -= x * x;
    _sum_y -= y;
    _sum_y2 -= y * y;
    _sum_xy -= x * y;
    --_n;
  }

  /*! @brief number of samples */
  inline int n() {
    return _n;
  }

  /*! @brief slope */
  Scalar a() {
    return (_n * _sum_xy - _sum_x * _sum_y) / (_n * _sum_x2 - _sum_x * _sum_x);
  }

  /*! @brief intercept */
  Scalar b() {
    return (_sum_y * _sum_x2 - _sum_x * _sum_xy) / (_n * _sum_x2 - _sum_x * _sum_x);
  }

  /*! @brief correlation*/
  Scalar r() {
    return (_n * _sum_xy - _sum_x * _sum_y) / std::sqrt((_n * _sum_x2 - _sum_x * _sum_x) * (_n * _sum_y2 - _sum_y * _sum_y));
  }

  /*! @brief standard deviation of x */
  Scalar sd_x() {
    return std::sqrt((_sum_x2 - _sum_x * _sum_x / _n) / (_n - 1));
  }

  /*! @brief standard deviation of y */
  Scalar sd_y() {
    return std::sqrt((_sum_y2 - _sum_y * _sum_y / _n) / (_n - 1));
  }

  /*! @brief mean of x */
  Scalar mean_x() {
    return _sum_x / _n;
  }

  /*! @brief mean of y */
  Scalar mean_y() {
    return _sum_y / _n;
  }

  /**
   * @brief predict the y-value for a given x.
   * @param x x-coordinate.
   * @return predicted y-coordinate.
   */
  Scalar y(Scalar x) {
    return a() * x + b();
  }

  /**
   * @brief predict the x-value for a given y.
   * @param y y-coordinate.
   * @return predicted x-coordinate.
   */
  Scalar x(Scalar y) {
    return (y - b()) / a();
  }

private:
  Scalar _sum_x, _sum_x2, _sum_y, _sum_y2, _sum_xy;
  int _n;
};

/**
 * @class expFit
 * @brief Fits data to an exponential curve y = b * exp(a * x)
 */

template<typename Scalar = float>
class expFit : public linFit<Scalar> {
public:

  /*! @brief add a sample */
  void add(Scalar x, Scalar y) {
    linFit<Scalar>::add(x, std::log(y));
  }

  /*! @brief remove previously added sample */
  void remove(Scalar x, Scalar y) {
    linFit<Scalar>::remove(x, std::log(y));
  }

  /*! @brief initial value */
  Scalar b() {
    return std::exp(linFit<Scalar>::b());
  }

  /*! @brief returns the geometric standard deviation of y */
  Scalar sd_y() {
    return std::exp(linFit<Scalar>::sd_y());
  }

  /*! @brief returns the geometric mean of y */
  Scalar mean_y() {
    return std::exp(linFit<Scalar>::mean_y());
  }

  /**
   * @brief predict the y-value for a given x.
   * @param x x-coordinate.
   * @return predicted y-coordinate.
   */
  Scalar y(Scalar x) {
    return std::exp(linFit<Scalar>::y(x));
  }

  /**
   * @brief predict the x-value for a given y.
   * @param y y-coordinate.
   * @return predicted x-coordinate.
   */
  Scalar x(Scalar y) {
    return linFit<Scalar>::x(std::log(y));
  }
};

/**
 * @class logFit
 * @brief Fits data to a logarithmic curve y = a * log(x) + b
 */

template<typename Scalar = float>
class logFit : public linFit<Scalar> {
public:

  /*! @brief add a sample */
  void add(Scalar x, Scalar y) {
    linFit<Scalar>::add(std::log(x), y);
  }

  /*! @brief remove previously added sample */
  void remove(Scalar x, Scalar y) {
    linFit<Scalar>::remove(std::log(x), y);
  }

  /*! @brief returns the geometric standard deviation of x */
  Scalar sd_x() {
    return std::exp(linFit<Scalar>::sd_x());
  }

  /*! @brief returns the geometric mean of x */
  Scalar mean_x() {
    return std::exp(linFit<Scalar>::mean_x());
  }

  /**
   * @brief predict the y-value for a given x.
   * @param x x-coordinate.
   * @return predicted y-coordinate.
   */
  Scalar y(Scalar x) {
    return linFit<Scalar>::y(std::log(x));
  }

  /**
   * @brief predict the x-value for a given y.
   * @param y y-coordinate.
   * @return predicted x-coordinate.
   */
  Scalar x(Scalar y) {
    return std::exp(linFit<Scalar>::x(y));
  }
};

/**
 * @class powFit
 * @brief Fits data to a power curve y = b * pow(x, a)
 */

template<typename Scalar = float>
class powFit : public linFit<Scalar> {
public:

  /*! @brief add a sample */
  void add(Scalar x, Scalar y) {
    linFit<Scalar>::add(std::log(x), std::log(y));
  }

  /*! @brief remove previously added sample */
  void remove(Scalar x, Scalar y) {
    linFit<Scalar>::remove(std::log(x), std::log(y));
  }

  /*! @brief scaling factor */
  Scalar b() {
    return std::exp(linFit<Scalar>::b());
  }

  /*! @brief returns the geometric standard deviation of x */
  Scalar sd_x() {
    return std::exp(linFit<Scalar>::sd_x());
  }

  /*! @brief returns the geometric standard deviation of y */
  Scalar sd_y() {
    return std::exp(linFit<Scalar>::sd_y());
  }

  /*! @brief returns the geometric mean of x */
  Scalar mean_x() {
    return std::exp(linFit<Scalar>::mean_x());
  }

  /*! @brief returns the geometric mean of y */
  Scalar mean_y() {
    return std::exp(linFit<Scalar>::mean_y());
  }

  /**
   * @brief predict the y-value for a given x.
   * @param x x-coordinate.
   * @return predicted y-coordinate.
   */
  Scalar y(Scalar x) {
    return std::exp(linFit<Scalar>::y(std::log(x)));
  }

  /**
   * @brief predict the x-value for a given y.
   * @param y y-coordinate.
   * @return predicted x-coordinate.
   */
  Scalar x(Scalar y) {
    return std::exp(linFit<Scalar>::x(std::log(y)));
  }
};
#endif
