#ifndef IWINDOWCONTROLLER_HPP
#define IWINDOWCONTROLLER_HPP

namespace cube {

    enum CursorIcon {
        Default,
        Hand,
        Input
    };

    class IWindowController{
    public:
        virtual ~IWindowController() = default;

        virtual void showCursor(bool) = 0;
        virtual void setCursor(CursorIcon) = 0;
        virtual void close() = 0;
    };

}

#endif //IWINDOWCONTROLLER_HPP
