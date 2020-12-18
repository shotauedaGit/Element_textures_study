#include "stdafx.h"
#include "CVT_v2.h"

EVec3d _d2_3_f(EVec2d _v) { return EVec3d(_v.x(), _v.y(), 0.0); }

bool is_point_on_f(EVec2d _p, EVec2d _a, EVec2d _b, EVec2d _c) {
    EVec3d P = _d2_3_f(_p), A = _d2_3_f(_a), B = _d2_3_f(_b), C = _d2_3_f(_c);
    EVec3d AP = P - A;
    EVec3d BP = P - B;
    EVec3d CP = P - C;
    double z1 = (B - A).cross(BP).z();
    double z2 = (C - B).cross(CP).z();
    double z3 = (A - C).cross(AP).z();
    if ((z1 > 0 && z2 > 0 && z3 > 0) || (z1 < 0 && z2 < 0 && z3 < 0)) return true;
    return false;
}
bool is_point_on_f(Point p, Point a, Point b, Point c) { return is_point_on_f(p.pos, a.pos, b.pos, c.pos); }

EVec2d ret_G_f(EVec2d _a, EVec2d _b, EVec2d _c) { return ((_a + _b + _c) / 3.0); }
EVec2d ret_G_f(Point a, Point b, Point c) { return ret_G_f(a.pos, b.pos, c.pos); }

double calc_area_f(EVec2d _a, EVec2d _b, EVec2d _c) {
    EVec2d P = _c - _a;
    EVec2d Q = _b - _a;
    double area = fabs(P.x() * Q.y() - P.y() * Q.x()) / 2.0;
    return area;
}
double calc_area_f(Point a, Point b, Point c) { return calc_area_f(a.pos, b.pos, c.pos); }

Circle calc_Outer_f(EVec2d a, EVec2d b, EVec2d c) {
    Circle ret;
    double tmp = 2.0 * ((b.x() - a.x()) * (c.y() - a.y()) - (b.y() - a.y()) * (c.x() - a.x()));
    double cx = ((c.y() - a.y()) * (b.x() * b.x() - a.x() * a.x() + b.y() * b.y() - a.y() * a.y()) + (a.y() - b.y()) * (c.x() * c.x() - a.x() * a.x() + c.y() * c.y() - a.y() * a.y())) / tmp;
    double cy = ((a.x() - c.x()) * (b.x() * b.x() - a.x() * a.x() + b.y() * b.y() - a.y() * a.y()) + (b.x() - a.x()) * (c.x() * c.x() - a.x() * a.x() + c.y() * c.y() - a.y() * a.y())) / tmp;
    EVec2d cent(cx, cy);

    ret.O.pos = cent;
    ret.r = (cent - a).norm();

    return ret;
}
Circle calc_Outer_f(Point a, Point b, Point c) { return calc_Outer_f(a.pos, b.pos, c.pos); }





