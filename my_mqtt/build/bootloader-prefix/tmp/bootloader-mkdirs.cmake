# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/asogw/Desktop/esp32/esp32idf/frameworks/esp-idf-v5.0.2/components/bootloader/subproject"
  "C:/Users/asogw/Desktop/esp32/my_mqtt/build/bootloader"
  "C:/Users/asogw/Desktop/esp32/my_mqtt/build/bootloader-prefix"
  "C:/Users/asogw/Desktop/esp32/my_mqtt/build/bootloader-prefix/tmp"
  "C:/Users/asogw/Desktop/esp32/my_mqtt/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/asogw/Desktop/esp32/my_mqtt/build/bootloader-prefix/src"
  "C:/Users/asogw/Desktop/esp32/my_mqtt/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/asogw/Desktop/esp32/my_mqtt/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/asogw/Desktop/esp32/my_mqtt/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
