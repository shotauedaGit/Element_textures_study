#pragma once
#include "COMMON\stdc++.h"
#include "COMMON\tmath.h"
#include <chrono>
using namespace std;
//https://qiita.com/ganariya/items/df35d253726269bda436
struct HashPair {
    //注意 constがいる
    template<class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const {

        //first分をハッシュ化する
        auto hash1 = hash<T1>{}(p.first);

        //second分をハッシュ化する
        auto hash2 = hash<T2>{}(p.second);

        //重複しないようにハッシュ処理
        size_t seed = 0;
        seed ^= hash1 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};
struct Info { //gr&id
    int gr = -1; //属するグループ
    int id = -1; //グループ内id

    Info() {}
    Info(int _gr, int _id) :gr(_gr), id(_id) {}

    bool operator==(const Info& src) { return (gr == src.gr) && (id == src.id); }
    bool operator!=(const Info& src) { return !((gr == src.gr) && (id == src.id)); }
};
struct Point {
    bool fixed = false;
    bool exist = true;
    Info _info; //変化しないはず
    EVec2d pos;
    int hEdge_idx = -1;
    Point() {
    } //?
    Point(EVec2d p, int _he_idx) : pos(p), hEdge_idx(_he_idx) {}
    ~Point() {}

    void Copy(const Point& src) {
        fixed = src.fixed;
        _info = src._info;
        pos = src.pos;
        hEdge_idx = src.hEdge_idx;
    };
    Point& operator=(const Point& src) { Copy(src); return *this; }
    vector<int> all_face_idx_touchs_me();

    int group() { return _info.gr; }
    int id() { return _info.id; }
    void set_group(int _gr) { _info.gr = _gr; }
    void set_id(int _id) { _info.id = _id; } //Group を決定してから
    void set_pos(EVec2d _pos) { pos = _pos; }//reloadVC用、3角形分割に影響を及ぼさない
};
struct halfEdge {
    bool exist = true;
    int id = -1;

    Info st_Info;//point
    Info to_Info;//point

    int next_idx = -1;
    int prev_idx = -1;
    int pair_idx = -1;

    int face_idx = -1;

    halfEdge() {}
    halfEdge(int _id, Info st, Info to, int nx, int pv, int pair, int face) {
        id = _id;
        st_Info = st;
        to_Info = to;
        next_idx = nx;
        prev_idx = pv;
        pair_idx = pair;

        face_idx = face;
    }

    vector<int> all_hE_idces_face_includes();

};
struct Circle {
    Point O;
    double r = 0.0;

    Circle() {}
    Circle(EVec2d _pos, double _r) :r(_r) {
        O.pos = _pos;
        O.fixed = true;
    }
};
struct Triangle {
    bool exist = true;
    Circle Outer;

    int id = -1;
    int hE_idx = -1;

