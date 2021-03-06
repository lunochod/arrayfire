/*******************************************************
 * Copyright (c) 2015, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <af/array.h>
#include <af/graphics.h>
#include "symbol_manager.hpp"


af_err af_create_window(af_window *out, const int width, const int height, const char* const title)
{
    return CALL(out, width, height, title);
}

af_err af_set_position(const af_window wind, const unsigned x, const unsigned y)
{
    return CALL(wind, x, y);
}

af_err af_set_title(const af_window wind, const char* const title)
{
    return CALL(wind, title);
}

af_err af_set_size(const af_window wind, const unsigned w, const unsigned h)
{
    return CALL(wind, w, h);
}

af_err af_draw_image(const af_window wind, const af_array in, const af_cell* const props)
{
    return CALL(wind, in, props);
}

af_err af_draw_plot(const af_window wind, const af_array X, const af_array Y, const af_cell* const props)
{
    return CALL(wind, X, Y, props);
}

af_err af_draw_hist(const af_window wind, const af_array X, const double minval, const double maxval, const af_cell* const props)
{
    return CALL(wind, X, minval, maxval, props);
}

af_err af_grid(const af_window wind, const int rows, const int cols)
{
    return CALL(wind, rows, cols);
}

af_err af_show(const af_window wind)
{
    return CALL(wind);
}

af_err af_is_window_closed(bool *out, const af_window wind)
{
    return CALL(out, wind);
}

af_err af_destroy_window(const af_window wind)
{
    return CALL(wind);
}
