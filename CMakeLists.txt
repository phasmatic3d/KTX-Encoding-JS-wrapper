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

project(libktx)

set(
  KTX_SOURCE_DIR "${PROJECT_SOURCE_DIR}/KTX-Software"
)

set(
  LIBKTX_EXPORTED_FUNCTIONS "_malloc,_free" 
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
  "SHELL:-s EXPORT_NAME=LIBKTX"
  "SHELL:-s EXPORTED_RUNTIME_METHODS=[\'GL\']"
  "SHELL:-s GL_PREINITIALIZED_CONTEXT=1"
  "SHELL:-s EXPORTED_FUNCTIONS=${LIBWEP_EXPORTED_FUNCTIONS}"
  "SHELL:-s EXPORTED_RUNTIME_METHODS=cwrap,getValue"
)

#message(STATUS "HELLOOOOOOOOOO Dir ${PROJECT_SOURCE_DIR}")

add_executable(libktx 
  src/ktx_encoder.cpp
  ${KTX_SOURCE_DIR}/interface/js_binding/ktx_wrapper.cpp 
  ${KTX_SOURCE_DIR}/tools/toktx/image.cc 
)
target_link_libraries( libktx ktx objUtil )
target_include_directories(
  libktx 
  PRIVATE 
  ${KTX_SOURCE_DIR}/other_include
  ${KTX_SOURCE_DIR}/tools/toktx
  ${KTX_SOURCE_DIR}/lib
  ${KTX_SOURCE_DIR}/lib/basisu
  ${KTX_SOURCE_DIR}/lib/dfdutils
  $<TARGET_PROPERTY:objUtil,INTERFACE_INCLUDE_DIRECTORIES>
  $<TARGET_PROPERTY:ktx,INTERFACE_INCLUDE_DIRECTORIES> )
target_include_directories(libktx PUBLIC  include)
target_link_options(
        libktx
    PUBLIC
        ${LIBKTX_EMC_LINK_FLAGS}
    )
set_target_properties( libktx PROPERTIES OUTPUT_NAME "libktx")

add_custom_command(
        TARGET libktx POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory dist
                COMMENT "Creating 'dist' directory")
add_custom_command(
        TARGET libktx POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/libktx.js
                ${CMAKE_CURRENT_SOURCE_DIR}/dist/libktx.js
                COMMENT "Copying 'libwebp.js' library to dist/libktx.js'")
add_custom_command(
        TARGET libktx POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/libktx.wasm
                ${CMAKE_CURRENT_SOURCE_DIR}/dist/libktx.wasm
                COMMENT "Copying 'libktx.wasm' library to 'dist/libktx.wasm'")
