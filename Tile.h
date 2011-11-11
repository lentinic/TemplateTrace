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

// General Case
template<int ux, int uy, int width, int height,
	template<int,int> class Function, int x=ux, int y=uy>
struct Tile
{
	typedef typename Function<x,y>::Result				Result;
	typedef Tile<ux,uy,width-1,height,Function,x+1,y>	Next;

	template<class Visitor>
	static void Visit(Visitor v)
	{
		v.template Call<Result>(x,y);
		Next::Visit(v);
	}
};

// First column
template<int ux, int uy, int width, int height,
	template<int,int> class Function, int y>
struct Tile<ux,uy,width,height,Function,ux,y>
{
	typedef typename Function<ux,y>::Result Result;

	typedef Tile<ux,uy,width-1,height,Function,ux+1,y> Next;
	typedef Tile<ux,uy,width,height-1,Function,ux,y+1> NextRow;

	template<class Visitor>
	static void Visit(Visitor v)
	{
		v.template Call<Result>(ux,y);
		Next::Visit(v);
		NextRow::Visit(v);
	}
};

// Terminators
template<int ux, int uy, int height,
	template<int,int> class Function, int x, int y>
struct Tile<ux,uy,0,height,Function,x,y>
{
	template<class Visitor>
	static void Visit(Visitor v) {}
};

template<int ux, int uy, int width, 
	template<int,int> class Function, int y>
struct Tile<ux,uy,width,0,Function,ux,y>
{
	template<class Visitor>
	static void Visit(Visitor v) {}
};
