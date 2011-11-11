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
#include "Sphere.h"
#include "Plane.h"
#include "Material.h"

///////////////////////////////////////////////////////////////////////////////
template<class Primitive, class Material, class Next>
struct SceneNode
{
	template<class R>
	struct IntersectScene
	{
		typedef Pair<Intersect<R, Primitive>, Material>				res0;
		typedef typename Next::template IntersectScene<R>::Result	res1;

		static const bool is_less = res0::One::Value < res1::One::Value;

		typedef typename If<res0::One::Type == Hit && res1::One::Type == Hit,
					If<is_less, res0, res1>,
					If<res0::One::Type == Hit, res0, res1> >::Result Result;
	};
};
///////////////////////////////////////////////////////////////////////////////
template<class Primitive, class Material>
struct SceneNode<Primitive,Material,Null>
{
	template<class R>
	struct IntersectScene
	{
		typedef Intersect<R,Primitive> intr0;
		typedef Pair<intr0, Material> Result;
	};
};
///////////////////////////////////////////////////////////////////////////////
template<class Scene, class Lights, int Width, int Height>
struct SceneTracer
{
private:
	static const int CX = Width/2;
	static const int CY = Height/2;

public:
	template<int x, int y>
	struct Tracer
	{
		typedef typename Normalize<typename Div<Vec3<ToFixed<x - CX, 0, 1>,
													 ToFixed<CY - y, 0, 1>,
													 One>,
												Vec3<ToFixed<CX,0,1>,
													 ToFixed<CY,0,1>,
													 One> >::Result>::Result Dir;

		typedef typename Scene::template IntersectScene<Ray<Vec3Zero, Dir> >::Result Intersection;

		static const bool is_hit = Intersection::One::Type == Hit;
		typedef typename EvalMaterial<is_hit, typename Intersection::Two,
			typename Intersection::One, Scene, Lights, 3>::Result Result;
	};
};
///////////////////////////////////////////////////////////////////////////////
