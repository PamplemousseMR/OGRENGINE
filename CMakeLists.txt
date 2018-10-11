#======================================== 
#	project
#========================================
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT("OGRENGINE")

#================================================================================  
#		init
#================================================================================ 

#======================================== 
#	variables
#========================================
SET(ogrengine_src_dir "src")
SET(ogrengine_header_dir "include")
SET(ogrengine_project_name "ogrengine")
SET(ogrengine_config_dir "config")
SET(ogrengine_media_dir "media")

#======================================== 
#	compiler specifications
#========================================
SET(CMAKE_CXX_STANDARD 17)

#======================================== 
#	find files
#========================================
FILE(GLOB_RECURSE ogrengine_src_files ${ogrengine_src_dir}/*)
FILE(GLOB_RECURSE ogrengine_header_files ${ogrengine_header_dir}/*)
FILE(GLOB_RECURSE ogrengine_config_files ${ogrengine_config_dir}/*)
FILE(GLOB_RECURSE ogrengine_media_files ${ogrengine_media_dir}/*)

#================================================================================  
#		create
#================================================================================

#======================================== 
#	create project
#========================================
ADD_EXECUTABLE(${ogrengine_project_name}  ${ogrengine_src_files} ${ogrengine_header_files} ${ogrengine_config_files} ${ogrengine_media_files})
TARGET_INCLUDE_DIRECTORIES(${ogrengine_project_name} PRIVATE ${ogrengine_header_dir} ${ogrengine_config_dir} ${ogrengine_media_dir})

#========================================
#	copy binary files
#========================================
FILE(COPY ${ogrengine_config_dir} DESTINATION ${CMAKE_BINARY_DIR})
FILE(COPY ${ogrengine_media_dir} DESTINATION ${CMAKE_BINARY_DIR})

#========================================
#	add OGRE
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

TARGET_INCLUDE_DIRECTORIES(${ogrengine_project_name} PRIVATE ${OGRENGINE_OGRE_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(${ogrengine_project_name} ${ogrengine_lib_files})

FILE(COPY ${ogrengine_shared_files} DESTINATION ${CMAKE_BINARY_DIR})

