#include <iostream>
using namespace std;

int main(){
    bool b{};
    double y{},x{};
    for (x = 0; x<=5; x=x+0.5){
        cout << endl << "X es: " << x;
        if (b == true){
            cout << endl<< "b es: " << b;
            y = y + 3;
        }
    }

    cout << endl << "b al final es: " << b;
    cout << endl << "y al final es: " << y;
    cout << endl << "y + x al final es: " << y*10000 + x;
    return x;
}