    Triangle() {}
    Triangle(int _id, int half_edge_idx) {
        id = _id;
        hE_idx = half_edge_idx;
    }
};
/*
struct Triangle_utils { //形としてだけの3角形。外心座標や内外判定、面積計算などができる。
    EVec3d A, B, C; EVec3d AB, BC, CA; EVec3d AC, CB, BA; EVec2d G;
    Circle Outer; Circle Inner; double area = -1.0;

    //関数に詰めてもらう用
    Triangle Tri;
    halfEdge heAB, heBC, heCA;

    Triangle_utils() {}
    /*
    Triangle_utils(EVec2d _a, EVec2d _b, EVec2d _c) {
        A = _d2_3(_a); B = _d2_3(_b); C = _d2_3(_c);

        AB = B - A; BC = C - B; CA = A - C;
        BA = -AB; CB = -BC; AC = -CA;

        double tmp = 2.0 * ((B.x() - A.x()) * (C.y() - A.y()) - (B.y() - A.y()) * (C.x() - A.x()));
        double cx = ((C.y() - A.y()) * (B.x() * B.x() - A.x() * A.x() + B.y() * B.y() - A.y() * A.y()) + (A.y() - B.y()) * (C.x() * C.x() - A.x() * A.x() + C.y() * C.y() - A.y() * A.y())) / tmp;
        double cy = ((A.x() - C.x()) * (B.x() * B.x() - A.x() * A.x() + B.y() * B.y() - A.y() * A.y()) + (B.x() - A.x()) * (C.x() * C.x() - A.x() * A.x() + C.y() * C.y() - A.y() * A.y())) / tmp;
        EVec2d cent(cx, cy);

        EVec2d P = _c - _a;
        EVec2d Q = _b - _a;
        area = fabs(P.x() * Q.y() - P.y() * Q.x()) / 2.0;

        Outer.O.pos = cent;
        Outer.r = (cent - _a).norm();

    }
    Triangle_utils(Point _A, Point _B, Point _C) {
        EVec2d _a = _A.pos, _b = _B.pos, _c = _C.pos;
        A = _d2_3(_a); B = _d2_3(_b); C = _d2_3(_c);

        AB = B - A; BC = C - B; CA = A - C;
        BA = -AB; CB = -BC; AC = -CA;

        double tmp = 2.0 * ((B.x() - A.x()) * (C.y() - A.y()) - (B.y() - A.y()) * (C.x() - A.x()));
        double cx = ((C.y() - A.y()) * (B.x() * B.x() - A.x() * A.x() + B.y() * B.y() - A.y() * A.y()) + (A.y() - B.y()) * (C.x() * C.x() - A.x() * A.x() + C.y() * C.y() - A.y() * A.y())) / tmp;
        double cy = ((A.x() - C.x()) * (B.x() * B.x() - A.x() * A.x() + B.y() * B.y() - A.y() * A.y()) + (B.x() - A.x()) * (C.x() * C.x() - A.x() * A.x() + C.y() * C.y() - A.y() * A.y())) / tmp;
        EVec2d cent(cx, cy);

        EVec2d P = _c - _a;
        EVec2d Q = _b - _a;
        area = fabs(P.x() * Q.y() - P.y() * Q.x()) / 2.0;
        Outer.O.pos = cent;
        Outer.r = (cent - _a).norm();
    }
    ~Triangle_utils() {}

    EVec3d _d2_3(EVec2d _v) { return EVec3d(_v.x(), _v.y(), 0.0); }
    bool is_point_on(EVec2d _p) {
        EVec3d P = _d2_3(_p);
        EVec3d AP = P - A;
        EVec3d BP = P - B;
        EVec3d CP = P - C;

        double z1 = AB.cross(BP).z();
        double z2 = BC.cross(CP).z();
        double z3 = CA.cross(AP).z();

        if ((z1 > 0 && z2 > 0 && z3 > 0) || (z1 < 0 && z2 < 0 && z3 < 0)) return true;
        return false;
    }
    bool is_point_on(Point _p) { return is_point_on(_p.pos); }
    void init(EVec2d _a, EVec2d _b, EVec2d _c) {
        A = _d2_3(_a); B = _d2_3(_b); C = _d2_3(_c);

        AB = B - A; BC = C - B; CA = A - C;
        BA = -AB; CB = -BC; AC = -CA;

        double tmp = 2.0 * ((B.x() - A.x()) * (C.y() - A.y()) - (B.y() - A.y()) * (C.x() - A.x()));
        double cx = ((C.y() - A.y()) * (B.x() * B.x() - A.x() * A.x() + B.y() * B.y() - A.y() * A.y()) + (A.y() - B.y()) * (C.x() * C.x() - A.x() * A.x() + C.y() * C.y() - A.y() * A.y())) / tmp;
        double cy = ((A.x() - C.x()) * (B.x() * B.x() - A.x() * A.x() + B.y() * B.y() - A.y() * A.y()) + (B.x() - A.x()) * (C.x() * C.x() - A.x() * A.x() + C.y() * C.y() - A.y() * A.y())) / tmp;
        EVec2d cent(cx, cy);

        EVec2d P = _c - _a;
        EVec2d Q = _b - _a;
        area = fabs(P.x() * Q.y() - P.y() * Q.x()) / 2.0;

        G = (_a + _b + _c) / 3.0;

        Outer.O.pos = cent;
        Outer.r = (cent - _a).norm();
    }
    void init(Point _A, Point _B, Point _C) { init(_A.pos, _B.pos, _C.pos); }
};
*/

struct CVT_v2 {
    int edge_len = -1;
    int tri_len = -1;
    const double wall_scale = 1.1;
    const int wall_reso = 8;

