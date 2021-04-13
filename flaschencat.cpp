#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
#include <ProgramOptions.hxx>
#include <memmap.hpp>
#include <time.hpp>
void do_file(int fd){
    struct stat st;
    fstat(fd, &st);
    if(S_ISREG(st.st_mode) && false){
        
        constexpr size_t BS = 1 << 16;
        auto m_size = lseek(fd, 0, SEEK_END);
        //std::cerr << m_size << "\n";
        char* data = (char*)mmap(0, m_size, (PROT_READ), MAP_SHARED, fd, 0);
        write(STDOUT_FILENO, data, m_size);
        /*for(size_t i = 0;i < m_size; i += BS){
            size_t nbs = std::min(m_size - i, BS);
            write(STDOUT_FILENO, data + i, nbs);
        }*/
    }
    else{
        constexpr size_t N = 1 << 20;
        char rdbuf[N];
        //auto timestmp = nanoTime();
        ssize_t accum = 0;
        loff_t offs = 0;
        int mypipe[2];
        if (pipe(mypipe)){
            //std::cerr << "Pipe failed.\n";
            return;
        }
        while(true){
            auto n = splice(fd, nullptr, mypipe[1], nullptr, N, SPLICE_F_MORE);
            //accum += n;
            if(n <= 0){
                break;
            }

            n = splice(mypipe[0], nullptr, STDOUT_FILENO, nullptr, N, SPLICE_F_MORE);
        }
    }
}
int main(int argc, char** args){
    po::parser parser;
    std::vector<std::string> files;
    parser[""].bind(files);
    parser(argc, args);
    if(files.empty()){
        do_file(STDIN_FILENO);
    }
    else{
        for(auto& file : files){
            int fd = open(file.c_str(), O_RDONLY);
            if(fd > 0) {
                do_file(fd);
                close(fd);
            }
        }
    }
}
