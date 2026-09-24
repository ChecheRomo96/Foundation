/**
 * @file Foundation/Functional/Callback/Callback.h
 * @brief Generic callback wrapper for free functions and member functions.
 */

#ifndef FOUNDATION_FUNCTIONAL_CALLBACK_H
#define FOUNDATION_FUNCTIONAL_CALLBACK_H

#include <Foundation_BuildSettings.h>

#ifdef FOUNDATION_EXCEPTIONS
    #include <stdexcept>
#endif

namespace Foundation {
namespace Functional {

/**
 * @class Callback
 * @brief Allocation-free callback with a non-void return value.
 * @ingroup Foundation_Callback
 *
 * Stores either a free/static function pointer or a member function bound to
 * an object instance. An empty `Args...` pack represents a callback without
 * arguments, for example `Callback<int>`.
 *
 * @tparam R Return type.
 * @tparam Args Callback argument types; the pack may be empty.
 *
 * @note A bound object instance is not owned. It must remain alive until the
 * callback is rebound or unbound and no invocation is in progress.
 * @note Binding a new target replaces the previous target.
 *
 * @code
 * int Add(int lhs, int rhs) {
 *     return lhs + rhs;
 * }
 *
 * Callback<int, int, int> callback;
 * callback.Bind(Add);
 * const int result = callback.Invoke(2, 3);
 * @endcode
 */
template <typename R, typename... Args>
class Callback {
private:

    struct CallbackContext {
        const void* instance;
        R (*invoke)(const void*, Args...);
    };

public:

    /** @brief Free/static function pointer type accepted by Bind(). */
    using CallbackType = R (*)(Args...);

    /** @brief Creates an empty callback. */
    Callback() noexcept
        : _callback(nullptr),
          _context{nullptr, nullptr} {}

    /**
     * @brief Binds a free or static function.
     * @param callback Function pointer to bind; null leaves the callback empty.
     */
    void Bind(CallbackType callback) noexcept {
        _callback = callback;
        _context = {nullptr, nullptr};
    }

    /**
     * @brief Binds a non-const member function.
     * @tparam T Class type.
     * @tparam Method Member function pointer.
     * @param instance Object instance used when invoking the callback.
     * @warning `instance` is stored as a non-owning pointer and must remain
     * valid for every invocation. Passing null leaves the callback empty.
     */
    template <typename T, R (T::*Method)(Args...)>
    void Bind(T* instance) noexcept {
        if (instance == nullptr) {
            Unbind();
            return;
        }

        _callback = nullptr;
        _context.instance = instance;
        _context.invoke = [](const void* object, Args... args) -> R {
            T* typedObject = const_cast<T*>(static_cast<const T*>(object));
            return (typedObject->*Method)(static_cast<Args&&>(args)...);
        };
    }

    /**
     * @brief Binds a const member function.
     * @tparam T Class type.
     * @tparam Method Const member function pointer.
     * @param instance Object instance used when invoking the callback.
     * @warning `instance` is stored as a non-owning pointer and must remain
     * valid for every invocation. Passing null leaves the callback empty.
     */
    template <typename T, R (T::*Method)(Args...) const>
    void Bind(const T* instance) noexcept {
        if (instance == nullptr) {
            Unbind();
            return;
        }

        _callback = nullptr;
        _context.instance = instance;
        _context.invoke = [](const void* object, Args... args) -> R {
            return (static_cast<const T*>(object)->*Method)(
                static_cast<Args&&>(args)...
            );
        };
    }

    /** @brief Removes the current binding. */
    void Unbind() noexcept {
        _callback = nullptr;
        _context = {nullptr, nullptr};
    }

    /**
     * @brief Checks whether the callback has an invocable target.
     * @return true when a free or member function is bound; otherwise false.
     */
    bool IsBound() const noexcept {
        return (_callback != nullptr) ||
            (_context.invoke != nullptr && _context.instance != nullptr);
    }

