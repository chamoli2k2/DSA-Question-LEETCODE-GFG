#include<bits/stdc++.h>
using namespace std;

class Singleton{
private:
    static Singleton *s;
    // Private costructor
    Singleton(){};

public:
    static Singleton* getInstance(){
        if(s == NULL){
            s = new Singleton();
        }
        return s;
    }
};

Singleton* Singleton::s = NULL;

int main(){
    Singleton *s1 = Singleton::getInstance();
    Singleton *s2 = Singleton::getInstance();

    if(s1 == s2){
        cout<<"This is following the Singleton pattern"<<endl;
    }
    
    return 0;
}