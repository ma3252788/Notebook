cmake_minimum_required(VERSION 3.14)
set(CMAKE_C_STANDARD 99)

project(FFT C)

add_executable(
        FFT
        main.c
        fft.c
        fft_tab.c
        fft_tab_creator.c
)

target_link_libraries(FFT m)
