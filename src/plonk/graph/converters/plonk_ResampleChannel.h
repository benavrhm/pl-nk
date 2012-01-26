/*
 -------------------------------------------------------------------------------
 This file is part of the Plink, Plonk, Plank libraries
  by Martin Robinson
 
 http://code.google.com/p/pl-nk/
 
 Copyright University of the West of England, Bristol 2011-12
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
 * Neither the name of University of the West of England, Bristol nor 
   the names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 DISCLAIMED. IN NO EVENT SHALL UNIVERSITY OF THE WEST OF ENGLAND, BRISTOL BE 
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
 GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
 OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 
 This software makes use of third party libraries. For more information see:
 doc/license.txt included in the distribution.
 -------------------------------------------------------------------------------
 */

#ifndef PLONK_RESAMPLECHANNEL_H
#define PLONK_RESAMPLECHANNEL_H

#include "../channel/plonk_ChannelInternalCore.h"
#include "../plonk_GraphForwardDeclarations.h"


/** Resampler. */
template<class SampleType>
class ResampleChannelInternal 
:   public ChannelInternal<SampleType, ChannelInternalCore::Data>
{
public:
    typedef ChannelInternalCore::Data                   Data;
    typedef ChannelBase<SampleType>                     ChannelType;
    typedef ResampleChannelInternal<SampleType>         ResampleInternal;
    typedef ChannelInternal<SampleType,Data>            Internal;
    typedef ChannelInternalBase<SampleType>             InternalBase;
    typedef UnitBase<SampleType>                        UnitType;
    typedef InputDictionary                             Inputs;
    typedef NumericalArray<SampleType>                  Buffer;
    typedef typename TypeUtility<SampleType>::IndexType IndexType;
        
    ResampleChannelInternal (Inputs const& inputs, 
                             Data const& data, 
                             BlockSize const& blockSize,
                             SampleRate const& sampleRate) throw()
    :   Internal (inputs, data, blockSize, sampleRate),
        tempBufferPos (0),
        nextInputTimeStamp (TimeStamp::getZero())
    {
        plonk_assert (sampleRate.getValue() > 0.0);       // no need to resample a DC signal
    }
    
    Text getName() const throw()
    {
        return "Resample";
    }        
    
    IntArray getInputKeys() const throw()
    {
        const IntArray keys (IOKey::Signal);
        return keys;
    }
    
    InternalBase* getChannel (const int index) throw()
    {
        const Inputs channelInputs = this->getInputs().getChannel (index);
        return new ResampleInternal (channelInputs, 
                                     this->getState(), 
                                     this->getBlockSize(), 
                                     this->getSampleRate());
    }    
    
    void initChannel (const int channel) throw()
    {        
        const UnitType& input = this->getInputAsUnit (IOKey::Signal);
                
        plonk_assert (input.getOverlap (channel) == DoubleVariable::getOne());
        
        const SampleType sourceValue = input.getValue (channel);
        this->initValue (sourceValue);        
                
        // one larger for interpolation
        resizeTempBuffer (input.getBlockSize (channel).getValue() + 1);
        tempBuffer.zero(); // was a bug on interpolation here..
        tempBufferPos = tempBufferPosMax;
        
        tempBuffer.put (tempBufferPos, sourceValue);
    }    
    
    inline void resizeTempBuffer (const int inputBufferLength) throw()
    {
        if (inputBufferLength != tempBuffer.length())
        {
            tempBuffer.setSize (inputBufferLength, false);
            tempBufferPosMax = IndexType (inputBufferLength) - Math<IndexType>::get1();
        }
    }
    
    void process (ProcessInfo& info, const int channel) throw()
    {        
        int i;
        
        const Data& data = this->getState();
        
        UnitType& inputUnit (this->getInputAsUnit (IOKey::Signal));

        const IndexType inputSampleRate = IndexType (inputUnit.getSampleRate (channel));
        
        const int outputBufferLength = this->getOutputBuffer().length();
        SampleType* outputSamples = this->getOutputSamples();
        
        if (inputSampleRate <= 0.0)
        {
            const SampleType inputValue = inputUnit.process (info, channel).atUnchecked (0);
            
            for (i = 0; i < outputBufferLength; ++i)
                outputSamples[i] = inputValue;
        }
        else
        {
            const TimeStamp infoTimeStamp = info.getTimeStamp();
            const IndexType tempBufferIncrement = inputSampleRate * IndexType (data.sampleDuration); 
            SampleType* const outputSamplesEnd = outputSamples + outputBufferLength;
            
            while (outputSamples < outputSamplesEnd)
            {
                int tempBufferLength = this->tempBuffer.length();
                SampleType* tempBufferSamples = this->tempBuffer.getArray();

                if (tempBufferPos >= tempBufferPosMax) // ran out of buffer
                {
                    info.setTimeStamp (nextInputTimeStamp);
                    const Buffer inputBuffer (inputUnit.process (info, channel));
                    nextInputTimeStamp = inputUnit.getNextTimeStamp (channel);
                    
                    const SampleType* const inputSamples = inputBuffer.getArray();
                    const int inputBufferLength = inputBuffer.length();
                                    
                    const SampleType interpolationValue = tempBufferSamples[tempBufferLength - 1];
                    
                    tempBufferPos -= tempBufferPosMax;        // move this before resize
                    tempBufferLength = inputBufferLength + 1; // one larger for interpolation
                    resizeTempBuffer (tempBufferLength);
                    tempBufferSamples = this->tempBuffer.getArray();
                    
                    tempBufferSamples[0] = interpolationValue;
                    
                    for (i = 0; i < inputBufferLength; ++i)
                        tempBufferSamples[i + 1] = inputSamples[i];
                }
                            
                while ((outputSamples < outputSamplesEnd) && (tempBufferPos < tempBufferPosMax))
                {
                    *outputSamples++ = plonk::lookup (tempBufferSamples, tempBufferPos);
                    tempBufferPos += tempBufferIncrement;
                }
            }
            
            info.setTimeStamp (infoTimeStamp); // reset for the parent graph
        }
    }
    
private:
    Buffer tempBuffer;
    IndexType tempBufferPos;
    IndexType tempBufferPosMax;
    TimeStamp nextInputTimeStamp;
};

