#pragma once

#include "COMMON\stdc++.h"
using namespace std;

//https://qiita.com/ganariya/items/df35d253726269bda436
struct HashPair {
    //���� const������
    template<class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const {

        //first�����n�b�V��������
        auto hash1 = hash<T1>{}(p.first);

        //second�����n�b�V��������
        auto hash2 = hash<T2>{}(p.second);

        //�d�����Ȃ��悤�Ƀn�b�V������
        size_t seed = 0;
        seed ^= hash1 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};


struct Point {
    bool fixed = false;
    int id = -1;
    EVec2d pos;
    int hEdge_idx = -1;

    Point(){
    } //?
    Point(EVec2d p, int _he_idx) : pos(p), hEdge_idx(_he_idx) {}
    vector<int> all_face_idx_touchs_me();
};

struct halfEdge {
    bool exist = true;
    int id = -1;

    int st_idx = -1;
    int to_idx = -1;

    int next_idx = -1;
    int prev_idx = -1;
    int pair_idx = -1;

    int face_idx = -1;

    vector<int> all_hE_idces_face_includes();

};

struct Circle {
    Point O;
    double r = 0.0;

    Circle() {}
    Circle(EVec2d _pos,double _r):r(_r){
        O.pos = _pos;
        O.fixed = true;
    }
};

struct Triangle_utils { //�`�Ƃ��Ă�����3�p�`�B�O�S���W����O����A�ʐόv�Z�Ȃǂ��ł���B
    EVec2d A, B, C;

    EVec2d AB, BC, CA;
    EVec2d AC, CB, BA;

};

struct Triangle {
    bool exist = true;
    Circle Outer;

    int id = -1;
    int hE_idx = -1;



};

struct CVT_v2{

    //���_�̃��X�g
    //3�p�`�̃��X�g
    //�ӂ̃��X�g�i����H�H�j





};

