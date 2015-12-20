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



#include "asplib_SpectrumVisProcessor.hpp"
#include "asplib_utils/constants_typedefs/asplib_dataFmtDefines.hpp"
#include "SpectrumVisProcessor/FFTSmoother/dlbeerSmoother/asplib_dlbeerSmootherOptions.hpp"

#include <vector>

using namespace std;


namespace asplib
{
CSpectrumVisProcessor::CSpectrumVisProcessor()
{
  m_FFT               = NULL;
  m_FFTWindow         = NULL;
  m_InBuf             = NULL;
  m_OutFFTBuf         = NULL;
  m_PostProcess       = NULL;
  m_PostProcessBuf[0] = NULL;
  m_PostProcessBuf[1] = NULL;
  
  
  m_FrameSize = 0;
  m_MaxProcessingSteps = -1;
}


CSpectrumVisProcessor::~CSpectrumVisProcessor()
{
  Destroy();
}


ASPLIB_ERR CSpectrumVisProcessor::Create(CSpectrumVisProcessorConfigurator &Config, uint32_t FrameSize)
{
  if (FrameSize <= 0)
  {
    return ASPLIB_ERR_INVALID_INPUT;
  }
  Destroy();
  m_FrameSize = FrameSize;

  ASPLIB_ERR err = ASPLIB_ERR_NO_ERROR;

  // create FFT window
  err = CFFTWindowingFactory::CreateWindow(m_FrameSize, Config.m_ConfigFFTWindowing.FFTWindowID, m_FFTWindow);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    Destroy();
    return err;
  }


  // create FFT library
  err = ASPLIB_FACTORY_FFT.Create(Config.m_ConfigFFT.FFTLibID, Config.m_ConfigFFT.fmt.inFmt, Config.m_ConfigFFT.fmt.outFmt, m_FFT);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    Destroy();
    return err;
  }

  err = m_FFT->Create(m_FrameSize, m_FrameSize, Config.m_ConfigFFT.options);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    Destroy();
    return err;
  }

  vector<IProcess*> postprocess;

  // create axis scaler
  if (Config.m_ConfigAxisScaler.axisScalerID > ASPLIB_AXIS_SCALER_INVALID_ID || Config.m_ConfigAxisScaler.axisScalerID < ASPLIB_AXIS_SCALER_MAX_ID)
  {
    IProcess *process = NULL;
    err = ASPLIB_FACTORY_AXIS_SCALER.Create(Config.m_ConfigAxisScaler.axisScalerID, Config.m_ConfigAxisScaler.fmt.inFmt, Config.m_ConfigAxisScaler.fmt.outFmt, process);
    if (err != ASPLIB_ERR_NO_ERROR)
    {
      Destroy();
      return err;
    }
    m_ProcessVector.push_back(process);

    err = process->Create(m_FrameSize, Config.m_ConfigAxisScaler.options);
    if (err != ASPLIB_ERR_NO_ERROR)
    {
      Destroy();
      return err;
    }
  }


  // create remapper
  if (Config.m_ConfigRemapper.remapperID > ASPLIB_REMAPPER_INVALID_ID || Config.m_ConfigRemapper.remapperID < ASPLIB_REMAPPER_MAX_ID)
  {
    IProcess *process = NULL;
    err = ASPLIB_FACTORY_REMAPPER.Create(Config.m_ConfigRemapper.remapperID, Config.m_ConfigRemapper.fmt.inFmt, Config.m_ConfigRemapper.fmt.outFmt, process);
    if (err != ASPLIB_ERR_NO_ERROR)
    {
      Destroy();
      return err;
    }
    m_ProcessVector.push_back(process);

    err = process->Create(m_FrameSize, Config.m_ConfigRemapper.options);
    if (err != ASPLIB_ERR_NO_ERROR)
    {
      Destroy();
      return err;
    }
  }

  // create FFT smoother
  if (Config.m_ConfigFFTSmoother.FFTSmootherID > ASPLIB_FFT_SMOOTHER_INVALID_ID || Config.m_ConfigFFTSmoother.FFTSmootherID < ASPLIB_FFT_SMOOTHER_MAX_ID)
  {
    IProcess *process = NULL;
    err = ASPLIB_FACTORY_SMOOTHER.Create(Config.m_ConfigFFTSmoother.FFTSmootherID, Config.m_ConfigFFTSmoother.fmt.inFmt, Config.m_ConfigFFTSmoother.fmt.outFmt, process);
    if (err != ASPLIB_ERR_NO_ERROR)
    {
      Destroy();
      return err;
    }
    m_ProcessVector.push_back(process);

    err = process->Create(m_FrameSize, Config.m_ConfigFFTSmoother.options);
    if (err != ASPLIB_ERR_NO_ERROR)
    {
      Destroy();
      return err;
    }
  }

  if (m_ProcessVector.size() > 0)
  {
    m_PostProcess = m_ProcessVector.data();
    m_MaxProcessingSteps = (int8_t)m_ProcessVector.size() -1;
  }
  else
  {
    m_PostProcess = NULL;
    m_MaxProcessingSteps = -1;
  }

  // create buffers
  m_OutFFTBuf = new FrameBuffer_NativeCpxFloat(m_FrameSize, 1);
  m_InBuf     = new FrameBuffer_NativeFloat(m_FrameSize, 1);
  m_PostProcessBuf[0] = new FrameBuffer_NativeFloat(m_FrameSize, 1);
  m_PostProcessBuf[1] = new FrameBuffer_NativeFloat(m_FrameSize, 1);
  

  return err;
}


