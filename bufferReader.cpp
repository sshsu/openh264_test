#include "bufferReader.h"
#include <stdio.h>
#include <assert.h>

namespace sshsu{
BufferReader::BufferReader(int picWidth, int picHeight, const char *file_name)
{
    width_ = picWidth;
    height_ = picHeight;
    int frame_size = width_ * height_ * 3 / 2;
    data_ = new unsigned char[frame_size];
    assert(data_ != nullptr);

    fd_ = fopen(file_name, "r+");
    assert(fd_ != nullptr);
}

unsigned char *BufferReader::Data()
{
    return data_;
}

bool BufferReader::ReadFrame(){
    int frame_size = width_ * height_;
    frame_size = frame_size * 3 / 2;
    if (fread(data_, 1, frame_size, fd_) == 0)
    {
        return false;
    }
    else 
        return true;
}

BufferReader::~BufferReader()
{
    fclose(fd_);
    delete[] data_;
}

}