//------------------------------------------------------------------------------

/** Resample a unit to a different sample rate and/or block size.
 @ingroup ConverterUnits */
template<class SampleType>
class ResampleUnit
{
public:    
    typedef ResampleChannelInternal<SampleType>     ResampleInternal;
    typedef typename ResampleInternal::Data         Data;
    typedef ChannelBase<SampleType>                 ChannelType;
    typedef ChannelInternal<SampleType,Data>        Internal;
    typedef UnitBase<SampleType>                    UnitType;
    typedef InputDictionary                         Inputs;    

    static inline UnitInfos getInfo() throw()
    {
        const double blockSize = (double)BlockSize::getDefault().getValue();
        const double sampleRate = SampleRate::getDefault().getValue();

        return UnitInfo ("Resample", "Resamples signals in terms of block size and/or sample rate.",
                         
                         // output
                         ChannelCount::VariableChannelCount, 
                         IOKey::Signal,     Measure::None,     IOInfo::NoDefault,   IOLimit::None,      IOKey::End,

                         // inputs
                         IOKey::Signal,     Measure::None,     IOInfo::NoDefault,   IOLimit::None,
                         IOKey::BlockSize,  Measure::Samples,  blockSize,           IOLimit::Minimum,   Measure::Samples,   1.0,
                         IOKey::SampleRate, Measure::Hertz,    sampleRate,          IOLimit::Minimum,   Measure::Hertz,     0.0,
                         IOKey::End);
    }    
    
    /** Create an audio rate sample rate converter. */
    static UnitType ar (UnitType const& input, 
                        BlockSize const& preferredBlockSize = BlockSize::getDefault(),
                        SampleRate const& preferredSampleRate = SampleRate::getDefault()) throw()
    {                
        plonk_assert (preferredSampleRate.getValue() > 0.0); // no  need to resample a DC signal
                
        Inputs inputs;
        inputs.put (IOKey::Signal, input);
        
        Data data = { -1.0, -1.0 };
                
        return UnitType::template createFromInputs<ResampleInternal> (inputs, 
                                                                      data, 
                                                                      preferredBlockSize, 
                                                                      preferredSampleRate);
    }
    
    static inline UnitType kr (UnitType const& input) throw()
    {
        return ar (input, 
                   BlockSize::getControlRateBlockSize(), 
                   SampleRate::getControlRate());
    }
    
};

typedef ResampleUnit<PLONK_TYPE_DEFAULT> Resample;
//typedef Resample Reblock;



#endif // PLONK_BLOCKRESIZECHANNEL_H

