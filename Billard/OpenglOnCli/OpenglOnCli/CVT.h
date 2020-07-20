#pragma once
#include "COMMON\stdc++.h"
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

	float Cross_z(Point tg) {//�N���X�ւ�z����.���ʏ�̍��W�Ȃ̂ł��ꂵ������Ȃ�.
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
		return ((v0 == e.v0) && (v1 == e.v1)) || ((v0 == e.v1) && (v1 == e.v0));//�t�����l�����Ă���
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

	//Triangle getTriangleShareEdge(Edge e);//�S�T��...

	float calcArea();

	Circle getOuterCentroid();//�O�S��Ԃ�
	Edge OppositeEdge(Point p);//���΂̕ӂ�Ԃ�
	Point OppositePonit(Edge e);//���΂̒��_��Ԃ�
	bool isIncluded(Point p);//���̒��_�͓����Ă��邩�H�H
	bool isIncluded(Edge e);//���̕ӂ��܂�ł��邩�H�H
	bool isPointShared(Triangle t);//���̒��_���܂�ł��邩�H�H
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
	���W���i�[���Ă����_��
	
	����3�p�`������
	
	�i�S�T���j�w��̓_���O�񂷂�O�p�`����
	�_�ŎO�p�`�𕪊�
	
	*/

	const float eps = 0.05f;
	bool isFinished = false;//�������I��������ǂ���

	Point errP;
	Edge errE;
	Triangle errT;

	int nV = 200;
	float width = 10.0f;
	float height = 10.0f;

	Triangle hugeTriangle;
	vector<Point> points;
	vector<Point> VCpoints;


	vector<Triangle> triangles; //�������Ă���O�p�`���X�g
	//vector<bool> TriangleExit; //�R�p�`���ǉ����ꂽ�炱����
	
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

	void DivideTriangleAtPoint(int Triangleidx,Point p);//(�w�肵���C���f�b�N�X��)�O�p�`�����̓_�ŕ���
	void Flip_FromTriangle(Triangle t);

	void delTriangle(Triangle t);
	void delTriangle_idx(int idx);//somosomo�O�p�`�����Ă�̂�Flip�̎�����������
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

