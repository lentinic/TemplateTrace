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

#include "Tile.h"

// General Case
template<int x, int y, int width, int height,
	template<int,int> class Function, int tilew=16, int tileh=16>
struct Buffer
{
	typedef Tile<x,y,tilew,tileh,Function> Data;
	typedef Buffer<x+tilew,y,width,height,Function,tilew,tileh> Next;

	template<class Visitor>
	static void Visit(Visitor v)
	{
		Data::Visit(v);
		Next::Visit(v);
	}
};

// First column
template<int y, int width, int height,
	template<int,int> class Function, int tilew, int tileh>
struct Buffer<0,y,width,height,Function,tilew,tileh>
{
	typedef Tile<0,y,tilew,tileh,Function> Data;
	typedef Buffer<tilew,y,width,height,Function,tilew,tileh> Next;
	typedef Buffer<0,y+tileh,width,height,Function,tilew,tileh> NextRow;

	template<class Visitor>
	static void Visit(Visitor v)
	{
		Data::Visit(v);
		Next::Visit(v);
		NextRow::Visit(v);
	}
};

// Terminal
template<int width, int height,
	template<int,int> class Function, int tilew, int tileh>
struct Buffer<0,height,width,height,Function,tilew,tileh>
{
	template<class Visitor>
	static void Visit(Visitor v) {}
};

template<int y, int width, int height,
	template<int,int> class Function, int tilew, int tileh>
struct Buffer<width,y,width,height,Function,tilew,tileh>
{
	template<class Visitor>
	static void Visit(Visitor v) {}
};
