#include "mgl_shape.h"
#include "mgl_types.h"
#include "mgl_dict.h"
#include "mgl_logger.h"

struct MglLines_S
{
    MglDict *_points;
};

MglLines *mgl_shape_lines_new()
{
    MglLines *lines;
    lines = (MglLines *)malloc(sizeof(MglLines));
    if (!lines)
    {
        mgl_logger_error("failed to create a new line sequence");
        return NULL;
    }
    lines->_points = mgl_dict_new_list();
    if (!lines->_points)
    {
        free(lines);
        mgl_logger_error("failed to create line sequence list");
        return NULL;
    }
    return lines;
}

void mgl_shape_lines_free(MglLines **lines)
{
    if ((!lines)||(!*lines))return;
    if ((*lines)->_points)
    {
        mgl_dict_free(&(*lines)->_points);
    }
    free(*lines);
    *lines = NULL;
}

void mgl_shape_lines_append_point(MglLines *lines, MglVec2D point)
{
    MglDict *pointObject;
    if (!lines)return;
    pointObject = mgl_dict_new_hash();
    mgl_dict_hash_insert(pointObject,"point",mgl_dict_new_vec2d(point));
    mgl_dict_list_append(lines->_points,pointObject);
}

MglUint mgl_shape_lines_get_count(MglLines *lines)
{
    if (!lines)return 0;
    if (!lines->_points)return 0;
    return mgl_dict_get_list_count(lines->_points);
}

void mgl_shape_lines_get_nth_point(MglVec2D *point,MglLines *lines, MglUint n)
{
    MglDict *temp;
    if ((!lines)||(!point))return;
    temp = mgl_dict_get_list_nth(lines->_points, n);
    if (!temp)return;
    mgl_dict_get_hash_value_as_vec2d(point, temp, "point");
    mgl_logger_warn("point data: (%f,%f)",point->x,point->y);
}

/*eol@eof*/
