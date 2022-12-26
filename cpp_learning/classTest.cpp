#include "iostream"
#include "vector"
using namespace std;

class Test{
public:
    vector<int> vec;
    vector<int> getVec(){
        return vec;
    }
    void setVec(vector<int> val){
        this->vec = val;
    }
};

int main(int argc, char* argv[])
{
    Test test;
    test.vec.push_back(1);
    test.vec.push_back(2);
    test.vec.push_back(3);
    for(int i : test.vec)
        cout << i << endl;
    cout << "------------" << endl;
    vector<int> v;
    v.push_back(3);
    v.push_back(2);
    v.push_back(1);
    test.setVec(v);
    for(int i : test.vec)
        cout << i << endl;
    cout << "------------" << endl;
    return 0;
}