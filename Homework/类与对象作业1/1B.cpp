#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;

class Complex {
private:
    double r,i;
public:
    void Print() {
        cout << r << "+" << i << "i" << endl;
    }
// 20260304 14:27

    Complex & operator=(string str){
        int r_ = 0, i_ = 0;
        bool flg = true;
        for(char ch : str){
            if(ch >= '0' && ch <= '9'){
                if(flg) r_ = r_ * 10 + ch - '0';
                else i_ = i_ * 10 + ch - '0';
            }else flg = false;
        }
        r = r_; i = i_;
        return *this;
    }
};

int main() {
    Complex a;
    a = "3+4i"; a.Print();
    a = "5+6i"; a.Print();
    return 0;
}