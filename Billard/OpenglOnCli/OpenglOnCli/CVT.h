#pragma once
#include "COMMON\stdc++.h"
//#include <unordered_set>
//#include <unordered_set>
using namespace std;

/*
namespace std {
	// �W�����C�u������ struct hash<>�Ƃ����f�[�^���n�b�V��������֐��I�u�W�F�N�g�̃e���v���[�g��񋟂��Ă���
	// ����͍ŏ������{�^�ɑ΂��Ă͓��ꉻ����Ă���̂ł��̂܂܎g����(��Lhash_combine���Ŏg�p)
	// ����N���X�̏ꍇ�͎����œ��ꉻ���Aoperator()���`���Ă��B
	template<>
	struct hash<Triangle> {
	public:
		size_t operator()(const Triangle& t)const {

			//�N���X�̃����o�̒l���ꂼ��ɂ��ăn�b�V���������āA�������������Ĉ�̃n�b�V���l�ɂ���
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
	//��{�^�Ɋւ���n�b�V�������͕W�����C�u�������񋟂��Ă���
	std::hash<T> primitive_type_hash;

	//���������n�b�V������������B���̃R�[�h��boost���̂��g�p����
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

	float Cross_z(Point tg) {//�N���X�ւ�z����.���ʏ�̍��W�Ȃ̂ł��ꂵ������Ȃ�.
		return x * tg.y - y * tg.x;
	}
};

struct Edge {// v0 ---> v1
	Edge() {}
	Edge(Point _v0, Point _v1) :v0(_v0), v1(_v1) {}
	bool operator==(Edge e) {
		return (v0 == e.v0) && (v1 == e.v1) || (v0 == e.v1) && (v1 == e.v0);//�t�����l�����Ă���
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

	//Triangle getTriangleShareEdge(Edge e);//�S�T��...

	Circle getOuterCentroid();//�O�S��Ԃ�
	Edge OppositeEdge(Point p);//���΂̕ӂ�Ԃ�
	Point OppositePonit(Edge e);//���΂̒��_��Ԃ�
	


	bool isIncluded(Point p);//���̒��_�͓����Ă��邩�H�H
	
	bool isIncluded(Edge e);//���̕ӂ��܂�ł��邩�H�H
	bool isPointShared(Triangle t);//���̒��_���܂�ł��邩�H�H







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

	//return;//��O
}

Point Triangle::OppositePonit(Edge e) {
	if (e == AB)return C;
	if (e == BC)return A;
	if (e == CA)return B;

	//return;//��O
}

bool Triangle::isIncluded(Point P) {
	Point vAB = Point(B.x - A.x, B.y - A.y);
	Point vBC = Point(C.x - B.x, C.y - B.y);
	Point vCA = Point(A.x - C.x, A.y - C.y);

	float ABxP = vAB.Cross_z(P);//�O�ς�z�����������Ă���
	float BCxP = vBC.Cross_z(P);
	float CAxP = vCA.Cross_z(P);

	if ((ABxP > 0 && BCxP > 0 && CAxP > 0) || (ABxP < 0 && BCxP < 0 && CAxP < 0)) {//�S�����������Ȃ�OK
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





/*�R�s�y���Ă����A�Ȃ�Ƃ�����
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
	���W���i�[���Ă����_��
	
	����3�p�`������
	
	�i�S�T���j�w��̓_���O�񂷂�O�p�`����
	�_�ŎO�p�`�𕪊�
	
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
	
	void DivideTriangleAtPoint(Triangle t,Point p);//�O�p�`�����̓_�ŕ���
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

//�������H�v����΁A���_���炻��ɑ�����O�p�`�̏W�������Ƃ߂���??

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

void DelaunayTriangulation::Init(float w, float h,int n){ //��_�̗�Ƌ���R�p�`�𐶐�
	nV = n;

	width = w;
	height = h;

	srand(time(NULL));
	points.resize(nV);

	// ����ȂR�p�`�����
	hugeTriangle = Triangle(Point(0.0,h+w) , Point(w+h*2.0,-h),Point(-w - h * 2.0, -h));
	triangles.insert(hugeTriangle);


	//��_���i�[�i�����_���Ɍ��߂�j
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
