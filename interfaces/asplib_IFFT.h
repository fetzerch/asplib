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
class IFFT
{
public:
  typedef enum eFFT_LibID
  {
    ASPLIB_FFT_LIB_INVALID = -1,
    
    ASPLIB_FFT_LIB_KISS_FFT,
    
    ASPLIB_FFT_LIB_MAX
  }eFFT_LibID;

  IFFT(eFFT_LibID FFTLibID)
  {
    m_fftLibID = FFTLibID;
  }

  virtual ~IFFT() {}

  inline eFFT_LibID GetfftLibID()
  {
    return m_fftLibID;
  }

  inline int fftFreqFrameSize()
  {
    return m_fftFreqFrameSize;
  }

  inline int fftTimeFrameSize()
  {
    return m_fftTimeFrameSize;
  }

  ASPLIB_ERR Create(int FreqFrameSize, int TimeFrameSize, void *Options = NULL)
  {
    if(FreqFrameSize <= 0)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    if(TimeFrameSize < 0)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    m_fftTimeFrameSize = TimeFrameSize;
    m_fftFreqFrameSize = FreqFrameSize;
    return CreateFFTLib(FreqFrameSize, TimeFrameSize, Options);
  }
  
  virtual ASPLIB_ERR fft(void *In, void *Out) = 0;
  virtual ASPLIB_ERR ifft(void *In, void *Out) = 0;
  virtual ASPLIB_ERR fft(void *RIn, void *CIn, void *Out) = 0;
  virtual ASPLIB_ERR ifft(void *In, void *ROut, void *COut) = 0;

  virtual ASPLIB_ERR DestroyFFT() = 0;

protected:
  virtual ASPLIB_ERR CreateFFTLib(int FreqFrameSize, int TimeSize, void *Options = NULL) = 0;
  int     m_fftTimeFrameSize;
  int     m_fftFreqFrameSize;

private:
  eFFT_LibID  m_fftLibID;
};
}
