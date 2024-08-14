#include <stdlib.h> // EXIT codes
#include <stdio.h> // printf
#include <unistd.h> // sysconf
#include <fcntl.h> // open file
#include <errno.h> // errno
#include <sys/time.h> // timeval
#include <string.h> // memcpy
#include <sys/mman.h> // mmap

int main()
{
    int uiofd;
    int err = 0;
    unsigned int i;
    fd_set uiofd_set;
    struct timeval tv;
    int interrupt_count;
    char* mem;


    // open uio0
    uiofd = open("/dev/uio0", O_RDONLY);
    if (uiofd < 0) {
        perror("uio open:");
        return errno;
    }
    
    // create mmap on uio0 / map0
    mem = mmap(NULL, sysconf(_SC_PAGE_SIZE), PROT_READ, MAP_SHARED, uiofd , 0);
    if (mem == MAP_FAILED){
        perror("mmap failed");
        close(uiofd);
        exit(EXIT_FAILURE);
    }

    // prepare select structure
    FD_ZERO(&uiofd_set);
    FD_SET(uiofd, &uiofd_set);

    for(i = 0;i < 10; ++i) {
        //  five second timeout (reset each time)
        tv.tv_sec = 5;
        tv.tv_usec = 0;

        // wait for interrupt
        err = select(uiofd+1, &uiofd_set, NULL, NULL, &tv);
        if (err < 0){
            perror("select()");
            break;
        }else if(err == 0){
            printf("Timeout, exiting\n");
            break;
        }
        
        // actualy read, acknoledges the interrupt
        char buf[4];
        err = read(uiofd, buf, 4);
        if (err !=4){
            perror("Read error.");
            break;
        }
        
        // read interrupt count from memory mapping
        memcpy(&interrupt_count, mem, sizeof(interrupt_count));
        printf("Read interrupt count : %d \n", interrupt_count);

    }
    
    munmap(mem, sysconf(_SC_PAGE_SIZE));
    close(uiofd);
    exit(err);
}
