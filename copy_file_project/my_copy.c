
#include <unistd.h> //reading
#include <fcntl.h> // open
#include <sys/types.h>
#include <sys/stat.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
	if (argc != 3) {
        char *msg = "Usage: ./my_copy <source_file> <destination_file>\n";
        write(2, msg, 46); // 2 is STDERR_FILENO
        return 1;
    }

    int src_fd = open(argv[1], O_RDONLY);
    if (src_fd < 0) {
        char *err = "Error: cannot open source file\n";
        write(2, err, 30); // 2 = STDERR
        return 1;
    }

int dest_fd = open(argv[2], O_WRONLY | O_EXCL);
    if (dest_fd < 0) {
        char *msg = "Destination file already exists. Overwrite? (y/n): ";
        write(1, msg, 45);
}
        char response;
while (1) {
    if (read(0, &response, 1) != 1) {
        write(2, "Error reading input\n", 20);
        close(src_fd);
        return 1;
    }

    // ignore newline
    if (response == '\n') continue;

    if (response == 'y' || response == 'Y') {
        // Open with truncate to overwrite
        dest_fd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
        if (dest_fd < 0) {
            write(2, "Cannot overwrite destination file\n", 33);
            close(src_fd);
            return 1;
        }
        break;
    } else if (response == 'n' || response == 'N') {
        write(1, "Copy cancelled by user\n", 23);
        close(src_fd);
        return 0;
    } else {
        write(1, "Please enter y or n: ", 21);
    }
}


    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        ssize_t bytes_written = 0;
        while (bytes_written < bytes_read) {
            ssize_t bw = write(dest_fd, buffer + bytes_written, bytes_read - bytes_written);
            if (bw < 0) {
                write(2, "Error writing to destination\n", 29);
                close(src_fd);
                close(dest_fd);
                return 1;
            }
            bytes_written += bw;
        }
    }

    if (bytes_read < 0) {
        write(2, "Error reading source file\n", 26);
    }
    //close files
    close(src_fd);
    close(dest_fd);


return 0;
}
