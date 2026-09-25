set(FOUNDATION_ARM_CPU "cortex-m4" CACHE STRING "Target Arm CPU" FORCE)
set(FOUNDATION_FLOAT_ABI "hard" CACHE STRING "Target floating-point ABI" FORCE)
set(FOUNDATION_FPU "fpv4-sp-d16" CACHE STRING "Target FPU name" FORCE)

include("${CMAKE_CURRENT_LIST_DIR}/../arm-none-eabi.cmake")

