
#pragma once
#include<cassert>
#include<array>
#include<cmath>
#include<vector>
#include<iostream>
#include"./surface.hpp"
#include"./util.hpp"

namespace math_space{
using namespace std;

constexpr double PI = 3.14159265359;
class matrix4;
class vec4;

template<typename T, int _n>
class vec{
    friend vec4;

protected: 
    int length = _n;
    array<T, _n> q;
public:
    vec(){for(int i = 0; i < _n; i++){q[i] = 0;}};
    vec(std::initializer_list<T> l){
        assert(l.size() == _n);
        int i = 0;
        for(auto it = l.begin(); it != l.end(); it++){
            this->q[i] = *it;
            i++;
        };
    }
    
    T& operator[](int i){
        return q[i];
    }
    const T& operator[](int i) const{
        return q[i];
    }

    vec<T, _n> operator+(const vec<T, _n>& a) const{
        vec<T, _n> ans;
        for(int i = 0; i < _n; i++){
            ans[i] = q[i] + a[i];
        }
        return ans;
    }

    vec<T, _n> operator-(const vec<T, _n>& a) const{
        vec<T, _n> ans;
        for(int i = 0; i < _n; i++){
            ans[i] = q[i] - a[i];
        }
        return ans;
    }

    vec<T, _n> operator*(const vec<T, _n>& a) const{
        vec<T, _n> ans;
        for(int i = 0; i < _n; i++){
            ans[i] = a[i] * q[i];
        }
        return ans;
    }
    
    vec<T, _n> operator*(T k) const{
        vec<T, _n> ans;
        for(int i = 0; i < _n; i++){
            ans[i] = k*q[i];
        }
        return ans;
    }

    vec<T, _n> operator-() const{
        vec<T, _n> ans;
        for(int i = 0; i < _n; i++){
            ans[i] = -q[i];
        }
        return ans;
    }

    T dot(const vec<T,_n>& a) const{
        T ans = 0;
        for(int i = 0; i < _n; i++){
            ans += q[i]*a[i];
        }
        return ans;
    }

    vec<double, _n> root() const{
        vec<double, _n> ans;
        for(int i = 0; i < _n; i++){
            ans[i] = sqrt(q[i]);
        }
        return ans;
    }

    vec<double, _n> unit() const{
        vec<double,_n> ans;
        double l = this->magnitude();
        for(int i = 0; i < _n; i++){
            ans[i] = double(q[i]) / l;
        }
        return ans;
    }

    double magnitude() const{
        double c = 0;
        for(int i = 0; i < _n; i++){
            c += q[i]*q[i];
        }
        return sqrt(c);
    }

    // dot = |A| * |B| * cos(v)
    // cos(v) = dot / (|A| * |B|)

    double angle_between(vec<T, _n> a) const{
        return acos(double(dot(a)) / double(this->magnitude() * a.magnitude()));
    }

    void print_it() const{
        for(int i = 0; i < length; i++){
            cout << q[i] << " ";
        }
        cout << endl;
    }

    const array<T, _n>& _data() const{
        return this->q;
    }
};


template<typename T, const int _n, const int _m>
class matrix{
    
protected: array<vec<T,_m>, _n> M;
    
public:
    matrix(array<vec<T, _m>, _n> _M) : M(_M){};
    matrix(initializer_list<initializer_list<T>> l) {
        assert(l.size() == _n);
        int i = 0;
        for(auto it = l.begin(); it != l.end(); it++){
            this->M[i] = vec<T, _m>(*it);
            i ++;
        }
    }
    matrix(bool identity=false){
        for(int i = 0; i < _n; i++){
            M[i] = vec<T,_m>();
            if(identity) M[i][i] = 1;
        }
    };

    vec<T,_m>& operator[](int i) {
        return M[i];
    }

    template<const int ot_n, const int ot_m>
    matrix<T,_n,ot_m> operator*(matrix<T, ot_n, ot_m> ot){
        matrix<T,_n, ot_m> ans;
        for(int i = 0; i < _n; i++){
            for(int j = 0; j < ot_m; j++){
                vec<T, _m> q;
                for(int k = 0; k < ot_n; k++){
                    q[k] = ot[k][j];
                }
                ans[i][j] = q.dot(M[i]);
            }
        }
        return ans;
    }

    vec<T, _n> operator*(vec<T, _m> ot){
        vec<T, _n> ans;
        for(int i = 0; i < _n; i++){
            ans[i] = M[i].dot(ot);
        }
        return ans;
    }

    matrix<T, _n, _m> operator*(double a){
        matrix<T, _n, _m> ans;
        for(int i = 0; i < _n; i++){
            for(int j = 0; j < _m; j++){
                ans[i][j] = a * M[i][j];
            }
        }
        return ans;
    }

    matrix<T, _n, _m> operator+(matrix<T, _n, _m> a){
        matrix<T, _n, _m> ans;
        for(int i = 0; i < _n; i++){
            for(int j = 0; j < _m; j++){
                ans[i][j] = M[i][j] + a[i][j];
            }
        }
        return ans;
    }

