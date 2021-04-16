cmake_minimum_required(VERSION 3.12)
project(pw-lock C)
set(CMAKE_C_STANDARD 11)
# uncomment it for sanitize
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fsanitize=undefined")
add_compile_options(-Wno-macro-redefined)

include_directories(deps/libecc/src)
# deps/ckb-c-std-lib
include_directories(c)
include_directories(deps/ckb-c-stdlib/molecule)
include_directories(tests/pw_webauthn_lib)
include_directories(deps/secp256k1/src)
include_directories(deps/secp256k1)
include_directories(build)
include_directories(tests/pw_anyone_can_pay)

add_definitions(-DWORDSIZE=64)
add_definitions(-DWITH_STDLIB)
add_definitions(-D__unix__)
add_definitions(-DCKB_SIMULATOR)
add_definitions(-DNN_CONSISTENCY_CHECK)
add_definitions(-DHAS_ETHEREUM)
add_definitions(-DHAS_EOS)
add_definitions(-DHAS_TRON)
add_definitions(-DHAS_BITCOIN)
add_definitions(-DHAS_DOGECOIN)
add_definitions(-DHAS_EXTENDED_VALIDATOR)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif()

add_executable(pw_webauthn_lib tests/pw_webauthn_lib/main_test.c)


add_executable(pw_anyone_can_pay tests/pw_anyone_can_pay/main_test.c)
