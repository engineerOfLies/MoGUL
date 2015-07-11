#ifndef __MGL_SHAPES__
#define __MGL_SHAPES__
/**
 * mgl_shapes
 * @license The MIT License (MIT)
   @copyright Copyright (c) 2015 EngineerOfLies
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */

/**
 * @prupose
 * Provide container data types for various shapes.
 */

#include "mgl_dict.h"
#include "mgl_vector.h"
#include "mgl_rect.h"

typedef struct MglLines_S MglLines;


/**
 * @brief allocated a new line sequence, initialized to empty
 * Free it with mgl_shape_lines_free();
 * @return NULL on error, or a newly allocated MglLine pointer.
 */
MglLines *mgl_shape_lines_new();

/**
 * @brief create a copy of the provided line sequence.
 * Free it with mgl_shape_lines_free();
 * @param src the original line sequence to copy
 * @return NULL on error, or a new copy of the MglLine pointer.
 */
MglLines *mgl_shape_lines_clone(MglLines *src);

/**
 * @brief free a line sequence from memory
 * @param lines a pointer to your lines pointer.  Sets your pointer to NULL.
 */
void mgl_shape_lines_free(MglLines **lines);

/**
 * @brief append a new endpoint to the line sequence
 * @param lines the line sequence to append to
 * @param point the point to append
 */
void mgl_shape_lines_append_point(MglLines *lines, MglVec2D point);

/**
 * @brief get the number of points in the line sequence
 * @param lines the line sequence to get the length of (number of end points)
 * @return 0 on error or empty line, the number of end points otherwise
 */
MglUint mgl_shape_lines_get_count(MglLines *lines);

/**
 * @brief get the nth point vector data fromt he Line Sequence
 * @param point [output] if the nth point exists, this will be set
 * @param lines the line squence to poll
 * @param n the nth end point will be checked
 */
void mgl_shape_lines_get_nth_point(MglVec2D *point,MglLines *lines, MglUint n);

/**
 * @brief subdivide the nth line in the list
 * @param lines the line sequence to subdivide
 * @param n the edge to subdivide
 */
void mgl_lines_subdivide_line(MglLines *lines,MglUint n);

/**
 * @brief get a pointer to the internal data dictionary for the line sequence
 * @return NULL on error, or the data dictionary
 */
MglDict *mgl_lines_get_data(MglLines *lines);

/**
 * @brief get the bounding rect for a line segment
 * @param p1 one end point of the line segment
 * @param p2 another end point of the line segment
 * @return the bounding rectangle
 */
MglRect mgl_line_segment_to_rect(MglVec2D p1,MglVec2D p2);

#endif
