
#ifndef FSM_SRC_FSM_ANY_STATE_H
#define FSM_SRC_FSM_ANY_STATE_H

#include <typeinfo>
#include <typeindex>
#include <memory>

namespace fsm {


/**
 * @brief Stores any state. This is a type erasure utility class to ease the storage by the FSM.
 */
class AnyState
{
public:
    /**
     * @brief Default ctor
     */
    AnyState() = default;
    /**
     * @brief Copy ctor
     */
    AnyState(AnyState const& p_other)
        : m_handle{p_other.m_handle->clone()}
    { }
    /**
     * @brief Copy assignment 
     */
    AnyState& operator= (AnyState const& p_rhs) 
    { 
        AnyState temp{p_rhs};
        std::swap(temp, *this);
        return *this;
    }
    /**
     * @brief
     */
    template <typename TState>
    AnyState& operator= (TState p_state) 
    {
        AnyState temp{std::forward<TState>(p_state)};
        std::swap(temp, *this);
        return *this;
    }
    /**
     * @brief
     */
    template <typename TState>
    AnyState(TState&& p_state) 
        : m_handle{std::unique_ptr<Handle<TState>>(new Handle<TState>{std::forward<TState>(p_state)})}
    { }
    /**
     * @brief Dctor
     */
    ~AnyState() = default;
    /**
     * @brief
     */
    void swap(AnyState& p_other) noexcept { m_handle.swap(p_other.m_handle); }
    /**
     * @brief
     */
    template <typename TState>
    TState& get()
    {
        if (std::type_index(typeid(TState)) == std::type_index(m_handle->type_info()))
        {
            return static_cast<Handle<TState>&>(*m_handle).m_state;
        }
        throw std::bad_cast();
    }
    /**
     * @brief
     */
    template <typename TState>
    constexpr bool is_same() const 
    {
        return std::type_index(typeid(TState)) == std::type_index(m_handle->type_info());
    }

private:

    struct HandleBase
    {
        virtual ~HandleBase() {};
        virtual std::type_info const& type_info() const = 0;
        virtual std::unique_ptr<HandleBase> clone() const = 0;
    };

    template <typename TState>
    struct Handle : public HandleBase {

        template <typename S>
        explicit Handle(S&& p_state) : m_state(std::forward<S>(p_state)) { }

        std::type_info const& type_info() const override { return typeid(TState); }

        std::unique_ptr<HandleBase> clone() const override 
        {
            return std::unique_ptr<Handle<TState>>(new Handle{m_state});
        }

        TState m_state;
    };

    std::unique_ptr<HandleBase> m_handle;
};

} // namespace fsm

#endif  // FSM_SRC_FSM_ANY_STATE_H
