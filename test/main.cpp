#include <iostream>

//using namespace std;
int main(){
    //while(true){
        std::string str;
        std::cout << "enter something."<<std::endl;
        std::getline(std::cin,str);
        std::cout << "you entered:" << str <<std::endl;
    //}
    return 0;
}