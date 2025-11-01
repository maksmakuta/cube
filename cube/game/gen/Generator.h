#ifndef CUBE_GENERATOR_H
#define CUBE_GENERATOR_H

#include <glm/vec2.hpp>

#include "game/models/Chunk.h"
#include "pipeline/IPipeline.h"

namespace cube{

    class Generator {
    public:
        explicit Generator(int seed);

        void addPipeline(const PipelinePtr& pipeline);
        void updateState() const;
        [[nodiscard]] ChunkPtr generate(const glm::ivec2& chunkPos) const;

    private:
        int m_seed;
        std::vector<PipelinePtr> m_pipelines;
    };


}

#endif //CUBE_GENERATOR_H