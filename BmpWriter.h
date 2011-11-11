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

struct BMPHEAD
{
	int filesize;
	short reserved[2];
	int headersize;
	int infoSize;
	int width;
	int height;
	short biPlanes;
	short bits;
	int biCompression;
	int biSizeImage;
	int biXPelsPerMeter;
	int biYPelsPerMeter;
	int biClrUsed;
	int biClrImportant;
};

void WriteBitmap(const char * filename, int width, int height, void * pbuffer)
{
	unsigned char * buf = (unsigned char *) pbuffer;
	int pitch = ((width * 3) + 3) & (-4);	

	BMPHEAD bh;
	memset((char *)&bh,0,sizeof(BMPHEAD));
	bh.headersize = 54L;
	bh.infoSize = 0x28L;
	bh.width = width;
	bh.height = height;
	bh.biPlanes =  1;
	bh.bits = 24;
	bh.biCompression = 0L;
	bh.biSizeImage = 54 + (pitch * height);
	int pad = pitch - (width * 3);
	unsigned char padbuf[] = {0,0,0};

	FILE * file = fopen(filename, "wb");
	fwrite("BM", 1, 2, file);
	fwrite(&bh, 1, sizeof(bh), file);
	for (int i=(height-1); i>=0; i--)
	{
		fwrite(buf + (i * width * 3), 1, width * 3, file);
		if (pad > 0)
			fwrite(padbuf,1,pad,file);
	}
	fclose(file);
}
