/*
 -------------------------------------------------------------------------------
 This file is part of the Plink, Plonk, Plank libraries
  by Martin Robinson
 
 http://code.google.com/p/pl-nk/
 
 Copyright University of the West of England, Bristol 2011-15
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

#ifndef PLANK_IFFFILEWRITER_H
#define PLANK_IFFFILEWRITER_H

#include "plank_IffFileCommon.h"
#include "plank_File.h"

#define PLANKIFFFILEWRITER_MODEREPLACEGROW      0
#define PLANKIFFFILEWRITER_MODEREPLACESHRINK    1
#define PLANKIFFFILEWRITER_MODEAPPEND           2

PLANK_BEGIN_C_LINKAGE

/** A generic IFF/RIFF file writer helper.
  
 @defgroup PlankIffFileWriterClass Plank IffFileWriter class
 @ingroup PlankClasses
 @{
 */

/** An opaque reference to the <i>Plank IffFileWriter</i> object. */
typedef struct PlankIffFileWriter* PlankIffFileWriterRef; 
typedef struct PlankIffFileWriterChunkInfo* PlankIffFileWriterChunkInfoRef;
typedef int PlankIffFileWriterMode;

/** Create and initialise a <i>Plank IffFileWriter</i> object and return an oqaque reference to it.
 @return A <i>Plank IffFileWriter</i> object as an opaque reference or PLANK_NULL. */
PlankIffFileWriterRef pl_IffFileWriter_CreateAndInit();

/** Create a <i>Plank IffFileWriter</i> object and return an oqaque reference to it.
 @return A <i>Plank IffFileWriter</i> object as an opaque reference or PLANK_NULL. */
PlankIffFileWriterRef pl_IffFileWriter_Create();

/** Initialise a <i>Plank IffFileWriter</i> object. 
 @param p The <i>Plank IffFileWriter</i> object. 
 @return A result code which will be PlankResult_OK if the operation was completely successful. */
PlankResult pl_IffFileWriter_Init (PlankIffFileWriterRef p);

/** Deinitialise a <i>Plank IffFileWriter</i> object. 
 @param p The <i>Plank IffFileWriter</i> object. 
 @return A result code which will be PlankResult_OK if the operation was completely successful. */
PlankResult pl_IffFileWriter_DeInit (PlankIffFileWriterRef p);

/** Destroy a <i>Plank IffFileWriter</i> object. 
 @param p The <i>Plank IffFileWriter</i> object. 
 @return A result code which will be PlankResult_OK if the operation was completely successful. */
PlankResult pl_IffFileWriter_Destroy (PlankIffFileWriterRef p);

/** Gets the underlying <i>Plank %File</i> object. 
 This is the raw file object that is performing the fundamental file access operations.
 @param p The <i>Plank IffFileWriter</i> object.
 @return The <i>Plank %File</i> object. */
PlankFileRef pl_IffFileWriter_GetFile (PlankIffFileWriterRef p);

/** */
PlankResult pl_IffFileWriter_OpenReplacing (PlankIffFileWriterRef p, 
                                            const char* filepath, 
                                            const PlankB bigEndian, 
                                            const char* mainID, 
                                            const char* formatID,
                                            const int idType);

PlankResult pl_IffFileWriter_OpenWithFile (PlankIffFileWriterRef p,
                                           PlankFileRef file,
                                           const char* mainID,
                                           const char* formatID,
                                           const int idType);

/** */
PlankResult pl_IffFileWriter_Close (PlankIffFileWriterRef p);
PlankResult pl_IffFileWriter_WriteHeader (PlankIffFileWriterRef p);
PlankResult pl_IffFileWriter_SeekChunk (PlankIffFileWriterRef p, const PlankLL startPosition, const char* chunkID, PlankIffFileWriterChunkInfoRef* chunkInfo, PlankB* isLastChunk);
PlankResult pl_IffFileWriter_WriteChunk (PlankIffFileWriterRef p, const PlankLL startPosition, const char* chunkID, const void* data, const PlankUI dataLength, const PlankIffFileWriterMode mode);
PlankResult pl_IffFileWriter_RenameChunk (PlankIffFileWriterRef p, const PlankLL startPosition, const char* oldChunkID, const char* newChunkID);
PlankResult pl_IffFileWriter_ResizeChunk (PlankIffFileWriterRef p, const PlankLL startPosition, const char* chunkID, const PlankLL newLength, const PlankB concatenateJunk);
PlankResult pl_IffFileWriter_PrepareChunk (PlankIffFileWriterRef p, const PlankLL startPosition, const char* chunkID, const void* data, const PlankUI dataLength);
PlankResult pl_IffFileWriter_PurgeChunkInfos (PlankIffFileWriterRef p);
PlankIffFileWriterChunkInfoRef pl_IffFileWriter_GetCurrentChunk (PlankIffFileWriterRef p);


/** @} */

PLANK_END_C_LINKAGE

#if !DOXYGEN
typedef struct PlankIffFileWriterChunkInfo
{
    PlankIffID chunkID;
    PlankLL chunkLength;
    PlankLL chunkPos;
} PlankIffFileWriterChunkInfo;

typedef struct PlankIffFileWriter
{
    PlankIffFile common;
    
    PlankDynamicArray chunkInfos;
    PlankIffFileWriterChunkInfo* currentChunk;
    
} PlankIffFileWriter;
#endif

//------------------------------------------------------------------------------



#endif // PLANK_IFFFILEWRITER_H
