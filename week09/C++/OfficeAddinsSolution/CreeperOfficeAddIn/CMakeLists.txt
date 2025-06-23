# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称
project(CreeperOfficeAddIn)


set(SOURCE 
  source/compreg.cpp
  source/creeperaddin.cpp 
  source/creeperofficeaddin.cpp 
  source/creeperofficeaddini.c 
  source/creeperofficeaddinp.c 
  # source/dlldata.c 
  source/dllmain.cpp 
  source/EXCEL.tli 
  source/MSADDNDR.tli 
  source/MSO.tli 
  source/MSPPT.tli 
  source/MSWORD.tli
  source/pch.cpp 
  source/vbe6ext.tli 
  source/xdlldata.c)

set_source_files_properties(
  source/creeperofficeaddini.c
  source/creeperofficeaddinp.c
  source/xdlldata.c
  PROPERTIES LANGUAGE C
)

add_library(${PROJECT_NAME} SHARED ${SOURCE})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/headers)

target_link_libraries(${PROJECT_NAME} PRIVATE
  ${PROJECT_SOURCE_DIR}/lib/Ole32.Lib
  ${PROJECT_SOURCE_DIR}/lib/OleAut32.Lib
  ${PROJECT_SOURCE_DIR}/lib/RpcRt4.Lib
  ${PROJECT_SOURCE_DIR}/lib/Uuid.Lib
)




# 设置输出目录（可选）
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# Windows 特定设置（如果是 MSVC）
if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE "WIN32;_WINDOWS;_USRDLL;_AFXDLL;_ATL_DLL")
endif()

target_compile_options(${PROJECT_NAME} PRIVATE
  $<$<CONFIG:Debug>:/MDd>
  $<$<CONFIG:Release>:/MD>
)
