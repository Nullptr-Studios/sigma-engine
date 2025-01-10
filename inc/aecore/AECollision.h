// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AECollision.h
// Author			:	Thomas Komair
// Creation Date	:	2012/02/15
// Purpose			:	main header file for the collision library
// History			:
// - 2012/02/15		:	- initial implementation
// ---------------------------------------------------------------------------

#ifndef AE_COLLISION_H
#define AE_COLLISION_H

// -----------------------------
#pragma region // 2D Line Segment Structure
//		Holds data for representing a 2D Line Segment.
//		Data Members:
//		- mP0 mP1	:	Endpoints of the line segment
//		- mN		:	Outward normal to the edge vector e = (P1-P0)
//		- mNdotP0	:	internal use only. Value of the dot product : mN . mP0
//
struct AELineSegment2D
{
    AEVec2  mP0;     // Point on the line
    AEVec2  mP1;     // Point on the line
    AEVec2 mN;      // Line's normal
    float     mNdotP0; // To avoid computing it every time it's needed
};

/*
This function builds a 2D line segment's data using 2 points
 - Computes the normal (Unit Vector)
 - Computes the dot product of the normal with one of the points

 - Parameters
	- LS:		The to-be-built line segment
	- Point0:	One point on the line
	- Point1:	Another point on the line

 - Returns true if the lise equation was built successfully 
*/
bool AEBuildLineSegment2D(AELineSegment2D * LS, AEVec2 * Point0, AEVec2 * Point1);

bool AEStaticLineToStaticLine(AELineSegment2D * l1, AELineSegment2D * l2, AEVec2 * Pi, float * ti1, float * ti2);

#pragma endregion

// ----------------------
#pragma region // Static Tests
//		The tests below allow to test if two static shapes are intersection.
//		They return true if the shapes are intersecting and false otherwise.
//		Note that they do not perform any other checks for exact point of intersection
//		or penetration depth.

#pragma region // Point Containment Tests
/*
This function determines if a static point is inside a static circle 

 - Parameters
	- P:		The static point whose location should be determined
	- Center:	Center of the static circle
	- Radius:	Radius of the static circle

 - Returned value: Boolean
	- True:		If the point and circle are intersecting
	- False:	If the point and circle are not intersecting
*/
bool AEStaticPointToStaticCircle(AEVec2 * P, AEVec2 * Center, float Radius);

/*
This function determines the distance separating a point from a line

 - Parameters
	- P:		The point whose location should be determined
	- LS:		The line segment

 - Returned value: The distance. Note that the returned value should be:
	- Negative if the point is in the line's inside half plane
	- Positive if the point is in the line's outside half plane
	- Zero if the point is on the line
*/
float AEStaticPointToStaticLineSegment(AEVec2 * P, AELineSegment2D * LS);

/*!
This function checks if the point Pos is colliding with the rectangle
whose center is Rect, width is "Width" and height is Height
*/
bool AEStaticPointToStaticRect(AEVec2 * Pos, AEVec2 * Rect, float Width, float Height);

/*!
	\fn		StaticPointToOrientedRect
	\brief	This function checks if the point Pos is colliding with an oriented rectangle
	whose center is Rect, width is "Width", height is Height and rotation "AngleRad"

	\return	true if the point is contained in the rectangle, false otherwise.
*/
bool AEStaticPointToOrientedRect(AEVec2 * Pos, AEVec2 * Rect, float Width, float Height, float AngleRad);

#pragma endregion

#pragma region // Primitive Overlap Tests

// Contact information computed by the functions below
struct AEContact
{
    AEVec2 mPi;          // Point of intersection
    AEVec2 mNormal;      // Normal of collision
    float  mPenetration; // Penetration: how much overlap between the shapes
};

/*!
	\brief This function checks for collision between 2 circles.
	\return	true if both shapes overlap, false otherwise.
*/
bool AEStaticCircleToStaticCircle(AEVec2 * Center0, float Radius0, AEVec2 * Center1, float Radius1, AEContact * pResult = NULL);