bool CVT_v2::initVarify() {
    if (nV == -1 || nV_exist == -1 || W == -1 || H == -1)
        return true;
}
void CVT_v2::init() {

    // POINT RESET
    srand(time(NULL));
    points.resize(nV); VC_points.resize(nV);
    double adj = 0.98;

    int point_id = 0;

    //*
    int cur = 0;
    int sq = sqrt(nV) + 1;
    double _w = (W*adj) / sq , _h = (H * adj) / sq;
    for (int i = 0; i < sq; ++i) {
        for (int j = 0; j < sq; ++j) {
            double x = ((double)rand() / RAND_MAX) * _w;
            double y = ((double)rand() / RAND_MAX) * _h;
            
            x += (-W * adj / 2) + _w * i;
            y += (-H * adj / 2) + _h * j;
            
            points[cur].pos = EVec2d(x,y);
            points[cur]._info = Info(1, cur);
            cur++;
            if (cur >= nV) break;
        }
        if (cur >= nV) break;
    }
    //*/


    for (int i = 0; i < nV; ++i) {
        /*
        double x = ((double)rand() / RAND_MAX) * W - (W / 2);
        double y = ((double)rand() / RAND_MAX) * H - (H / 2);
        points[i].pos = EVec2d(x * adj, y * adj);
        points[i]._info = Info(1, i);
        //*/
        
        //copy points
        VC_points[i] = points[i];//初回のreloadVCで変化を及ぼさないように
    }

    //MAKE WALL
    double l = -(W * wall_scale / 2);
    double r = (W * wall_scale / 2);
    double u = (H * wall_scale / 2);
    double d = -(H * wall_scale / 2);
    EVec2d _p;
    EVec2d lu(l, u), ru(r, u), ld(l, d), rd(r, d);
    //end
    //壁を建設
    // Group 2
    int wall_id = 0; Point wall_point; wall_point.set_group(2);
    for (int i = 1; i < wall_reso; ++i) {
        _p = (lu * ((double)wall_reso - i) + ru * i) / wall_reso;
        wall_point.pos = _p; wall_point.set_id(wall_id); wall.push_back(wall_point);
        ++wall_id;
    }
    for (int i = 0; i < wall_reso; ++i) {
        _p = (ru * ((double)wall_reso - i) + rd * i) / wall_reso;
        wall_point.pos = _p; wall_point.set_id(wall_id); wall.push_back(wall_point);
        ++wall_id;
    }
    for (int i = 0; i < wall_reso; ++i) {
        _p = (rd * ((double)wall_reso - i) + ld * i) / wall_reso;
        wall_point.pos = _p; wall_point.set_id(wall_id); wall.push_back(wall_point);
        ++wall_id;
    }
    for (int i = 0; i < wall_reso; ++i) {
        _p = (ld * ((double)wall_reso - i) + lu * i) / wall_reso;
        wall_point.pos = _p; wall_point.set_id(wall_id); wall.push_back(wall_point);
        ++wall_id;
    }
    {
        wall_point.pos = lu; wall_point.set_id(wall_id); wall.push_back(wall_point);
        ++wall_id;
    }
    //MAKE super triangle
    //*
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
    //*/
}
void CVT_v2::DT() {
    Point p;
    for (int i = 0; i < points.size(); ++i) {
        p = points[i];
        //cout << "[DivTri] point idx : " << p._info.gr << "," << p._info.id << "  triSize: " << triangles.size() << endl;
        exec_DivTri_edgeFlip(p);
    }
    //cout << "[DT point]" << endl;
    for (int i = 0; i < wall.size(); ++i) {
        p = wall[i];
        //cout << "[DivTri] wall idx : " << p._info.gr << "," << p._info.id << "  triSize: " << triangles.size() << endl;
        exec_DivTri_edgeFlip(p);
    }
    //cout << "[DT wall]" << endl;
}
void CVT_v2::exec_DivTri_edgeFlip(Point p) {
    if (p.exist == false)return;
    
    //DivTri
    //Triangle_utils tu;
    Point a, b, c;
    int e1 = -1, e2 = -1, e3 = -1;
    int nT = triangles.size();
    int divide_tgt_idx = -1;

    int cur;
    for (int i = 0; i < nT; ++i) {
        //cout << "\n TriSearch: " << i << "th";
        TriSearch++;
        //cur = i;
        cur = nT - 1 - i;

        if (!triangles[cur].exist)continue;
        int _e1 = -1, _e2 = -1, _e3 = -1;
        Point _a, _b, _c;
        getPoint_includes(cur, _e1, _e2, _e3, _a, _b, _c);
        //tu.init(_a, _b, _c);

        if (/*tu.is_point_on(p)*/ is_point_on_f(p,_a,_b,_c)) {
            //cout << " is TGT!!";
            a = _a; b = _b; c = _c;
            e1 = _e1; e2 = _e2; e3 = _e3;

            divide_tgt_idx = cur; break;
        }
        else
        {
            //cout << " is Not TGT!!";
        }
    }

    /*if(divide_tgt_idx != -1)*/triangles[divide_tgt_idx].exist = false;
    //cout << "TriSize: " << triangles.size() << "edgeSize: " << edges.size() << endl;
    {
        int inTr1 = getNewidx(true, 2);
        int inTr2 = getNewidx(true, 2);
        int inTr3 = getNewidx(true, 2);

        int ine1 = getNewidx(true, 1);
        int ine2 = getNewidx(true, 1);
        int ine3 = getNewidx(true, 1);
        int ine4 = getNewidx(true, 1);
        int ine5 = getNewidx(true, 1);
        int ine6 = getNewidx(true, 1);

        halfEdge ne1(ine1, p._info, a._info, e1, ine4, ine6, inTr1); edges.push_back(ne1);
        halfEdge ne2(ine2, p._info, b._info, e2, ine5, ine4, inTr2); edges.push_back(ne2);
        halfEdge ne3(ine3, p._info, c._info, e3, ine6, ine5, inTr3); edges.push_back(ne3);

        halfEdge ne4(ine4, b._info, p._info, ine1, e1, ine2, inTr1); edges.push_back(ne4);
        halfEdge ne5(ine5, c._info, p._info, ine2, e2, ine3, inTr2); edges.push_back(ne5);
        halfEdge ne6(ine6, a._info, p._info, ine3, e3, ine1, inTr3); edges.push_back(ne6);

        Triangle nTr1(inTr1, e1); nTr1.exist = true; triangles.push_back(nTr1);
        Triangle nTr2(inTr2, e2); nTr2.exist = true; triangles.push_back(nTr2);
        Triangle nTr3(inTr3, e3); nTr3.exist = true; triangles.push_back(nTr3);

        edges[e1].face_idx = inTr1; edges[e1].next_idx = ine4; edges[e1].prev_idx = ine1;
        edges[e2].face_idx = inTr2; edges[e2].next_idx = ine5; edges[e2].prev_idx = ine2;
        edges[e3].face_idx = inTr3; edges[e3].next_idx = ine6; edges[e3].prev_idx = ine3;

        SetParamFromInfo(p._info, ine1);//新しく追加した頂点からのedge

    }
    /*
    cout << "-DivTri Done-" << endl;
    cout << "TriSize: " << triangles.size() << "edgeSize: " << edges.size() << endl;
    //cout << "[DivTri] point idx : " << p._info.gr << "," << p._info.id << endl;
    cout << "a,b,c = " << a._info.gr << "," << a._info.id << "  ";
    cout << b._info.gr << "," << b._info.id << "  ";
    cout << c._info.gr << "," << c._info.id << "  " << endl;
    cout << "e1,e2,e3 = " << e1 << ", " << e2 << ", " << e3 << endl;
    //*/
    if (e1 == -1 || e2 == -1 || e3 == -1)cout << "[DivTri] point idx : " << p._info.gr << "," << p._info.id << "3角形が見つかってない" << endl;
    //EdgeFlip
    stack<int> st;
    st.push(e1); st.push(e2); st.push(e3);

    while (!st.empty()) {

        Flip++;

        Point A, A1, B1, B, C, D;
        int iAB = -1, iBC = -1, iCA = -1, iBA = -1, iAD = -1, iDB = -1;
        int e_idx = st.top(); st.pop(); iAB = e_idx;

        if ((edges[iAB].st_Info.gr > 1 && edges[iAB].to_Info.gr > 1)) {
            if ((edges[iAB].st_Info.gr == 3 && edges[iAB].to_Info.gr == 3))continue;
        }

        iBA = edges[iAB].pair_idx;
        /*
        cout << "AB(st_top) : " << iAB << " BA : " << iBA;
        cout << "  Ai" << (edges[iAB].st_Info).gr << "," << (edges[iAB].st_Info).id << "  ";
        cout << "(iBA_st)" << (edges[iBA].st_Info).gr << "," << (edges[iBA].st_Info).id << "  ";
        cout <<"(iAB_to)" <<(edges[iAB].to_Info).gr << "," << (edges[iAB].to_Info).id << "  "<<endl;
        //*/
        if (edges[iBA].face_idx == -1)continue;

        getPoint_includes(iAB, iBC, iCA, A, B, C);   //A1==A B1==B のはず
        getPoint_includes(iBA, iAD, iDB, B1, A1, D); //

        if (A._info != A1._info || B._info != B1._info) { return; }
        //ここはいる前にリセットしたい

        //tu.init(A, B, C);
        Circle Outer = calc_Outer_f(A, B, C);
        double dist = (Outer.O.pos - D.pos).norm();

        if (dist < Outer.r) {
            //FLIP
            //cout << "(Flipped!)" << endl;
            int iT1 = edges[iAB].face_idx, iT2 = edges[iBA].face_idx;
            if (iT2 == -1)break;

            //assign T1,T2,half_edges ds related
            int iDC = edges[iAB].id, iCD = edges[iBA].id;

            Info in;//Point を編集するため、Infoを取り出す
            //Assign T1 edge point(Inst)
            halfEdge DC(iDC, D._info, C._info, iCA, iAD, iCD, iT1); edges[iDC] = DC;

            edges[iAD].st_Info = A._info; edges[iAD].to_Info = D._info;
            edges[iAD].next_idx = iDC; edges[iAD].face_idx = iT1;
            edges[iAD].prev_idx = iCA;

            edges[iCA].st_Info = C._info; edges[iCA].to_Info = A._info;
            edges[iCA].next_idx = iAD; edges[iCA].face_idx = iT1;
            edges[iCA].prev_idx = iDC;

            triangles[iT1].hE_idx = iDC;
            D.hEdge_idx = iDC;

            //SetParamFromInfoで置き換え
            /*
            in = D._info;
            if (in.gr == 1)points[in.id] = D;
            else if (in.gr == 2)wall[in.id] = D;
            else if (in.gr == 3) //SupTri_points[in.id] = D; //means gr=3
            //*/
            SetParamFromInfo(D._info, iDC);
            SetParamFromInfo(A._info, iAD);

            //Assign T2 edge point(Inst)
            halfEdge CD(iCD, C._info, D._info, iDB, iBC, iDC, iT2); edges[iCD] = CD;

            edges[iDB].st_Info = D._info; edges[iDB].to_Info = B._info;
            edges[iDB].next_idx = iBC; edges[iDB].face_idx = iT2;
            edges[iDB].prev_idx = iCD;

            edges[iBC].st_Info = B._info; edges[iBC].to_Info = C._info;
            edges[iBC].next_idx = iCD; edges[iBC].face_idx = iT2;
            edges[iBC].prev_idx = iDB;

            triangles[iT2].hE_idx = iCD;
            C.hEdge_idx = iCD;

            //SetParamFromInfoで置き換え
            /*
            in = C._info;
            if (in.gr == 1)points[in.id] = C;
            else if (in.gr == 2)wall[in.id] = C;
            else if (in.gr == 3)SupTri_points[in.id] = C; //means gr=3
            //*/
            SetParamFromInfo(C._info, iCD);
            SetParamFromInfo(B._info, iBC);

            st.push(iAD);
            st.push(iDB);
        }
        else
        {

        }
    }

    //WaitKey();
}
void CVT_v2::VC() {
    Point p;
    for (int i = 0; i < points.size(); ++i) {
        p = points[i];
        //cout << "[VC] point idx : " << p._info.gr << "," << p._info.id << endl;
        calc_VC_at_point(p);
    }
    //cout << "[VC done]" << endl;
}
void CVT_v2::reloadVC() {
    for (int i = 0; i < points.size(); ++i) {
        points[i].set_pos(VC_points[i].pos);
        //cout <<"[VC] assigned : " << points[i]._info.gr << "," << points[i]._info.id << endl;
    }
}
void CVT_v2::calc_VC_at_point(Point p) {
    if (p.exist == false)return;

    //Triangle_utils tu;
    double curArea = -1, totArea = 0.0;
    EVec2d curG, G_vc(0.0, 0.0);
    /*
    Info q1i = edges[p.hEdge_idx].to_Info,p_nx_i = q1i;
    Point q1 = getPointFromInfo(q1i);
    Info q2i = edges[q1.hEdge_idx].to_Info;
    Point q2 = getPointFromInfo(q2i);
    */
    Point q1, q2, q3;
    halfEdge curFace_edge = edges[p.hEdge_idx];
    halfEdge curPair = edges[curFace_edge.pair_idx];
    halfEdge curP_Next = edges[curPair.next_idx];
    int curT_idx = curFace_edge.face_idx;
    int firstT_idx = curT_idx;
    vector<Point> Outer_vec;

    //cout << "Triangle around p:" << p._info.gr << "," << p._info.id << ")" << endl;

    while (1) {
        /*
        cout<<"  見ている△: "<< curT_idx<< "  次△: " << curP_Next.face_idx <<endl;
        cout << "  自分から出るE        : (" << curFace_edge.st_Info.gr << "," << curFace_edge.st_Info.id << ")->(" << curFace_edge.to_Info.gr << "," << curFace_edge.to_Info.id << ")" << endl;
        cout << "  自分に向かうE        : (" << curPair.st_Info.gr << "," << curPair.st_Info.id << ")->(" << curPair.to_Info.gr << "," << curPair.to_Info.id << ")" << endl;
        cout << "  それのnx(次のcurFace): (" << curP_Next.st_Info.gr << "," << curP_Next.st_Info.id << ")->(" << curP_Next.to_Info.gr << "," << curP_Next.to_Info.id << ")" << endl;
        //*/
        getPoint_includes(curT_idx, q1, q2, q3);
        /*
        cout << "q1,q2,q3 = (" << q1._info.gr << "," << q1._info.id << ")";
        cout << "(" << q2._info.gr << ", " << q2._info.id << ")";
        cout << "(" << q3._info.gr << ", " << q3._info.id << ")" <<endl;
        //*/
        //tu.init(q1, q2, q3);
        curArea = calc_area_f(q1, q2, q3);
        
        //curArea = tu.area;
        
        //totArea += curArea;
        Point Opb = calc_Outer_f(q1, q2, q3).O;
        Outer_vec.push_back(Opb);

        curPair = edges[curFace_edge.pair_idx];
        curP_Next = edges[curPair.next_idx];
        curFace_edge = curP_Next;

        curT_idx = curFace_edge.face_idx;
        if (curT_idx == firstT_idx)break;
    }

    int numT = Outer_vec.size();
    Point O1, O2;
    for (int i = 0; i < numT; ++i) {
        int i2 = (i + 1) % numT;
        O1 = Outer_vec[i];
        O2 = Outer_vec[i2];

        //tu.init(p, O1, O2);
        curArea = calc_area_f(p, O1, O2);
        curG = ret_G_f(p, O1, O2);

        G_vc += (curArea)*curG;
        totArea += curArea;
    }
    G_vc /= totArea;
    //cout << "G_vc = (" << G_vc.x() << ", " << G_vc.y() <<") NumT = "<< numT<< endl;

    if (p.group() == 1) {
        p.pos = G_vc;
        double _x = p.pos.x();
        double _y = p.pos.y();

        if (fabs(_x) <= W / 2 && fabs(_y) <= H / 2) {
            VC_points[p.id()] = p;
        }
        else {
            VC_points[p.id()].set_pos(points[p.id()].pos);
        }
    }
}