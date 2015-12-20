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

#include "interfaces/asplib_IProcess.hpp"


namespace asplib
{
typedef struct TLog10ScalerOptions_t
{
  long double ScaleVal;
  long double InftyApprox;

  TLog10ScalerOptions_t()
  {
    ScaleVal    = 20.0;
    InftyApprox = -150;
  }
}TLog10ScalerOptions_t;


template<typename T>
class TLog10Scaler : public IProcess
{
public:
  TLog10Scaler()
  {
    m_ScaleVal    = 20;
    m_FrameSize   = 0;
    m_InftyApprox = -150;
  }


  virtual ASPLIB_ERR Create(uint32_t FrameSize, void *Options = NULL)
  {
    if (FrameSize <= 0)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    if (Options)
    {
      TLog10ScalerOptions_t *options = static_cast<TLog10ScalerOptions_t*>(Options);

      m_ScaleVal    = static_cast<T>(options->ScaleVal);
      m_InftyApprox = static_cast<T>(options->InftyApprox);
    }

    m_FrameSize = FrameSize;

    return ASPLIB_ERR_NO_ERROR;
  }


  virtual ASPLIB_ERR Process(void *In, void *Out)
  {
    T *input = (T*)In;
    T *output = (T*)Out;
    for (uint32_t ii = 0; ii < m_FrameSize; ii++)
    {
      if (input[ii] <= 0)
      {
        output[ii] = m_InftyApprox;
      }
      else
      {
        output[ii] = m_ScaleVal*log10(input[ii]);
      }
    }

    return ASPLIB_ERR_NO_ERROR;
  }


  virtual ASPLIB_ERR Destroy()
  {
    m_ScaleVal = 1;
    m_FrameSize = 0;
    m_InftyApprox = -1000;

    return ASPLIB_ERR_NO_ERROR;
  }


private:
  uint32_t  m_FrameSize;
  T         m_ScaleVal;
  T         m_InftyApprox;
};
}
