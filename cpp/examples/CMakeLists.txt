
list(APPEND apps
    main
)

foreach(name ${apps})
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} ${PROJECT_NAME})
endforeach()
