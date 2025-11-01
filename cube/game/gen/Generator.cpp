#include "Generator.h"

namespace cube{

    Generator::Generator(const int seed) : m_seed(seed){}

    void Generator::addPipeline(const PipelinePtr& pipeline){
        if (pipeline == nullptr) return;
        m_pipelines.push_back(pipeline);
    }

    void Generator::updateState() const{
        for (const auto& pipeline : m_pipelines){
            pipeline->setSeed(m_seed);
        }
    }

    ChunkPtr Generator::generate(const glm::ivec2& chunkPos) const{
        const auto chunk = std::make_shared<Chunk>();
        for (const auto& pipeline : m_pipelines){
            pipeline->pipe(chunk, chunkPos);
        }
        return chunk;
    }


}

