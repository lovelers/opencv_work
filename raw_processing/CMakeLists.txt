cmake_minimum_required(VERSION 2.8)
set(project_name "raw_process")
project(${project_name})
find_package( OpenCV REQUIRED )
set(bayer_domain_headers "${CMAKE_CURRENT_SOURCE_DIR}/bayer_domain")
set(rgb_domain_headers "${CMAKE_CURRENT_SOURCE_DIR}/rgb_domain")
set(yuv_domain_headers "${CMAKE_CURRENT_SOURCE_DIR}/yuv_domain")
set(tunning_headers "${CMAKE_CURRENT_SOURCE_DIR}/tunning")
set(util_headers "${CMAKE_CURRENT_SOURCE_DIR}/util")

set(bayer_domain_cpps
    bayer_domain/bayer_buffer.cpp
    bayer_domain/bayer_noise_reduction.cpp
    bayer_domain/white_balance.cpp
    )
set(rgb_domain_cpps
    rgb_domain/color_correction.cpp
    rgb_domain/gamma_correction.cpp
    rgb_domain/rgb_analyzer.cpp
    )
set(yuv_domain_cpps
    yuv_domain/yuv_processing.cpp
    )
set(tuning_cpps
    tuning/gamma_tuning.cpp
    )
set(util_cpps
    util/color_space_conversion.cpp
    util/raw_processing.cpp
    )

add_definitions(-DOPENCV)

include_directories(${OpenCV_INCLUDE_DIRS} ${bayer_domain_headers} 
    ${rgb_domain_headers} ${yuv_domain_headers}
    ${tunning_headers} ${CMAKE_CURRENT_SOURCE_DIR}
    ${util_headers})

add_executable(${project_name} "main.cpp" ${bayer_domain_cpps} ${rgb_domain_cpps} ${yuv_domain_cpps} ${tuning_cpps} ${util_cpps})

target_link_libraries(${project_name} ${OpenCV_LIBS} )
