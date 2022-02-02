// Module to detect collisions between moving rectangles with sides parallel to axis. 
// Depends only on standard math library (abs function), so can be used separately

#ifndef CHEAP_COLLISIONS_H
#define CHEAP_COLLISIONS_H

#include <math.h>

// Vector2 for returning objects
typedef struct { float x, y; } Vector2F;

// Returns true if number b is between number a and number c
int between(float a, float b, float c) {
    return (a <= b && b <= c) || (a >= b && b >= c);
}

// Returns true if segment starting in a1 with length l1 intersects segment starting in a2 with length l2
int segIntersect(float a1, float l1, float a2, float l2) {
    return (a1 < a2 && a2 < a1 + l1) || (a1 < a2 + l2 && a2 + l2 < a1 + l1);
}

// Returns true if two rects defined by top-left corner and dimensions overlap.
int isRectOverlap(
    float r1x, float r1y, float r1w, float r1h,
    float r2x, float r2y, float r2w, float r2h
) {
    return (r1x < r2x + r2w && r1x + r1w > r2x && r1y < r2y + r2h && r1y + r1h > r2y);
}

// Checks is two Vertical segments, seg2 is moving by the vector (seg2dx, seg2dy) per frame, 
// will collide in the next frame. Returns how much of a frame will pass before they collide, or 1.0f 
// if they won't
float segVseg(
    float seg1x, float seg1y, float seg1l, 
    float seg2x, float seg2y, float seg2l,
    float seg2dx, float seg2dy
) {
    if (abs(seg2dx) > 0 && between(seg2x, seg1x, seg2x + seg2dx)) {
        float k = (seg1x - seg2x) / seg2dx;
        if (segIntersect(seg1y, seg1l, seg2y + seg2dy * k, seg2l)) 
            return k;
        else 
            return 1.0f;
    }
    else return 1.0f;
}

// Checks is two Horizontal segments, seg2 is moving by the vector (seg2dx, seg2dy) per frame, 
// will collide in the next frame. Returns how much of a frame will pass before they collide, or 1.0f 
// if they won't
float segHseg(
    float seg1x, float seg1y, float seg1l, 
    float seg2x, float seg2y, float seg2l,
    float seg2dx, float seg2dy
) {
    if (abs(seg2dy) > 0 && between(seg2y, seg1y, seg2y + seg2dy)) {
        float k = (seg1y - seg2y) / seg2dy;
        if (segIntersect(seg1x, seg1l, seg2x + seg2dx * k, seg2l))
            return k;
        else 
            return 1.0f;
    }
    else return 1.0f;
}

// Checks if the second rectangle, moving by the vector (r2dx, r2dy) per frame, collides with
// the first rectangle in the next frame. Returns tuple of two variables, kX and kY.
// kX is the how much of a frame will pass before vertical walls of rectangles will collide
// kY is the how much of a frame will pass before horizontal walls of rectangles will collide
// they might be 1.0 if respecting walls of rectangles won't collide in the next frame.
//
// For example minimum of those two variables is how much of a 
// frame will pass until rectangles touch eachother.
// 
// Also if the second rectangle will move by (r2dx * kX, r2dy * kY) per frame, then it will 
// move by (r2dx, r2dy), then when it meets first, it will slide along its sides and 
// then continue moving by (r2dx, r2dy).
// 
// See better example at README.md
Vector2F rectWRect(
    float r1x, float r1y, float r1w, float r1h, 
    float r2x, float r2y, float r2w, float r2h, 
    float r2dx, float r2dy
) {
    if (isRectOverlap(r1x, r1y, r1w, r1h, r2x, r2y, r2w, r2h)) {
        return (Vector2F){ 0.0f, 0.0f };
    }
    float tV = 1.0f, tH = 1.0f;
    if (r2dx < 0) {
        tV = segVseg(r1x + r1w, r1y, r1h, r2x, r2y, r2h, r2dx, r2dy);
    }
    else {
        tV = segVseg(r1x, r1y, r1h, r2x + r2w, r2y, r2h, r2dx, r2dy);
    }
    if (r2dy < 0) {
        tH = segHseg(r1x, r1y + r1h, r1w, r2x, r2y, r2w, r2dx, r2dy);
    }
    else {
        tH = segHseg(r1x, r1y, r1w, r2x, r2y + r2h, r2w, r2dx, r2dy);
    }
    return (Vector2F){ tV, tH };
}

#endif