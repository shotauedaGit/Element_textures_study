#include "stdafx.h"
#include "CVT.h"


/*
Triangle Triangle::getTriangleShareEdge(Edge) {

}
*/

float Triangle::calcArea() {
	Point P = C - A;
	Point Q = B - A;
	return (fabsf(P.x * Q.y - P.y * Q.x)) / 2.0f;
}


void Triangle::del() {
	exist = false;
}

Circle Triangle::getOuterCentroid() {
	float tmp = 2.0 * ((B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x));
	float cx = ((C.y - A.y) * (B.x * B.x - A.x * A.x + B.y * B.y - A.y * A.y) + (A.y - B.y) * (C.x * C.x - A.x * A.x + C.y * C.y - A.y * A.y)) / tmp;
	float cy = ((A.x - C.x) * (B.x * B.x - A.x * A.x + B.y * B.y - A.y * A.y) + (B.x - A.x) * (C.x * C.x - A.x * A.x + C.y * C.y - A.y * A.y)) / tmp;

	Point cent(cx, cy);
	float cr = cent.Dist(A);
	Circle ret(cent, cr);
	return ret;
}

Edge Triangle::OppositeEdge(Point p) {
	if (p == A)return BC;
	if (p == B)return CA;
	if (p == C)return AB;

	//return;//例外
}

Point Triangle::OppositePonit(Edge e) {
	if (e == AB)return C;
	if (e == BC)return A;
	if (e == CA)return B;

	//return;//例外
}

bool Triangle::isIncluded(Point P) {
	Point vAB = Point(A.x - C.x, A.y - C.y);
	Point vBC = Point(B.x - A.x, B.y - A.y);
	Point vCA = Point(C.x - A.x, C.y - A.y);

	float ABxP = (B-A).Cross_z(P-B);//外積のz成分を持ってくる
	float BCxP = (C-B).Cross_z(P-C);
	float CAxP = (A-C).Cross_z(P-A);

	if ((ABxP > 0 && BCxP > 0 && CAxP > 0) || (ABxP < 0 && BCxP < 0 && CAxP < 0)) {//全部同じ方向ならOK
		return true;
	}
	else {
		return false;
	}
}

bool Triangle::isPointShared(Triangle t) {
	return  (t.A == A || t.A == B || t.A == C) ||
		(t.B == A || t.B == B || t.B == C) ||
		(t.C == A || t.C == B || t.C == C);
}

bool Triangle::isPointShared(Point p) {
	return  (p == A || p == B || p == C);
}


bool Triangle::operator==(Triangle& t) {
	return (AB == t.AB || AB == t.BC || AB == t.CA) && (BC == t.AB || BC == t.BC || BC == t.CA);
}


bool Triangle::isIncluded(Edge e) {
	//Edge rAB(B, A);
	//Edge rBC(C, B);
	//Edge rCA(A, C);
	return (AB == e)||(BC == e)||(CA == e)/*(rAB == e) || (rBC == e) || (rCA == e)*/;
}

bool Triangle::isEdgeShared(Triangle T) {
	return isIncluded(T.AB) || isIncluded(T.BC) || isIncluded(T.CA);
}





/*コピペしてきた、なんとかする
struct HalfEdge {
	// invalid or undefined = -1
	int opposite_halfedge{ -1 };  // index to the halfedges array.
	int next_halfedge{ -1 };      // index to the halfedges array.
	// int64_t vertex_index{-1}; // vertex index at the start of the edge
	int face_index{ -1 };  // index to face indices
	int edge_index{ -1 };  // index to edge indices
};
*/



bool CVT::isSuperTriEdge(Edge e) {
	return (hugeTriangle.isIncluded(e));
}






int CVT::sameTriangleIdx(Triangle T) {//exist check ok
	int lim = triangles.size();
	for (int i = lim - 1; i >= 0; --i) {
		if (triangles[i] == T && triangles[i].exist)return i;
	}
	cout << "Same Triangle :: not found" << endl;
	return 0;
}


int CVT::getTriangleIdxShareEdge(Edge e) {//exist check ok
	int lim = triangles.size();
	for (int i = lim - 1; i >= 0; --i) {
		if (triangles[i].isIncluded(e) && triangles[i].exist)return i;
	}
	cout << "Share Edge :: not found" << endl;
	return 0;
}

int CVT::getTriangleIdxWrapingPoint(Point p) {//exist check ok
	int lim = triangles.size();
	for (int i = lim - 1; i >= 0; --i) {
		if (triangles[i].isIncluded(p) && triangles[i].exist)return i;
	}
	cout << "Wrap point :: not found" << endl;
	return 0;
}

vector<int> CVT::getALLTriangleIdxSharePoint(Point p) {//exist check ok
	vector<int> ret;
	for (int i = 0; i < triangles.size(); ++i) {
		if (triangles[i].exist && triangles[i].isPointShared(p)) {
			if (!triangles[i].isEdgeShared(hugeTriangle))ret.push_back(i);
		}
	}
	return ret;
}

