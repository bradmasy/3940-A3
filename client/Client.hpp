#ifndef CLIENT_HPP_INCLUDED
#define CLIENT_HPP_INCLUDED

#include <iostream>

using namespace std;

class Client{


public:
    Client();
    void getUserInput();

private:
    string imagePath;
    string MultiKeyword;
    string MultiCaption;
    string MultiDate;
};


#endif // CLIENT_HPP_INCLUDED


