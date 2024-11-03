/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** DLLoader
*/

#ifndef DLLOADER_HPP_
#define DLLOADER_HPP_

#include "Error.hpp"

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#define NOGDI

#define CloseWindow RaylibCloseWindow
#define ShowCursor RaylibShowCursor
#endif

#include <asio.hpp>

#ifdef _WIN32
#include <windows.h>
#include <winsock2.h>
#else
#include <dlfcn.h>
#endif

#ifdef _WIN32
#undef CloseWindow
#undef ShowCursor
#endif

/**
 * @class DLLoader
 * @brief Class responsible for dynamically loading shared libraries.
 * Contains methods that load shared libraries, close shared libraries, and return instances from them.
 * @tparam T The type that will be loaded by the DLLoader.
 */
template <typename T> class DLLoader {
  public:
    /**
     * @brief Constructor for the `DLLoader` class.
     * @param pathLib The path of the shared library that we want to load.
     * This constructor will call the loadLib method.
     */
    DLLoader(const std::string &pathLib) {
        loadLib(pathLib);
    };

    /**
     * @brief Destructor for the `DLLoader` class.
     * This destructor calls the closeLib method.
     */
    ~DLLoader() {
        closeLib();
    }

    /**
     * @brief Load the shared library passed as parameter.
     * @param pathLib The path of the shared library that we want to load.
     * @throws Error if the library can't be opened.
     */
    void loadLib(const std::string &pathLib) {
#ifdef _WIN32
        _handle = LoadLibrary(pathLib.c_str());
        if (!_handle)
            throw Error("Failed to load library: " + pathLib);
#else
        _handle = dlopen(pathLib.c_str(), RTLD_LAZY);
        if (!_handle)
            throw Error(dlerror());
#endif
    }

    /**
     * @brief Return an instance of the server loaded shared library by calling its constructor with its parameters.
     * @param port The port on which the server listens.
     * @param pool A thread pool to manage concurrent tasks.
     * @throws Error if the load of the server's instance fails.
     */
    std::unique_ptr<T> getInstance(const std::size_t &port, asio::thread_pool &pool) {
        T *(*cosine)(const std::size_t &, asio::thread_pool &);
#ifdef _WIN32
        cosine = (T * (*)(const std::size_t &, asio::thread_pool &)) GetProcAddress((HMODULE)_handle, "entryPoint");
        if (!cosine)
            throw Error("Failed to get entry point");
#else
        cosine = (T * (*)(const std::size_t &, asio::thread_pool &)) dlsym(_handle, "entryPoint");
        char *errorMessage = dlerror();
        if (errorMessage != NULL)
            throw Error(errorMessage);
#endif
        return std::unique_ptr<T>((*cosine)(port, pool));
    };

    /**
     * @brief Return an instance of the client loaded shared library by calling its constructor with its parameters.
     * @param host The server's hostname or IP address.
     * @param port The server's port number.
     * @throws Error if the load of the client's instance fails.
     */
    std::unique_ptr<T> getInstance(const std::string &host, const std::string &port) {
        T *(*cosine)(const std::string &, const std::string &);
#ifdef _WIN32
        cosine = (T * (*)(const std::string &, const std::string &)) GetProcAddress((HMODULE)_handle, "entryPoint");
        if (!cosine)
            throw Error("Failed to get entry point");
#else
        cosine = (T * (*)(const std::string &, const std::string &)) dlsym(_handle, "entryPoint");
        char *errorMessage = dlerror();
        if (errorMessage != NULL)
            throw Error(errorMessage);
#endif
        return std::unique_ptr<T>((*cosine)(host, port));
    };

    /**
     * @brief Close the loaded shared library
     * @throws Error if the library can't be closed.
     */
    void closeLib() {
#ifdef _WIN32
        if (_handle && !FreeLibrary((HMODULE)_handle))
            throw Error("Failed to free library");
#else
        if (_handle && dlclose(_handle) != 0)
            throw Error(dlerror());
#endif
    };

  private:
#ifdef _WIN32
    HMODULE _handle; ///< The loaded shared library
#else
    void *_handle; ///< The loaded shared library
#endif
};

#endif /* !DLLOADER_HPP_ */
