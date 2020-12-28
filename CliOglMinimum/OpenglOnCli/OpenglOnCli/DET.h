#pragma once
#include "COMMON/stdc++.h"
#include "COMMON/tmath.h"
#include "CVT_v2.h"
#include <unordered_set>
#include <unordered_map>
#define KEY_A 65
#define KEY_B 66
#define KEY_C 67
#define KEY_D 68
#define KEY_E 69
#define KEY_F 70

#define KEY_Z 90
#define KEY_X 88

#define KEY_SPACE 32


using namespace std;
/*Interface memo
* DET 状態
* マウスホバーしている
*   Abst elements
*   Sample elements
*   

* サンプル入力
*   マウスホバーで要素に枠（Abst elements,sample elements(のうち表示されてるやつ) ）

* 合成・イテレーション後
*   

*/

struct DiscreteElement {
    int type_id = 0,id = 0;//いる！？！？
    GLint type = -1;
    int pic_w = -1,pic_h = -1;

    double scare = 1.0;
    double H = 1.0,W = 1.0;
    double rot = 0.0; //Rotation angle
    
    Point point;
    EVec2d cent, ab_pos = EVec2d(-10.0, 10.0);//position;
    double Z_draw = 0.0;

    EVec4d mycolor = EVec4d(1, 1, 1, 1);
    DiscreteElement() { cent = EVec2d(0, 0); };
    DiscreteElement(const DiscreteElement& src) { Copy(src); };
    DiscreteElement(GLint _texID, int _w, int _h) {
        cent = EVec2d(0, 0);
        type = _texID;
        W = (double)_w / max(_w, _h);
        H = (double)_h / max(_w, _h);

        pic_w = _w;
        pic_h = _h;
    }
    DiscreteElement(GLint _texID,int type_id,int id, int _w, int _h) { //_w = picture width
        cent = EVec2d(0, 0);
        type = _texID;
        W = (double)_w / max(_w, _h);
        H = (double)_h / max(_w, _h);

        pic_w = _w;
        pic_h = _h;
    };
    ~DiscreteElement() {};

    DiscreteElement& operator=(const DiscreteElement& src) { Copy(src); return *this; }
    void Copy(const DiscreteElement &src) {
        Copy_without_Point(src);
        this->point = src.point;
    }
    void Copy_without_Point(const DiscreteElement& src) {
        this->type = src.type;
        this->cent = src.cent;
        this->pic_w = src.pic_w;
        this->pic_h = src.pic_h;

        this->scare = src.scare;
        this->ab_pos = src.ab_pos;
        this->rot = src.rot;

        this->H = src.H;
        this->W = src.W;
    }

    void Draw();
    void Draw_SelectedFlame(double line_width);
    void SetColor(const EVec4d& _col) { mycolor = _col; }
    void SetA(double _a) { mycolor.w() = _a; }
    void SetZ(double _z) { Z_draw = _z; }

    void SetSize_direct(double _W, double _H) { W = _W; H = _H;}
    void SetTexture(DiscreteElement src) {
        type = src.type;
        type_id = src.type_id;
        
        H = src.H; W = src.W;
        mycolor = src.mycolor;
    }
    EVec2d get_relt_pos() { return point.pos; }
    EVec2d get_abst_pos() { return point.pos+cent; }
    void DBG() {
        cout << "[DE_DBG]type: " << type;
        cout << "cent: ( " << cent.x() << " , " << cent.y() << " ) ";
        cout << "pos: ( "<<point.pos.x()<<" , "<< point.pos.y()<< " ) ";
        cout << "id = " << point._info.id;
        cout << endl;
    }
};
struct interface_data{

    bool left_down = false;
    bool left_up = false;
    bool left = false;

    int E_abst_hover = -1; double abst_hover_th = 1.0;
    int E_smpl_hover = -1; double smpl_hover_th = 1.0;
    int E_edit_hover = -1; double edit_hover_th = 1.0;//3

    int E_abst_just_cl = -1;
    int E_smpl_just_cl = -1;

    int E_edit_just_cl = -1;//3

