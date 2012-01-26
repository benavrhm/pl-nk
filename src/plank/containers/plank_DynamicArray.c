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

#include "../core/plank_StandardHeader.h"
#include "plank_DynamicArray.h"
#include "../maths/plank_Maths.h"

PlankDynamicArrayRef pl_DynamicArray_CreateAndInit()
{
    PlankDynamicArrayRef p;
    p = pl_DynamicArray_Create();
    
    if (p != PLANK_NULL)
    {
        if (pl_DynamicArray_Init (p) != PlankResult_OK)
            pl_DynamicArray_Destroy (p);
        else
            return p;
    }
    
    return PLANK_NULL;
}

PlankDynamicArrayRef pl_DynamicArray_Create()
{
    PlankMemoryRef m;
    PlankDynamicArrayRef p;
    
    m = pl_MemoryGlobal();
    p = (PlankDynamicArrayRef)pl_Memory_AllocateBytes (m, sizeof (PlankDynamicArray));
    
    if (p != PLANK_NULL)
        pl_MemoryZero (p, sizeof (PlankDynamicArray));
    
    return p;    
}

PlankResult pl_DynamicArray_Init (PlankDynamicArrayRef p)
{
    return pl_DynamicArray_InitWithItemSize (p, PLANKDYNAMICARRAY_DEFAULTITEMSIZE);
}

PlankResult pl_DynamicArray_InitWithItemSize (PlankDynamicArrayRef p, const PlankLL itemSize)
{
    return pl_DynamicArray_InitWithItemSizeAndCapacity (p, itemSize, PLANKDYNAMICARRAY_DEFAULTGRANULARITY);
}

PlankResult pl_DynamicArray_InitWithItemSizeAndCapacity (PlankDynamicArrayRef p, const PlankLL itemSize, const PlankLL initialCapacity)
{
    PlankResult result = PlankResult_OK;
    PlankMemoryRef m;

    p->itemSize = itemSize;
    p->allocatedItems = initialCapacity;
    p->usedItems = 0;
    
    m = pl_MemoryGlobal();
    p->data = pl_Memory_AllocateBytes (m, p->itemSize * p->allocatedItems);
    
    if (p->data == PLANK_NULL)
    {
        result = PlankResult_MemoryError;
        goto exit;
    }
    
exit:
    return result;    
}

PlankResult pl_DynamicArray_DeInit (PlankDynamicArrayRef p)
{
    PlankResult result = PlankResult_OK;
    PlankMemoryRef m = pl_MemoryGlobal();

    if (p == PLANK_NULL)
    {
        result = PlankResult_MemoryError;
        goto exit;
    }
    
    if (p->data)
    {
        if ((result = pl_Memory_Free (m, p->data)) != PlankResult_OK)
            goto exit;
    }
    
exit:
    return result;
}

PlankResult pl_DynamicArray_Destroy (PlankDynamicArrayRef p)
{
    PlankResult result = PlankResult_OK;
    PlankMemoryRef m = pl_MemoryGlobal();
    
    if (p == PLANK_NULL)
    {
        result = PlankResult_MemoryError;
        goto exit;
    }
    
    if ((result = pl_DynamicArray_DeInit (p)) != PlankResult_OK)
        goto exit;
        
    result = pl_Memory_Free (m, p);
    
exit:
    return result;
}

PlankB pl_DynamicArray_IsIndexInRange (PlankDynamicArrayRef p, const PlankLL index)
{
    return (index >= 0) && (index < p->usedItems);
}

PlankLL pl_DynamicArray_GetSize (PlankDynamicArrayRef p)
{
    return p->usedItems;
}

PlankResult pl_DynamicArray_AddItem (PlankDynamicArrayRef p, const PlankP item)
{
    PlankResult result = PlankResult_OK;
    PlankLL index;
    
    index = p->usedItems;
    p->usedItems++;
    
    if (p->usedItems > p->allocatedItems)
    {        
        result = pl_DynamicArray_Grow (p, PLANKDYNAMICARRAY_DEFAULTGRANULARITY);
        
        if (result != PlankResult_OK)
            goto exit;
    }
    
    memcpy (p->data + index * p->itemSize, item, p->itemSize);
    
exit:
    return result;
}

PlankResult pl_DynamicArray_SetItem (PlankDynamicArrayRef p, const PlankLL index, const PlankP item)
{
    PlankResult result = PlankResult_OK;
    
    if ((index < 0) || (index >= p->usedItems))
    {
        result = PlankResult_ArrayParameterError;
        goto exit;
    }
    
    memcpy (p->data + index * p->itemSize, item, p->itemSize);
    
exit:
    return result;        
}

