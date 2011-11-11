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

template<class Position_, class Color_, class Next_>
struct PointLight
{
	typedef Position_ Position;
	typedef Color_ Color;
	typedef Next_ Next;

	template<class Pixel, class Scene>
	struct CalcLight
	{
		// Light distance
		typedef typename Length<typename Sub<Position, 
			typename Pixel::Position>::Result>::Result Dist;

		// Pixel -> Light direction
		typedef typename Div<typename Sub<Position, 
			typename Pixel::Position>::Result, Dist>::Result Dir;

		// Shadow test
		typedef typename Scene::template IntersectScene<Ray<
			typename Pixel::Position, Dir> >::Result ShadowHit;
		typedef typename If<ShadowHit::One::Type == Hit, Zero, One>::Result shadow;

		// NDL
		typedef typename Max<typename Dot<Dir, typename Pixel::Normal>::Result, Zero>::Result NDL;
		
		// attenuation 1 / D^2
		typedef typename Div<One, typename Add<Fixed<1>, 
			typename Mul<Dist,Dist>::Result>::Result>::Result atten;

		// Combine
		typedef typename Mul<Color, NDL>::Result diff0;
		typedef typename Mul<diff0, atten>::Result diff1;
		typedef typename Mul<diff1, shadow>::Result Result;
	};
};

template<class Direction, class Color, class _Next>
struct DirectionalLight 
{
	typedef _Next Next;

	typedef typename Sub<Vec3Zero,Direction>::Result NegDir;

	template<class Pixel, class Scene>
	struct CalcLight
	{
		// NDL
		typedef typename Max<Zero, typename Dot<NegDir, typename Pixel::Normal>::Result>::Result NDL;

		// Shadowing
		typedef typename Scene::template IntersectScene<Ray<
			typename Pixel::Position, NegDir> >::Result ShadowHit;
		typedef typename If<ShadowHit::One::Type == Hit, Zero, One>::Result shadow;

		// Combine
		typedef typename Mul<Color, typename Mul<NDL,shadow>::Result>::Result Result;
	};
};

template<class Color, class _Next>
struct AmbientLight 
{
	typedef _Next Next;

	template<class Pixel, class Scene>
	struct CalcLight
	{
		typedef Color Result;
	};
};

template<class Pixel, class Scene, class Lights, class Total>
struct SumLighting
{
	typedef typename Add<Total, typename Lights::template CalcLight<Pixel,Scene>::Result>::Result Current;
	typedef typename SumLighting<Pixel,Scene,typename Lights::Next,Current>::Result Result;
};

template<class Pixel, class Scene, class Total>
struct SumLighting<Pixel,Scene,Null,Total>
{
	typedef Total Result;
};
