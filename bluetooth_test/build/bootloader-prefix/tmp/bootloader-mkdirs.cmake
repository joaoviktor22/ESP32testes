# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/joaoviktor/esp/esp-idf/components/bootloader/subproject"
  "/Users/joaoviktor/ESP-TESTES/bluetooth_test/build/bootloader"
  "/Users/joaoviktor/ESP-TESTES/bluetooth_test/build/bootloader-prefix"
  "/Users/joaoviktor/ESP-TESTES/bluetooth_test/build/bootloader-prefix/tmp"
  "/Users/joaoviktor/ESP-TESTES/bluetooth_test/build/bootloader-prefix/src/bootloader-stamp"
  "/Users/joaoviktor/ESP-TESTES/bluetooth_test/build/bootloader-prefix/src"
  "/Users/joaoviktor/ESP-TESTES/bluetooth_test/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/joaoviktor/ESP-TESTES/bluetooth_test/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/joaoviktor/ESP-TESTES/bluetooth_test/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
