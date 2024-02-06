#pragma once
#include "DrawDebugHelpers.h"

class DebugMacros
{
#define DRAW_SPHERE(Location, Radius, Segments, Color, bPersistentLines, LifeTime) \
DrawDebugSphere(GetWorld(), Location, Radius, Segments, Color, bPersistentLines, LifeTime);
#define DRAW_LINE(Start, End, Color, bPersistentLines, LifeTime, Thickness) \
DrawDebugLine(GetWorld(), Start, End, Color, bPersistentLines, LifeTime, 0, Thickness);
#define DRAW_POINT(Location, Size, Color, bPersistentLines, LifeTime) \
DrawDebugPoint(GetWorld(), Location, Size, Color, bPersistentLines, LifeTime);
#define DRAW_BOX(Center, Extent, Color, bPersistentLines, LifeTime) \
DrawDebugBox(GetWorld(), Center, Extent, Color, bPersistentLines, LifeTime);
#define DRAW_VECTOR(Start, End, Color, bPersistentLines, LifeTime, Thickness) \
	{ \
		DrawDebugLine(GetWorld(), Start, End, Color, bPersistentLines, LifeTime, 0, Thickness); \
		DrawDebugPoint(GetWorld(), End, 10, Color, bPersistentLines, LifeTime); \
	}
};
