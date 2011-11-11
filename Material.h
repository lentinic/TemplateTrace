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
#include "Light.h"

// Material evaluation
template<bool valid, class Material, class Pixel, class Scene, class Lights, int Depth>
struct EvalMaterial
{
	typedef typename Material::template Evaluate<Scene, Lights, 
		Pixel, Depth>::Result Result;
};

// Terminal cases
template<class Material, class Pixel, class Scene, class Lights, int Depth>
struct EvalMaterial<false,Material,Pixel,Scene,Lights,Depth>
{
	typedef Vec3Zero Result;
};

template<class Material, class Pixel, class Scene, class Lights>
struct EvalMaterial<true,Material,Pixel,Scene,Lights,0>
{
	typedef Vec3Zero Result;
};

// A simple lit material
template<class R, class G, class B>
struct FlatColor
{
	typedef Vec3<R,G,B> Color;

	template<class Scene, class Lights, class Pixel, int Depth>
	struct Evaluate
	{
		typedef typename SumLighting<Pixel, Scene, Lights, Vec3Zero>::Result Lighting;
		typedef typename Mul<Color, Lighting>::Result Result;
	};
};

// A simple reflective material
template<class R, class G, class B, class Strength>
struct Reflective
{
	typedef Vec3<R,G,B> Color;

	template<class Scene, class Lights, class Pixel, int Depth>
	struct Evaluate
	{
		// Calculate reflection ray
		typedef typename Dot<typename Pixel::View, typename Pixel::Normal>::Result VDN;
		typedef typename Sub<typename Pixel::View, typename Mul<typename Pixel::Normal,
			typename Mul<ToFixed<2,0,1>, VDN>::Result>::Result>::Result ReflDir;
		typedef Ray<typename Pixel::Position, ReflDir> ReflRay;

		// Get reflection hit and shading
		typedef typename Scene::template IntersectScene<ReflRay>::Result ReflectionHit;
		static const bool is_hit = ReflectionHit::One::Type == Hit;
		typedef typename EvalMaterial<is_hit, typename ReflectionHit::Two,
			typename ReflectionHit::One, Scene, Lights, Depth-1>::Result ReflectionColor;

		// Sum lighting
		typedef typename SumLighting<Pixel,Scene,Lights,Vec3Zero>::Result Lighting;

		// Add reflection and lighting * color
		typedef typename Add<typename Mul<Color,Lighting>::Result, 
			typename Mul<Strength, ReflectionColor>::Result>::Result Result;
	};
};
