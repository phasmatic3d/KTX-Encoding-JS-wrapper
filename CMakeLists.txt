if(APPLE)
  cmake_minimum_required(VERSION 3.17)
else()
  cmake_minimum_required(VERSION 3.7)
endif()

if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif()

message(STATUS "Source Dir ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/KTX-Software ${CMAKE_CURRENT_SOURCE_DIR}/KTX-Software/build)

project(toktx)

set(
  KTX_SOURCE_DIR "${PROJECT_SOURCE_DIR}/KTX-Software"
)

set(
  LIBKTX_EXPORTED_FUNCTIONS "_malloc,_free,_main" 
)
set(
  LIBKTX_EMC_LINK_FLAGS
  --bind
  "SHELL:--source-map-base ./"
  "SHELL:-s ALLOW_MEMORY_GROWTH=1"
  "SHELL:-s ASSERTIONS=0"
  "SHELL:-s ERROR_ON_UNDEFINED_SYMBOLS=0"
  "SHELL:-s MALLOC=emmalloc"
  "SHELL:-s MODULARIZE=1"
  "SHELL:-s FULL_ES3=1"
  "SHELL:-s INVOKE_RUN=0"
  "SHELL:-s EXPORT_NAME=TOKTX"
  "SHELL:-s EXPORTED_RUNTIME_METHODS=[\'GL\']"
  "SHELL:-s GL_PREINITIALIZED_CONTEXT=1"
  "SHELL:-s EXPORTED_FUNCTIONS=${LIBKTX_EXPORTED_FUNCTIONS}"
  "SHELL:-s EXPORTED_RUNTIME_METHODS=cwrap,getValue,writeArrayToMemory"
)

#message(STATUS "HELLOOOOOOOOOO Dir ${PROJECT_SOURCE_DIR}")

add_executable(toktx 
  ${KTX_SOURCE_DIR}/lib/basisu/encoder/jpgd.cpp
  ${KTX_SOURCE_DIR}/interface/js_binding/ktx_wrapper.cpp 
  ${KTX_SOURCE_DIR}/tools/toktx/image.cc
  ${KTX_SOURCE_DIR}/tools/toktx/jpgimage.cc
  ${KTX_SOURCE_DIR}/tools/toktx/lodepng.cc
  ${KTX_SOURCE_DIR}/tools/toktx/npbmimage.cc
  ${KTX_SOURCE_DIR}/tools/toktx/pngimage.cc
  src/toktx_js/toktx_wrapper.cc
)
create_version_header( src/toktx_js toktx )

target_link_libraries( toktx ktx objUtil )
target_include_directories(
  toktx 
  PRIVATE 
  ${KTX_SOURCE_DIR}/other_include
  ${KTX_SOURCE_DIR}/tools/toktx
  ${KTX_SOURCE_DIR}/lib/basisu
  ${KTX_SOURCE_DIR}/lib/basisu/zstd
  ${KTX_SOURCE_DIR}/lib/dfdutils
  $<TARGET_PROPERTY:objUtil,INTERFACE_INCLUDE_DIRECTORIES>
  $<TARGET_PROPERTY:ktx,INTERFACE_INCLUDE_DIRECTORIES> )
target_include_directories(toktx PUBLIC  include)
target_link_options(
        toktx
    PUBLIC
        ${LIBKTX_EMC_LINK_FLAGS}
    )
set_target_properties( toktx PROPERTIES OUTPUT_NAME "toktx")

add_custom_command(
        TARGET toktx POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory dist
                COMMENT "Creating 'dist' directory")
add_custom_command(
        TARGET toktx POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/toktx.js
                ${CMAKE_CURRENT_SOURCE_DIR}/dist/toktx.js
                COMMENT "Copying 'toktx.js' library to dist/toktx.js'")
add_custom_command(
        TARGET toktx POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/toktx.wasm
                ${CMAKE_CURRENT_SOURCE_DIR}/dist/toktx.wasm
                COMMENT "Copying 'toktx.wasm' library to 'dist/toktx.wasm'")
add_custom_command(
        TARGET toktx POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/toktx.js
                ${CMAKE_CURRENT_SOURCE_DIR}/examples/toktx.js
                COMMENT "Copying 'toktx.js' library to examples/toktx.js'")
add_custom_command(
        TARGET toktx POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/toktx.wasm
                ${CMAKE_CURRENT_SOURCE_DIR}/examples/toktx.wasm
                COMMENT "Copying 'toktx.wasm' library to 'examples/toktx.wasm'")
