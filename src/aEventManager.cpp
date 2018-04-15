#include "aEventManager.h"

aEventManager::aEventManager()
{
    //ctor
}

aEventManager::~aEventManager()
{
    //dtor
}

void aEventManager::update(sf::RenderWindow *Window)
{
    while (Window->pollEvent(mEvent))
    {

//LMB
        if(Input.leftMouseButton==aInput::key::clicked)
        {
            Input.leftMouseButton=aInput::key::pressed;
        }
        if (mEvent.mouseButton.button == sf::Mouse::Left && mEvent.type == sf::Event::MouseButtonPressed)
        {
            Input.leftMouseButton=aInput::key::clicked;
            Input.lastMouseClickX=Input.mouseX;
            Input.lastMouseClickY=Input.mouseY;
            Input.enteredText="";
        }
        if(Input.leftMouseButton==aInput::key::released)
        {
            Input.leftMouseButton=aInput::key::idle;
        }
        if (mEvent.mouseButton.button == sf::Mouse::Left && mEvent.type == sf::Event:: MouseButtonReleased)
        {
            Input.leftMouseButton=aInput::key::released;
        }

//RMB
        if(Input.rightMouseButton==aInput::key::clicked)
        {
            Input.rightMouseButton=aInput::key::pressed;
        }
        if (mEvent.mouseButton.button == sf::Mouse::Right && mEvent.type == sf::Event::MouseButtonPressed)
        {
            Input.rightMouseButton=aInput::key::clicked;
        }
        if(Input.rightMouseButton==aInput::key::released)
        {
            Input.rightMouseButton=aInput::key::idle;
        }
        if (mEvent.mouseButton.button == sf::Mouse::Right && mEvent.type == sf::Event:: MouseButtonReleased)
        {
            Input.rightMouseButton=aInput::key::released;
        }

//WHEEL
        if (mEvent.type ==sf::Event::MouseWheelMoved)
        {
             Input.mouseWheel=mEvent.mouseWheel.delta;
        }

//ESC
        if(mEvent.type == sf::Event::KeyPressed && mEvent.key.code == sf::Keyboard::Escape || mEvent.type ==sf::Event::Closed)
        {
            Window->close();
        }

//A
        if(mEvent.type == sf::Event::KeyPressed && mEvent.key.code == sf::Keyboard::A)
        {

            if(Input.a==aInput::key::idle || Input.a==aInput::key::released)
            {
                Input.a=aInput::key::clicked;
            }
            else
            {
                Input.a=aInput::key::pressed;
            }
        }
        if(mEvent.type == sf::Event::KeyReleased && mEvent.key.code == sf::Keyboard::A)
        {
            Input.a=aInput::key::released;
        }

 //D
        if(mEvent.type == sf::Event::KeyPressed && mEvent.key.code == sf::Keyboard::D)
        {

            if(Input.d==aInput::key::idle || Input.d==aInput::key::released)
            {
                Input.d=aInput::key::clicked;
            }
            else
            {
                Input.d=aInput::key::pressed;
            }
        }
        if(mEvent.type == sf::Event::KeyReleased && mEvent.key.code == sf::Keyboard::D)
        {
            Input.d=aInput::key::released;
        }

 //W
        if(mEvent.type == sf::Event::KeyPressed && mEvent.key.code == sf::Keyboard::W)
        {

            if(Input.w==aInput::key::idle || Input.w==aInput::key::released)
            {
                Input.w=aInput::key::clicked;
            }
            else
            {
                Input.w=aInput::key::pressed;
            }
        }
        if(mEvent.type == sf::Event::KeyReleased && mEvent.key.code == sf::Keyboard::W)
        {
            Input.w=aInput::key::released;
        }

 //S
        if(mEvent.type == sf::Event::KeyPressed && mEvent.key.code == sf::Keyboard::S)
        {

            if(Input.s==aInput::key::idle || Input.s==aInput::key::released)
            {
                Input.s=aInput::key::clicked;
            }
            else
            {
                Input.s=aInput::key::pressed;
            }
        }
        if(mEvent.type == sf::Event::KeyReleased && mEvent.key.code == sf::Keyboard::S)
        {
            Input.s=aInput::key::released;
        }
//P
        if(mEvent.type == sf::Event::KeyPressed && mEvent.key.code == sf::Keyboard::P)
        {

            if(Input.p==aInput::key::idle || Input.p==aInput::key::released)
            {
                Input.p=aInput::key::clicked;
            }
            else
            {
                Input.p=aInput::key::pressed;
            }
        }
        if(mEvent.type == sf::Event::KeyReleased && mEvent.key.code == sf::Keyboard::P)
        {
            Input.p=aInput::key::released;
        }

//MINUS
        if(mEvent.type == sf::Event::KeyPressed && mEvent.key.code == sf::Keyboard::Dash)
        {

            if(Input.minus==aInput::key::idle || Input.minus==aInput::key::released)
            {
                Input.minus=aInput::key::clicked;
            }
            else
            {
                Input.minus=aInput::key::pressed;
            }
        }
        if(mEvent.type == sf::Event::KeyReleased && mEvent.key.code == sf::Keyboard::Dash)
        {
            Input.minus=aInput::key::released;
        }

//PLUS
        if(mEvent.type == sf::Event::KeyPressed && mEvent.key.code == sf::Keyboard::Equal)
        {

            if(Input.plus==aInput::key::idle || Input.plus==aInput::key::released)
            {
                Input.plus=aInput::key::clicked;
            }
            else
            {
                Input.plus=aInput::key::pressed;
            }
        }
        if(mEvent.type == sf::Event::KeyReleased && mEvent.key.code == sf::Keyboard::Equal)
        {
            Input.plus=aInput::key::released;
        }

//FOCUS
        if(mEvent.type == sf::Event::GainedFocus)
        {
            mInFocus=true;
        }
        if(mEvent.type == sf::Event::LostFocus)
        {
            mInFocus=false;
        }


    }
}
