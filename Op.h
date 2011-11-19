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

///////////////////////////////////////////////////////////////////////////////
template<class L, class R> 
struct Add
{
	typedef Fixed<L::Value + R::Value> Result;
};

template<class x0, class y0, class z0, class x1, class y1, class z1>
struct Add<Vec3<x0,y0,z0>, Vec3<x1,y1,z1> >
{
	typedef Vec3<typename Add<x0,x1>::Result, typename Add<y0,y1>::Result, 
		typename Add<z0,z1>::Result> Result;
};
///////////////////////////////////////////////////////////////////////////////
template<class L, class R> 
struct Sub
{
	typedef Fixed<L::Value - R::Value> Result;
};

template<class x0, class y0, class z0, class x1, class y1, class z1>
struct Sub<Vec3<x0,y0,z0>, Vec3<x1,y1,z1> >
{
	typedef Vec3<typename Sub<x0,x1>::Result, typename Sub<y0,y1>::Result,
		typename Sub<z0,z1>::Result> Result;
};
///////////////////////////////////////////////////////////////////////////////
template<class L, class R> 
struct Mul
{
	typedef Fixed<(L::Value * R::Value) / fixed_shift> Result;
};

template<class x0, class y0, class z0,
		 class x1, class y1, class z1>
struct Mul<Vec3<x0,y0,z0>, Vec3<x1,y1,z1> >
{
	typedef Vec3<typename Mul<x0,x1>::Result, typename Mul<y0,y1>::Result,
		typename Mul<z0,z1>::Result> Result;
};

template<class x, class y, class z, class Scalar>
struct Mul<Vec3<x,y,z>, Scalar>
{
	typedef Vec3<typename Mul<x,Scalar>::Result, typename Mul<y,Scalar>::Result,
		typename Mul<z,Scalar>::Result> Result;
};

template<class x, class y, class z, class Scalar>
struct Mul<Scalar, Vec3<x,y,z> >
{
	typedef Vec3<typename Mul<x,Scalar>::Result, typename Mul<y,Scalar>::Result,
		typename Mul<z,Scalar>::Result> Result;
};
///////////////////////////////////////////////////////////////////////////////
template<class L, class R> 
struct Div
{
	typedef Fixed<(L::Value * fixed_shift) / R::Value> Result;
};

template<class x0, class y0, class z0,
		 class x1, class y1, class z1>
struct Div<Vec3<x0,y0,z0>, Vec3<x1,y1,z1> >
{
	typedef Vec3<typename Div<x0,x1>::Result, typename Div<y0,y1>::Result,
		typename Div<z0,z1>::Result> Result;
};

template<class x0, class y0, class z0, class Scalar>
struct Div<Vec3<x0,y0,z0>, Scalar>
{
	typedef Vec3<typename Div<x0,Scalar>::Result, typename Div<y0,Scalar>::Result,
		typename Div<z0,Scalar>::Result> Result;
};
///////////////////////////////////////////////////////////////////////////////
template<class L, class R> struct Dot;

template<class x0, class y0, class z0,
		 class x1, class y1, class z1>
struct Dot<Vec3<x0,y0,z0>, Vec3<x1,y1,z1> >
{
	typedef typename Add<typename Add<typename Mul<x0,x1>::Result, typename Mul<y0,y1>::Result>::Result,
		typename Mul<z0,z1>::Result>::Result Result;
};
///////////////////////////////////////////////////////////////////////////////
template<int64_t V, int64_t L = 1, int64_t U = V> 
struct ISqrt;

template<int64_t V, int64_t R>
struct ISqrt<V, R, R>
{
	static const int64_t Result = R;
};

template<int64_t L, int64_t U>
struct ISqrt<0,L,U>
{
	static const int64_t Result = 0;
};

template<int64_t V, int64_t L, int64_t U>
struct ISqrt
{
	static const int64_t t0 = (L + U) / 2;
	static const int64_t t1 = t0 * t0;
	static const bool CND = t1 >= V;
	static const int64_t Result = ISqrt<V, CND ? L : (t0 + 1),
		CND ? t0 : U>::Result;
};

template<int64_t val>
struct SqrtImpl
{
	static const bool CND = val < One::Value;
	static const int64_t UPPER = CND ? (val * fixed_root) : ((val/fixed_root)+1);
	typedef Fixed<ISqrt<val, 1, UPPER>::Result * fixed_root> Result;
};

template<class V> 
struct Sqrt
{
	typedef typename SqrtImpl<V::Value>::Result Result;
};
///////////////////////////////////////////////////////////////////////////////
template<class V>
struct Length
{
	typedef typename Sqrt<typename Dot<V,V>::Result>::Result Result;
};
///////////////////////////////////////////////////////////////////////////////
template<class V> struct Normalize
{
	typedef typename Div<V, typename Length<V>::Result>::Result Result;
};
///////////////////////////////////////////////////////////////////////////////
template<class A, class B> struct Max;

template<int64_t a, int64_t b>
struct Max<Fixed<a>, Fixed<b> >
{
	static const int64_t val = (a >= b) ? a : b;
	typedef Fixed<val> Result;
};
///////////////////////////////////////////////////////////////////////////////