    int E_abst_last_cl = -1;
    int E_smpl_last_cl = -1;
    
    int E_edit_last_cl = -1;//3

    int E_abst_drag = -1;
    int E_smpl_drag = -1;

    int E_edit_drag = -1;//3

    int s2_edit1 = -1;
    int s2_edit2 = -1;

    void update_step(bool down,bool up,bool hold) {
        left_down = down;
        left_up = up;
        left = hold;

        if (left_down) {
                E_abst_just_cl = E_abst_hover;
                E_abst_drag = E_abst_hover;
                E_abst_last_cl = E_abst_hover;
                
                E_smpl_just_cl = E_smpl_hover;
                E_smpl_drag = E_smpl_hover;
                E_smpl_last_cl = E_smpl_hover;

                E_edit_just_cl = E_edit_hover;
                E_edit_drag = E_edit_hover;
                E_edit_last_cl = E_edit_hover;
        }
        else {
            if (left_up) {
                E_abst_drag = -1;
                E_smpl_drag = -1;
                E_edit_drag = -1;
            }
            
            E_abst_just_cl = -1;
            E_smpl_just_cl = -1;
            E_edit_just_cl = -1;
        }
    }
};

struct edit_elements {
    //2つの要素、周辺要素（描画のみ）
    //2つの要素にマウスホバーしながらのキー操作で実行
    //
    //要素自身をドラッグして位置変更
    //パレットからドラッグしてきたら、それは種類変更
    double draw_scare=1.0;
    

    //1 or 2 (3 : ほかの要素に対して行っている)
    int hover_idx = -1;
    int draging_idx = -1;
    int just_cl_idx = -1;
    int last_cl_idx = -1;
    
    bool isEditing = false;
    DiscreteElement pointer;

    bool exist_edit_1 = true;//片方は存在するように
    bool exist_edit_2 = true;

    DiscreteElement E_A0, E_B0;
    EVec2d A0, B0, M0=EVec2d(-15,-5), AB, ABt;//OLD pos
    double ABl,ABl_real;
    vector<DiscreteElement> neibhor;

    //utils
    double theta;//A0B0の仰角

    double p_A0 = -1.0 , q_A0 = 0.0;//合成に使う係数
    double p_B0 = 1.0, q_B0 = 0.0;//合成に使う係数

    edit_elements() {
    }

    void start_edit(DiscreteElement _A0, DiscreteElement _B0, vector<DiscreteElement> arround);
    void end_edit() { isEditing = false;}

    void update_pq();//ドラッグしている間pointerの位置から計算し続ける
    void update_input_info(DiscreteElement p, int hover, int just_cl, int last_cl, int drag);

    //void exec_interaction(DiscreteElement& Ai,int Bi_idx);
    pair<DiscreteElement, DiscreteElement> exec_interaction(DiscreteElement Ai, DiscreteElement Bi);



    void select_E_del(int tgt);//1 or 2
    void step();
    void redraw_all();
};


struct DET {//stands Dis
    EVec2d sample_cent = EVec2d(0.0,0.0);
    double src_w = 5.0,src_h = 5.0;
    double sample_w = 17.0,sample_h = 17.0;

    double selected_threshold = 1.0;
    int IterReps = 20;
    bool hover_synth_button = false,is_synth_done = false;
    int SynthCount = 0;
    int default_sample_texture;

    //配列の番号でアクセスしたい
    int next_Abst_element_Idx = 0;
    int next_out_elements_Idx = 0;
    int next_sample_elements_Idx = 0;

    //state 0,3(要素の置き換えを行う場合)
    int selected_Abst_element_Idx = -1;
    int current_Abst_element_Idx = -1;
    
    //state 0(要素の置く場所),2(何を選ぼうとしているのか)
    int selected_sample_element_Idx = -1;

    //state 2
    int current_sample_element_Idx_1 = -1; //2つ選択してからことが始まるので
    int current_sample_element_Idx_2 = -1;

