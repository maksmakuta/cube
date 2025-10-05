#ifndef CUBE_LAMBDAVISITOR_HPP
#define CUBE_LAMBDAVISITOR_HPP

namespace cube {

    template<class... Ts>
    struct LambdaVisitor : Ts... { using Ts::operator()...; };

    template<class... Ts>
    LambdaVisitor(Ts...) -> LambdaVisitor<Ts...>;

}

#endif //CUBE_LAMBDAVISITOR_HPP