#include <iostream>
#include <unistd.h>     // For pipe(), fork(), read(), write()
#include <sys/wait.h>   
#include <cstring>

int main() {
    int fd[2];

    if (pipe(fd) == -1){
        std::cerr << "Failed to create pipe" << std::endl;
        return 1;
    }

    pid_t pid = fork();     // Create a child process

    if (pid < 0) {          // Error occur during the fork
        std::cerr << "Failed to fork";
        return 1;
    }

    if (pid > 0) {          // The child's PID in the parent process
        close(fd[0]);       // Close the read end, making the parent read only

        const char* message = "Hello from parent"; 
        write (fd[1], message, strlen(message)+1);      // Write to pipe
        std::cout << "Parent sent a message" << std::endl;

        close(fd[1]);   // Close the write end after sending the data
        wait(NULL);     // Wait for the child to finish
    }

    if (pid == 0){      // Child process
        close(fd[1]);   // Close the write end, making the children read only

        char buffer[100];
        read(fd[0], buffer, sizeof(buffer));    // Read from pipe
        std::cout << "Child received the message " << buffer << std::endl;
        close(fd[0]);                       // Close the read end
    }


    return 0;
}