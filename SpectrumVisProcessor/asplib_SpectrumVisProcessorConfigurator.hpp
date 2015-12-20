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

#include "asplib_utils/base/extendedStructs/asplib_ExtendedStructContainer.hpp"
#include "asplib_utils/constants_typedefs/asplib_dataFmtDefines.hpp"

#include "asplib_utils/fft/fftWindowing/fftWindowingFactory.h"
#include "asplib_utils/fft/asplib_FFTFactory.hpp"
#include "SpectrumVisProcessor/AxisScaler/asplib_AxisScalerFactory.hpp"
#include "SpectrumVisProcessor/Remapper/asplib_RemapperFactory.hpp"
#include "SpectrumVisProcessor/FFTSmoother/asplib_FFTSmootherFactory.hpp"


namespace asplib
{
class CSpectrumVisProcessor;

class CSpectrumVisProcessorConfigurator
{
friend class CSpectrumVisProcessor;

private:
  typedef void* moduleOptions_t;

  typedef struct ConfigFmt_t
  {
    asplibFmt_t inFmt;
    asplibFmt_t outFmt;

    ConfigFmt_t()
    {
      inFmt  = ASPLIB_FMT_INVALID;
      outFmt = ASPLIB_FMT_INVALID;
    }
  }ConfigFmt_t;


  typedef struct ConfigFFTWindowing_t
  {
    ConfigFmt_t                         fmt;
    CFFTWindowFunctions::eFFT_WINDOW_ID FFTWindowID;
    CExtendedStructContainer            options;

    ConfigFFTWindowing_t()
    {
      FFTWindowID = CFFTWindowFunctions::FFT_WINDOW_ID_HANN;
      fmt.inFmt   = ASPLIB_FMT_NATIVE_FLOAT;
      fmt.outFmt  = ASPLIB_FMT_NATIVE_FLOAT;
    }
  }ConfigFFTWindowing_t;

  typedef struct ConfigFFT_t
  {
    ConfigFmt_t fmt;
    FFTLibID_t  FFTLibID;
    CExtendedStructContainer options;

    ConfigFFT_t()
    {
      FFTLibID    = ASPLIB_FFT_KISS_FFT;
      fmt.inFmt   = ASPLIB_FMT_NATIVE_FLOAT;
      fmt.outFmt  = ASPLIB_FMT_NATIVE_CPX_FLOAT;
    }
  }ConfigFFT_t;

  typedef struct ConfigFFTSmoother_t
  {
    ConfigFmt_t     fmt;
    FFTSmootherID_t FFTSmootherID;
    CExtendedStructContainer options;

    ConfigFFTSmoother_t()
    {
      FFTSmootherID = ASPLIB_FFT_SMOOTHER_dlbeer;
      fmt.inFmt     = ASPLIB_FMT_NATIVE_FLOAT;
      fmt.outFmt    = ASPLIB_FMT_NATIVE_FLOAT;
    }
  }ConfigFFTSmoother_t;

  typedef struct ConfigRemapper_t
  {
    ConfigFmt_t   fmt;
    RemapperID_t  remapperID;
    CExtendedStructContainer options;

    ConfigRemapper_t()
    {
      remapperID  = ASPLIB_REMAPPER_GammeCorrector;
      fmt.inFmt   = ASPLIB_FMT_NATIVE_FLOAT;
      fmt.outFmt  = ASPLIB_FMT_NATIVE_FLOAT;
    }
  }ConfigRemapper_t;

  typedef struct ConfigAxisScaler_t
  {
    ConfigFmt_t     fmt;
    AxisScalerID_t  axisScalerID;
    CExtendedStructContainer options;

    ConfigAxisScaler_t()
    {
      axisScalerID  = ASPLIB_AXIS_SCALER_Log10Scaler;
      fmt.inFmt     = ASPLIB_FMT_NATIVE_FLOAT;
      fmt.outFmt    = ASPLIB_FMT_NATIVE_FLOAT;
    }
  }ConfigAxisScaler_t;


public:
  ASPLIB_ERR          ConfigFFT(asplibFmt_t InFmt, asplibFmt_t OutFmt, FFTLibID_t FFTLibID, void* Options = NULL);
  ASPLIB_ERR ConfigFFTWindowing(asplibFmt_t InFmt, asplibFmt_t OutFmt, CFFTWindowFunctions::eFFT_WINDOW_ID FFTWindowID, void* Options = NULL);
  ASPLIB_ERR  ConfigFFTSmoother(asplibFmt_t InFmt, asplibFmt_t OutFmt, FFTSmootherID_t FFTSmootherID, void* Options = NULL);
  ASPLIB_ERR     ConfigRemapper(asplibFmt_t InFmt, asplibFmt_t OutFmt, RemapperID_t RemapperID, void* Options = NULL);
  ASPLIB_ERR   ConfigAxisScaler(asplibFmt_t InFmt, asplibFmt_t OutFmt, AxisScalerID_t ScalerID, void* Options = NULL);

private:
  ASPLIB_ERR SetFmt(asplibFmt_t InFmt, asplibFmt_t OutFmt, ConfigFmt_t &Fmt);

  ConfigFFTWindowing_t  m_ConfigFFTWindowing;
  ConfigFFT_t           m_ConfigFFT;
  ConfigRemapper_t      m_ConfigRemapper;
  ConfigAxisScaler_t    m_ConfigAxisScaler;
  ConfigFFTSmoother_t   m_ConfigFFTSmoother;
};
}
