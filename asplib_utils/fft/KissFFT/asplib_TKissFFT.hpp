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

#include "interfaces/asplib_IFFT.h"
#include <KissFFT/kiss_fft.h>

#define KISS_FFT_INVERSE  1
#define KISS_FFT_FORWARD  0


namespace asplib
{
//|--------------------------|
//| requiered converters     |
//|--------------------------|
//| fft:                     |
//| - extern to fft          |
//| - fft to intern          |
//|                          |
//| ifft:                    |
//| - intern to fft          |
//| - fft to extern          |
//|--------------------------|
template<class ExternTOfftConverter, class fftTOExternConverter, class InternTOfftConverter, class fftTOInternConverter>
class TKissFFT : public IFFT
{
public:
  TKissFFT() : IFFT(ASPLIB_FFT_LIB_KISS_FFT) {}
  ~TKissFFT()
  {
    DestroyFFT();
  }

  static IFFT* GetHandle()
  {
    return new TKissFFT<ExternTOfftConverter, fftTOExternConverter, InternTOfftConverter, fftTOInternConverter>;
  }

  virtual ASPLIB_ERR fft(void *In, void *Out)
  {
    if (!In || !Out)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    kiss_fft(m_fftConfig, m_ExternTOfftConverter(In), m_fftTOInternConverter(Out));
    m_fftTOInternConverter.fmtc_fft_TO_Fmt(Out);

    return ASPLIB_ERR_NO_ERROR;
  }

  virtual ASPLIB_ERR fft(void *RIn, void *CIn, void *Out)
  {
    if (!RIn || !CIn || !Out)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    kiss_fft(m_fftConfig, m_ExternTOfftConverter(RIn, CIn), m_fftTOInternConverter(Out));
    m_fftTOInternConverter.fmtc_fft_TO_Fmt(Out);

    return ASPLIB_ERR_NO_ERROR;
  }

  virtual ASPLIB_ERR ifft(void *In, void *Out)
  {
    if (!In || !Out)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    kiss_fft(m_ifftConfig, m_InternTOfftConverter(In), m_fftTOExternConverter(Out));
    m_fftTOExternConverter.fmtc_fft_TO_Fmt(Out);

    return ASPLIB_ERR_NO_ERROR;
  }

  virtual ASPLIB_ERR ifft(void *In, void *ROut, void *COut)
  {
    if (!In || !ROut || !COut)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    kiss_fft(m_ifftConfig, m_InternTOfftConverter(In), m_fftTOExternConverter(ROut, COut));
    m_fftTOExternConverter.fmtc_fft_TO_SplitFmt(ROut, COut);

    return ASPLIB_ERR_NO_ERROR;
  }

  virtual ASPLIB_ERR DestroyFFT()
  {
    if (m_fftConfig)
    {
      kiss_fft_free(m_fftConfig);
    }
    m_fftConfig = NULL;

    if (m_ifftConfig)
    {
      kiss_fft_free(m_ifftConfig);
    }
    m_ifftConfig = NULL;

    m_ExternTOfftConverter.Destroy();
    m_fftTOExternConverter.Destroy();
    m_InternTOfftConverter.Destroy();
    m_fftTOInternConverter.Destroy();

    return ASPLIB_ERR_NO_ERROR;
  }

  // todo implement configuration part for overlapping frames
  virtual ASPLIB_ERR CreateFFTLib(int FreqFrameSize, int TimeSize, void *Options = NULL)
  {
    m_fftFreqFrameSize = kiss_fft_next_fast_size(TimeSize);

    m_fftConfig = kiss_fft_alloc(m_fftFreqFrameSize, KISS_FFT_FORWARD, 0, 0);
    m_ifftConfig = kiss_fft_alloc(m_fftFreqFrameSize, KISS_FFT_INVERSE, 0, 0);
    if (!m_fftConfig || !m_ifftConfig)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    if (m_ExternTOfftConverter.Create(m_fftFreqFrameSize, m_fftTimeFrameSize, Options) != ASPLIB_ERR_NO_ERROR)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    if (m_fftTOExternConverter.Create(m_fftFreqFrameSize, m_fftTimeFrameSize, Options) != ASPLIB_ERR_NO_ERROR)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    if (m_InternTOfftConverter.Create(m_fftFreqFrameSize, m_fftTimeFrameSize, Options) != ASPLIB_ERR_NO_ERROR)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    if (m_fftTOInternConverter.Create(m_fftFreqFrameSize, m_fftTimeFrameSize, Options) != ASPLIB_ERR_NO_ERROR)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    return ASPLIB_ERR_NO_ERROR;
  }

private:
  kiss_fft_cfg m_fftConfig;
  kiss_fft_cfg m_ifftConfig;
  
  // extern to intern converter objects
  ExternTOfftConverter m_ExternTOfftConverter;
  fftTOInternConverter m_fftTOExternConverter;

  // intern to extern converter objects
  InternTOfftConverter m_InternTOfftConverter;
  fftTOInternConverter m_fftTOInternConverter;
};
}
