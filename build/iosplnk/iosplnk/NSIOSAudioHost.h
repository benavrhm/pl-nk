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

#ifndef _UIKitAUIOHost_H_
#define _UIKitAUIOHost_H_

#include <AudioToolbox/AudioQueue.h>
#include <AudioToolbox/AudioServices.h>
#include <AudioToolbox/AudioToolbox.h>
#include <AudioUnit/AudioUnit.h>

#include "plonk/plonk.h"

@interface NSIOSAudioHost : NSObject 
{
	AudioStreamBasicDescription format;
	AURenderCallbackStruct		inputProc;
	Float64						hwSampleRate;
	AudioUnit					rioUnit;

    int							bufferSize;
	float						bufferDuration;
	float						reciprocalBufferDuration;
	float						*floatBuffer;
	UInt32						audioInputIsAvailable;
	UInt32						numInputChannels;
	UInt32						numOutputChannels;
	bool						isRunning;

	int							preferredBufferSize;
    double                      preferredSampleRate;
	float						cpuUsage;
    
    UInt32                      audioCategory;
    
    ProcessInfo     info;
    Unit            outputUnit;  
    Busses          busses;
    Lock            lock;
}

@property UInt32 audioCategory;
@property int preferredBufferSize;
@property double preferredSampleRate;


- (void)start;
- (void)restart;
- (id)initAudio;
- (void)setFormat;
- (int)setupRemoteIO;
- (OSStatus)renderCallback:(UInt32)inNumberFrames 
		   withActionFlags:(AudioUnitRenderActionFlags*)ioActionFlags
			   atTimeStamp:(const AudioTimeStamp*)inTimeStamp 
			   withBuffers:(AudioBufferList*)ioData;
- (void)propertyCallback:(AudioSessionPropertyID)inID
				withSize:(UInt32)inDataSize	
			withProperty:(const void *)inPropertyValue;
- (void)fixAudioRouteIfSetToReceiver;
- (void)interruptionCallback:(UInt32)inInterruption;

- (Unit)constructGraph;
- (float)getCpuUsage;

- (void)lock;
- (void)unlock;
- (BOOL)tryLock;

- (void)isAboutToShutdown;
- (void)shutdown;

@end




#endif // _UIKitAUIOHost_H_
