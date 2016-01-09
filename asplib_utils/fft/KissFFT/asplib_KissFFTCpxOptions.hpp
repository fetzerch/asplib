#pragma once

/* Copyright (C) 2014-2015 Achim Turan, Achim.Turan@o2online.de
 * https://github.com/AchimTuran/asplib
 *
 * This file is part of asplib (Achim's Signal Processing LIBrary)
 *
 * asplib (Achim's Signal Processing LIBrary) is free software:
 * you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * asplib (Achim's Signal Processing LIBrary) is distributed
 * in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with asplib (Achim's Signal Processing LIBrary).
 * ifnot, see <http://www.gnu.org/licenses/>.
 *
 */



#include "asplib_utils/os/asplib_base_os.h"
#include "asplib_utils/constants_typedefs/asplib_typedefs.h"

#include "asplib_utils/base/extendedStructs/asplib_ExtendedStructs.hpp"


namespace asplib
{
struct KissFFT_CpxOptions : ASPLIB_EXTENDED_STRUCT(KissFFT_CpxOptions)
{
  bool enableZeroPadding; // This options lets Kiss FFT complex choose a frame size that is a power of two.
                          // This is achieved by inserting zeros at the end of the array, 
                          // which will have the highest performance. The resulting spectrum will have this frame size.

  KissFFT_CpxOptions() : ASPLIB_EXTENDED_STRUCT_REGISTER(ASPLIB_EXTENDED_STRUCT_KissFFTCpxOptions)
  {
    enableZeroPadding = false;
  }
};
}