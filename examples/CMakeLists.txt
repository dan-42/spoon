
#project(example_spoon)
#cmake_minimum_required(VERSION 3.2)


if(TARGET libspoon)
 # add_library(libspoon::libspoon ALIAS libspoon)
message(INFO "TARGET")
else()  
 # find_package(libspoon CONFIG REQUIRED)
 # message(INFO "find_package")
endif()

hunter_add_package(nlohmann_json)
find_package(nlohmann_json CONFIG REQUIRED)



hunter_add_package(Boost COMPONENTS )
set(BOOST_ALL_DYN_LINK         ON)
set(Boost_USE_MULTITHREADED    ON)
set(Boost_USE_STATIC_LIBS      OFF)
set(Boost_USE_STATIC_RUNTIME   OFF)

find_package(Boost COMPONENTS  REQUIRED )
link_directories (${Boost_LIBRARY_DIRS})
include_directories (${Boost_INCLUDE_DIRS})
add_definitions(-DBOOST_LOG_DYN_LINK)

include_directories(${CMAKE_CURRENT_LIST_DIR} )
include_directories(${CMAKE_CURRENT_LIST_DIR}/../src )

set (CMAKE_CXX_FLAGS "-std=c++1z ${CMAKE_CXX_FLAGS}")
add_compile_options(--std=c++14 -save-temps=obj)
add_definitions(--std=c++14 -save-temps=obj)
add_compile_options(
      -s
      -fvisibility=hidden 
      -O3
      -ffunction-sections
      -fdata-sections)

function(add_new_example example_name file_path)
    set(executable_name "${PROJECT_NAME}-${example_name}")

    file(GLOB SPOON_TEST_SOURCE_CPP "${CMAKE_CURRENT_SOURCE_DIR}/${file_path}")

    add_executable ("${executable_name}"  "${SPOON_TEST_SOURCE_CPP}")
    target_link_libraries (${executable_name}
                #                libspoon::libspoon 
                #nlohmann_json::nlohmann_json
                                ${Boost_LIBRARIES} 
                                ${PSAPI_LIBRARIES} 
                                pthread                                
                                )
    target_link_libraries(${executable_name} nlohmann_json)                                  
    add_test (NAME ${executable_name} COMMAND $<TARGET_FILE:${executable_name}>)

endfunction(add_new_example)

#add_new_example(poc poc.cpp)
add_new_example(poc2 poc2.cpp)
add_new_example(bacnet_npdu bacnet_npdu.cpp)

#add_new_example(continuiation continuiation.cpp)


