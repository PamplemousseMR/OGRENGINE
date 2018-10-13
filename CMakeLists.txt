#======================================== 
#	project
#========================================
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT("OGRENGINE" CXX)

#================================================================================  
#		init
#================================================================================ 

#======================================== 
#	variables
#========================================
SET(ogrengine_lib_src_dir "src")
SET(ogrengine_lib_header_dir "include")
SET(ogrengine_lib_name "ogrengine")
SET(ogrengine_config_dir "config")
SET(ogrengine_media_dir "media")

#======================================== 
#	compiler specifications
#========================================
SET(CMAKE_CXX_STANDARD 17)

#======================================== 
#	find files
#========================================
FILE(GLOB_RECURSE ogrengine_lib_src_files ${ogrengine_lib_src_dir}/*)
FILE(GLOB_RECURSE ogrengine_lib_header_files ${ogrengine_lib_header_dir}/*)
FILE(GLOB_RECURSE ogrengine_config_files ${ogrengine_config_dir}/*)
FILE(GLOB_RECURSE ogrengine_media_files ${ogrengine_media_dir}/*)

#================================================================================  
#		create
#================================================================================

#======================================== 
#	create project
#========================================
ADD_LIBRARY(${ogrengine_lib_name}  ${ogrengine_lib_src_files} ${ogrengine_lib_header_files} ${ogrengine_config_files} ${ogrengine_media_files})
TARGET_INCLUDE_DIRECTORIES(${ogrengine_lib_name} PRIVATE ${ogrengine_lib_header_dir} ${ogrengine_config_dir} ${ogrengine_media_dir})

#========================================
#	copy binary files
#========================================
FILE(COPY ${ogrengine_config_dir} DESTINATION ${CMAKE_BINARY_DIR})
FILE(COPY ${ogrengine_media_dir} DESTINATION ${CMAKE_BINARY_DIR})

#========================================
#	find OGRE
#========================================

SET(OGRENGINE_OGRE_INCLUDE_DIR CACHE PATH "")
SET(OGRENGINE_OGRE_LIBS_DIR CACHE PATH "")
SET(OGRENGINE_OGRE_SHARED_DIR CACHE PATH "")

IF(NOT OGRENGINE_OGRE_INCLUDE_DIR)
    MESSAGE(FATAL_ERROR "OGRENGINE_OGRE_INCLUDE_DIR variable is missing.")
ENDIF()

IF(NOT OGRENGINE_OGRE_LIBS_DIR)
    MESSAGE(FATAL_ERROR "OGRENGINE_OGRE_LIBS_DIR variable is missing.")
ENDIF()

IF(NOT OGRENGINE_OGRE_SHARED_DIR)
    MESSAGE(FATAL_ERROR "OGRENGINE_OGRE_SHARED_DIR variable is missing.")
ENDIF()

FILE(GLOB_RECURSE ogrengine_lib_files ${OGRENGINE_OGRE_LIBS_DIR}/*.lib)
FILE(GLOB_RECURSE ogrengine_shared_files ${OGRENGINE_OGRE_SHARED_DIR}/*.dll)

TARGET_INCLUDE_DIRECTORIES(${ogrengine_lib_name} PUBLIC ${OGRENGINE_OGRE_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(${ogrengine_lib_name} ${ogrengine_lib_files})

#========================================
#	create one project by name
#========================================
FUNCTION(ogrengineTest)

    #========================================
    #	variables
    #========================================
    SET(ogrengine_src_dir "src")
    SET(ogrengine_header_dir "include")

    #====================
    #	find files
    #====================
    FILE(GLOB_RECURSE ogrengine_src_files ${ogrengine_src_dir}/*)
    FILE(GLOB_RECURSE ogrengine_header_files ${ogrengine_header_dir}/*)

    #====================
    #	create project
    #====================
    GET_FILENAME_COMPONENT(ogrengine_current_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)

    ADD_EXECUTABLE(${ogrengine_current_name} ${ogrengine_src_files} ${ogrengine_header_files})
    TARGET_INCLUDE_DIRECTORIES(${ogrengine_current_name} PRIVATE ${ogrengine_header_dir} "../../${ogrengine_lib_header_dir}")
    TARGET_LINK_LIBRARIES(${ogrengine_current_name} PRIVATE ${ogrengine_lib_name})
    FILE(COPY ${ogrengine_shared_files} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
ENDFUNCTION()

#========================================
#	find subdirectories
#========================================
SET(ogrengine_project_dir "")

FILE(GLOB ogrengine_glob_dir "./sample/*")
FOREACH(ogrengine_dir ${ogrengine_glob_dir})
    LIST(APPEND ogrengine_project_dir ${ogrengine_dir})
ENDFOREACH()

#========================================
#	create projects
#========================================
FOREACH(ogrengine_dir ${ogrengine_project_dir})
    MESSAGE(INFO " generate project : " ${ogrengine_dir})
    ADD_SUBDIRECTORY(${ogrengine_dir})
ENDFOREACH()

