#ifndef CUBE_IENTITY_HPP
#define CUBE_IENTITY_HPP

#include <glm/vec3.hpp>

namespace cube {

    class IEntity {
    public:
        virtual ~IEntity() = default;

        virtual glm::vec3 getPos() = 0;
        virtual glm::vec3 getBox() = 0;
        virtual glm::vec3 getLook() = 0;

        virtual void setPos(const glm::vec3&) = 0;
        virtual void setBox(const glm::vec3&) = 0;
        virtual void setLook(const glm::vec3&) = 0;
    };

}

#endif //CUBE_IENTITY_HPP