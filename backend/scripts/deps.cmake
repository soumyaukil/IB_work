SET(IB_API_ROOT "/home/local/ANT/soumukil/ib/IB_work/dependencies/IB/install/")
SET(QUICKFIX_ROOT "/home/local/ANT/soumukil/ib/IB_work/dependencies/quickfix-install/")

#### Interactive Brokers API library dependency ####
include_directories(${IB_API_ROOT}/include)
set(IB_API_LIB_PATH "${IB_API_ROOT}/lib")


include(ExternalProject)
ExternalProject_Add(
    boost
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/boost
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    GIT_TAG "boost-1.66.0"
    CONFIGURE_COMMAND "./bootstrap.sh"
    BUILD_COMMAND ./b2 --with-filesystem --with-thread --with-date_time --with-program_options --with-system --with-regex toolset=gcc variant=debug link=static install --prefix=${CMAKE_CURRENT_BINARY_DIR}/boostinstall
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND ""
)


add_library(boost::date_time STATIC IMPORTED)
add_library(boost::program_options STATIC IMPORTED)
add_library(boost::system STATIC IMPORTED)
add_library(boost::filesystem STATIC IMPORTED)
add_library(boost::thread STATIC IMPORTED)
add_library(boost::algorithm STATIC IMPORTED)

#set_property(TARGET boost::date_time PROPERTY IMPORTED_LOCATION ${BOOST_LIB_DIR}/libboost_date_time${BOOST_LIBRARY_SUFFIX})
#set_property(TARGET boost::program_options PROPERTY IMPORTED_LOCATION ${BOOST_LIB_DIR}/libboost_program_options${BOOST_LIBRARY_SUFFIX})
#set_property(TARGET boost::system PROPERTY IMPORTED_LOCATION ${BOOST_LIB_DIR}/libboost_iostreams${BOOST_LIBRARY_SUFFIX})
#set_property(TARGET boost::filesystem PROPERTY IMPORTED_LOCATION ${BOOST_LIB_DIR}/libboost_filesystem${BOOST_LIBRARY_SUFFIX})



add_dependencies(boost::date_time boost)
add_dependencies(boost::program_options boost)
add_dependencies(boost::system boost)
add_dependencies(boost::filesystem boost)
add_dependencies(boost::thread boost)
add_dependencies(boost::algorithm boost)

set(Boost_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR}/boostinstall/include)
set(Boost_LIBRARIES ${CMAKE_CURRENT_BINARY_DIR}/boostinstall/lib/libboost_date_time.a 
                    ${CMAKE_CURRENT_BINARY_DIR}/boostinstall/lib/libboost_thread.a 
                    ${CMAKE_CURRENT_BINARY_DIR}/boostinstall/lib/libboost_program_options.a
                    ${CMAKE_CURRENT_BINARY_DIR}/boostinstall/lib/libboost_system.a)

set_property(TARGET boost::program_options PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
set_property( TARGET boost::filesystem PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS} )
set_property( TARGET boost::thread PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS} )
#add_dependencies(boost::filesystem boost)
#add_dependencies(boost::thread boost)

include_directories(${Boost_INCLUDE_DIRS})


#### System library dependency ####
set(PLATFORM_SPECIFIC_LIBRARIES pthread)

#### QuickFix library dependency ####
include_directories(${QUICKFIX_ROOT}/include)
include_directories(${QUICKFIX_ROOT}/include/quickfix) # This is needed for shared lib
set(QUICKFIX_LIB_PATH "${QUICKFIX_ROOT}/lib")


set(CMAKE_LIBRARY_PATH ${IB_API_LIB_PATH} ${QUICKFIX_LIB_PATH})

find_library(QUICKFIX_LIB quickfix ${QUICKFIX_LIB_PATH})

find_library(IB_API_LIB ib_api)

include_directories(${CMAKE_BINARY_DIR})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
find_package(ODB REQUIRED COMPONENTS mysql)
include(${ODB_USE_FILE})
