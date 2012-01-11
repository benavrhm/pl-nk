/*
 -------------------------------------------------------------------------------
 This file is part of the Plink, Plonk, Plank libraries
  by Martin Robinson
 
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
 -------------------------------------------------------------------------------
 */

#ifndef PLONK_CHANNELDATA_H
#define PLONK_CHANNELDATA_H

/** Base class for channel state data */
template<class OwnerType>
struct ChannelData
{
    double sampleRate;
    double sampleDuration;
    
    static ChannelData create (double initSampleRate) 
    { 
        ChannelData data = { initSampleRate, 1.0 / initSampleRate };
        return data;
    }     
};

/** Macro to make typing this hideous construction more bearable. */
#define CHANNELDATA_NAME(CHANNELTYPE,SAMPLETYPE) ChannelData<CHANNELTYPE<SAMPLETYPE> >

/** Macro to make typing this hideous construction more bearable. */
#define CHANNELDATA_DECLARE(CHANNELTYPE,SAMPLETYPE)\
    template<class SAMPLETYPE> struct CHANNELDATA_NAME(CHANNELTYPE,SAMPLETYPE)

#define CHANNELDATA_SPECIAL(CHANNELTYPE,SAMPLETYPE)\
    template<> struct CHANNELDATA_NAME(CHANNELTYPE,SAMPLETYPE)



#endif // PLONK_CHANNELDATA_H