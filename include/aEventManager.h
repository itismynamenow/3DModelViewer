#ifndef AEVENTMANAGER_H
#define AEVENTMANAGER_H
#include "SFML/Graphics.hpp"
#include "aInput.h"

extern aInput Input;

class aEventManager
{
    public:
        aEventManager();
        virtual ~aEventManager();

        bool                            mInFocus;

        sf::Event                       mEvent;
        void                            update(sf::RenderWindow *Window);

    protected:

    private:
};

#endif // AEVENTMANAGER_H
