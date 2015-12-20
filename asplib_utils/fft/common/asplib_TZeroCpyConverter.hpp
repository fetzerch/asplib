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



namespace asplib
{
// This template converts (casts) between the fft-library, extern and intern used data format.
// FFT: data format used by the fft-library
// EXTERN: this indicates the data format that is used by the application
// INTERN: data format that is used by asplib for further processing
template<typename FFT, typename FMT>
class TZeroCpyConverter
{
public:
  // convert FFT to FMT
  inline void fmtc_fft_TO_Fmt(void *Fmt)
  {// not used by zero copy converter
  }

  // convert FMT to FFT
  inline FFT* operator () (void *Fmt)
  {
    return static_cast<FFT*>(Fmt);
  }

  inline void fmtc_fft_TO_SplitFmt(void *Extern1, void *Extern2)
  {
  }

  // TODO: implement functionality for split arrays (real and complex part are separate pointers)
  inline FFT* operator () (void *Fmt1, void *Fmt2)
  {
    return static_cast<FFT*>(Fmt1);
  }

  // create converter
  inline ASPLIB_ERR Create(int FreqFrameSize, int TimeSize, void *Options = NULL)
  {// not used by zero copy converter
    return ASPLIB_ERR_NO_ERROR;
  }

  // destroy converter
  inline ASPLIB_ERR Destroy()
  {// not used by zero copy converter
    return ASPLIB_ERR_NO_ERROR;
  }
};
}
