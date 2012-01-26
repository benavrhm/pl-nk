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

#ifndef PLONK_INLINEMISCOPS_H
#define PLONK_INLINEMISCOPS_H

#include "plonk_Constants.h"
#include "plonk_InlineUnaryOps.h"

#define TERNARYOPFUNCTION(TYPE,NAME)       TYPE (*NAME)(TYPE,TYPE,TYPE)
#define QUATERNARYOPFUNCTION(TYPE,NAME)    TYPE (*NAME)(TYPE,TYPE,TYPE,TYPE)
#define QUINARYOPFUNCTION(TYPE,NAME)       TYPE (*NAME)(TYPE,TYPE,TYPE,TYPE,TYPE)

//------------------------------------------------------------------------------

/** Miscellaneous arithmetic functions.
 
 @defgroup PlonkMiscArithmeticFunctions Miscellaneous arithmetic functions 
 @ingroup PlonkOtherUserFunctions
 @{
 */

template<class Type> inline Type clip(Type value, Type lower, Type upper) throw() { return min (max (lower, value), upper); }

inline int wrap (int value, int lower, int upper) throw()
{
	int range;
    
	if (value >= upper) 
	{
		range = upper - lower;
		value -= range;
        
		if (value < upper) 
            return value;
	} 
	else if (value < lower) 
	{
		range = upper - lower;
		value += range;
        
		if (value >= lower) 
            return value;
	} 
	else
    {
        return value;
	}
    
	if (upper == lower) 
        return lower;
	else
        return value % range + lower;
}

template<class Type>
inline Type wrap (Type value, Type lower, Type upper) throw()
{
	Type range;
    
	if (value >= upper) 
	{
		range = upper - lower;
		value -= range;
        
		if (value < upper) 
            return value;
	} 
	else if (value < lower) 
	{
		range = upper - lower;
		value += range;
        
		if (value >= lower) 
            return value;
	} 
	else
    {
        return value;
	}
    
	if (upper == lower) 
        return lower;
    else
        return value - range * floor ((value - lower) / range); 
}



template<class Type>
inline Type linlin(Type input, 
                   Type inLow, Type inHigh,
                   Type outLow, Type outHigh) throw()
{
	Type inRange = inHigh - inLow;
	Type outRange = outHigh - outLow;
	return (input - inLow) * outRange / inRange + outLow;
}

template<class Type>
inline Type linlin2(Type input, 
                    Type inLow, Type inRange,
                    Type outLow, Type outRange) throw()
{
	return (input - inLow) * outRange / inRange + outLow;
}

template<class Type>
inline Type linsin(Type input, 
                   Type inLow, Type inHigh,
                   Type outLow, Type outHigh) throw()
{
	Type inRange = inHigh - inLow;
	Type outRange = outHigh - outLow;
    
	Type inPhase = (input - inLow) * Math<Type>::getPi() / inRange + Math<Type>::getPi();
	Type cosInPhase = plonk::cos (inPhase) * Math<Type>::get0_5() + Math<Type>::get0_5();
	
	return cosInPhase * outRange + outLow;	
}

template<class Type>
inline Type linsin2(Type input, 
                    Type inLow, Type inRange,
                    Type outLow, Type outRange) throw()
{	
	Type inPhase = (input - inLow) * Math<Type>::getPi() / inRange + Math<Type>::getPi();
	Type cosInPhase = plonk::cos (inPhase) * Math<Type>::get0_5() + Math<Type>::get0_5();
	return cosInPhase * outRange + outLow;	
}

template<class Type>
inline Type linexp(Type input, 
                   Type inLow, Type inHigh,
                   Type outLow, Type outHigh) throw()
{
	Type outRatio = outHigh / outLow;
	Type reciprocalInRange = Type (1) / (inHigh - inLow);
	Type inLowOverInRange = reciprocalInRange * inLow;
	return outLow * plonk::pow (outRatio, input * reciprocalInRange - inLowOverInRange);	
}

template<class Type>
inline Type linexp2(Type input, 
                    Type reciprocalInRange, Type inLowOverInRange,
                    Type outLow, Type outRatio) throw()
{
	return outLow * plonk::pow (outRatio, input * reciprocalInRange - inLowOverInRange);	
}

template<class Type>
inline Type linwelch (Type input, 
                      Type inLow, Type inHigh,
                      Type outLow, Type outHigh) throw()
{
	Type inRange = inHigh - inLow;
	Type outRange = outHigh - outLow;
	Type inPos = (input - inLow) / inRange;
    
	if (outLow < outHigh)
		return outLow + outRange * plonk::sin (Math<Type>::getPi_2() * inPos);
	else
		return outHigh - outRange * plonk::sin (Math<Type>::getPi_2() - Math<Type>::getPi_2() * inPos);
}

template<class Type>
inline Type explin (Type input, Type inLow, Type inHigh, Type outLow, Type outHigh)
{
    if (input <= inLow) 
        return outLow;
    
    if (input >= inHigh) 
        return outHigh;
    
    return plonk::log (input / inLow) / plonk::log (inHigh / inLow) * (outHigh - outLow) + outLow;
}

template<class ValueType, class IndexType>
inline ValueType lininterp (const ValueType value0, const ValueType value1, const IndexType frac) throw()
{
    return value0 + ValueType (frac * (value1 - value0));
}

template<class ValueType, class IndexType>
inline ValueType lookup (const ValueType* table, const IndexType index) throw()
{
    const int index0 = int (index);
    const int index1 = index0 + 1;
    const IndexType frac = index - IndexType (index0);
    return lininterp (table[index0], table[index1], frac);
}

/// @}

#endif // PLONK_INLINEMISCOPS_H