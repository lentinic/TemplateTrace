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

///////////////////////////////////////////////////////////////////////////////
template<int64_t ipart, int64_t numer, int64_t denom>
struct ToFixed : public Fixed<(ipart * fixed_shift) + ((numer * fixed_shift) / denom)>
{};
///////////////////////////////////////////////////////////////////////////////
template<bool CND, class Then, class Else> 
struct If : public Then
{
	typedef Then Result;
};

template<class Then, class Else>
struct If<false, Then, Else> : public Else
{
	typedef Else Result;
};
///////////////////////////////////////////////////////////////////////////////
template<class A, class B>
struct Pair
{
	typedef A One;
	typedef B Two;
};
///////////////////////////////////////////////////////////////////////////////
template<bool ishit, class Origin, class Dir, class Dist>
struct CalcPosition
{
	typedef typename Add<Origin, typename Mul<Dir, Dist>::Result>::Result Result;
};

template<class Origin, class Dir, class Dist>
struct CalcPosition<false,Origin,Dir,Dist>
{
	typedef Vec3Zero Result;
};
///////////////////////////////////////////////////////////////////////////////
