#include "aInput.h"

aInput::aInput()
{
    mouseWheel=0;
    rightMouseButton=idle,
    leftMouseButton=idle,
    w=idle,
    s=idle,
    a=idle,
    d=idle,
    p=idle;
    plus=idle;
    minus=idle;
    enteredText="";
}

aInput::~aInput()
{
    //dtor
}

void aInput::update (sf::RenderWindow *Window)
{
    mouseX=sf::Mouse::getPosition(*Window).x;
    mouseY=sf::Mouse::getPosition(*Window).y;
    mouseWheel=0;
//    std::cout<<mouseX<<" "<<mouseY<<std::endl;
}
