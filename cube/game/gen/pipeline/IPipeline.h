#ifndef CUBE_IPIPELINE_H
#define CUBE_IPIPELINE_H

#include <glm/vec2.hpp>

#include "game/models/Chunk.h"

namespace cube{

    class IPipeline{
    public:
        explicit IPipeline(int seed);
        virtual ~IPipeline() = default;

        virtual void pipe(const ChunkPtr& chunk, const glm::ivec2& pos) = 0;

        void setSeed(int seed);
        [[nodiscard]] int getSeed() const;

    private:
        int m_seed;
    };

    using PipelinePtr = std::shared_ptr<IPipeline>;

}

#endif //CUBE_IPIPELINE_H