    matrix<T, _n, _m> operator-(matrix<T,_n,_m> a){
        return (*this) + (a * (-1.0));
    }

    matrix<T, _n, _m> operator+(int a){
        matrix<T, _n, _m> ans;
        for(int i = 0; i < _n; i++){
            for(int j = 0; j < _m; j++){
                ans[i][j] += a;
            }
        }
    }

    void print_it(){
        cout << "---------------" << "\n";
        for(int i = 0; i < _n; i++){
            this->M[i].print_it();
        }
    }
};

class vec4 : public vec<double, 4>{
public:
    using vec<double, 4>::vec;
    vec4(vec<double, 4>& ot) : vec<double, 4>(ot){}
    vec4(vec<double, 4>&& ot) : vec<double, 4>(ot){}

    double magnitude() const{
        double c = 0;
        //We de not include the last 1 in the calculations
        for(int i = 0; i < 3; i++){
            c += q[i]*q[i];
        }
        return sqrt(c);
    }
};


namespace util{
    inline vec4 cross_product(const vec4& a, const vec4& b){
        return {
            a[1]*b[2] - a[2]*b[1],
            a[2]*b[0] - a[0]*b[2],
            a[0]*b[1] - a[1]*b[1],
            1
        };
    }

    inline double clamp(double x, double a, double b){
        return max(min(x, b), a);
    }

}

//Representing a transform
class matrix4 : public matrix<double,4,4> {
public:
    using matrix<double,4,4>::matrix;
    using matrix<double,4,4>::operator*;
    using matrix<double,4,4>::operator+;
    using matrix<double,4,4>::operator[];

    matrix4(const matrix<double, 4,4>& ot) : matrix(ot) {}
    matrix4(const matrix<double, 4,4>&& ot) : matrix(ot) {}

private:
    vec4 col(int j) const{
        return vec4{this->M[0][j], this->M[1][j], this->M[2][j], this->M[3][j]};
    }
    vec4 row(int i) const{
        return vec4{this->M[i][0], this->M[i][1], this->M[i][2], this->M[i][3]};
    }

public:
    inline vec4 right_vector() const {return this->col(0);}
    inline vec4 up_vector() const {return this->col(1);}
    inline vec4 look_vector() const {return this->col(2);}
    inline vec4 offset() const {return {this->M[0][3], this->M[1][3], this->M[2][3], 0};}

    static matrix4 from_identity(){
        return {
            {1,0,0,0},
            {0,1,0,0},
            {0,0,1,0},
            {0,0,0,1}
        };
    }

    static matrix4 from_x_rotation(double u){
        double cos_u = cos(u);
		double sin_u = sin(u);
		return {
			{1.0, 0.0, 0.0, 0.0},
			{0.0, cos_u, sin_u, 0.0},
			{0.0, -sin_u, cos_u, 0.0},
            {0.0, 0.0, 0.0, 1.0}
		};
    }

    static matrix4 from_y_rotation(double u){
        double cos_u = cos(u);
		double sin_u = sin(u);
		return {
			{cos_u, 0.0, -sin_u, 0.0},
			{0.0, 1.0, 0.0, 0.0},
			{sin_u, 0, cos_u, 0.0},
            {0.0, 0.0, 0.0, 1.0}
		};
    }

    static matrix4 from_z_rotation(double u){
        double cos_u = cos(u);
		double sin_u = sin(u);
        return {
			{cos_u, -sin_u, 0.0, 0.0},
			{sin_u, cos_u, 0.0, 0.0},
			{0.0, 0.0, 1.0, 0.0},
            {0.0, 0.0, 0.0, 1.0}
		};
    }

    static matrix4 from_translation(vec4 translation){
        return {
            {1.0, 0.0, 0.0, translation[0]},
            {0.0, 1.0, 0.0, translation[1]},
            {0.0, 0.0, 1.0, translation[2]},
            {0.0, 0.0, 0.0, 1.0}
        };
    }

    static matrix4 from_projection(vec4 u){
        return matrix4{
            {u[0]*u[0], u[0]*u[1], u[0]*u[2], 0},
            {u[1]*u[0], u[1]*u[1], u[1]*u[2], 0},
            {u[2]*u[0], u[2]*u[1], u[2]*u[2], 0},
            {0        , 0        , 0        , u.dot(u)}
        } * (1.0/u.dot(u));
    }

    static matrix4 from_line_reflection(vec4 l){
        return (matrix4::from_projection(l) * 2.0) - matrix4::from_identity();
    }

};

#define NO_INTERSECTION (intersection_result{false,0,{0,0,0,0},{0,0,0,0},OBJECT_TYPE::UNKNOWN})
struct intersection_result{
    bool did_intersect;
    double t;
    vec4 color;
    vec4 normal;
    OBJECT_TYPE typ;
};

class ray{
    vec4 point;
    vec4 dir;
public:
    ray(vec4 _point, vec4 _dir) : point(_point), dir(_dir) {}
    ray(vec4 _dir) : dir(_dir) {};

    inline vec4 get_point() const {return this->point;}
    inline vec4 get_dir() const {return this->dir;}
};
}

