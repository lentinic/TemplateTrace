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
#include "Scene.h"
#include "Light.h"
#include "Material.h"

// Materials
typedef FlatColor<One,Zero,Zero>	                Red;
typedef FlatColor<Zero,One,Zero>	                Green;
typedef Reflective<Zero,Zero,One,ToFixed<0,2,3> >       Blue;
typedef Reflective<One,One,One,ToFixed<0,3,5> >		White;

// Scene primitives
typedef SceneNode<Sphere<Vec3<Zero,Zero,ToFixed<5,0,1> >,
	ToFixed<1,0,1> >, Red, Null> node0;

typedef SceneNode<Sphere<Vec3<ToFixed<2,0,1>,Zero,ToFixed<2,0,1> >,
	ToFixed<1,1,2> >, Green, node0> node1;

typedef SceneNode<Sphere<Vec3<ToFixed<-3,0,1>,ToFixed<2,0,1>,ToFixed<3,0,1> >,
	ToFixed<1,1,2> >, Blue, node1> node2;

typedef SceneNode<Plane<Vec3<Zero,One,Zero>, One>, White, node2> TestScene;

// Lights
typedef PointLight<Vec3<Zero, ToFixed<0,1,2>, ToFixed<3,0,1> >,
	Vec3<ToFixed<1,2,3>, ToFixed<1,2,3>, One>, Null> light0;

typedef DirectionalLight<Normalize<Vec3<Fixed<-65536>,Fixed<-65536>,Fixed<-65536> > >::Result,
	Vec3<ToFixed<0,4,5>, ToFixed<0,1,2>, ToFixed<0,4,5> >, light0> light1;

typedef AmbientLight<Vec3<ToFixed<0,1,8>, ToFixed<0,1,8>, ToFixed<0,1,8> >, light1> TestLights;
