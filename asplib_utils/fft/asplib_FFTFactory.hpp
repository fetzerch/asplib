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



#include "asplib_utils/constants_typedefs/asplib_constants.h"
#include "asplib_utils/constants_typedefs/asplib_typedefs.h"
#include "asplib_utils/os/asplib_base_os.h"

#include "interfaces/asplib_IFFT.h"
#include "asplib_utils/base/autoFactory/asplib_TAutoFactory.hpp"
#include "asplib_utils/constants_typedefs/asplib_dataFmtDefines.hpp"


#define CREATE_ASPLIB_FFT_CLASS(Class, ClassID, InFmt, OutFmt, FFTObject)   \
class Class :                                                               \
  public CFFTFactory::TRegisterProduct<Class, ClassID, InFmt, OutFmt>,      \
  public FFTObject                                                          \
{                                                                           \
public:                                                                     \
  Class() {}                                                                \
};

#define ASPLIB_FACTORY_FFT asplib::CFFTFactory()


namespace asplib
{
// prototype class definition, implementation is available in <asplib_root>/asplib_utils/fft/asplib_FFTFactory.cpp
class CFFTFactoryMetaData
{
public:
  const static std::string Name;
};

typedef enum FFTLibID_t
{
  ASPLIB_FFT_LIB_INVALID_ID = -1,

  ASPLIB_FFT_KISS_FFT,

  ASPLIB_FFT_LIB_MAX_ID
}FFTLibID_t;

typedef TAutoFactory<IFFT, FFTLibID_t, asplibFmt_t, CFFTFactoryMetaData> TFFTFactory;

class CFFTFactory : public TFFTFactory
{
public:
  CFFTFactory();
  ~CFFTFactory();
};
}
