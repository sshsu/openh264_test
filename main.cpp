#include <iostream>
#include <wels/codec_api.h>
#include <wels/codec_app_def.h>
#include <wels/codec_def.h>
#include <wels/codec_ver.h>
#include <assert.h>
#include <string.h>
#include "bufferReader.h"
#include <stdio.h>

struct Args{
    int picHeight= 288;
    int picWidth = 352;
    const char* srcFile =  "/media/soccor.yuv";
    const char* dstFile =  "./output.264";
};

void InitParam(SEncParamBase& param, const int picWidth, const int picHeight){
    memset(&param, 0, sizeof(SEncParamBase));
    param.iUsageType = CAMERA_VIDEO_REAL_TIME;
    param.fMaxFrameRate = 50;
    param.iPicWidth = picWidth;
    param.iPicHeight = picHeight;
    param.iTargetBitrate = 900 * 1000;
    param.iRCMode = RC_BITRATE_MODE;
}

void  SetupEncoder(ISVCEncoder*& encoder, const SEncParamBase& param){
    int rv = WelsCreateSVCEncoder(&encoder);
    assert(0 == rv);
    assert(encoder != NULL);
    assert(encoder->Initialize(&param) == 0);

    //changing logging granularity
    int g_LevelSetting = 1;
    encoder->SetOption(ENCODER_OPTION_TRACE_LEVEL, &g_LevelSetting);

    int videoFormat = videoFormatI420;
    encoder->SetOption(ENCODER_OPTION_DATAFORMAT, &videoFormat);

    //changing encoding param
    //encoder->SetOption(ENCODER_OPTION_SVC_ENCODE_PARAM_BASE, &param);

}

void SetupPic(unsigned char* data, SFrameBSInfo& info, SSourcePicture& pic, const int& picHeight, const int& picWidth){  
    memset(&info, 0, sizeof(SFrameBSInfo));
    memset(&pic, 0, sizeof(SSourcePicture));
    pic.iPicWidth = picWidth;
    pic.iPicHeight = picHeight;
    pic.iColorFormat = videoFormatI420;
    pic.iStride[0] = pic.iPicWidth;
    pic.iStride[1] = pic.iStride[2] = pic.iPicWidth >> 1;
    pic.pData[0] = data;
    pic.pData[1] = pic.pData[0] + picWidth * picHeight;
    pic.pData[2] = pic.pData[1] + (picWidth * picHeight >> 2);
}

void Encode(ISVCEncoder* encoder, const Args& args){
    
    const char* file_name = "/media/soccor.yuv";
    sshsu::BufferReader buf(args.picWidth, args.picHeight, file_name);
    FILE* outFd = fopen("./output.264", "w+");
    assert(outFd!= nullptr);
    
    SFrameBSInfo info;
    SSourcePicture pic;
    SetupPic(buf.Data(), info, pic, args.picHeight, args.picWidth);

    
    SEncParamExt encParamExt;
    encoder->GetOption(ENCODER_OPTION_SVC_ENCODE_PARAM_EXT, &encParamExt);

    int frame_num = 0;
    int total_bytes = 0;
    while(buf.ReadFrame()){
        if(frame_num == 100){

            encParamExt.sSpatialLayers[0].iSpatialBitrate =  100 * 1000;
            encParamExt.sSpatialLayers[0].iMaxSpatialBitrate =  100 * 1000;
            encoder->SetOption(ENCODER_OPTION_SVC_ENCODE_PARAM_EXT, &encParamExt);
        }
        int rv = encoder->EncodeFrame(&pic, &info);
        assert(rv == cmResultSuccess);
        if (info.eFrameType != videoFrameTypeSkip)
        {   
            
            int iLayerNum = info.iLayerNum; 
            for(int i = 0;i<iLayerNum;++i){
                SLayerBSInfo &layer = info.sLayerInfo[i];
                int iLayerSize = 0;
                int iLNalIdx = layer.iNalCount - 1;
                do
                {
                    iLayerSize += layer.pNalLengthInByte[iLNalIdx];
                } while (--iLNalIdx >= 0);
                fwrite(layer.pBsBuf, 1, iLayerSize, outFd);
                ++frame_num;
                total_bytes += iLayerSize;
                std::cout << "save frame: " << frame_num << ", size:" << iLayerSize << std::endl;
            }
        }
        else{
            std::cout<<"videoFrameTypeskip for rate control"<<std::endl;
        }
    }
    std::cout << "total bytes: " << total_bytes <<std::endl;
    fflush(outFd);
    fclose(outFd);
}



int main(int, char**) {
    Args args;
    SEncParamBase encoderParam;
    InitParam(encoderParam, args.picWidth, args.picHeight);
    ISVCEncoder* encoder;
    SetupEncoder(encoder, encoderParam);
    Encode(encoder, args);
    encoder->Uninitialize();
    WelsDestroySVCEncoder(encoder);
    return 0;
}
