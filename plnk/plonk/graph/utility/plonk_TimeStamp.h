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

#ifndef PLONK_TIMESTAMP_H
#define PLONK_TIMESTAMP_H

/** Measures time.
 This uses 128 bits (64 in an @c int and 64 in a @c double) to store, measure and
 timestamp blocks and samples. It is used mainly to idenfiy the start time of
 blocks whcih channels use to determine if they need to render yet. 
 @see ProcessInfo */
class TimeStamp
{
public:
    TimeStamp (int initialTime) throw();
    TimeStamp (LongLong initialTime) throw();
    TimeStamp (LongLong initialTime = 0, double initialFraction = 0.0) throw();
    
    TimeStamp (TimeStamp const& other) throw();
    TimeStamp& operator= (TimeStamp const& other) throw();
    
    bool isFinite() const throw();
    bool isInfinite() const throw();
    
    TimeStamp& operator+= (const double offset) throw();
    TimeStamp& operator+= (TimeStamp const& other) throw();
    TimeStamp operator+ (const double offset) const throw();
    TimeStamp operator+ (TimeStamp const& other) const throw();
    
    TimeStamp& operator-= (const double offset) throw();
    TimeStamp& operator-= (TimeStamp const& other) throw();
    TimeStamp operator- (const double offset) const throw();
    TimeStamp operator- (TimeStamp const& other) const throw();
    
    
    void set (LongLong newTime, double newFraction = 0.0) throw();
    PLONK_INLINE_LOW LongLong getTime() const throw() { return time; }
    PLONK_INLINE_LOW double getFraction() const throw() { return fraction; }
    PLONK_INLINE_LOW int getFractionInTicks() const throw() { return int (fraction * getTicks()); }
    
    PLONK_INLINE_LOW double getValue() const throw() 
    { 
        plonk_assert (((double (time) + 1.0) - (double (time))) == 1.0); // lost precision
        return double (time) + fraction; 
    }
    
    PLONK_INLINE_LOW operator double () const throw() { return this->getValue(); }
    PLONK_INLINE_LOW double toSeconds() const throw() { return this->getValue() * getReciprocalTicks(); }
    double toSamples (const double sampleRate) const throw();
    
    bool operator== (TimeStamp const& other) const throw();
    bool operator!= (TimeStamp const& other) const throw();
    
    bool operator<  (TimeStamp const& other) const throw();
    bool operator<= (TimeStamp const& other) const throw();
    bool operator>  (TimeStamp const& other) const throw();
    bool operator>= (TimeStamp const& other) const throw();
    
    static double getTicks() throw();
    static double getReciprocalTicks() throw();
    
    static TimeStamp fromSeconds (const double seconds) throw();
    static TimeStamp fromSamples (const double samples, const double sampleRate) throw();
    static const TimeStamp& getSentinel() throw();
    static const TimeStamp& getZero() throw();
    static const TimeStamp& getMaximum() throw();

private:
    LongLong time;
    double fraction;
    
    static bool fractionIsValid (const double f) throw();
};



#endif // PLONK_TIMESTAMP_H