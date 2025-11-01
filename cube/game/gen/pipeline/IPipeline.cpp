#include "IPipeline.h"

namespace cube{

    IPipeline::IPipeline(const int seed) : m_seed(seed){}

    void IPipeline::setSeed(const int seed){
        m_seed = seed;
    }

    int IPipeline::getSeed() const{
        return m_seed;
    }

}


