#pragma once
#include "COMMON\stdc++.h"
//#include <unordered_set>
//#include <unordered_set>
using namespace std;

/*
namespace std {
	// 標準ライブラリは struct hash<>というデータをハッシュ化する関数オブジェクトのテンプレートを提供している
	// これは最初から基本型に対しては特殊化されているのでそのまま使える(上記hash_combine内で使用)
	// 自作クラスの場合は自分で特殊化し、operator()を定義してやる。
	template<>
	struct hash<Triangle> {
	public:
		size_t operator()(const Triangle& t)const {

			//クラスのメンバの値それぞれについてハッシュ生成して、それらを結合して一つのハッシュ値にする
			std::size_t seed = 0;
			hash_combine(seed, );
			hash_combine(seed, );
			hash_combine(seed, );
			return seed;
		}
	};
}

template<typename T>
void hash_combine(size_t& seed, T const& v) {
	//基本型に関するハッシュ生成は標準ライブラリが提供している
	std::hash<T> primitive_type_hash;

	//生成したハッシュを合成する。このコードはboostものを使用する
	seed ^= primitive_type_hash(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

*/




struct Point {
	Point() {}
	Point(float _x,float _y) :x(_x), y(_y) {}
	float x=0.0,y=0.0;

	bool operator==(Point p) {
		return (x == p.x) && (y == p.y);
	}

	float Dist(Point tg) {
		float dx = fabsf(x - tg.x);
		float dy = fabsf(y - tg.y);
		return sqrtf(dx * dx + dy * dy);
	}

	float Cross_z(Point tg) {//クロス関のz成分.平面上の座標なのでこれしかいらない.
		return x * tg.y - y * tg.x;
	}
};

struct Edge {// v0 ---> v1
	Edge() {}
	Edge(Point _v0, Point _v1) :v0(_v0), v1(_v1) {}
	bool operator==(Edge e) {
		return (v0 == e.v0) && (v1 == e.v1) || (v0 == e.v1) && (v1 == e.v0);//逆順も考慮しておく
	}
	Point v0, v1;
};

struct Circle {
	Circle() {}
	Circle(Point c, float _r) :center(c), r(_r) {}
	
	bool operator==(Circle c) {
		return (center == c.center) && (r == c.r);
	}
	Point center;
	float r = 0.0;
};

struct Triangle {
	Point A, B, C;
	Edge AB, BC, CA;
	Circle Outer;

	Triangle() {}
	Triangle(Point _a,Point _b,Point _c):A(_a), B(_b), C(_c){
		/*
		AB.v0 = A; AB.v1 = B;
		BC.v0 = B; BC.v1 = C;
		CA.v0 = C; CA.v1 = A;
		*/

		AB = Edge(A, B);
		BC = Edge(B, C);
		CA = Edge(C, A);

		Outer = getOuterCentroid();
	}

	//Triangle getTriangleShareEdge(Edge e);//全探索...

	Circle getOuterCentroid();//外心を返す
	Edge OppositeEdge(Point p);//反対の辺を返す
	Point OppositePonit(Edge e);//反対の頂点を返す
	


	bool isIncluded(Point p);//この頂点は内包されているか？？
	
	bool isIncluded(Edge e);//この辺を含んでいるか？？
	bool isPointShared(Triangle t);//この頂点を含んでいるか？？







};

/*
Triangle Triangle::getTriangleShareEdge(Edge) {
	
}
*/

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
	Point vAB = Point(B.x - A.x, B.y - A.y);
	Point vBC = Point(C.x - B.x, C.y - B.y);
	Point vCA = Point(A.x - C.x, A.y - C.y);

	float ABxP = vAB.Cross_z(P);//外積のz成分を持ってくる
	float BCxP = vBC.Cross_z(P);
	float CAxP = vCA.Cross_z(P);

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

