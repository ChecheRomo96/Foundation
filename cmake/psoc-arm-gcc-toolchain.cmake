set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(PSoC_GCC_PATH "C:/Program Files (x86)/Cypress/PSoC Creator/4.4/PSoC Creator/import/gnu/arm/5.4.1/bin")

set(CMAKE_C_COMPILER   "${PSoC_GCC_PATH}/arm-none-eabi-gcc.exe")
set(CMAKE_CXX_COMPILER "${PSoC_GCC_PATH}/arm-none-eabi-g++.exe")
set(CMAKE_AR           "${PSoC_GCC_PATH}/arm-none-eabi-ar.exe")
set(CMAKE_RANLIB       "${PSoC_GCC_PATH}/arm-none-eabi-ranlib.exe")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)