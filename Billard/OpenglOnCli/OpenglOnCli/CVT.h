#pragma once
#include "COMMON\stdc++.h"
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
	float x = 0.0f, y = 0.0f;

	Point() { x = 0.0f; y = 0.0f; }
	Point(float _x, float _y) :x(_x), y(_y) {}
	

	bool operator==(Point p) {
		return (x == p.x) && (y == p.y);
	}

	Point operator+(const Point& p) { return Point(x + p.x, y + p.y); }
	Point operator-(const Point& p) { return Point(x - p.x, y - p.y); }
	Point operator*(double k) { return Point(k * x, k * y); }
	Point operator/(double k) { return Point(x / k, y / k); }


	float Dist(Point tg) {
		float dx = fabsf(x - tg.x);
		float dy = fabsf(y - tg.y);
		return sqrtf(dx * dx + dy * dy);
	}

	float Cross_z(Point tg) {//クロス関のz成分.平面上の座標なのでこれしかいらない.
		return x * tg.y - y * tg.x;
	}

	float dot(Point p) {
		return x * p.x + y * p.y;
	}

	void DBG(string name) {
		cout << fixed << setprecision(3) <<name<<"("<< x << "," << y <<")"<< endl;
	}
};

struct Edge {// v0 ---> v1
	Edge() {}
	Edge(Point _v0, Point _v1) :v0(_v0), v1(_v1) {}
	bool operator==(Edge e) {
		return ((v0 == e.v0) && (v1 == e.v1)) || ((v0 == e.v1) && (v1 == e.v0));//逆順も考慮しておく
	}
	Point v0, v1;

	void DBG(string name) {
		cout << name << endl;
		v0.DBG("v0");v1.DBG("v1");
	}
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
	float S = 0.0f;

	float MyCol[4];

	bool exist = true;

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
		S = calcArea();

		MyCol[0] = (float)rand() / RAND_MAX;
		MyCol[1] = (float)rand() / RAND_MAX;
		MyCol[2] = (float)rand() / RAND_MAX;
		MyCol[3] = 0.3f;
	}

	//Triangle getTriangleShareEdge(Edge e);//全探索...

	float calcArea();

	Circle getOuterCentroid();//外心を返す
	Edge OppositeEdge(Point p);//反対の辺を返す
	Point OppositePonit(Edge e);//反対の頂点を返す
	bool isIncluded(Point p);//この頂点は内包されているか？？
	bool isIncluded(Edge e);//この辺を含んでいるか？？
	bool isPointShared(Triangle t);//この頂点を含んでいるか？？
	bool isEdgeShared(Triangle T);

	bool operator==(Triangle &t);
	void del();//delete this triangles


	void DBG(string name) {
		cout << name << endl;
		A.DBG("A"); B.DBG("B"); C.DBG("C");
		Outer.center.DBG("O");
		if (exist)cout << "exist" << endl;
		else cout << "deleted" << endl;
	}

};


struct CVT {
	/*
	座標を格納しておく点列
	
	巨大3角形をつくる
	
	（全探索）指定の点を外報する三角形検索
	点で三角形を分割
	
	*/

	const float eps = 0.05f;
	bool isFinished = false;//収束が終わったかどうか

	Point errP;
	Edge errE;
	Triangle errT;

	int nV = 200;
	float width = 10.0f;
	float height = 10.0f;

	Triangle hugeTriangle;
	vector<Point> points;
	vector<Point> VCpoints;


	vector<Triangle> triangles; //現存している三角形リスト
	//vector<bool> TriangleExit; //３角形が追加されたらこいつも
	
	//list<Triangle> triangles;
	//unordered_set<Triangle> triangles;

	CVT() {
		Init(40.0f, 40.0f, 25);
	}

	CVT(float _w, float _h, int _numberOfVertices) {
		Init(_w, _h, _numberOfVertices);
	}


	int getTriangleIdxShareEdge(Edge e);// ALL SEACH find ONE Triangle
	int getTriangleIdxWrapingPoint(Point p);// all search find ONE Triangle

	bool isSuperTriEdge(Edge e);

	vector<int> getALLTriangleIdxSharePoint(Point p);//All search :find ALL Triangle s.t. shares given point
	int sameTriangleIdx(Triangle T);// All search from "triangles(vector)"

	void DivideTriangleAtPoint(int Triangleidx,Point p);//(指定したインデックスの)三角形をその点で分割
	void Flip_FromTriangle(Triangle t);

	void delTriangle(Triangle t);
	void delTriangle_idx(int idx);//somosomo三角形消してるのはFlipの時だけだった
	void addTriangle(Triangle t);

	void Assignpoints(vector<Point> pts);
	void AssignCentroid();

	void Init(float w, float h,int n);

	void DelaunayTrianglaion();

	void DBG_idx_DelTri(int tgtIdx);
	void DBG_idx_CentVolo(int tgtIdx);

	vector<int> FixOrder(vector<int> poly);
	vector<Point> CentroidVoronoi();

	void IterStep();
	void Randomize();
};

