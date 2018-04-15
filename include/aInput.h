#ifndef AINPUT_H
#define AINPUT_H
#include "SFML/Graphics.hpp"
#include <string>
#include <iostream>




class aInput
{
    public:
        aInput();
        virtual ~aInput();
        int mouseX, mouseY, mouseWheel, lastMouseClickX,lastMouseClickY;
        std::string enteredText;
        enum key {pressed, released, idle, clicked};
        key rightMouseButton,
            leftMouseButton,
            w,
            s,
            a,
            d,
            p,
            plus,
            minus;
        void update (sf::RenderWindow *Window);

    protected:

    private:
};

#endif // AINPUT_H
