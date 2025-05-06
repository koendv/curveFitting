/**
 * @file curveFitting.h
 * @brief Lightweight curve fitting library for linear, exponential, logarithmic, and power regressions.
 * @details Designed for minimal RAM usage and efficient use on microcontrollers (e.g., Arduino).
 * @url http://github.com/koendv/curveFitting
 */

#ifndef _CURVE_FITTING_H_
#define _CURVE_FITTING_H_

#include <math.h>

/**
 * @class linFit
 * @brief Performs simple linear regression on streamed (x, y) data.
 * @details Computes slope, intercept, correlation, mean, and standard deviation.
 */

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
  void add(float x, float y) {
    _sum_x += x;
    _sum_x2 += x * x;
    _sum_y += y;
    _sum_y2 += y * y;
    _sum_xy += x * y;
    _n++;
  }

  /*! @brief remove previously added sample */
  void remove(float x, float y) {
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
  float a() {
    return (_n * _sum_xy - _sum_x * _sum_y) / (_n * _sum_x2 - _sum_x * _sum_x);
  }

  /*! @brief intercept */
  float b() {
    return (_sum_y * _sum_x2 - _sum_x * _sum_xy) / (_n * _sum_x2 - _sum_x * _sum_x);
  }

  /*! @brief correlation*/
  float r() {
    return (_n * _sum_xy - _sum_x * _sum_y) / sqrtf((_n * _sum_x2 - _sum_x * _sum_x) * (_n * _sum_y2 - _sum_y * _sum_y));
  }

  /*! @brief standard deviation of x */
  float std_x() {
    return sqrtf((_sum_x2 - _sum_x * _sum_x / _n) / (_n - 1));
  }

  /*! @brief standard deviation of y */
  float std_y() {
    return sqrtf((_sum_y2 - _sum_y * _sum_y / _n) / (_n - 1));
  }

  /*! @brief mean of x */
  float mean_x() {
    return _sum_x / _n;
  }

  /*! @brief mean of y */
  float mean_y() {
    return _sum_y / _n;
  }

  /**
   * @brief predict the y-value for a given x.
   * @param x x-coordinate.
   * @return predicted y-coordinate.
   */
  float y(float x) {
    return a() * x + b();
  }

  /**
   * @brief predict the x-value for a given y.
   * @param y y-coordinate.
   * @return predicted x-coordinate.
   */
  float x(float y) {
    return (y - b()) / a();
  }

private:
  float _sum_x, _sum_x2, _sum_y, _sum_y2, _sum_xy;
  int _n;
};

/**
 * @class expFit
 * @brief Fits data to an exponential curve y = b * exp(a * x)
 */

class expFit : public linFit {
public:

  /*! @brief add a sample */
  void add(float x, float y) {
    linFit::add(x, logf(y));
  }

  /*! @brief remove previously added sample */
  void remove(float x, float y) {
    linFit::remove(x, logf(y));
  }

  /*! @brief initial value */
  float b() {
    return expf(linFit::b());
  }

  /*! @brief returns the geometric standard deviation of y */
  float std_y() {
    return expf(linFit::std_y());
  }

  /*! @brief returns the geometric mean of y */
  float mean_y() {
    return expf(linFit::mean_y());
  }

  /**
   * @brief predict the y-value for a given x.
   * @param x x-coordinate.
   * @return predicted y-coordinate.
   */
  float y(float x) {
    return expf(linFit::y(x));
  }

  /**
   * @brief predict the x-value for a given y.
   * @param y y-coordinate.
   * @return predicted x-coordinate.
   */
  float x(float y) {
    return linFit::x(logf(y));
  }
};

/**
 * @class logFit
 * @brief Fits data to a logarithmic curve y = a * log(x) + b
 */

class logFit : public linFit {
public:

  /*! @brief add a sample */
  void add(float x, float y) {
    linFit::add(logf(x), y);
  }

  /*! @brief remove previously added sample */
  void remove(float x, float y) {
    linFit::remove(logf(x), y);
  }

  /*! @brief returns the geometric standard deviation of x */
  float std_x() {
    return expf(linFit::std_x());
  }

  /*! @brief returns the geometric mean of x */
  float mean_x() {
    return expf(linFit::mean_x());
  }

  /**
   * @brief predict the y-value for a given x.
   * @param x x-coordinate.
   * @return predicted y-coordinate.
   */
  float y(float x) {
    return linFit::y(logf(x));
  }

  /**
   * @brief predict the x-value for a given y.
   * @param y y-coordinate.
   * @return predicted x-coordinate.
   */
  float x(float y) {
    return expf(linFit::x(y));
  }
};

/**
 * @class powFit
 * @brief Fits data to a power curve y = b * pow(x, a)
 */

class powFit : public linFit {
public:

  /*! @brief add a sample */
  void add(float x, float y) {
    linFit::add(logf(x), logf(y));
  }

  /*! @brief remove previously added sample */
  void remove(float x, float y) {
    linFit::remove(logf(x), logf(y));
  }

  /*! @brief scaling factor */
  float b() {
    return expf(linFit::b());
  }

  /*! @brief returns the geometric standard deviation of x */
  float std_x() {
    return expf(linFit::std_x());
  }

  /*! @brief returns the geometric standard deviation of y */
  float std_y() {
    return expf(linFit::std_y());
  }

  /*! @brief returns the geometric mean of x */
  float mean_x() {
    return expf(linFit::mean_x());
  }

  /*! @brief returns the geometric mean of y */
  float mean_y() {
    return expf(linFit::mean_y());
  }

  /**
   * @brief predict the y-value for a given x.
   * @param x x-coordinate.
   * @return predicted y-coordinate.
   */
  float y(float x) {
    return expf(linFit::y(logf(x)));
  }

  /**
   * @brief predict the x-value for a given y.
   * @param y y-coordinate.
   * @return predicted x-coordinate.
   */
  float x(float y) {
    return expf(linFit::x(logf(y)));
  }
};
#endif
