set (LIB_SRC
    main.cpp
    HeaderGenerator.cpp
    HeaderGenerator.h
    SQLParserDefinitions.h
    SQLParserDefinitions.cpp
    TypeDefinitions.h
    TypeDefinitions.cpp
)


add_executable( ${PROJECT_NAME} ${LIB_SRC} )

target_link_libraries( ${PROJECT_NAME}
    PUBLIC ${Boost_LIBRARIES}
)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${Boost_INCLUDE_DIRS}
)