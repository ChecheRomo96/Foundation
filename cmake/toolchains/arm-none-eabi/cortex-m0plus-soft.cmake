set(FOUNDATION_ARM_CPU "cortex-m0plus" CACHE STRING "Target Arm CPU" FORCE)
set(FOUNDATION_FLOAT_ABI "soft" CACHE STRING "Target floating-point ABI" FORCE)
set(FOUNDATION_FPU "" CACHE STRING "Target FPU name" FORCE)

include("${CMAKE_CURRENT_LIST_DIR}/../arm-none-eabi.cmake")

