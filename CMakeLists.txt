if(APPLE)
  cmake_minimum_required(VERSION 3.17)
else()
  cmake_minimum_required(VERSION 3.7)
endif()

if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/KTX-Software ${CMAKE_CURRENT_SOURCE_DIR}/KTX-Software/build)

project(libktx_js)

set(
  KTX_SOURCE_DIR "${PROJECT_SOURCE_DIR}/KTX-Software"
)

set(
    KTX_EMC_LINK_FLAGS
    --bind
    "SHELL:--source-map-base ./"
    "SHELL:-s ALLOW_MEMORY_GROWTH=1"
    "SHELL:-s ASSERTIONS=0"
    "SHELL:-s MALLOC=emmalloc"
    "SHELL:-s MODULARIZE=1"
    "SHELL:-s FULL_ES3=1"
)

add_executable( libktx_js src/libktx_js/ktx_wrapper.cpp )
target_link_libraries( libktx_js ktx )
target_include_directories( 
  libktx_js 
  PRIVATE 
  ${KTX_SOURCE_DIR}/other_include
  $<TARGET_PROPERTY:ktx,INTERFACE_INCLUDE_DIRECTORIES>
)
target_link_options(
    libktx_js
PUBLIC
    ${KTX_EMC_LINK_FLAGS}
    "SHELL:-s EXPORT_NAME=LIBKTX"
    "SHELL:-s EXPORTED_RUNTIME_METHODS=[\'GL\']"
    "SHELL:-s GL_PREINITIALIZED_CONTEXT=1"
)
set_target_properties( libktx_js PROPERTIES OUTPUT_NAME "libktx")

add_custom_command(
        TARGET libktx_js POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/libktx.js
                ${CMAKE_CURRENT_SOURCE_DIR}/dist/libktx.js
                COMMENT "Copying 'libktx.js' library to dist/libktx.js'")
add_custom_command(
        TARGET libktx_js POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/libktx.wasm
                ${CMAKE_CURRENT_SOURCE_DIR}/dist/libktx.wasm
                COMMENT "Copying 'libktx.wasm' library to 'dist/libktx.wasm'")

add_custom_command(
        TARGET libktx_js POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/libktx.js
                ${CMAKE_CURRENT_SOURCE_DIR}/examples/libktx.js
                COMMENT "Copying 'libktx.js' library to examples/libktx.js'")
add_custom_command(
        TARGET libktx_js POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/libktx.wasm
                ${CMAKE_CURRENT_SOURCE_DIR}/examples/libktx.wasm
                COMMENT "Copying 'libktx.wasm' library to 'examples/libktx.wasm'")
