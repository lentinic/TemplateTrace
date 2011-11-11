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
#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////
struct Null {};

static const int64_t fixed_shift = (1 << 16);
static const int64_t fixed_root = (1 << 8);

template<int64_t VAL>
struct Fixed
{
	static const int64_t Value = VAL;

	static float ToFloat()
	{
		return Value / (float)fixed_shift;
	}
};

typedef Fixed<fixed_shift> One;
typedef Fixed<0> Zero;
typedef Fixed<0x7fffffffffffffffLL> MaxFixed;
typedef Fixed<0x8000000000000000LL> MinFixed;
///////////////////////////////////////////////////////////////////////////////
template<class X_, class Y_, class Z_>
struct Vec3
{
	typedef X_ X;
	typedef Y_ Y;
	typedef Z_ Z;
};

typedef Vec3<Fixed<0>, Fixed<0>, Fixed<0> > Vec3Zero;
///////////////////////////////////////////////////////////////////////////////
template<class O, class D> struct Ray;

template<class Ox, class Oy, class Oz,
		 class Dx, class Dy, class Dz>
struct Ray<Vec3<Ox,Oy,Oz>, Vec3<Dx,Dy,Dz> >
{
	typedef Vec3<Ox,Oy,Oz> Origin;
	typedef Vec3<Dx,Dy,Dz> Dir;
};
///////////////////////////////////////////////////////////////////////////////
enum HitType
{
	Missed,
	Hit
};

template<HitType TYPE, class DIST>
struct HitResult : public DIST
{
	static const HitType Type = TYPE;
};
///////////////////////////////////////////////////////////////////////////////
