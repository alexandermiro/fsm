
#ifndef FSM_SRC_UTIL_FOR_EACH_TUPLE_H
#define FSM_SRC_UTIL_FOR_EACH_TUPLE_H


#include <type_traits>
#include <utility>          // std::index_sequence
#include <initializer_list>

namespace fsm::util::algo {

namespace impl {

template <typename Tuple, typename Func, std::size_t... Indices>
constexpr Func for_each_tuple_impl(Tuple&& p_tuple
                                  , Func&& p_func
                                  , std::index_sequence<Indices...>)
{
    // The (void) is used to avoid 'expression result unused' warning
    return (void) std::initializer_list<int>{
            // This is a pack expansion using a comma operator expressoin: (arg, 0). Arguments of the
            // comma operator expression are always evaluated left-to-right, but the result is equal
            // to the last argument - the zero is returned as a result of the comma operator.
            //
            // For each parameter passed to the function one 'int' is added to the comman-separated
            // initialization list which is passed to the constructor of std::initializer_list in a form
            // of { (arg1, 0), (arg2, 0), ...}
            //
            // Each of the inner statements evaluates to 0 (the value passed to initializer_list ctor), 
            // but all the statements before comma are evaluated first - in this case the function 
            // execution for a tuple element.
            (std::forward<Func>(p_func)(std::get<Indices>(std::forward<Tuple>(p_tuple)), Indices), 0) ... }, p_func;
}


} // namespace impl


template <typename Tuple, typename Func>
constexpr Func for_each_tuple(Tuple&& p_tuple, Func&& p_func)
{
    return impl::for_each_tuple_impl(std::forward<Tuple>(p_tuple)
                                    , std::forward<Func>(p_func)
                                    // Create consecutive integrals: 0, 1, 2, 3, ... so as to access tuple elements
                                    , std::make_index_sequence< std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
}


} // namespace fsm::util

#endif  // FSM_SRC_UTIL_FOR_EACH_TUPLE_H