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

#include <algorithm>
#include <stdlib.h>
#include "BmpWriter.h"
#include "TestScene.h"
#include "Tile.h"
#include "Buffer.h"

#ifndef OUTW
#define OUTW 4
#define OUTH 4
#define UX 0
#define UY 0
#define TILEW 2
#define WRITE_BMP
#endif

struct BGR
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
};

struct PixelWriter
{
	BGR *		buffer;
	
	template<class RES> 
	void Call(int _x, int _y)
	{
		int x = _x - UX;
		int y = _y - UY;
		buffer[(y * TILEW) + x].r = (unsigned char)(255 * std::min(RES::X::ToFloat(),1.f));
		buffer[(y * TILEW) + x].g = (unsigned char)(255 * std::min(RES::Y::ToFloat(),1.f));
		buffer[(y * TILEW) + x].b = (unsigned char)(255 * std::min(RES::Z::ToFloat(),1.f));
	}
};

int main()
{
#if defined(WRITE_BMP)
	BGR buf[OUTW*OUTH];
#else
	BGR buf[TILEW];
#endif

	memset(buf, 0, sizeof(buf));

	PixelWriter pix;
	pix.buffer = &buf[0];

#if defined(WRITE_BMP)
	Buffer<0,0,OUTW,OUTH,SceneTracer<TestScene,TestLights,OUTW,OUTH>::Tracer,
		TILEW,1>::Visit(pix);
	WriteBitmap("output.bmp", OUTW, OUTH, &buf[0]);
#else
	Tile<UX,UY,TILEW,1,SceneTracer<TestScene,TestLights,OUTW,OUTH>::Tracer>::Visit(pix);
	fwrite(&buf[0], 1, sizeof(buf), stdout);
#endif

	return 0;
}
