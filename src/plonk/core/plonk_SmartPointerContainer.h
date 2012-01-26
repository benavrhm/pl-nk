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

#ifndef PLONK_SMARTPOINTERCONTAINER_H
#define PLONK_SMARTPOINTERCONTAINER_H

#include "plonk_SmartPointer.h"
#include "plonk_WeakPointer.h"


/** This manages a SmartPointer pointer. 
 This class and its subclasses are passed around by value (or reference) and 
 if a copy is made it actually just copies the SmartPointer it contains 
 incrementing its reference count. As SmartPointerContainer objects are deleted 
 the reference count of its contained SmartPointer are decremented (thus 
 deleting them only if the reference count reaches zero). Almost all other 
 Plonk concepts (i.e., arrays, channels, units) comprise an 'internal' 
 (which is a subclass of SmartPointer) and 'external' (which is a subclass of 
 SmartPointerContainer) representation. 
 @see SenderContainer */
//template<class SmartPointerType>
//class SmartPointerContainerBase
//{
//public:         
//    typedef SmartPointerType Internal;
//    
//	SmartPointerContainerBase (SmartPointerType* internalToUse = 0) throw() 
//	:	internal (internalToUse)
//	{
//        if (internal.getPtr() != 0)
//            internal.getPtr()->incrementRefCount();
//	}
//    
//	~SmartPointerContainerBase()
//	{        
//		if (internal.getPtr() != 0) 
//			internal.getPtr()->decrementRefCount();
//		
//		internal.setPtr (0);        
//	}
//    
//	inline SmartPointerType* getInternal() throw() 
//	{
//		return internal.getPtr();
//	}
//	
//	inline SmartPointerType* operator->() throw() 
//	{
//		return internal.getPtr();
//	}
//	
//	inline const SmartPointerType* getInternal() const throw() 
//	{
//		return internal.getPtr();
//	}
//    
//	inline const SmartPointerType* operator->() const throw() 
//	{
//		return internal.getPtr();
//	}
//    
//    SmartPointerContainerBase containerCopy() const throw()
//    {
//        return *this;
//    }
//    
//	bool isNull() const throw()
//	{
//		return internal.getPtr() == 0;
//	}
//	
//	bool isNotNull() const throw()
//	{
//		return internal.getPtr() != 0;
//	}
//	
//	inline void setInternal (SmartPointerType* newInternal) throw()
//	{
//        if (newInternal != 0)
//            newInternal->incrementRefCount();
//        
//        if (internal != 0)
//            internal.getPtr()->decrementRefCount();
//        
//        internal.setPtr (newInternal);        
//	}
//    
//    void swapWith (SmartPointerContainerBase const& other) throw()
//    {
//        const Internal* const otherInternal = other.internal.getPtr();
//        other.internal.setPtr (internal.getPtr());
//        internal.setPtr (otherInternal);
//    }
//	
//	SmartPointerContainerBase (SmartPointerContainerBase const& copy) throw()
//	:	internal (copy.internal)
//	{
//		if (internal != 0) 
//            internal.getPtr()->incrementRefCount();
//	}
//    
//	SmartPointerContainerBase& operator= (SmartPointerContainerBase const& other) throw()
//	{
//		if (this != &other)
//            this->setInternal (other.internal);
//		
//		return *this;		
//	}    
//    
//	bool operator== (SmartPointerContainerBase const& other) const throw()
//	{
//		return internal.getPtr() == other.internal.getPtr();
//	}
//	
//	bool operator!= (SmartPointerContainerBase const& other) const throw()
//	{
//		return internal.getPtr() != other.internal.getPtr();
//	}
//    
//private:
//	AtomicExtended<SmartPointerType*> internal;
//};

template<class SmartPointerType>
class SmartPointerContainerBase
{
public:         
    typedef SmartPointerType Internal;

	SmartPointerContainerBase (SmartPointerType* internalToUse = 0) throw() 
	:	internal (internalToUse)
	{
        if (internal != 0)
            internal->incrementRefCount();
	}
    	
	~SmartPointerContainerBase()
	{        
		if (internal != 0) 
			internal->decrementRefCount();
		
		internal = 0;        
	}
    	
	inline SmartPointerType* getInternal() throw() 
	{
		return internal;
	}
	
	inline SmartPointerType* operator->() throw() 
	{
		return internal;
	}
	
	inline const SmartPointerType* getInternal() const throw() 
	{
		return internal;
	}
    	
	inline const SmartPointerType* operator->() const throw() 
	{
		return internal;
	}
    
    SmartPointerContainerBase containerCopy() const throw()
    {
        return *this;
    }

	bool isNull() const throw()
	{
		return internal == 0;
	}
	
	bool isNotNull() const throw()
	{
		return internal != 0;
	}
	
	inline void setInternal (SmartPointerType* newInternal) throw()
	{
        if (newInternal != 0)
            newInternal->incrementRefCount();
        
        if (internal != 0)
            internal->decrementRefCount();
        
        internal = newInternal;        
	}
    
    void swapWith (SmartPointerContainerBase const& other) throw()
    {
        const Internal* const otherInternal = other.internal;
        other.internal = internal;
        internal = otherInternal;
    }
	
	SmartPointerContainerBase (SmartPointerContainerBase const& copy) throw()
	:	internal (copy.internal)
	{
		if (internal != 0) 
            internal->incrementRefCount();
	}
    
	SmartPointerContainerBase& operator= (SmartPointerContainerBase const& other) throw()
	{
		if (this != &other)
            this->setInternal (other.internal);
		
		return *this;		
	}    
    
	bool operator== (SmartPointerContainerBase const& other) const throw()
	{
		return internal == other.internal;
	}
	
	bool operator!= (SmartPointerContainerBase const& other) const throw()
	{
		return internal != other.internal;
	}
    
	
private:
	SmartPointerType* internal;
};


template<class SmartPointerType, bool enableWeak>
class SmartPointerContainer : public SmartPointerContainerBase<SmartPointerType>
{
public:
    SmartPointerContainer (SmartPointerType* internalToUse = 0) throw() 
	:	SmartPointerContainerBase<SmartPointerType> (internalToUse)
	{
	}
    
    SmartPointerContainer (SmartPointerContainer const& copy) throw()
	:	SmartPointerContainerBase<SmartPointerType> (copy)
	{
	}
    
};

template<class SmartPointerType>
class SmartPointerContainer<SmartPointerType, true> : public SmartPointerContainerBase<SmartPointerType>
{
public:    
    SmartPointerContainer (SmartPointerType* internalToUse = 0) throw() 
	:	SmartPointerContainerBase<SmartPointerType> (internalToUse)
	{
	}
    
    SmartPointerContainer (SmartPointerContainer const& copy) throw()
	:	SmartPointerContainerBase<SmartPointerType> (copy)
	{
	}    
    
    WeakPointer* getWeakPointer() const throw()
    {
        const SmartPointerType* p = this->getInternal();
            
        if (p != 0)
        {
            // todo: should this be static_cast?
            WeakPointer* w = reinterpret_cast<WeakPointer*> (p->getWeak());
            
            return w;
        }
        
        return 0;
    }
    
};


template<class SmartPointerContainerType>
void swap (SmartPointerContainerType const& a, 
           SmartPointerContainerType const& b) throw()
{
    a.swapWith (b);
}


#endif // PLONK_SMARTPOINTERCONTAINER_H