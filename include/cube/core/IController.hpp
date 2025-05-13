#ifndef ICONTROLLER_HPP
#define ICONTROLLER_HPP

namespace cube {

    class IController {
    public:
        virtual ~IController() = default;

        virtual void setCursorVisibility(bool) = 0;
    };

}

#endif //ICONTROLLER_HPP
