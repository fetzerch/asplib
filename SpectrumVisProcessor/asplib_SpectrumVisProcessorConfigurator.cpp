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



#include "asplib_SpectrumVisProcessorConfigurator.hpp"


namespace asplib
{
ASPLIB_ERR CSpectrumVisProcessorConfigurator::ConfigFFT(asplibFmt_t InFmt, asplibFmt_t OutFmt, FFTLibID_t FFTLibID, void* Options/* = NULL*/)
{
  ASPLIB_ERR err = SetFmt(InFmt, OutFmt, m_ConfigFFT.fmt);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    return err;
  }

  m_ConfigFFT.FFTLibID = FFTLibID;
  m_ConfigFFT.options.SaveStruct(Options);

  return ASPLIB_ERR_NO_ERROR;
}


ASPLIB_ERR CSpectrumVisProcessorConfigurator::ConfigFFTWindowing(asplibFmt_t InFmt, asplibFmt_t OutFmt, CFFTWindowFunctions::eFFT_WINDOW_ID FFTWindowID, void* Options/* = NULL*/)
{
  ASPLIB_ERR err = SetFmt(InFmt, OutFmt, m_ConfigFFTWindowing.fmt);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    return err;
  }

  m_ConfigFFTWindowing.FFTWindowID = FFTWindowID;
  m_ConfigFFTWindowing.options.SaveStruct(Options);

  return ASPLIB_ERR_NO_ERROR;
}


ASPLIB_ERR CSpectrumVisProcessorConfigurator::ConfigFFTSmoother(asplibFmt_t InFmt, asplibFmt_t OutFmt, FFTSmootherID_t FFTSmootherID, void* Options/* = NULL*/)
{
  ASPLIB_ERR err = SetFmt(InFmt, OutFmt, m_ConfigFFTSmoother.fmt);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    return err;
  }

  m_ConfigFFTSmoother.FFTSmootherID = FFTSmootherID;
  m_ConfigFFTSmoother.options.SaveStruct(Options);

  return ASPLIB_ERR_NO_ERROR;
}


ASPLIB_ERR CSpectrumVisProcessorConfigurator::ConfigRemapper(asplibFmt_t InFmt, asplibFmt_t OutFmt, RemapperID_t RemapperID, void* Options/* = NULL*/)
{
  ASPLIB_ERR err = SetFmt(InFmt, OutFmt, m_ConfigRemapper.fmt);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    return err;
  }

  m_ConfigRemapper.remapperID = RemapperID;
  m_ConfigRemapper.options.SaveStruct(Options);

  return ASPLIB_ERR_NO_ERROR;
}


ASPLIB_ERR CSpectrumVisProcessorConfigurator::ConfigAxisScaler(asplibFmt_t InFmt, asplibFmt_t OutFmt, AxisScalerID_t ScalerID, void* Options/* = NULL*/)
{
  ASPLIB_ERR err = SetFmt(InFmt, OutFmt, m_ConfigAxisScaler.fmt);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    return err;
  }

  m_ConfigAxisScaler.axisScalerID = ScalerID;
  m_ConfigAxisScaler.options.SaveStruct(Options);

  return ASPLIB_ERR_NO_ERROR;
}


ASPLIB_ERR CSpectrumVisProcessorConfigurator::SetFmt(asplibFmt_t InFmt, asplibFmt_t OutFmt, ConfigFmt_t &Fmt)
{
  if (InFmt <= ASPLIB_FMT_INVALID || InFmt >= ASPLIB_FMT_MAX || OutFmt <= ASPLIB_FMT_INVALID || OutFmt >= ASPLIB_FMT_MAX)
  {
    return ASPLIB_ERR_INVALID_INPUT;
  }

  Fmt.inFmt  = InFmt;
  Fmt.outFmt = OutFmt;

  return ASPLIB_ERR_NO_ERROR;
}
}
