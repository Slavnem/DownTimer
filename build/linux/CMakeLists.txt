# CMake En Düşük Sürüm
cmake_minimum_required(VERSION 3.10)

# Proje Adı ve C Standartı
project(DownTimer C)

# C11 Standartını Kullan
set(CMAKE_C_STANDARD 11)

# Ana Klasör
set(ROOT_DIR "${CMAKE_SOURCE_DIR}/../../src")

# "core" Dizinindeki Tüm ".c" Dosyalarını Bul ve Listele
file(GLOB CORE_SOURCES ${ROOT_DIR}/core/*.c)

# Derlenecek Kaynak Dosyalar
set(MAIN_SOURCE ${ROOT_DIR}/main.c)

# Linux
message(STATUS "Linux Icin Konfigure Ediliyor...")

# 32-Bit (x86) ve 64-Bit (x64) Derleme
if(CMAKE_SIZEOF_VOID_P EQUAL 8) # void* Eğer 8 Bayt ise x64, Değilse 4 Bayt x86
	message(STATUS "Building For Linux x64...")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64")
else()
	message(STATUS "Building For Linux x86...")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
endif()

# GCC Optimizasyon Bayrakları
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2")

# Çıktı Dosyası (Program) Oluştur
add_executable(DownTimer ${MAIN_SOURCE} ${CORE_SOURCES})
