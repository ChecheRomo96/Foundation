if(FOUNDATION_MATH)
    add_subdirectory(Math)
endif()

if(FOUNDATION_TYPE_TRAITS)
    add_subdirectory(TypeTraits)
endif()

if(FOUNDATION_CONTAINERS)
    add_subdirectory(Containers)
endif()

if(FOUNDATION_FUNCTIONAL)
    add_subdirectory(Functional)
endif()

if(FOUNDATION_TIME)
    add_subdirectory(Time)
endif()

if(FOUNDATION_SCHEDULING)
    add_subdirectory(Scheduling)
endif()

if(FOUNDATION_UTILS)
    add_subdirectory(Utils)
endif()