    int nV = -1;  //DET.NumOfElementとおなじ
    int nV_exist = -1;
    double W = -1, H = -1;

    int TriSearch = 0,Flip = 0;

    std::chrono::system_clock::time_point  start;
    void Timer_start() {
        start = std::chrono::system_clock::now(); // 計測開始時間
    }
    double Timer_end() {
        auto end = std::chrono::system_clock::now();  // 計測終了時間
        double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); //処理に要した時間をミリ秒に変換
        return elapsed;
    }



    vector<Point> points;//group:1
    vector<Point> wall;//20*4 group:2
    vector<Point> VC_points; //一次的に置いておく用、後でpointsに代入
    vector<halfEdge> edges;
    vector<Triangle> triangles;
    vector<Point> SupTri_points;//3こだけ、group:3

    bool initVarify();
    void init();

    void IterStep() { //繰り返しやればLloyd
        TriSearch = 0;
        Flip = 0;
        Timer_start();
        
        reloadVC();
        resetEdge_Tri_vec();
        DT();
        VC();

        
        cout << "Dur: " << Timer_end();
        cout << "   Search = " << TriSearch;
        cout << "  Flip = " << Flip;
        cout << " , " << (double)Flip / nV << "(ave.)" << endl;
    }

    void makeSuperTriangle() {
        //MAKE super triangle

        float margin = 1.5;
        float h1 = (H / 2) * margin;
        float w1 = (W / 2) * margin;
        float r3 = 1.7320508f;

        EVec2d pA(0.0, h1 + r3 * w1), pB(-w1 - 0.66f * r3 * h1, -h1), pC(w1 + 0.66f * r3 * h1, -h1);
        Info iA = Info(3, 0), iB = Info(3, 1), iC = Info(3, 2);

        Point A(pA, 0); A._info = iA; SupTri_points.push_back(A);
        Point B(pB, 1); B._info = iB; SupTri_points.push_back(B);
        Point C(pC, 2); C._info = iC; SupTri_points.push_back(C);


        //edge
        halfEdge AB(0, iA, iB, 1, 2, 3, 0); edges.push_back(AB);
        halfEdge BC(1, iB, iC, 2, 0, 5, 0); edges.push_back(BC);
        halfEdge CA(2, iC, iA, 0, 1, 4, 0); edges.push_back(CA);

        //pairは一応設定しとく、nextもprevもないやつ
        halfEdge BA(3, iB, iA, -1, -1, 0, -1); edges.push_back(BA);
        halfEdge AC(4, iA, iC, -1, -1, 2, -1); edges.push_back(AC);
        halfEdge CB(5, iC, iB, -1, -1, 1, -1); edges.push_back(CB);

        edge_len = 6;

        //triangle
        Triangle superTriangle;
        superTriangle.id = 0;
        superTriangle.hE_idx = 0;

        triangles.push_back(superTriangle);
        tri_len = 1;
    }
    void DT(); //3角形分割実行
    void exec_DivTri_edgeFlip(Point p);
    void VC(); //ボロノイ重心を計算、座標を更新
    void calc_VC_at_point(Point p);
    void reloadVC();

