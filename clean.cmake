if (UNIX)
    set(DOCS ./build/CMakeCache.txt ./build/Makefile ./build/cmake_install.cmake ./build/CMakeFiles ./build/src ./rtype_client ./rtype_server)

    foreach(DOC ${DOCS})
        execute_process(COMMAND rm -rf ./${DOC})
    endforeach()

    if (APPLE)
        set (LIBS ./sharedLibraries/libclient.dylib ./sharedLibraries/libserver.dylib)

        foreach(LIB ${LIBS})
            execute_process(COMMAND rm -rf ./${LIB})
        endforeach()

    elseif (NOT APPLE)
        set (LIBS ./sharedLibraries/libclient.so ./sharedLibraries/libserver.so)

        foreach(LIB ${LIBS})
            execute_process(COMMAND rm -rf ./${LIB})
        endforeach()
    endif ()
endif ()
