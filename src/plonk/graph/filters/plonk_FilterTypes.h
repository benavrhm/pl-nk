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

#ifndef PLONK_FILTERTYPES_H
#define PLONK_FILTERTYPES_H

/*
 
 P1
    LPF
    Lag
 
 P1Z1 (see PoleZero in STK)
    LeakDC
    Allpass
  
 P2Z2
    LPF f, q
    HPF f, q
    BPF f, bw
    BRF f, bw
    Notch f, q, g
    LS f, s, g
    HS f, s, g
    AP f, q
 */

class FilterFormType
{
public:
    enum Name
    {
        Unknown = 0,
        P1,
        P1a,
        P2,
        Z1,
        Z2,
        P1Z1,
        P2Z2,
        B2,
        NumNames
    };
    
    static FilterFormType::Name fromInt (const int value) throw();
    static Text getName (const int index) throw();
};

//------------------------------------------------------------------------------


class FilterShapeType
{
public:
    enum Name
    {
        Unknown = 0,
        LPF,
        HPF,
        BPF,
        BRF,
        LowShelf,
        HighShelf,
        Notch,
        Allpass,
        Other,
        NumNames
    };
   
    static FilterShapeType::Name fromInt (const int value) throw();
    static Text getName (const int index) throw();
};




#endif // PLONK_FILTERTYPES_H