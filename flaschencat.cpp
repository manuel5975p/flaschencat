#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
#include <ProgramOptions.hxx>
#include <sys/types.h>
#include <sys/stat.h>
void do_file(int fd){
    constexpr size_t N = 1 << 20;
    char rdbuf[N];
    ssize_t accum = 0;
    loff_t offs = 0;
    int mypipe[2];
    if (pipe(mypipe)){
        return;
    }
    while(true){
        auto n = splice(fd, nullptr, mypipe[1], nullptr, N, SPLICE_F_MORE);
        if(n <= 0){
            break;
        }
        n = splice(mypipe[0], nullptr, STDOUT_FILENO, nullptr, N, SPLICE_F_MORE);
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
