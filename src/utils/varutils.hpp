#ifndef _VARUTILS_HPP
#define _VARUTILS_HPP

/**
 * @file      varutils.hpp
 *
 * @version   1.0-1b
 *
 * @author    xanthos@mail.ntua.gr <br>
 *            danast@mail.ntua.gr
 *
 * @date      October, 2014
 *
 * @brief     Various functions to perform utility tasks.
 *
 * @details   This file contains a number of functions to perform simple but
 *            often encountered tasks (e.g. split strings).
 *
 * @note      This library must be compiled under the C++11 standard.
 *
 * @todo      Function readbackwards is not used (?). Propably should be deleted.
 *
 * @copyright No COPYRIGHT whatsoever.
 *
 * <b><center><hr>
 * National Technical University of Athens <br>
 *      Dionysos Satellite Observatory     <br>
 *        Higher Geodesy Laboratory        <br>
 *      http://dionysos.survey.ntua.gr
 * <hr></center></b>
 *
 */

/** @example test_varutils.cpp
 *  Example usage of vutils library.
 */

#include <vector>
#include <string>

/**
 * @brief Split a string to a vector of strings, using char \c delim as delimeter.
 */
std::vector<std::string> splitstr (const std::string&,const char delim=' ');
/**
 * @brief Remove all instances of char \c c, both from the begining and the end of a string.
 */
std::string stripstr (const std::string&, const char c=' ');
/**
 * @brief Split a string at the column number given and return the two parts.
 */
std::string splitstr (const std::string&, std::string&, int d=80);
/**
 * @brief Convert a FORTRAN floating point string to a C-style floating point string
 */
std::string fortran2cpp_sci (const std::string&, bool&, bool check=false);
/**
 * @brief position an input stream in the start of previous line, before the stream's
 * current position; for more than one lines, use num_of_lines
 */
int readbackwards (std::istream* is, int num_of_lines=1);

#endif