    CVT_v2 sample_tex;
    DET() { 
            pointer = DiscreteElement(-1, 0.3, 0.3);
            pointer.SetColor(EVec4d(1, 0, 0, 1));
            pointer.SetSize_direct(0.1, 0.1);
            pointer.point.set_pos(EVec2d(0, 0));
            pointer.cent = EVec2d(0, 0);

            pointer_copy = DiscreteElement(-1, 0.3, 0.3);
            pointer_copy.SetColor(EVec4d(1, 0, 0, 1));
            pointer_copy.SetSize_direct(0.1, 0.1);
            pointer_copy.point.set_pos(EVec2d(0, 0));
            pointer_copy.cent = EVec2d(0, 0);



    }

    DiscreteElement pointer,pointer_copy;
    DiscreteElement Flame_Selected;
    DiscreteElement edit_element_1;
    DiscreteElement edit_element_2;

    vector<DiscreteElement> Abst_elements;
    vector<bool> is_handmade_element;

    //int NumOfout_elements = 300;
    //vector<DiscreteElement> out_elements;
    
    int NumOfsample_elements = 300;
    //int NumOfsample_elements = 600;
    vector<DiscreteElement> sample_elements;
    vector<int> handmade_element_idx;

    int NumOfTexType = 30;
    GLuint texID[30];
    void prPos(EVec2d p);
    void prPos(string txt, EVec2d p);

    void LoadTexture();
    void SetTexture(char *filepath,GLuint texID);
    void setup_sample_elements(); //outももろともしてしまいたい

    bool should_draw(int sample_element_idx) {//テクスチャ合成前：書かれているものしか選べない、合成後、すべてが描かれすべてが選べる
        if (is_synth_done) {
            return true;
        }
        else
        {
            DiscreteElement sm_e = sample_elements[sample_element_idx];
            double x = sm_e.point.pos.x();
            double y = sm_e.point.pos.y();
            if ((fabs(x) <= src_w / 2 && fabs(y) <= src_h / 2))return true;
            else return false;
        }
    }
    void TEST();

    int get_hover_Abst_element_Idx();
    int get_hover_Sample_element_Idx();
    int get_hover_Edit_element_Idx();

    void update_Point_CVT2DET();//これはこの後も使いそう
    void update_Point_DET2CVT();

    void DrawRectFlame(DiscreteElement& src);
    void DrawRectFlame(int selected_Idx,int mode);//0:abst 1:sample 2:out element's vector

    void CVT_DrawTriangle() {
        for (halfEdge e : sample_tex.edges) {
            if (e.st_Info.gr == 1 && e.to_Info.gr == 1) {
                int st = e.st_Info.id;
                int to = e.to_Info.id;

                if (!(st >= 0 && to >= 0)) {
                    cout << "[Draw Tri] invalid" << endl;
                    continue;
                }

                {
                    EVec2d a = sample_elements[st].get_abst_pos(),b = sample_elements[to].get_abst_pos();
                    glPushMatrix();
                    //glTranslatef(point.pos.x() + cent.x(), point.pos.y() + cent.y(), Z_draw);
                    glBegin(GL_LINES);
                    glColor4d(0.5, 0, 1, 1);
                    glVertex3f(a.x(), a.y(), 0.0);
                    glVertex3f(b.x(), b.y(), 0.0);

                    glPopMatrix();
                    glEnd();

                }




            }
        }
    }
    int nextIdx_process(int mode) {//0:abst 1:sample 2:out element's vector
        if (mode == 0) { next_Abst_element_Idx++; return next_Abst_element_Idx; }
        else if (mode == 1) { next_sample_elements_Idx++; return next_sample_elements_Idx; }
        else if(mode ==2){ next_out_elements_Idx++; return next_out_elements_Idx;
        }else {
            cout << "nextIdx_process input error input ::" << mode << endl;
            return -1;
        }
    }


    //texture synth系の関数
    void synthesis();
    void synth_Iter();
    int select_similar_element(DiscreteElement E, double _th);
    int get_Another_element_if_exist(EVec2d rtv_pos_Etar_i, vector<EVec2d> rtv_pos_Wtar, vector<EVec2d> rtv_pos_Wref);
    double calc_neibhor_similarlity(DiscreteElement Etar, DiscreteElement Eref);

