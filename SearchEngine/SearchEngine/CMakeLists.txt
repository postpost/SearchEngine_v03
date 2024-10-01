cmake_minimum_required(VERSION 3.8)
project (SearchEngine)

# Add source to this project's executable.
add_executable (${CMAKE_PROJECT_NAME}
				#executable main
				SearchEngine.cpp

				#other source files
				#Crawler
				Crawler.h
				DataBaseManager.h
				DataBaseManager.cpp
				Indexer.h
				Indexer.cpp
				IniParser.h
				#Server
				Server.h
				Server.cpp
			
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SearchEngine PROPERTY CXX_STANDARD 20)
endif()

#OpenSSL
find_package(OpenSSL REQUIRED)
if (OpenSSL_FOUND)

 # Add the static lib for linking
 # target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES}) #OpenSSL::SSL OpenSSL::Crypto
  
 message(STATUS "Found OpenSSL ${OPENSSL_VERSION}")

else()

  message(STATUS "OpenSSL Not Found")

endif()


#add files
file(GLOB MY_FILES "*.ini") 
file(COPY ${MY_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY config.ini
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
               
#libpqxx
add_subdirectory("C:/libpqxx-7.9.2" libpqxx-build)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_compile_definitions(${PROJECT_NAME}
			  PUBLIC
				BOOST_LOCALE_NO_LIB
			  PRIVATE
				BOOST_LOCALE_SOURCE
				_CRT_SECURE_NO_WARNINGS
				_SCL_SECURE_NO_WARNINGS
)

#BOOST LIBS
find_package(Boost 1.82.00 REQUIRED)

target_include_directories(${PROJECT_NAME}
			PUBLIC
				$<INSTALL_INTERFACE:include>
				$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
				${Boost_INCLUDE_DIRS}
				Boost::assert
				${OPENSSL_INCLUDE_DIR}
			PRIVATE
)

target_link_libraries(${PROJECT_NAME}
			PUBLIC
				pqxx
				${Boost_LIBRARIES}
				${OPENSSL_LIBRARIES}
)


#add subdirectory for HTML Database
add_subdirectory(HTMLDB)
add_subdirectory(HTMLDownloaded)

cmake_policy(SET CMP0074 NEW)
