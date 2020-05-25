@echo off

cd src
gcc find_frequent_words.c file_io/file_io.c hash_table/hash_table.c word_mod/word_mod.c linked_list/linked_list.c -Wall -o ../_start_program

cd ..
_start_program.exe "big.txt"