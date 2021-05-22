#pragma once
#include <stdio.h>

namespace sshsu
{
    class BufferReader
    {
    public:
        BufferReader()=delete;
        
        BufferReader(int picWidth, int picHeight, const char *file_name);

        unsigned char *Data();

        ~BufferReader();

        bool ReadFrame();

    private:
        FILE *fd_;
        int width_;
        int height_;
        unsigned char *data_;
    };
}