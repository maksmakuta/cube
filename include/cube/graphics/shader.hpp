#ifndef SHADER_HPP
#define SHADER_HPP

#include <cstdint>
#include <string>

namespace cube::graphics {

    class Shader {
    public:
        Shader();

        void loadMem(const std::string& vert,const std::string& frag);
        void loadFile(const std::string& vert_path,const std::string& frag_path);
        void loadFilePair(const std::string &path, const std::string &name);
        void clear();

        [[nodiscard]] bool isOK() const;
        [[nodiscard]] uint32_t handle() const;

        void setBool(const std::string& name, bool value) const;
        void setFloat(const std::string& name, bool value) const;
        void setInt(const std::string& name, bool value) const;
        void seUInt(const std::string& name, bool value) const;
        void setVec2(const std::string& name, bool value) const;
        void setVec3(const std::string& name, bool value) const;
        void setVec4(const std::string& name, bool value) const;
        void setMat3(const std::string& name, bool value) const;
        void setMat4(const std::string& name, bool value) const;
    private:
        uint32_t program{0};
    };
}

#endif //SHADER_HPP