vector<int> CVT::FixOrder(vector<int> poly) {//getALLTriangleIdxSharePointの順番を周回する順に並べ替える
	//cout << "fixorder" << endl;

	int n = poly.size();
	vector<bool> isfound(n+10, false);
	vector<int> ret;

	ret.push_back(poly[0]);
	isfound[0] = true;

	for (int i = 0; i < n - 1; ++i) {
		Triangle curTri = triangles[ret[i]];
		for (int j = 0; j < n; ++j) {
			if (isfound[j] == false && curTri.isEdgeShared(triangles[poly[j]])) {
				isfound[j] = true;
				ret.push_back(poly[j]);
			}
		}
	}

	return ret;
}



void CVT::DivideTriangleAtPoint(int Triangleidx, Point p) {
	if (Triangleidx != -1) {
		triangles[Triangleidx].exist = false;
		//cout << "delete " << Triangleidx << " th triangle" << endl;
	}
	else {
		//DBG
	}

	Triangle t = triangles[Triangleidx];

	Triangle ABP(t.A, t.B, p);
	Triangle BCP(t.B, t.C, p);
	Triangle ACP(t.A, t.C, p);

	addTriangle(ABP);
	addTriangle(BCP);
	addTriangle(ACP);

	//delTriangle_idx(Triangleidx);
}

void CVT::Flip_FromTriangle(Triangle t) {
	stack<Edge> st;
	st.push(t.AB);
	st.push(t.BC);
	st.push(t.CA);

	while (!st.empty()) {
		//cout << "    Flipping st.size() = " << st.size() << endl;
		Edge e = st.top(); st.pop();
		//e.DBG("Now AB");

		if (isSuperTriEdge(e))continue;

		int cnt = 0;
		Triangle T1, T2;
		int idx_T1 = -1;//見つからないときバグってる
		int idx_T2 = -1;

		int lim = triangles.size();
		for (int i = lim - 1; i >= 0; --i) {
			if (triangles[i].exist &&triangles[i].isIncluded(e) && cnt == 0) { T1 = triangles[i]; idx_T1 = i; ++cnt;}
			else if (triangles[i].exist &&triangles[i].isIncluded(e) && cnt == 1) { T2 = triangles[i]; idx_T2 = i; break; }
		}

		//cout << "    flipping::ABC,ABD ::" << idx_T1 << "," << idx_T2 << endl;
		if (idx_T2 == -1) {
			//cout << "      idx_T2 == -1 continue !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			continue;
		}

		Point A = e.v0, B = e.v1;
		Point C = T1.OppositePonit(e);
		Point D = T2.OppositePonit(e);

		Circle O = T1.Outer;

		if (O.center.Dist(D) < O.r) { //FLIP!!!!!!!!!!
			//cout << "FLIP occured" << endl;

			delTriangle_idx(idx_T1);
			delTriangle_idx(idx_T2);

			addTriangle(Triangle(A, C, D));
			addTriangle(Triangle(B, C, D));

			st.push(Edge(A, D));
			st.push(Edge(B, D));
			st.push(Edge(C, D));
			st.push(Edge(A, C));
		}
	}
}

//ここを工夫すれば、頂点からそれに属する三角形の集合をもとめられる??

void CVT::delTriangle(Triangle t) {//使わないかも（消去するとき、その直前に全探索して探してきてるものなので）
	int idx = sameTriangleIdx(t);
	triangles[idx].exist = false;
}

void CVT::delTriangle_idx(int idx) { triangles[idx].exist = false; }

void CVT::addTriangle(Triangle t) {
	triangles.push_back(t);
}

void CVT::Assignpoints(vector<Point> pts) {//足りないやつ入れたら何もしないよ
	if (pts.size() >= nV) {
		for (int i = 0; i < nV; ++i) {
			points[i] = pts[i];
		}
	}
}

void CVT::AssignCentroid() {
	Assignpoints(VCpoints);
}




void CVT::Init(float w, float h, int n) { //母点の列と巨大３角形を生成
	nV = n;

	width = w;
	height = h;

	cout << "CVT::Init" << endl;

	
	srand(time(NULL));
	points.resize(nV);
	VCpoints.resize(nV);

	//ここってRandmize()でいい..................

	// 巨大な３角形を作る
	
	/*
	triangles.clear();
	hugeTriangle = Triangle(Point(0.0, h + w ), Point(w + h * 2.0, -h), Point(-w - h * 2.0, -h));
	addTriangle(hugeTriangle);
	cout << "CVT::Init vector resized" << endl;

	//母点を格納（ランダムに決める）
	for (int i = 0; i < nV; ++i) {
		float rx = ((float)rand() / RAND_MAX) * width * 2.0 - width;
		float ry = ((float)rand() / RAND_MAX) * height * 2.0 - height;

		points[i] = Point(rx*0.95, ry*0.95);
	}

	doneDelauny = -1;
	doneVolonoi = -1;
	*/

	Randomize();
}

void CVT::InitTriangleVector() {
	triangles.clear();
	hugeTriangle = Triangle(Point(0.0, height + width), Point(width + height * 2.0, -height), Point(-width - height * 2.0, -height));
	addTriangle(hugeTriangle);
}

