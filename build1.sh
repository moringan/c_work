set -e    # if error, stop the build
gcc -g -std=gnu99 -c -I /home/mike/Downloads/raylib/src hellow.c -o ./obj/hellow.o 
gcc -o hellow obj/hellow.o -s -Wall -std=c99 -I/home/mike/Downloads/raylib/src -L/home/mike/raylib/release/libs/linux -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