    void resetEdge_Tri_vec() {
        //cout << "Reset begin" << endl;
        triangles.clear();
        edges.clear();
        makeSuperTriangle();
        //cout << "Reset END" << endl;
    }
    int getNewidx(bool addidx, int mode) {//mode 1:edge 2:tri
        int ret = -1;
        if (mode == 1) { ret = edge_len; if (addidx)++edge_len; }
        else /*if (mode == 2)*/ { ret = tri_len; if (addidx)++tri_len; }

        return ret;
    }
    Point getPointFromInfo(Info in) { //実態を返す！注意！！
        if (in.gr == 1)return points[in.id];
        else if (in.gr == 2)return wall[in.id];
        else /*if (in.gr == 3)*/return SupTri_points[in.id]; //means gr=3
    }
    void SetParamFromInfo(Info in, int _hE_idx) {
        if (in.gr == 1)points[in.id].hEdge_idx = _hE_idx;
        else if (in.gr == 2)wall[in.id].hEdge_idx = _hE_idx;
        else /*if (in.gr == 3)*/SupTri_points[in.id].hEdge_idx = _hE_idx; //means gr=3
    }
    void SetParamFromInfo(Info in, EVec2d _pos) {
        if (in.gr == 1) {
            points[in.id].set_pos(_pos);
        }
    }
    void getPoint_opposite(int hE_idx, Point& op) {

    }
    void getPoint_includes(int hE_idx, int& nxt, int& prev, Point& st, Point& to, Point& oppos) {
        nxt = edges[hE_idx].next_idx;
        prev = edges[hE_idx].prev_idx;

        st = getPointFromInfo(edges[hE_idx].st_Info);
        to = getPointFromInfo(edges[nxt].st_Info);
        oppos = getPointFromInfo(edges[prev].st_Info);
    }
    void getPoint_includes(int Tri_idx, int& e1, int& e2, int& e3, Point& a, Point& b, Point& c) {
        e1 = triangles[Tri_idx].hE_idx;
        e2 = edges[e1].next_idx;
        e3 = edges[e1].prev_idx;

        a = getPointFromInfo(edges[e1].st_Info);
        b = getPointFromInfo(edges[e2].st_Info);
        c = getPointFromInfo(edges[e3].st_Info);
    }
    void getPoint_includes(int Tri_idx, Point& a, Point& b, Point& c) {
        int e1 = triangles[Tri_idx].hE_idx;
        int e2 = edges[e1].next_idx;
        int e3 = edges[e1].prev_idx;

        a = getPointFromInfo(edges[e1].st_Info);
        b = getPointFromInfo(edges[e2].st_Info);
        c = getPointFromInfo(edges[e3].st_Info);
    }

};


