# CMake Sürüm
cmake_minimum_required(VERSION 3.10)

# Proje Adı
project(DownTimer)

# C Standartı
set(CMAKE_C_STANDARD 99)

# "tools" Klasöründeki Tüm Başlık (Header) Dosyalarını Bir Değişkende Topla
file(GLOB TOOLS_HEADERS "../tools/*.h")

# Projeye Dahil Edilecek Başlık Dosyalarını Belirt
include_directories(tools)

# Derleme Sırası: Önce "tools" Klasöründeki Başlık Dosyaları
add_custom_target(DT_ToolsHeaders ALL DEPENDS ${TOOLS_HEADERS})

# Ana Klasördeki "dt_init.h" Dosyasını Projeye Ekle
add_custom_target(DT_InitHeader ALL DEPENDS ${CMAKE_SOURCE_DIR}/../dt_init.h)

# "dt_kernel.c" Dosyasını Derle
add_executable(DownTimer ../dt_kernel.c)

# Derleme Sırasını Belirle
add_dependencies(DownTimer DT_ToolsHeaders DT_InitHeader)