void CVT::Randomize() {//isFinishedを初期化している
	isFinished = false;

	/*
	triangles.clear();
	hugeTriangle = Triangle(Point(0.0, height + width), Point(width + height * 2.0, -height), Point(-width - height * 2.0, -height));
	addTriangle(hugeTriangle);
	*/

	for (int i = 0; i < nV; ++i) {
		float rx = ((float)rand() / RAND_MAX) * width * 2.0 - width;
		float ry = ((float)rand() / RAND_MAX) * height * 2.0 - height;
		points[i] = Point(rx * 0.95, ry * 0.95);
	}
	doneDelauny = -1;
	doneVolonoi = -1;
}

void CVT::DelaunayTrianglaion() {

	int i = 0;
	for (Point pi : points) {
		//cout << "*** " << i << "  th point processing *********" << endl;
		int stidx = getTriangleIdxWrapingPoint(pi);
		//cout << "    DT::stidx" <<stidx<<"/ num of tri::"<<triangles.size()<< endl;
		//pi.DBG("    DT::cur Point");
		DivideTriangleAtPoint(stidx, pi);

		//cout << "all tri info(cur) " << endl;
		//int id = 0;
		//for (Triangle& t : triangles) { if(t.exist)t.DBG("ti"); id++; }

		//cout << "start Flip" << endl;
		Flip_FromTriangle(triangles[stidx]);


		++i;
	}
}

vector<Point> CVT::CentroidVoronoi() {//ボロノイ重心を求めていく
	vector<Point> ctv(nV);

	float change = 0.0f;

	for (int i = 0; i < points.size(); ++i) {
		//cout << "VOLONOI ** " << i << "th points processing" << endl;

		Point pi = points[i];//  i番目の母点
		Point cent(0.0f, 0.0f);

		vector<int> IdxAroundp = getALLTriangleIdxSharePoint(pi);
		vector<int> fixed = FixOrder(IdxAroundp);

		int n = fixed.size();
		vector<Triangle> poly;
		float polyS = 0.0f;

		Point p, q;
		Triangle tj;
		for (int j = 0; j < n; ++j) {
			p = triangles[fixed[j]].Outer.center;
			q = triangles[fixed[(j + 1) % n]].Outer.center;

			tj = Triangle(pi, p, q);
			cent = cent + ((pi+p+q)/3.0)*tj.S;
			polyS += tj.S;
		}




		cent = cent / polyS;//i番目の母点のボロノイ重心
		
		if (hugeTriangle.isIncluded(cent))ctv[i] = cent;
		else ctv[i] = points[i];
		
		change += pi.Dist(cent);
	}

	if (change < eps * nV)isFinished = true;
	return ctv;
}


void CVT::IterStep() {// step by step 使用作っとく？？
	/*
	cout << "all tri info" << endl;
	int id = 0;
	for (Triangle& t : triangles) { t.DBG("ti"); id++;}
	*/

	InitTriangleVector();
	cout << "CVT::IterStep" << endl;
	DelaunayTrianglaion();
	cout << "CVT::Triangulation Done" << endl;
	VCpoints = CentroidVoronoi();
	cout << "CVT::Centroid calclated" << endl;
	AssignCentroid();
	cout << "CVT::Centroid assigned (as Point)" << endl;
}

void CVT::DBG_idx_DelTri(int tgtidx) {//execute whole processing around i th point.

	Point pi = points[tgtidx];
	//cout << "*** " << tgtidx << "  th point processing *********" << endl;
	int stidx = getTriangleIdxWrapingPoint(pi);
	//cout << "    DT::stidx" << stidx << "/ num of tri::" << triangles.size() << endl;
	pi.DBG("    DT::cur Point");
	DivideTriangleAtPoint(stidx, pi);

	//cout << "all tri info(cur) " << endl;
	int id = 0;
	for (Triangle& t : triangles) { if (t.exist)t.DBG("ti"); id++; }

	//cout << "start Flip" << endl;
	Flip_FromTriangle(triangles[stidx]);
	doneDelauny = tgtidx;

}

Point CVT::DBG_idx_CentVolo(int tgtidx) {//returns volonoi centroid of i th point.
	//cout << "VOLONOI ** " << tgtidx << "th points processing" << endl;

	Point pi = points[tgtidx];//  i番目の母点
	Point cent(0.0f, 0.0f);

	vector<int> IdxAroundp = getALLTriangleIdxSharePoint(pi);
	vector<int> fixed = FixOrder(IdxAroundp);

	int n = fixed.size();
	vector<Triangle> poly;
	float polyS = 0.0f;
	for (int j = 0; j < n; ++j) {
		Point p = triangles[fixed[j]].Outer.center;
		Point q = triangles[fixed[(j + 1) % n]].Outer.center;

		Triangle tj(pi, p, q);
		cent = cent + ((pi + p + q) / 3.0) * tj.S;
		polyS += tj.S;
	}

	cent = cent / polyS;//i番目の母点のボロノイ重心
	VCpoints[tgtidx] = cent;
	doneVolonoi = tgtidx;
	return cent;
}

