/*
Copyright (c) 2011 Chris Lentini
http://divergentcoder.com

Permission is hereby granted, free of charge, to any person obtaining a copy of 
this software and associated documentation files (the "Software"), to deal in 
the Software without restriction, including without limitation the rights to use, 
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the 
Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS 
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER 
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once
#include "BasicTypes.h"
#include "Op.h"
#include "Util.h"

///////////////////////////////////////////////////////////////////////////////
template<class N, class D> 
struct Plane
{
	typedef N Normal;
	typedef D Distance;
};
///////////////////////////////////////////////////////////////////////////////
template<bool DO, class NUMER, class DENOM> 
struct CalcPlaneHit
{
	typedef HitResult<Missed,MaxFixed> Result;
};

template<class NUMER, class DENOM>
struct CalcPlaneHit<true, NUMER, DENOM>
{
	typedef typename Div<NUMER, DENOM>::Result HitDist;
	static const bool CND = (HitDist::Value < 0);
	typedef If<CND, HitResult<Missed, MaxFixed>,
		HitResult<Hit, HitDist> > Result;
};

template<class ORIGIN, class DIR, class NORM, class DIST>
struct IntersectPlane
{
	typedef typename Dot<DIR,NORM>::Result DN;
	typedef typename Dot<ORIGIN,NORM>::Result ON;
	typedef typename Sub<Fixed<0>, typename Add<ON, DIST>::Result>::Result NUMER;
	
	static const bool CND = DN::Value < 0;

	typedef typename CalcPlaneHit<CND, NUMER, DN>::Result intersection;
};

template<class R, class P> struct Intersect;

template<class ORIGIN, class DIR, class NORM, class DIST>
struct Intersect<Ray<ORIGIN,DIR>, Plane<NORM,DIST> > : 
	public IntersectPlane<ORIGIN,DIR,NORM,DIST>::intersection
{
private:
	typedef typename IntersectPlane<ORIGIN,DIR,NORM,DIST>::intersection Base;

public:
	typedef typename CalcPosition<Base::Type == Hit, ORIGIN, DIR, Base>::Result Position;
	typedef NORM Normal;
	typedef DIR View;
};
///////////////////////////////////////////////////////////////////////////////