PlankResult pl_DynamicArray_InsertItem (PlankDynamicArrayRef p, const PlankLL index, const PlankP item)
{
    PlankResult result = PlankResult_OK;
    PlankP src, dst;
    PlankLL size;
    
    if (index < 0)
    {
        result = PlankResult_ArrayParameterError;
        goto exit;
    }
    
    if ((result = pl_DynamicArray_EnsureSize (p, p->usedItems + 1)) != PlankResult_OK) goto exit;
    
    if (index < p->usedItems)
    {
        src = p->data + index * p->itemSize;
        dst = p->data + (index + 1) * p->itemSize;
        size = p->itemSize * (p->usedItems - index);
        memcpy (dst, src, size);
    
        dst = src;
        src = item;
        size = p->itemSize;
        memcpy (dst, src, size);
    }
    else
    {
        dst = p->data + p->usedItems * p->itemSize;
        src = item;
        size = p->itemSize;
        memcpy (dst, src, size);        
    }
    
    p->usedItems++;
    
exit:
    return result;
}

PlankResult pl_DynamicArray_GetItem (PlankDynamicArrayRef p, const PlankLL index, PlankP item)
{
    PlankResult result = PlankResult_OK;

    if ((index < 0) || (index >= p->usedItems))
    {
        result = PlankResult_ArrayParameterError;
        goto exit;
    }
    
    memcpy (item, p->data + index * p->itemSize, p->itemSize);
    
exit:
    return result;    
}

PlankResult pl_DynamicArray_RemoveItem (PlankDynamicArrayRef p, const PlankLL index)
{
    PlankResult result = PlankResult_OK;
    PlankP src, dst;
    PlankLL size;
    
    if ((index < 0) || (index >= p->usedItems))
    {
        result = PlankResult_ArrayParameterError;
        goto exit;
    }
    
    p->usedItems--;
    
    if (p->usedItems > 0)
    {
        dst = p->data + index * p->itemSize;
        src = p->data + (index + 1) * p->itemSize;
        size = p->itemSize * (p->usedItems - index);
        memcpy (dst, src, size);
    }
    
exit:
    return result;            
}

PlankResult pl_DynamicArray_EnsureSize (PlankDynamicArrayRef p, const PlankLL capacity)
{
    PlankResult result = PlankResult_OK;
    PlankMemoryRef m;
    PlankLL newAllocatedItems;
    PlankP newData;
    
    if (capacity <= p->allocatedItems)
        goto exit;
    
    m = pl_MemoryGlobal();
    
    newAllocatedItems = pl_MaxLL (capacity, p->allocatedItems + PLANKDYNAMICARRAY_DEFAULTGRANULARITY);
    newData = pl_Memory_AllocateBytes (m, p->itemSize * newAllocatedItems);
    
    if (newData == PLANK_NULL)
    {
        result = PlankResult_MemoryError;
        goto exit;
    }
    
    memcpy (newData, p->data, p->itemSize * p->allocatedItems);
    
    pl_Memory_Free (m, p->data);
    p->data = newData;
    p->allocatedItems = newAllocatedItems;
    
exit:
    return result;    
}

PlankResult pl_DynamicArray_Grow (PlankDynamicArrayRef p, const int amount)
{
    PlankResult result = PlankResult_OK;

    if (amount < 1)
    {
        result = PlankResult_ArrayParameterError;
        goto exit;
    }

    result = pl_DynamicArray_EnsureSize (p, p->allocatedItems + amount);
    
exit:
    return result;
}

PlankResult pl_DynamicArray_Purge (PlankDynamicArrayRef p)
{
    PlankResult result = PlankResult_OK;
    PlankMemoryRef m;
    PlankLL newAllocatedItems;
    PlankP newData;
    
    if (p->usedItems == p->allocatedItems)
        goto exit;
    
    m = pl_MemoryGlobal();
    
    newAllocatedItems = p->usedItems;
    newData = pl_Memory_AllocateBytes (m, p->itemSize * newAllocatedItems);
    
    if (newData == PLANK_NULL)
    {
        result = PlankResult_MemoryError;
        goto exit;
    }
    
    memcpy (newData, p->data, p->itemSize * p->usedItems);
    
    pl_Memory_Free (m, p->data);
    p->data = newData;
    p->allocatedItems = newAllocatedItems;
    
exit:
    return result;    
}