    vector<int> type_tp_idx;//elements idx types tp(= A0 will be replaced C0 )
    vector<int> getSameTexElements_idx(GLint tp) {
        vector<int> ret;
        for (int i = 0; i < sample_elements.size(); ++i) {
            if (sample_elements[i].type == tp)ret.push_back(i);
        }
        return ret;
    }
    //rule apply系の関数

    EVec2d A0, B0, C0;// A0:基準 B0:消される方 C0:Aの移転先
    DiscreteElement C0_inst;
    //AiBi -> Ci 用の辺
    EVec2d AC, AB, ABt, A0C0;
    double ACl,ABl,ABtl,cos_CAB,sin_CAB;

    vector<pair<int, int>> ret_Combine_pair_list(DiscreteElement _A0, DiscreteElement _B0) {
        vector<pair<int, int> > ret;
        vector<pair<int, int> > ai_pair;
        unordered_set<pair<int, int>, HashPair> ert_set;

        type_tp_idx = getSameTexElements_idx(_A0.type);
        for (int ai : type_tp_idx) {
            ai_pair = sample_tex.get_pair_element(sample_elements[ai].point,-1,2);
            for (pair<int, int> p : ai_pair) {
                if (sample_elements[p.second].type == _B0.type) {
                    ret.push_back(p);
                    cout << "ai',bi': " << p.first << "," << p.second << "is type suitable" << endl;
                }
            }
        }
        return ret;
    }
    vector<pair<int, int>> exec_Combine_all(vector<pair<int, int> > aibi_idx,int mode,bool preview = false) {//first ai sec bi
        
        //double ABl_real = e_edit.ABl / e_edit.draw_scare;//((B0 - A0).normalized()).norm();
        vector<pair<int, int>> ret;
        unordered_map<int, double> Er_min;
        unordered_map<int, int> bi_idx;
        unordered_map<int, bool> isCombined;
        for (int ai : type_tp_idx) {
            isCombined[ai] = false;
            Er_min[ai] = 1e9;
            bi_idx[ai] = -1;
        }

        for (pair<int, int> p : aibi_idx) {
            int ai = p.first, bi = p.second;
            double AiBil = (sample_elements[ai].point.pos - sample_elements[bi].point.pos).norm();
            double Err = abs(AiBil - e_edit.ABl_real);
            
            if(Err < Er_min[ai]){
                Er_min[ai] = Err;
                bi_idx[ai] = bi;
            }
        }

        ret.push_back(make_pair(current_sample_element_Idx_1, current_sample_element_Idx_2));
        isCombined[current_sample_element_Idx_1] = true;
        isCombined[current_sample_element_Idx_2] = true;

        for (int ai : type_tp_idx){
            if (isCombined[ai] == true)continue;
            int bi = bi_idx[ai];
            if(isCombined[bi] == true)continue;

            cout << "ai,bi: " << ai << "," << bi;
            if (preview == true) { ret.push_back(make_pair(ai,bi));}
            else if (mode == 0)exec_Combine(ai, bi, C0_inst);
            else if (mode == 1)exec_Expand(ai, bi, 0.75);

            isCombined[ai] = true;
            isCombined[bi] = true;
        }

        return ret;
    }
    void exec_Combine(int ai, int bi, DiscreteElement C0) {
        if (bi == -1)return;
        EVec2d Ai = sample_elements[ai].point.pos, Bi = sample_elements[bi].point.pos;

        double AiBil = (sample_elements[ai].point.pos - sample_elements[bi].point.pos).norm();
        double k = ACl*AiBil / ABl*state3_scare;
        double k2 = AiBil / ABl;

        EVec2d AiBi = sample_elements[bi].point.pos - sample_elements[ai].point.pos;
        EVec2d AiBit(-AiBi.y(), AiBi.x());
        AiBi.normalize(); AiBit.normalize();
        EVec2d AiCi = (AiBi*cos_CAB + AiBit*sin_CAB) * k;

        
        EVec2d Mi = (Ai + Bi) / 2.0,AiMi = Mi-Ai;
        // AiBi
        EVec2d ui= (Bi - Ai).normalized(), vi(-ui.y(), ui.x());
        EVec2d MiCi = ui * combine_p + vi * combine_q; AiCi* k2;

        cout << "ai: "<<ai; prPos(sample_elements[ai].point.pos);
        cout << " += "; prPos(AiMi+MiCi);
        cout << "bi: " << bi << " erased" << endl;
        cout << "ci ty: " << C0.type << endl;

        sample_elements[ai].point.set_pos(calc_Ci(Ai,Bi));
        //sample_elements[ai].point.pos += AiMi + MiCi;
        //sample_elements[ai].SetTexture(C0);
        sample_elements[ai].Copy_without_Point(C0);

        sample_elements[bi].point.exist = false;
    }
    EVec2d calc_Ci(EVec2d Ai, EVec2d Bi) {
        double AiBil = (Bi - Ai).norm(),k2 = AiBil / ABl;
        EVec2d Mi = (Ai + Bi) / 2.0, AiMi = Mi - Ai;
        EVec2d ui = (Bi - Ai).normalized(), vi(-ui.y(), ui.x());
        EVec2d MiCi = ui * combine_p + vi * combine_q; MiCi *= k2;

        return Mi+MiCi;
    }
    void exec_Expand(int ai, int bi,double r) {
        if (bi == -1)return;
        EVec2d Ai = sample_elements[ai].point.pos, Bi = sample_elements[bi].point.pos;
        EVec2d m = (Ai+Bi)/2.0;
        EVec2d nAi = m * (1.0 - r) + Ai * r;
        EVec2d nBi = m * (1.0 - r) + Bi * r;

        sample_elements[ai].point.set_pos(nAi);
        sample_elements[bi].point.set_pos(nBi);

    }

