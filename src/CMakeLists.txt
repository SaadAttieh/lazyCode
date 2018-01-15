
cmake_minimum_required (VERSION 3.6)



add_library (lazyCode INTERFACE)
target_include_directories (lazyCode INTERFACE
                            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                            $<INSTALL_INTERFACE:include>)
target_compile_features (lazyCode INTERFACE ${CXX_FEATURES_USED})
target_compile_definitions (lazyCode INTERFACE AUTOARGPARSE_HEADER_ONLY=1)
