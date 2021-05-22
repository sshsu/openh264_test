#说明
该例子来自于openh264的[wiki example](https://github.com/cisco/openh264/wiki/UsageExampleForEncoder)，主要是测试openh264编码过程中的更改码率的能力，直接设置编码器的bitrate没有效果，需要改变空域层bitrate，实测发现变更的码率设置的太低会导致一些帧被过滤掉



# 编译运行
请保证已经编译安装了openh264
```
mkdir build
cd build
cmake ..
make 
```
运行, 就可以在根目录的res下看到编码好的output.264
```
./openh264_encode
```
