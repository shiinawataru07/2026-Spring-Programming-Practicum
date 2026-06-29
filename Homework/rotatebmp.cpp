#include<iostream>
#include<fstream>
#include<cstdint> 
#include<string>
#include<vector>
using namespace std;

#pragma pack(push, 1)
struct BITMAPFILEHEADER{
	uint16_t bfType = 0x4D42;
	uint32_t bfSize = 0;
	uint16_t bfReserved1 = 0;
	uint16_t bfReserved2 = 0;
	uint32_t bfOffBits = 0;
};

struct BITMAPINFOHEADER {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};
#pragma pack(pop)

void rotateBMP90(const string& inputPath, const string& outputPath) {
    ifstream fin(inputPath.c_str(), ios::binary);
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    fin.read((char*)&fileHeader, sizeof(fileHeader));
    fin.read((char*)&infoHeader, sizeof(infoHeader));
    int extraHeaderSize =
        fileHeader.bfOffBits - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
    vector<unsigned char> extraHeader(extraHeaderSize);
    if (extraHeaderSize > 0) {
        fin.read((char*)extraHeader.data(), extraHeaderSize);
    }
    int oldWidth = infoHeader.biWidth;
    int oldHeight = infoHeader.biHeight;
    int bytesPerPixel = infoHeader.biBitCount / 8;
    int oldStride = ((oldWidth * bytesPerPixel + 3) / 4) * 4;
    int oldImageSize = oldStride * oldHeight;
    vector<unsigned char> imageData(oldImageSize);
    fin.seekg(fileHeader.bfOffBits, ios::beg);
    fin.read((char*)imageData.data(), oldImageSize);
    fin.close();
    int newWidth = oldHeight;
    int newHeight = oldWidth;
    int newStride = ((newWidth * bytesPerPixel + 3) / 4) * 4;
    int newImageSize = newStride * newHeight;
    vector<unsigned char> newData(newImageSize, 0);
    for (int oldY = 0; oldY < oldHeight; oldY++) {
        for (int oldX = 0; oldX < oldWidth; oldX++) {
            int newX = oldY;
            int newY = oldWidth - 1 - oldX;
            unsigned char* srcPixel =
                &imageData[oldY * oldStride + oldX * bytesPerPixel];
            unsigned char* dstPixel =
                &newData[newY * newStride + newX * bytesPerPixel];
            for (int k = 0; k < bytesPerPixel; k++) {
                dstPixel[k] = srcPixel[k];
            }
        }
    }
    infoHeader.biWidth = newWidth;
    infoHeader.biHeight = newHeight;
    infoHeader.biSizeImage = newImageSize;
    fileHeader.bfSize = fileHeader.bfOffBits + newImageSize;
    ofstream fout(outputPath.c_str(), ios::binary);
    fout.write((char*)&fileHeader, sizeof(fileHeader));
    fout.write((char*)&infoHeader, sizeof(infoHeader));
    if (extraHeaderSize > 0) {
        fout.write((char*)extraHeader.data(), extraHeaderSize);
    }
    fout.write((char*)newData.data(), newImageSize);
    fout.close();
}

int main(int argc, char* argv[]){
	string inputPath = argv[1];
	string outputPath = argv[2];
	rotateBMP90(inputPath, outputPath);
	return 0;
}

