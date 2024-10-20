/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** DLLoader
*/

#ifndef DLLOADER_HPP_
#define DLLOADER_HPP_

#include <dlfcn.h>
#include "Error.hpp"

template<typename T>
class DLLoader {
    public:
        DLLoader(const std::string pathLib) {
            _handle = dlopen(pathLib.c_str(), RTLD_LAZY);
            if (!_handle)
                throw Error(dlerror());
        };
        ~DLLoader() = default;

        std::unique_ptr<T> getInstance(const std::string &host, const std::size_t &port, asio::thread_pool &pool) {
            T *(*cosine)(const std::string &, const std::size_t &, asio::thread_pool &);
            cosine = (T*(*)(const std::string &, const std::size_t &, asio::thread_pool &)) dlsym(_handle, "entryPoint");
            char *errorMessage = dlerror();
            if (errorMessage != NULL)
                throw Error(errorMessage);

            return std::unique_ptr<T>((*cosine)(host, port, pool));
        };

        std::unique_ptr<T> getInstance(const std::string &host, const std::string &port) {
            T *(*cosine)(const std::string &, const std::string &);
            cosine = (T*(*)(const std::string &, const std::string &)) dlsym(_handle, "entryPoint");
            char *errorMessage = dlerror();
            if (errorMessage != NULL)
                throw Error(errorMessage);

            return std::unique_ptr<T>((*cosine)(host, port));
        };

        // void closeLib() {
        //     if (dlclose(_handle) != 0)
        //         throw Error(dlerror());
        // };

    private:
        void *_handle;
};

#endif /* !DLLOADER_HPP_ */
