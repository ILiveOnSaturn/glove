# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/media/shared drive/electronics/pico git stuff/pico-sdk/tools/pioasm"
  "/media/shared drive/electronics/glove/onboard code/glove/build/pioasm"
  "/media/shared drive/electronics/glove/onboard code/glove/build/pico-sdk/src/rp2_common/cyw43_driver/pioasm"
  "/media/shared drive/electronics/glove/onboard code/glove/build/pico-sdk/src/rp2_common/cyw43_driver/pioasm/tmp"
  "/media/shared drive/electronics/glove/onboard code/glove/build/pico-sdk/src/rp2_common/cyw43_driver/pioasm/src/PioasmBuild-stamp"
  "/media/shared drive/electronics/glove/onboard code/glove/build/pico-sdk/src/rp2_common/cyw43_driver/pioasm/src"
  "/media/shared drive/electronics/glove/onboard code/glove/build/pico-sdk/src/rp2_common/cyw43_driver/pioasm/src/PioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/media/shared drive/electronics/glove/onboard code/glove/build/pico-sdk/src/rp2_common/cyw43_driver/pioasm/src/PioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/media/shared drive/electronics/glove/onboard code/glove/build/pico-sdk/src/rp2_common/cyw43_driver/pioasm/src/PioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