/*!
	\brief This functions checks if 2 rectangles are colliding
	\return	true if both shapes overlap, false otherwise.
*/
bool AEStaticRectToStaticRect(AEVec2 * Rect0, float Width0, float Height0, AEVec2 * Rect1, float Width1, float Height1, AEContact * pResult = NULL);

/*!
	\brief	This function checks if a circle is colliding with a rectangle
	\return	true if both shapes overlap, false otherwise.
*/
bool AEStaticRectToStaticCircle(AEVec2 * Rect, float Width, float Height, AEVec2 * Center, float Radius, AEContact * pResult = NULL);

/*!
	\brief	This function checks if a circle is colliding with an oriented rectangle
			Rectangle: Defined at center "Rect" of width and height "Width", "Height", 
			and rotation "AngelRad" respectively
			Center: Defined at center "Center", and of radius "Radius"

	\return	true if both shapes overlap, false otherwise.
*/
bool AEOrientedRectToStaticCircle(AETransform2D * OBB, AEVec2 * Center, float Radius, AEContact * pResult = NULL);

/*! ---------------------------------------------------------------------------
 \brief	Checks overlap between two oriented box, using the Separating Axis
 			Theorem described in class. If an overlap exists, it stores the 
 			contact data into pResult.
  \return	true if the shapes overlap, false otherwise
*/
bool AEOrientedRectToOrientedRect(AETransform2D * OBB1, AETransform2D * OBB2, AEContact * pResult = NULL);

/*! ---------------------------------------------------------------------------
	\brief	Checks overlap between two convex polygons, using the Separating Axis
				Theorem described in class. If an overlap exists, it stores the 
				contact data into pResult.
	
	\param[in]	p1	Array of vertices in world space representing the first polygon
	\param[in]	p1	Array of vertices in world space representing the second polygon
	\param[in]	vtxCount1	Size of the first array
	\param[in]	vtxCount2	Size of the second array
	\param[out]	pResult	Contact structure filled with correct information. 

	\remarks	
		- This function doesn't compute the point of intersection correctly yet.
	
	 \return	true if the shapes overlap, false otherwise
*/
bool AEPolygonToPolygon(AEVec2 * p1, unsigned vtxCount1, AEVec2 * p2, unsigned vtxCount2, AEContact * pResult = NULL);

#pragma endregion // Primitive Overlap Tests

#pragma endregion // Static Tests

