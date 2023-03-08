# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(glove C CXX ASM)

# Add executable. Default name is the project name, version 0.1

add_executable(glove glove.c )

pico_set_program_name(glove "glove")
pico_set_program_version(glove "0.1")

pico_enable_stdio_uart(glove 0)
pico_enable_stdio_usb(glove 1)

target_sources(glove PUBLIC
		glove.c
		usb_descriptors.c
		imu_handler.c
		)

# Add the standard library to the build
target_link_libraries(glove
        pico_stdlib
		hardware_i2c
        tinyusb_device
		pico_bootrom
		pico-tflmicro
	)

# Add the standard include files to the build
target_include_directories(glove PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

pico_add_extra_outputs(glove)