    int curState = 0;
    bool btn_left = false, btn_middle = false, btn_right = false;
    bool btn_left_down = false,btn_left_up = false;

    //stores keycode just pushed/holded and released
    //curK : 押された瞬間含め離される直前まで
    int K_Down = -1, K_Up = -1,curK = -1;

    int draging_sample_element_idx = -1;

    // 0 :サンプル要素の手動配置
    // 1 :テクスチャ合成が完了するまで (合成完了後に状態２となる)(ここは一瞬)
    // 2 :合成後、（要素が2つクリックされることで状態３になる）
    // 3 :規則登録状態
    vector<DiscreteElement> around_elements_editing;
    
    vector<pair<int, int>> combine_pair;
    vector<DiscreteElement> Combine_blueprint;
    
    vector<int> state3_around_elements_editing(int idx1,int idx2) {
        vector<int> ret,nbh2;
        ret = sample_tex.get_1_neibhor_point_idx(sample_elements[idx1].point);
        nbh2 = sample_tex.get_1_neibhor_point_idx(sample_elements[idx2].point);

        for (int id : nbh2) {
            bool found = false;
            for (int reti : ret) {
                if (reti == id)found = true;
            }
            if (!found)ret.push_back(id);
        }

        for (int i = 0; i < ret.size(); i++) {
            if (ret[i] == idx1 || ret[i] == idx2) {
                ret.erase(ret.begin() + i);
            }
        }

        return ret;
    }
    EVec2d state3_translate;
    double state3_scare = 1.0;
    double combine_p = 0, combine_q = 0;


    edit_elements e_edit;
    interface_data d_inter;

    int inter_hover_abst();
    int inter_hover_smpl();
    int inter_hover_edit();//curState == 3




