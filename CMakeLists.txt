cmake_minimum_required( VERSION 2.8 )
project ( myslam )

set( CMAKE_BUILD_TYPE "Release" )
set( CMAKE_CXX_FLAGS "-std=c++11 -O3" )

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/out )
set( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/out )

############### 3rd party dependencies ######################
# Eigen
include_directories( "/usr/include/eigen3" )

# OpenCV Sophus G2O
include_directories( "/usr/local/include" )
if(NOT DEFINED ENV{TOOLCHAIN_PREFIX})
    message( "Compiling for x86." )
else()
    message( "Cross compiling for armv7a." )
    include_directories( "/opt/ti-processor-sdk-linux-am57xx-evm-03.01.00.06/linux-devkit/sysroots/armv7ahf-neon-linux-gnueabi/usr/include" )
endif()

if(NOT DEFINED ENV{TOOLCHAIN_PREFIX})
    set( OpenCV_LIBS
        -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui
        -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_viz -lopencv_core
    )
else()
    set( OpenCV_LIBS
        -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib
        -lopencv_cvv -lopencv_dnn -lopencv_dpm -lopencv_fuzzy -lopencv_line_descriptor -lopencv_optflow -lopencv_plot -lopencv_reg -lopencv_saliency
        -lopencv_stereo -lopencv_structured_light -lopencv_rgbd -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text
        -lopencv_face -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann
        -lopencv_xobjdetect -lopencv_objdetect -lopencv_ml -lopencv_xphoto -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_photo
        -lopencv_imgproc -lopencv_core
    )
endif()
set( Sophus_LIBS Sophus )
set( G2O_LIBS g2o_core g2o_stuff g2o_types_sba )
set( THIRD_PARTY_LIBS
    ${OpenCV_LIBS}
    ${Sophus_LIBS}
    ${G2O_LIBS}
)
if(NOT DEFINED ENV{TOOLCHAIN_PREFIX})
    link_directories( "/usr/local/lib" )
else()
    link_directories( "/opt/ti-processor-sdk-linux-am57xx-evm-03.01.00.06/linux-devkit/sysroots/armv7ahf-neon-linux-gnueabi/usr/lib" )
    link_directories( "/opt/ti-processor-sdk-linux-am57xx-evm-03.01.00.06/linux-devkit/sysroots/armv7ahf-neon-linux-gnueabi/usr/share/OpenCV/3rdparty/lib" )
endif()

############### dependencies ######################
include_directories( ${PROJECT_SOURCE_DIR}/include )
add_subdirectory( src )
add_subdirectory( test )