all: copy_my

copy_my: my_copy.c
    gcc -o copy_my my_copy.c

clean:
    rm -f copy_my