// ----------------------
#pragma region // Animated Tests
//		The tests below allow to test if two animated shapes are intersecting.
//		Contrary to the static tests above, these test perform some extra computation
//		to determine the exact time, location of intersection
#pragma region Animated_Point
/*
This function checks whether an animated point is colliding with a line segment

 - Parameters
	- Ps		:	The point's starting location
	- Pe		:	The point's ending location
	- LS		:	The line segment
	- [out]Pi	:	This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AEAnimatedPointToStaticLineSegment(AEVec2 * Ps, AEVec2 * Pe, AELineSegment2D * LS, AEVec2 * Pi);

/*
This function reflects an animated point on a line segment.
It should first make sure that the animated point is intersecting with the line 

 - Parameters
	- Ps		:	The point's starting location
	- Pe		:	The point's ending location
	- LS		:	The line segment
	- [out]Pi	:	This will be used to store the intersection point's coordinates (In case there's an intersection)
	- [out]R	:	Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AEReflectAnimatedPointOnStaticLineSegment(AEVec2 * Ps, AEVec2 * Pe, AELineSegment2D * LS, AEVec2 * Ph, AEVec2 * R);

/*
This function checks whether an animated point is colliding with a static circle

 - Parameters
	- Ps		:	The point's starting location
	- Pe		:	The point's ending location
	- Center	:	The circle's center
	- Radius	:	The circle's radius
	- [out]Pi	:	This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AEAnimatedPointToStaticCircle(AEVec2 * Ps, AEVec2 * Pe, AEVec2 * Center, float Radius, AEVec2 * Pi);

/*
This function reflects an animated point on a static circle.
It should first make sure that the animated point is intersecting with the circle 

 - Parameters
	- Ps		:	The point's starting location
	- Pe		:	The point's ending location
	- Center	:	The circle's center
	- Radius	:	The circle's radius
	- [out]Pi	:	This will be used to store the intersection point's coordinates (In case there's an intersection)
	- [out]R	:	Normalized Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AEReflectAnimatedPointOnStaticCircle(AEVec2 * Ps, AEVec2 * Pe, AEVec2 * Center, float Radius, AEVec2 * Pi, AEVec2 * R);
#pragma endregion
#pragma region Animated_Circle

/*
This function checks whether an animated circle is colliding with a line segment

 - Parameters
	- Ps		:	The center's starting location
	- Pe		:	The center's ending location
	- Radius	:	The circle's radius
	- LS		:	The line segment
	- [out]Pi	:	This will be used to store the intersection point's coordinates (In case there's an intersection)
	

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AEAnimatedCircleToStaticLineSegment(AEVec2 * Ps, AEVec2 * Pe, float Radius, AELineSegment2D * LS, AEVec2 * Pi);

/*
This function reflects an animated circle on a line segment.
It should first make sure that the animated point is intersecting with the line 

 - Parameters
	- Ps		:	The circle's starting location
	- Pe		:	The circle's ending location
	- Radius	:	The circle's radius
	- LS		:	The line segment
	- [out]Pi	:	This will be used to store the intersection point's coordinates (In case there's an intersection)
	- [out]R	:	Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AEReflectAnimatedCircleOnStaticLineSegment(AEVec2 * Ps, AEVec2 * Pe, float Radius, AELineSegment2D * LS, AEVec2 * Pi, AEVec2 * R);

/*
This function checks whether an animated circle is colliding with a static circle

 - Parameters
	- Center0s	:	The starting position of the animated circle's center 
	- Center0e	:	The ending position of the animated circle's center 
	- Radius0	:	The animated circle's radius
	- Center1	:	The static circle's center
	- Radius1	:	The static circle's radius
	- [out]Pi	:	This will be used to store the intersection point's coordinates (In case there's an intersection)
					Note that the point of intersection is equal to the moving center 's position at time ti of intersection
					such that Pi = Ci + N * Radius0. where N is the normal to the tangent of the circle at the point of intersection.

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AEAnimatedCircleToStaticCircle(AEVec2 * Center0s, AEVec2 * Center0e, float Radius0, AEVec2 * Center1, float Radius1, AEVec2 * Pi);

/*
This function reflects an animated circle on a static circle.
It should first make sure that the animated circle is intersecting with the static one 

 - Parameters
	- Center0s:		The starting position of the animated circle's center 
	- Center0e:		The ending position of the animated circle's center 
	- Radius0:		The animated circle's radius
	- Center1:		The static circle's center
	- Radius1:		The static circle's radius
	- [out]Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)
					Note is the same as above function 
	- [out]R:		Reflected vector R along same normal as described above

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AEReflectAnimatedCircleOnStaticCircle(AEVec2 * Center0s, AEVec2 * Center0e, float Radius0, AEVec2 * Center1, float Radius1, AEVec2 * Pi, AEVec2 * R);

/*
	- Purpose:
		This function checks for the intersection of two animated circles.
		It makes use of the previous function by changing the velocities's to a frame of reference where one of the circles appears to be 
		static. 

	- Parameters:
		- Center0s, Center0e : Start and End position of circle0
		- Center1s, Center1e : Start and End position of circle1
		- Radius0, Radius 1  : Radii of the circles
		- [out] Pi			 : Point of intersection that corresponds to the closest point between two circles at the time of intersection. 
*/
float AEAnimatedCircleToAnimatedCircle(AEVec2 * Center0s, AEVec2 * Center0e, float Radius0, AEVec2 * Center1s, AEVec2 * Center1e, float Radius1, AEVec2 * Pi);

#pragma endregion
#pragma endregion

#pragma region //Collision Resolution
void AEResolveContactVelocities(AEVec2 * vel1, AEVec2 * vel2, AEContact * contact, float invMass1 = 1, float invMass2 = 1, float restitution = 0.908f);
void AEResolveContactPenetration(AEVec2 * pos1, AEVec2 * pos2, AEContact * contact, float invMass1 = 1, float invMass2 = 1);
#pragma endregion

#endif
