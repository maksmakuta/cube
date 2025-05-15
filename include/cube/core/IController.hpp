#ifndef ICONTROLLER_HPP
#define ICONTROLLER_HPP

#include "IWindowController.hpp"

namespace cube {

    class IScreen;

    class IController : public IWindowController{
    public:
        ~IController() override = default;

        virtual void navigate(IScreen*) = 0;
    };

}

#endif //ICONTROLLER_HPP