    /**
     * @brief Invokes the bound callback.
     * @param args Arguments forwarded according to the callback signature.
     * @return Callback return value.
     *
     * @note If `FOUNDATION_EXCEPTIONS` is enabled, an empty callback throws
     * `std::runtime_error`. Otherwise, it returns a default-constructed `R`.
     */
    R Invoke(Args... args) const {
        if (_context.invoke != nullptr && _context.instance != nullptr) {
            return _context.invoke(
                _context.instance,
                static_cast<Args&&>(args)...
            );
        }

        if (_callback != nullptr) {
            return _callback(static_cast<Args&&>(args)...);
        }

#ifdef FOUNDATION_EXCEPTIONS
        throw std::runtime_error("Callback is not bound");
#else
        return R{};
#endif
    }

private:

    CallbackType _callback;
    CallbackContext _context;
};

/**
 * @brief Allocation-free callback with no return value.
 * @ingroup Foundation_Callback
 *
 * An empty `Args...` pack represents a callback with neither a return value nor
 * arguments: `Callback<void>`.
 *
 * @tparam Args Callback argument types; the pack may be empty.
 */
template <typename... Args>
class Callback<void, Args...> {
private:

    struct CallbackContext {
        const void* instance;
        void (*invoke)(const void*, Args...);
    };

public:

    /** @brief Free/static function pointer type accepted by Bind(). */
    using CallbackType = void (*)(Args...);

    /** @brief Creates an empty callback. */
    Callback() noexcept
        : _callback(nullptr),
          _context{nullptr, nullptr} {}

    /**
     * @brief Binds a free or static function.
     * @param callback Function pointer to bind; null leaves the callback empty.
     */
    void Bind(CallbackType callback) noexcept {
        _callback = callback;
        _context = {nullptr, nullptr};
    }

    /**
     * @brief Binds a non-const member function.
     * @tparam T Class type.
     * @tparam Method Member function pointer.
     * @param instance Object instance used when invoking the callback.
     * @warning `instance` is stored as a non-owning pointer and must remain
     * valid for every invocation. Passing null leaves the callback empty.
     */
    template <typename T, void (T::*Method)(Args...)>
    void Bind(T* instance) noexcept {
        if (instance == nullptr) {
            Unbind();
            return;
        }

        _callback = nullptr;
        _context.instance = instance;
        _context.invoke = [](const void* object, Args... args) {
            T* typedObject = const_cast<T*>(static_cast<const T*>(object));
            (typedObject->*Method)(static_cast<Args&&>(args)...);
        };
    }

    /**
     * @brief Binds a const member function.
     * @tparam T Class type.
     * @tparam Method Const member function pointer.
     * @param instance Object instance used when invoking the callback.
     * @warning `instance` is stored as a non-owning pointer and must remain
     * valid for every invocation. Passing null leaves the callback empty.
     */
    template <typename T, void (T::*Method)(Args...) const>
    void Bind(const T* instance) noexcept {
        if (instance == nullptr) {
            Unbind();
            return;
        }

        _callback = nullptr;
        _context.instance = instance;
        _context.invoke = [](const void* object, Args... args) {
            (static_cast<const T*>(object)->*Method)(
                static_cast<Args&&>(args)...
            );
        };
    }

    /** @brief Removes the current binding. */
    void Unbind() noexcept {
        _callback = nullptr;
        _context = {nullptr, nullptr};
    }

    /**
     * @brief Checks whether the callback has an invocable target.
     * @return true when a free or member function is bound; otherwise false.
     */
    bool IsBound() const noexcept {
        return (_callback != nullptr) ||
            (_context.invoke != nullptr && _context.instance != nullptr);
    }

    /**
     * @brief Invokes the bound callback.
     * @param args Arguments forwarded according to the callback signature.
     *
     * @note If `FOUNDATION_EXCEPTIONS` is enabled, an empty callback throws
     * `std::runtime_error`. Otherwise, invocation is a no-op.
     */
    void Invoke(Args... args) const {
        if (_context.invoke != nullptr && _context.instance != nullptr) {
            _context.invoke(
                _context.instance,
                static_cast<Args&&>(args)...
            );
            return;
        }

        if (_callback != nullptr) {
            _callback(static_cast<Args&&>(args)...);
            return;
        }

#ifdef FOUNDATION_EXCEPTIONS
        throw std::runtime_error("Callback is not bound");
#endif
    }

private:

    CallbackType _callback;
    CallbackContext _context;
};

} // namespace Functional
} // namespace Foundation

#endif // FOUNDATION_FUNCTIONAL_CALLBACK_H
