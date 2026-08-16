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
 * @brief Generic callback wrapper.
 *
 * Stores and invokes either:
 *
 * - A free/static function pointer.
 * - A non-static member function bound to an object instance.
 *
 * @tparam R Return type.
 * @tparam Args Callback argument types.
 *
 * @code
 * void OnEvent(int value) {}
 *
 * Callback<void, int> callback;
 * callback.bind(OnEvent);
 * callback.invoke(10);
 * @endcode
 */
template <typename R, typename... Args>
class Callback {
private:

    /**
     * @brief Internal context for bound member functions.
     */
    struct CallbackContext {
        void* instance;
        R (*invoke)(void*, Args... args);
    };

public:

    /**
     * @brief Free/static function pointer type.
     */
    using CallbackType = R (*)(Args...);

    /**
     * @brief Creates an empty callback.
     */
    Callback()
        : _callback(nullptr),
          _context{nullptr, nullptr} {}

    /**
     * @brief Binds a free or static function.
     *
     * @param callback Function pointer to bind.
     */
    inline void bind(CallbackType callback) {
        _callback = callback;
        _context = {nullptr, nullptr};
    }

    /**
     * @brief Binds a non-static member function.
     *
     * @tparam T Class type.
     * @tparam Method Member function pointer.
     *
     * @param instance Object instance used when invoking the callback.
     */
    template <typename T, R (T::*Method)(Args...)>
    void bind(T* instance) {
        _callback = nullptr;
        _context.instance = static_cast<void*>(instance);
        _context.invoke = [](void* obj, Args... args) -> R {
            return (static_cast<T*>(obj)->*Method)(args...);
        };
    }

    /**
     * @brief Removes the current binding.
     */
    inline void unbind() {
        _callback = nullptr;
        _context = {nullptr, nullptr};
    }

    /**
     * @brief Checks whether the callback is bound.
     *
     * @return true if a function or member function is bound.
     * @return false otherwise.
     */
    inline bool status() const {
        return (_callback != nullptr) || (_context.invoke && _context.instance);
    }

    /**
     * @brief Invokes the bound callback.
     *
     * @param args Arguments forwarded to the callback.
     *
     * @return Callback return value.
     *
     * @note If FOUNDATION_EXCEPTIONS is enabled, an unbound callback throws
     * std::runtime_error. Otherwise, a default-constructed return value is
     * returned.
     */
    R invoke(Args... args) const {
        if (_context.invoke && _context.instance) {
            return _context.invoke(_context.instance, args...);
        }

        if (_callback) {
            return _callback(args...);
        }

#ifdef FOUNDATION_EXCEPTIONS
        throw std::runtime_error("Callback is not bound");
#endif

        return R();
    }

private:

    CallbackType _callback;
    CallbackContext _context;
};

/// \cond INTERNAL

/**
 * @brief Specialization for callbacks without arguments and non-void return.
 *
 * @tparam R Return type.
 */
template <typename R>
class Callback<R, void> {
private:

    struct CallbackContext {
        void* instance;
        R (*invoke)(void*);
    };

public:

    using CallbackType = R (*)();

    Callback()
        : _callback(nullptr),
          _context{nullptr, nullptr} {}

    inline void bind(CallbackType callback) {
        _callback = callback;
        _context = {nullptr, nullptr};
    }

    template <typename T, R (T::*Method)()>
    void bind(T* instance) {
        _callback = nullptr;
        _context.instance = static_cast<void*>(instance);
        _context.invoke = [](void* obj) -> R {
            return (static_cast<T*>(obj)->*Method)();
        };
    }

    inline void unbind() {
        _callback = nullptr;
        _context = {nullptr, nullptr};
    }

    inline bool status() const {
        return (_callback != nullptr) || (_context.invoke && _context.instance);
    }

    R invoke() const {
        if (_context.invoke && _context.instance) {
            return _context.invoke(_context.instance);
        }

        if (_callback) {
            return _callback();
        }

#ifdef FOUNDATION_EXCEPTIONS
        throw std::runtime_error("Callback is not bound");
#endif

        return R();
    }

private:

    CallbackType _callback;
    CallbackContext _context;
};

/**
 * @brief Specialization for callbacks with void return.
 *
 * @tparam Args Callback argument types.
 */
template <typename... Args>
class Callback<void, Args...> {
private:

    struct CallbackContext {
        void* instance;
        void (*invoke)(void*, Args...);
    };

public:

    using CallbackType = void (*)(Args...);

    Callback()
        : _callback(nullptr),
          _context{nullptr, nullptr} {}

    inline void bind(CallbackType callback) {
        _callback = callback;
        _context = {nullptr, nullptr};
    }

    template <typename T, void (T::*Method)(Args...)>
    void bind(T* instance) {
        _callback = nullptr;
        _context.instance = static_cast<void*>(instance);
        _context.invoke = [](void* obj, Args... args) {
            (static_cast<T*>(obj)->*Method)(args...);
        };
    }

    inline void unbind() {
        _callback = nullptr;
        _context = {nullptr, nullptr};
    }

    inline bool status() const {
        return (_callback != nullptr) || (_context.invoke && _context.instance);
    }

    void invoke(Args... args) const {
        if (_context.invoke && _context.instance) {
            _context.invoke(_context.instance, args...);
            return;
        }

        if (_callback) {
            _callback(args...);
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

/**
 * @brief Specialization for callbacks without arguments and void return.
 */
template <>
class Callback<void, void> {
private:

    struct CallbackContext {
        void* instance;
        void (*invoke)(void*);
    };

public:

    using CallbackType = void (*)();

    Callback()
        : _callback(nullptr),
          _context{nullptr, nullptr} {}

    inline void bind(CallbackType callback) {
        _callback = callback;
        _context = {nullptr, nullptr};
    }

    template <typename T, void (T::*Method)()>
    void bind(T* instance) {
        _callback = nullptr;
        _context.instance = static_cast<void*>(instance);
        _context.invoke = [](void* obj) {
            (static_cast<T*>(obj)->*Method)();
        };
    }

    inline void unbind() {
        _callback = nullptr;
        _context = {nullptr, nullptr};
    }

    inline bool status() const {
        return (_callback != nullptr) || (_context.invoke && _context.instance);
    }

    void invoke() const {
        if (_context.invoke && _context.instance) {
            _context.invoke(_context.instance);
            return;
        }

        if (_callback) {
            _callback();
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

/// \endcond

} // namespace Functional
} // namespace Foundation

#endif // FOUNDATION_FUNCTIONAL_CALLBACK_H