bool Triangle::isIncluded(Edge e) {
	return (AB == e) || (BC == e) || (CA == e);
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



struct DelaunayTriangulation {
	/*
	座標を格納しておく点列
	
	巨大3角形をつくる
	
	（全探索）指定の点を外報する三角形検索
	点で三角形を分割
	
	*/

	Point errP;
	Edge errE;
	Triangle errT;

	int nV = 200;

	float width = 10.0f;
	float height = 10.0f;

	Triangle hugeTriangle;
	vector<Point> points;
	//vector<Triangle> triangles;
	unordered_set<Triangle> triangles;


	Triangle getTriangleShareEdge(Edge e);// ALL SEACH
	Triangle getTriangleWrapingPoint(Point p);// all search
	
	void DivideTriangleAtPoint(Triangle t,Point p);//三角形をその点で分割
	void Flip_FromTriangle(Triangle t);

	void delTriangle(Triangle t);
	void addTriangle(Triangle t);

	void Assignpoints(vector<Point> pts);
	void Init(float w, float h,int n);

	void execute();

};


Triangle DelaunayTriangulation::getTriangleShareEdge(Edge e) {
	for (Triangle t : triangles) {
		if (t.isIncluded(e))return t;
	}

	return errT;
}

Triangle DelaunayTriangulation::getTriangleWrapingPoint(Point p) {
	for (Triangle t : triangles) {
		if (t.isIncluded(p))return t;
	}

	return errT;
}

void DelaunayTriangulation::DivideTriangleAtPoint(Triangle t,Point p){
	triangles.erase(t);

	Triangle ABP(t.A, t.B, p);
	Triangle BCP(t.B, t.C, p);
	Triangle ACP(t.A, t.C, p);

	triangles.insert(ABP);
	triangles.insert(BCP);
	triangles.insert(ACP);
}

void DelaunayTriangulation::Flip_FromTriangle(Triangle t) {
	stack<Edge> st;
	st.push(t.AB);
	st.push(t.BC);
	st.push(t.CA);

	while (!st.empty()) {
		Edge e = st.top(); st.pop();

		int cnt = 0;
		Triangle T1, T2;
		for (Triangle t : triangles) {
			if (t.isIncluded(e) && cnt == 0) { T1 = t; ++cnt; }
			if (t.isIncluded(e) && cnt == 1) { T2 = t; break; }
		}

		Point D = T2.OppositePonit(e);
		Circle O = T1.getOuterCentroid();

		if (O.center.Dist(D) < O.r) { //FLIP!!!!!!!!!!
			delTriangle(T1);
			delTriangle(T2);

			addTriangle(Triangle(T1.A, T1.C, D));
			addTriangle(Triangle(T1.B, T1.C, D));
		
			st.push(Edge(T1.A, D));
			st.push(Edge(T1.B, D));
			st.push(Edge(T1.C, D));
			st.push(Edge(T1.A, T1.C));
		}
	}
}

//ここを工夫すれば、頂点からそれに属する三角形の集合をもとめられる??

void DelaunayTriangulation::delTriangle(Triangle t) {
	triangles.erase(t);
}
void DelaunayTriangulation::addTriangle(Triangle t) {
	triangles.insert(t);
}

void DelaunayTriangulation::Assignpoints(vector<Point> pts) {
	if (pts.size() >= nV) {
		for (int i = 0; i < nV; ++i) {
			points[i] = pts[i];
		}
	}
}

void DelaunayTriangulation::Init(float w, float h,int n){ //母点の列と巨大３角形を生成
	nV = n;

	width = w;
	height = h;

	srand(time(NULL));
	points.resize(nV);

	// 巨大な３角形を作る
	hugeTriangle = Triangle(Point(0.0,h+w) , Point(w+h*2.0,-h),Point(-w - h * 2.0, -h));
	triangles.insert(hugeTriangle);


	//母点を格納（ランダムに決める）
	for (int i = 0; i < nV; ++i) {
		float rx = ((float)rand() / RAND_MAX) * width * 2.0 - width;
		float ry = ((float)rand() / RAND_MAX) * height * 2.0 - height;
	
		points[i] = Point(rx, ry);
	}
}



void DelaunayTriangulation::execute() {
	
	for (Point pi : points) {
		Triangle T = getTriangleWrapingPoint(pi);
		DivideTriangleAtPoint(T, pi);
		Flip_FromTriangle(T);
	}
}






struct CentroidVoronoi {

};
