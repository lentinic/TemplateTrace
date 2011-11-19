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
template<class P, class R> struct Sphere
{
	typedef P Position;
	typedef R Radius;
};
///////////////////////////////////////////////////////////////////////////////
template<class D2, class B>
struct CalcSphereHit
{
private:
	typedef typename Sqrt<D2>::Result		Root;
	typedef typename Sub<B,Root>::Result	I0;
	typedef typename Add<B,Root>::Result	I1;

	static const bool CND0 = I1::Value > 0;
	static const bool CND1 = I0::Value < 0;

public:
	typedef If<CND0, If<CND1, HitResult<Missed, MaxFixed>, HitResult<Hit, I0> >,
					 HitResult<Missed, MaxFixed> > Result;
};

template<bool IsPos, class D2, class B> 
struct SphereHit : public HitResult<Missed,MaxFixed> {};

template<class D2, class B>
struct SphereHit<true, D2, B> : public CalcSphereHit<D2,B>::Result {};

template<class ORIGIN, class DIR, class CENTER, class RAD>
struct IntersectSphere
{
	typedef typename Sub<CENTER,ORIGIN>::Result	V;
	typedef typename Dot<V,DIR>::Result			B;
	typedef typename Mul<RAD,RAD>::Result		R2;
	typedef typename Dot<V,V>::Result			V2;
	typedef typename Mul<B,B>::Result			B2;
	
	typedef typename Add<R2, typename Sub<B2,V2>::Result>::Result D2;
	typedef SphereHit<(D2::Value > 0), D2, B> intersection;
};

template<bool IsHit, class Pos, class Center>
struct SphereNormal
{
	typedef typename Normalize<typename Sub<Pos, Center>::Result>::Result Result;
};

template<class Pos, class Center>
struct SphereNormal<false,Pos,Center>
{
	typedef Vec3<One,One,One> Result;
};

template<class R, class P> struct Intersect;

template<class ORIGIN, class DIR, class CENTER, class RAD>
struct Intersect<Ray<ORIGIN,DIR>, Sphere<CENTER,RAD> > : 
	public IntersectSphere<ORIGIN,DIR,CENTER,RAD>::intersection
{
private:
	typedef typename IntersectSphere<ORIGIN,DIR,CENTER,RAD>::intersection Base;
public:
	typedef typename CalcPosition<Base::Type == Hit, ORIGIN, DIR, Base>::Result Position;
	typedef typename SphereNormal<Base::Type == Hit, Position, CENTER>::Result Normal;
	typedef DIR View;
};
///////////////////////////////////////////////////////////////////////////////
