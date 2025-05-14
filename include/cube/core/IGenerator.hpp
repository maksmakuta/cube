#ifndef IGENERATOR_HPP
#define IGENERATOR_HPP

namespace cube {

    class IGenerator {
    public:
        explicit IGenerator(const int seed = 0) : m_seed(seed){}
        virtual ~IGenerator() = default;

//        virtual std::shared_ptr<Chunk> generateAt(const glm::vec2& pos) = 0;

        [[nodiscard]] int getSeed() const {
            return m_seed;
        }

    private:
        int m_seed{0};
    };

}

#endif //IGENERATOR_HPP