/*
class Element {
public:
    int gHandle = -1;
    int W = 6, H = 6;
    double Rota = 0.0, scare = 1.0;
    int r = 200, g = 200, b = 200;

    Point p;

    Element() {
    }
    Element(char* filepath) {
        load_assign_element(filepath);
    }
    ~Element() {}
    int load_assign_element(char* filepath) {
        gHandle = LoadGraph(filepath, 0);
        GetGraphSize(gHandle, &W, &H);
        cout << filepath << " W,H = " << W << H << endl;

        return gHandle;
    }
    void Copy(const Element& src) {
        gHandle = src.gHandle;
        H = src.H;
        W = src.W;
        Rota = src.Rota;
    }
    Element& operator=(const Element& src) { Copy(src); return *this; }
    void SetPoint(Point _p) {
        p = _p;
    }
    void RNDcolor() {
        r = GetRand(200) + 55;
        g = GetRand(200) + 55;
        b = GetRand(200) + 55;

        int gr = p.group();
        if (gr == 1) { r = 0; g = 0; }
        else if (gr == 2) { g = 0; b = 0; }
        else if (gr == 3) { b = 0; r = 0; }
        else { r = 255; g = 255; b = 255; }
    }

    void draw(double cent_x = 0, double cent_y = 0) {
        //coutDx << "(x,y) =" << p.pos.x() << "," << p.pos.y() << endlDx;
        int x1 = (int)p.pos.x() - W / 2 + cent_x;
        int y1 = (int)p.pos.y() - H / 2 + cent_y;
        DrawBox(x1, y1, x1 + W, y1 + H, GetColor(r, g, b), true);
    }

    void draw(EVec2d cent2d) { draw(cent2d.x(), cent2d.y()); }
};

struct edge_visual { //each edge.pair is ignored
    int line_width = 1;
    int r = 200, g = 200, b = 200;

    Point to_Point, st_Point;

    void RNDcolor() {
        r = GetRand(200) + 55;
        g = GetRand(200) + 55;
        b = GetRand(200) + 55;
    }

    void draw() {
        double x1 = st_Point.pos.x();
        double y1 = st_Point.pos.y();
        double x2 = to_Point.pos.x();
        double y2 = to_Point.pos.y();

        unsigned int color;
        if (to_Point.group() == 3 || st_Point.group() == 3)color = GetColor(0, 255, 0);
        else if (to_Point.group() == 2 || st_Point.group() == 2)color = GetColor(255, 0, 0);
        else color = GetColor(0, 0, 255);

        DrawLine(x1, y1, x2, y2, color, 1);
    }

    void apply_point(Point _st, Point _to) {
        to_Point = _to;
        st_Point = _st;
    }

};

class m_set_util {
public:
    int NumOfElement = 500;
    int wall_size = -1;
    int window_H = 1000;
    int window_W = 1600;
    vector<Element> element;
    vector<Element> wall_element;
    vector<edge_visual> edge_line;

    vector<unsigned int>RNDcolor;

    EVec2d cent;//相対座標
    CVT_v2 m_cvt;

    m_set_util() {
        //init();
    };
    m_set_util(int _h, int _w) {
        window_H = _h;
        window_W = _w;
    }
    void set_members(int N, double H, double W);
    void init() {// CVT_v2のinit後に行う point　丸ごと代入
        cent = EVec2d(window_W / 2, window_H / 2);

        m_cvt.nV = NumOfElement;
        m_cvt.W = 800;
        m_cvt.H = 800;

        m_cvt.init();

        wall_size = m_cvt.wall.size();
        int edge_size = m_cvt.edges.size();

        wall_element.resize(wall_size);
        edge_line.resize(edge_size);

        RNDcolor.resize(NumOfElement);

        for (int i = 0; i < NumOfElement; ++i) {
            int r = GetRand(200) + 55, g = GetRand(200) + 55, b = GetRand(200) + 55;
            RNDcolor[i] = GetColor(r, g, b);
        }

        element.resize(NumOfElement);

        for (int i = 0; i < NumOfElement; ++i) { element[i].p = m_cvt.points[i]; element[i].RNDcolor(); }
        for (int i = 0; i < wall_size; ++i) { wall_element[i].p = m_cvt.wall[i]; wall_element[i].RNDcolor(); }
        for (int i = 0; i < edge_size; ++i) {
            Point st = m_cvt.getPointFromInfo(m_cvt.edges[i].st_Info);
            Point to = m_cvt.getPointFromInfo(m_cvt.edges[i].st_Info);
            edge_line[i].apply_point(st, to);

        }

    }
    //Triangle_utils tu;
    void dbg() {
        EVec2d a(-10, -10), b(10, -10), c(20, 10);
        Point A(a, -1), B(b, -1), C(c, -1);
        tu.init(a, b, c);

        draw_Edge_Point2Point(A, B);
        draw_Edge_Point2Point(B, C);
        draw_Edge_Point2Point(C, A);

        draw_Sq(tu.Outer.O, 2);
        draw_Circ(tu.Outer.O, tu.Outer.r);

        cout << "Area : " << tu.area << endl;
        cout << "O:(x,y) = " << tu.Outer.O.pos.x() << "," << tu.Outer.O.pos.y() << endl;


    }
    void draw_ALL();
    void draw_Circ(Point _p, double r) {
        int x1 = (int)_p.pos.x() + cent.x();
        int y1 = (int)_p.pos.y() + cent.y();
        DrawCircle(x1, y1, r, GetColor(0, 200, 200), 0, 1);
    }
    void ColorScaleBCGYR(double in_value, int& r, int& g, int& b) {
        // 0.0～1.0 の範囲の値をサーモグラフィみたいな色にする
        // 0.0                    1.0
        // 青    水    緑    黄    赤
        // 最小値以下 = 青
        // 最大値以上 = 赤
        int ret;
        int a = 255;    // alpha値
        //int r, g, b;    // RGB値
        double  value = in_value;
        double  tmp_val = cos(4 * M_PI * value);
        int     col_val = (int)((-tmp_val / 2 + 0.5) * 255);
        if (value >= (4.0 / 4.0)) { r = 255;     g = 0;       b = 0; }   // 赤
        else if (value >= (3.0 / 4.0)) { r = 255;     g = col_val; b = 0; }   // 黄～赤
        else if (value >= (2.0 / 4.0)) { r = col_val; g = 255;     b = 0; }   // 緑～黄
        else if (value >= (1.0 / 4.0)) { r = 0;       g = 255;     b = col_val; }   // 水～緑
        else if (value >= (0.0 / 4.0)) { r = 0;       g = col_val; b = 255; }   // 青～水
        else { r = 0;       g = 0;       b = 255; }   // 青
        ret = (a & 0x000000FF) << 24
            | (r & 0x000000FF) << 16
            | (g & 0x000000FF) << 8
            | (b & 0x000000FF);
    }
    void draw_Sq_idxCol(Point _p, int half_A) {
        double Mx = 0;
        if (_p.group() == 3)Mx = 3;
        else if (_p.group() == 2)Mx = m_cvt.wall.size();
        else if (_p.group() == 1)Mx = m_cvt.points.size();
        double col = (double)_p.id() / Mx;
        int r, g, b;
        ColorScaleBCGYR(col, r, g, b);

        //draw_Sq(_p, half_A, GetColor(r, g, b));

        //DrawFormatString(_p.pos.x()+cent.x() + half_A*2, _p.pos.y() + cent.y() + half_A*2, GetColor(255, 255, 255), "%d", _p.id());
        draw_Sq(_p, half_A, RNDcolor[_p.id()]);
    }
    void draw_Sq(Point _p, int half_A, int _c = -1) {

        unsigned int color;
        if (_p.group() == 3)color = GetColor(0, 255, 0);
        else if (_p.group() == 2)color = GetColor(255, 0, 0);
        else if (_p.group() == 1)color = GetColor(0, 0, 255);
        else color = GetColor(255, 0, 255);

        int x1 = (int)_p.pos.x() + cent.x();
        int y1 = (int)_p.pos.y() + cent.y();

        if (_c != -1)color = _c;
        DrawBox(x1 - half_A, y1 - half_A, x1 + half_A, y1 + half_A, color, true);
    }
    void draw_Edge_Point2Point(Point st_Point, Point to_Point) {
        double x1 = st_Point.pos.x() + cent.x();
        double y1 = st_Point.pos.y() + cent.y();
        double x2 = to_Point.pos.x() + cent.x();
        double y2 = to_Point.pos.y() + cent.y();

        unsigned int color;
        if (to_Point.group() == 3 || st_Point.group() == 3)color = GetColor(0, 255, 0);
        else if (to_Point.group() == 2 || st_Point.group() == 2)color = GetColor(255, 0, 0);
        else if (to_Point.group() == 1 || st_Point.group() == 1)color = GetColor(0, 0, 255);
        else color = GetColor(255, 0, 255);

        DrawLine(x1, y1, x2, y2, color, 1);
    }
    void draw_Edge(halfEdge e, int _c = -1) {
        Point st_Point = m_cvt.getPointFromInfo(e.st_Info);
        Point to_Point = m_cvt.getPointFromInfo(e.to_Info);

        double x1 = st_Point.pos.x() + cent.x();
        double y1 = st_Point.pos.y() + cent.y();
        double x2 = to_Point.pos.x() + cent.x();
        double y2 = to_Point.pos.y() + cent.y();

        unsigned int color;
        if (to_Point.group() == 3 || st_Point.group() == 3)color = GetColor(0, 200, 0);
        else if (to_Point.group() == 2 || st_Point.group() == 2)color = GetColor(255, 0, 0);
        else color = GetColor(0, 30, 255);

        if (_c != -1)color = _c;

        DrawLine(x1, y1, x2, y2, color, 1);
    }
    void mainLoop();
    void test();
};
*/