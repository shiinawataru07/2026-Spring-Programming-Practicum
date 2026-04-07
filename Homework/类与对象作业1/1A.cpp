#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <vector>
using namespace std;

//20260304 13:42

class Student {
private:
    string name, studentid;
    int age;
    vector<int> scr{0, 0, 0, 0};
    double average_scr;
public:
    void input(){
        string str;
        getline(cin, str);
        name = str.substr(0, str.find(','));
        str = str.substr(str.find(',') + 1);
        age = stoi(str.substr(0, str.find(',')));
        str = str.substr(str.find(',') + 1);
        studentid = str.substr(0, str.find(','));
        str = str.substr(str.find(',') + 1);
        for(int i = 0; i < 3; i++){
            scr[i] = stoi(str.substr(0, str.find(',')));
            str = str.substr(str.find(',') + 1);
        }
        scr[3] = stoi(str);
    }
    void calculate(){
        int sum = 0;
        for(int num : scr){
            sum += num;
        }
        average_scr = sum / 4.0;
    }
    void output(){
        cout<<name<<','<<age<<','<<studentid<<','<<average_scr;
    }
};

int main() {
	Student student;        // 定义类的对象
	student.input();        // 输入数据
	student.calculate();    // 计算平均成绩
	student.output();       // 输出数据
}