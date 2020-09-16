#pragma once

#include "COMMON\stdc++.h"
using namespace std;

struct Point {
    EVec2d pos;
    halfEdge *hEdge;

    Point(){} //?
    Point(EVec2d p, halfEdge *hE) : pos(p), hEdge(hE) {}
};

struct halfEdge {
    Point *st;
    halfEdge *prev;
    halfEdge *next;
    halfEdge *pair;

    Triangle *faceTr;
};

struct Triangle { // Only triangle this time
    halfEdge *hEdge;
};