    void Interface_Process() {
        int sel = get_hover_Sample_element_Idx(), sel_nbh_cur1;
        int sel_Abst = get_hover_Abst_element_Idx();
        get_hover_Edit_element_Idx();

        int& cur1 = current_sample_element_Idx_1;
        int& cur2 = current_sample_element_Idx_2;
        vector<int> tmp;
        vector<DiscreteElement> arround_inst;
        double normalized_e1e2 = 2.0;
        EVec2d edit1(-15.0, -3.0);
        EVec2d tmp2d;
        if (K_Down != -1) {
            cout << "Key(Down) " << K_Down << endl;
            if (K_Down == KEY_SPACE) {
                CVT_DrawTriangle();
            }else if (K_Down == KEY_D) {
                sample_tex.DelTri();
            }
            cout << "curS : " << curState << endl;
            
        }

        //cout << "Ab : " << d_inter.E_abst_hover
        //    << "Sm : " << d_inter.E_smpl_hover
        //    << "Ed : " << d_inter.E_edit_hover << endl;

        d_inter.update_step(btn_left_down, btn_left_up, btn_left);



        if (btn_left_down) {
            {
                

            }
        }

        switch (curState){
        case 0:
            //
            //1キーが押されたら状態を1にする
            break;
        case 1:
            //ランダム合成、イテレーションまでやってしまう
            //速攻状態2にしてブレーク
            break;

        case 2:
            if (sel != -1) {
                
                //cout << "case2 : sel= " << sel << endl;
                if (K_Down == KEY_A) {
                    //sample_tex.get_pair_element(sample_elements[sel].point, -1, 100);
                }

                if (btn_left_down) {
                    if (cur1 == -1) { cur1 = sel; current_sample_element_Idx_1 = sel;
                    }
                    else if (cur2 == -1) {
                        cur2 = sel; current_sample_element_Idx_2 = sel;
                        curState = 3;

                        /*
                        edit_element_1.Copy_without_Point(sample_elements[cur1]); edit_element_1.cent = sample_elements[cur1].cent;
                        edit_element_2.Copy_without_Point(sample_elements[cur2]); edit_element_2.cent = sample_elements[cur2].cent;
                        state3_translate = edit1 - edit_element_1.point.pos;
                        state3_scare = normalized_e1e2 /((sample_elements[cur2].point.pos) - (sample_elements[cur1].point.pos)).norm();

                        edit_element_1.point.set_pos(edit1);
                        edit_element_1.SetZ(-0.05);
                        edit_element_1.scare *= state3_scare;

                        around_elements_editing.clear();
                        tmp = state3_around_elements_editing(cur1, cur2);

                        tmp2d = ((sample_elements[cur2].point.pos) - (sample_elements[cur1].point.pos)) * state3_scare;
                        edit_element_2.point.set_pos(edit1 + tmp2d);
                        edit_element_2.SetZ(-0.05);

                        edit_element_2.scare *= state3_scare;

                        combine_pair = exec_Combine_all(ret_Combine_pair_list(edit_element_1, edit_element_2),0,true);
                        Combine_blueprint.resize(combine_pair.size());

                        for (int ard : tmp) {
                            //cout << ard << " ";
                            around_elements_editing.push_back(sample_elements[ard]);
                        }
                        //cout << "case 2 to 3  scare: "<<state3_scare <<"(tmp2d.norm() = "<<tmp2d.norm()<< endl;
                        for (int i = 0; i < around_elements_editing.size(); ++i) {
                            //around_elements_editing[i].DBG();
                            tmp2d = ((around_elements_editing[i].point.pos) - (sample_elements[cur1].point.pos)) * state3_scare;
                            around_elements_editing[i].SetTexture(sample_elements[tmp[i]]);
                            around_elements_editing[i].point.set_pos(tmp2d + edit1);
                            around_elements_editing[i].SetZ(-0.05);
                            around_elements_editing[i].scare *= state3_scare;
                            around_elements_editing[i].cent = sample_elements[cur1].cent;
                        }
                        //*/
                        
                        //*
                        around_elements_editing.clear();
                        

                        tmp = state3_around_elements_editing(cur1, cur2);
                        for (int i : tmp)arround_inst.push_back(sample_elements[i]);
                        e_edit.start_edit(sample_elements[cur1], sample_elements[cur2],arround_inst);

                        //暫定的
                        edit_element_1 = e_edit.E_A0; edit_element_1.point.set_pos(e_edit.A0);
                        edit_element_2 = e_edit.E_B0; edit_element_2.point.set_pos(e_edit.B0);

                        combine_pair = exec_Combine_all(ret_Combine_pair_list(edit_element_1, edit_element_2), 0, true);
                        cout << combine_pair.size() << " pairs to be edit" << endl;
                        Combine_blueprint.resize(combine_pair.size());




                        //*/
                    }
                    else
                    {
                    }
                }
                else
                {
                    //draw rect??
                }
            }
            else
            {
                
            }
            //cout << "cur1,cur2= " << current_sample_element_Idx_1 << "," << current_sample_element_Idx_2 << endl;
            if (cur1 != -1) {
                sample_elements[cur1].Draw_SelectedFlame(2);
                if (cur2 == -1) {
                    for (int nbh_idx : sample_tex.get_1_neibhor_point_idx(sample_elements[cur1].point)) {
                        sample_elements[nbh_idx].Draw_SelectedFlame(1);
                    }
                }
            }
            if (cur2 != -1)sample_elements[cur2].Draw_SelectedFlame(2);
            //要素が2つ選択されている状態で　何かキー（TBD）が押されたら状態3へ
            break;

        case 3:
            //cout << "case 3" << endl;

            if (!e_edit.isEditing)e_edit.update_input_info(pointer, -1, -1, -1, -1);
            else e_edit.update_input_info(pointer, d_inter.E_edit_hover, d_inter.E_edit_just_cl, d_inter.E_edit_last_cl, d_inter.E_edit_drag);

            if (sel_Abst != -1) {
                if (btn_left_down) {
                    current_Abst_element_Idx = sel_Abst;
                    pointer.Copy_without_Point(Abst_elements[sel_Abst]);
                    C0_inst.Copy_without_Point(Abst_elements[sel_Abst]);

                    pointer.scare = state3_scare;
                }
            }
            else {
            }
            if (pointer.type != pointer_copy.type) {
                //C0_inst.SetTexture(pointer);

                if ((pointer.point.pos - edit_element_1.point.pos).norm() < 2.5 ||
                    (pointer.point.pos - edit_element_2.point.pos).norm() < 2.5) {

                    //prPos("pointer", pointer.point.pos);
                    //prPos("A0", edit_element_1.point.pos);
                    //prPos("B0", edit_element_2.point.pos);

                    A0 = edit_element_1.point.pos; B0 = edit_element_2.point.pos;
                    C0 = pointer.point.pos;
                    ABl = (B0 - A0).norm();

                    EVec2d M0 = (A0 + B0) / 2.0, M0C0 = C0 - M0, u0 = (B0 - A0).normalized();
                    combine_p = M0C0.x() * u0.x() + M0C0.y() * u0.y();
                    combine_q = M0C0.x() * u0.y() - M0C0.y() * u0.x();

                    for (int i = 0; i < combine_pair.size(); i++)
                    {
                        {
                            if (combine_pair[i].first == -1 || combine_pair[i].second == -1)continue;
                            EVec2d _ai = sample_elements[combine_pair[i].first].point.pos;
                            EVec2d _bi = sample_elements[combine_pair[i].second].point.pos;

                            //Combine_blueprint[i].mycolor.w() = 0.5;
                            Combine_blueprint[i].point.set_pos(calc_Ci(_ai, _bi));
                            Combine_blueprint[i].Copy_without_Point(C0_inst);
                            Combine_blueprint[i].cent = sample_cent;

                        }
                    }


                    std::string txt = "C";
                    for (int i = 0; i < Combine_blueprint.size(); i++) {
                        //txt += i;
                        //prPos(txt, Combine_blueprint[i].point.pos);
                        Combine_blueprint[i].Draw();
                    }
                }



            }

            e_edit.step();

            {
            
                if (curK == 13) {
                    vector<pair<DiscreteElement, DiscreteElement>> p_inst;
                    for (int i = 0; i < combine_pair.size();++i) {
                        pair<int, int> p = combine_pair[i];
                        //cout << " " << p.first << "," << p.second;
                        if (p.first == -1 || p.second == -1)continue;

                        p_inst.push_back(e_edit.exec_interaction(sample_elements[p.first], sample_elements[p.second]));
                    }

                    cout << p_inst.size() << " = " << combine_pair.size() << endl;
                    int adjust = 0;
                    for (int i = 0; i < combine_pair.size(); ++i) {
                        pair<int, int> p = combine_pair[i];
                        //cout << " " << p.first << "," << p.second;
                        if (p.first == -1 || p.second == -1) {
                            ++adjust;
                            continue;
                        }

                        cout << " " << p.first << "," << p.second;
                        sample_elements[p.first] = p_inst[i-adjust].first;
                        sample_elements[p.second] = p_inst[i-adjust].second;
                    }

                    cout << endl;
                    e_edit.end_edit();
                    curState = 2;

                    update_Point_DET2CVT();
                    sample_tex.DelTri();
                    update_Point_CVT2DET();
                }
            
            }

            /*
            edit_element_1.Draw(); //edit_element_1.DBG();
            edit_element_2.Draw(); //edit_element_2.DBG();
            //cout << "NBH" << endl;
            for (int i = 0; i < around_elements_editing.size(); ++i) {
                around_elements_editing[i].Draw();
                //around_elements_editing[i].DBG();
            }
            //*/

            if (btn_left_up) {
                //範囲内に入ってる
                {

                    
                    AC = C0 - A0;AB = (B0 - A0); ABt = EVec2d(-AB.y(), AB.x());
                    ACl = AC.norm(); ABl = AB.norm(); ABtl = ABt.norm();
                    cos_CAB = (AB.dot(AC)) / (ABl*ACl);sin_CAB = sqrt(1 - cos_CAB * cos_CAB);
                    A0C0 = AB.normalized()*(ACl * cos_CAB) + ABt.normalized()*(ACl * sin_CAB);
                    //*/


                    /*
                    cout <<"relat pos "<<AC.x() << " , " << AC.y() << " = " << A0C0.x() << " , " << A0C0.y() << endl;
                    cout << "AB = " << AB.x() << " , " << AB.y() <<" Real A0B0: "<<ABl/ state3_scare << endl;
                    cout << "ABt = " << ABt.x() << " , " << ABt.y() << endl;
                    //*/
                    
                    //規則登録
                    
                    if (curK == KEY_Z) {
                        exec_Combine_all(ret_Combine_pair_list(edit_element_1, edit_element_2),0);

                    }
                    else if (curK == KEY_X) {
                        exec_Combine_all(ret_Combine_pair_list(edit_element_1, edit_element_2), 1);
                        
                    }

                    if (curK == KEY_Z || curK == KEY_X) {
                        edit_element_1.scare = 1.0; edit_element_2.scare = 1.0;
                        around_elements_editing.clear();

                        cur1 = -1; cur2 = -1;
                        curState = 2;
                        update_Point_DET2CVT();
                        sample_tex.DelTri();
                        update_Point_CVT2DET();
                    }
                }
                current_Abst_element_Idx = -1;
                pointer.SetTexture(pointer_copy);
                pointer.scare = 1.0;

                

            }

            //要素の編集
            //別の場所に注目中の2要素を表示
            //規則適用ボタンが押される
            //sample_elementsに編集が加えられる
            
            //規則登録モードの終了
                //注目していた隣接要素はリセット
            //cur1 = -1;
            //cur2 = -1;
            //curState = 2;
            
            break;

        case 4://debug mode
            if (sel != -1) {
                if (btn_left_down) {
                    draging_sample_element_idx = sel;
                }
                else {
                    if (btn_left_up || !(btn_left)) {//upじゃない方はただの保険
                        draging_sample_element_idx = -1;
                    }
                    
                }

                if (btn_left) {
                    
                }
            }

            break;

        default:
            break;
        }


        //関数実行後のkeycodeは無意味
        //一瞬だけフラグがたっていてほしいので
        K_Down = -1; K_Up = -1;
        btn_left_down = false;
        btn_left_up = false;
    }

    void s0_Arrengement();// state 0 ( done )
    void s1_Synthesis(); // state 1 (automatically done)
    void s2_After_synth();   // state 2 ( mainloop -2element has sellected,go state 3)
    void s3_Apply_rule();        // state 3 (Has finished then return state 2(mainloop))
};