ASPLIB_ERR CSpectrumVisProcessor::Process(float *In, float *Out)
{
  asplibFmt_NativeCPXFloat *cpxSamples = m_OutFFTBuf->get_Frame(0);
  asplibFmt_NativeFloat *rSamples = (asplibFmt_NativeFloat*)(cpxSamples);
  asplibFmt_NativeFloat *inBuf = NULL;
  asplibFmt_NativeFloat *outBuf = m_PostProcessBuf[1]->get_Frame(0);

  if (m_MaxProcessingSteps >= 0)
  {
    inBuf = m_PostProcessBuf[0]->get_Frame(0);
  }
  else
  {
    inBuf = Out;
  }

  m_FFTWindow->Process(In, m_InBuf->get_Frame(0));

  m_FFT->fft(m_InBuf->get_Frame(0), cpxSamples);

  for (uint32_t ii = 0; ii < m_FrameSize; ii++)
  {
    inBuf[ii] = sqrtf(cpxSamples[ii][0]*cpxSamples[ii][0] + cpxSamples[ii][1]*cpxSamples[ii][1]);
  }

  if (m_MaxProcessingSteps >= 0)
  {
    for (int8_t step = 0; step < m_MaxProcessingSteps; step++)
    {
      m_PostProcess[step]->Process(inBuf, outBuf);

      asplibFmt_NativeFloat *tmp = inBuf;
      inBuf = outBuf;
      outBuf = tmp;
    }

    m_PostProcess[m_MaxProcessingSteps]->Process(inBuf, Out);
  }

  return ASPLIB_ERR_NO_ERROR;
}


ASPLIB_ERR CSpectrumVisProcessor::Destroy()
{
  ASPLIB_FACTORY_FFT.Destroy(m_FFT);
  CFFTWindowingFactory::DestroyWindow(m_FFTWindow);

  for (IProcess::ProcessVector_t::iterator iter = m_ProcessVector.begin(); iter != m_ProcessVector.end(); ++iter)
  {
    (*iter)->Destroy();
    delete *iter;
    *iter = NULL;
  }
  m_ProcessVector.clear();
  m_PostProcess = NULL;

  //ASPLIB_FACTORY_SMOOTHER.Destroy(m_FFTSmoother);
  //ASPLIB_FACTORY_REMAPPER.Destroy(m_Remapper);
  //ASPLIB_FACTORY_AXIS_SCALER.Destroy(m_AxisScaler);

  delete m_InBuf;
  m_InBuf = NULL;

  delete m_PostProcessBuf[0];
  m_PostProcessBuf[0] = NULL;
  delete m_PostProcessBuf[1];
  m_PostProcessBuf[1] = NULL;

  delete m_OutFFTBuf;
  m_OutFFTBuf = NULL;

  return ASPLIB_ERR_NO_ERROR;
}
}
