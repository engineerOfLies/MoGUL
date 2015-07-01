#ifndef __MGL_INPUT_H__
#define __MGL_INPUT_H__
/**
 * mgl_input
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

#include "mgl_types.h"

/**
 * @purpose provide an abstraction for individual input actions.  These may be keyboard, mouse, gamepad, etc. referenced by name.
 */

typedef struct MglInput_S MglInput;

typedef enum
{
    MglInputNull,
    MglInputKey,
    MglInputMod,
    MglInputButton,
    MglInputAxisPositive,
    MglInputAxisNegative
}MglInputType;

typedef enum
{
    MglInputKeyboard,
    MglInputMouse,
    MglInputJoystick
}MglInputDevice;

/**
 * @brief initialize the input manager
 */
void mgl_input_init(
    MglUint maxInputs
);

/**
 * @brief update tracked inputs
 */
void mgl_input_update();


#endif