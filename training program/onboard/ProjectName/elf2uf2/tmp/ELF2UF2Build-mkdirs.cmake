# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/media/shared drive/electronics/pico git stuff/pico-sdk/tools/elf2uf2"
  "/media/shared drive/electronics/test/ProjectName/elf2uf2"
  "/media/shared drive/electronics/test/ProjectName/elf2uf2"
  "/media/shared drive/electronics/test/ProjectName/elf2uf2/tmp"
  "/media/shared drive/electronics/test/ProjectName/elf2uf2/src/ELF2UF2Build-stamp"
  "/media/shared drive/electronics/test/ProjectName/elf2uf2/src"
  "/media/shared drive/electronics/test/ProjectName/elf2uf2/src/ELF2UF2Build-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/media/shared drive/electronics/test/ProjectName/elf2uf2/src/ELF2UF2Build-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/media/shared drive/electronics/test/ProjectName/elf2uf2/src/ELF2UF2Build-stamp${cfgdir}") # cfgdir has leading slash
endif()
