
#include <unistd.h>    //reading
#include <fcntl.h>     // open
#include <sys/types.h> //
#include <sys/stat.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[])
{

    // check 2 arguments provided (source file and destination file)
    if (argc != 3)
    {
        char *msg = "Usage: ./my_copy <source_file> <destination_file>\n";
        write(2, msg, 46); // error message
        return 1;
    }

    // open source file for reading
    int src_fd = open(argv[1], O_RDONLY);
    if (src_fd < 0)
    {
        char *err = "Error: cannot open source file\n";
        write(2, err, 30); // print error to stderr
        return 1;
    }
    // try opening destination file for writing, fail if exists
    int dest_fd = open(argv[2], O_WRONLY | O_EXCL);
    if (dest_fd < 0) //dest file exists
    {
        char *msg = "Destination file already exists. Overwrite? (y/n): ";
        write(1, msg, 45);
    }
    char response;
    // till valid input
    while (1)
    {
        //read single char response
        if (read(0, &response, 1) != 1)
        {
            write(2, "Error reading input\n", 20);
            close(src_fd);
            return 1;
        }

        // ignore newline
        if (response == '\n')
            continue;

        if (response == 'y' || response == 'Y')
        {
            //open with truncate to overwrite
            dest_fd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
            if (dest_fd < 0)
            {
                write(2, "Cannot overwrite destination file\n", 33);
                close(src_fd);
                return 1;
            }
            break;
        }

        //user declined overwrite
        else if (response == 'n' || response == 'N')
        {
            write(1, "Copy cancelled by user\n", 23);
            close(src_fd);
            return 0;
        }
        else // invalid input
        {
            write(1, "Please enter y or n: ", 21);
        }
    }
    //buffer used to transfer data
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    //read from file in parts
    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0)
    {
        ssize_t bytes_written = 0;
        //making sure all read bytes are written
        while (bytes_written < bytes_read)
        {
            ssize_t bw = write(dest_fd, buffer + bytes_written, bytes_read - bytes_written);
            if (bw < 0)
            {
                write(2, "Error writing to destination\n", 29);
                close(src_fd);
                close(dest_fd);
                return 1;
            }
            bytes_written += bw;
        }
    }

    //check for read error
    if (bytes_read < 0)
    {
        write(2, "Error reading source file\n", 26);
    }
    // close files
    close(src_fd);
    close(dest_fd);

    return 0;
}
