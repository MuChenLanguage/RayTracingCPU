#ifndef COMMOM_UTILS_H
#define COMMOM_UTILS_H

#include <glad\glad.h>
#include <iostream>

namespace RayTracing {
    class Utils {
    public:
        static bool LoadTexture(const char* filename, GLuint* out_texture, int* out_width, int* out_height);

        static GLuint LoadTexture(const char* filename);


        static void trimString(std::string& str) {
            const char* whiteSpace = " \t\n\r";
            size_t location;
            location = str.find_first_not_of(whiteSpace);
            str.erase(0, location);
            location = str.find_last_not_of(whiteSpace);
            str.erase(location + 1);
        }

        static GLuint loadTexture(const std::string& fName);
        static GLuint loadCubeMap(const std::string& baseName, const std::string& extention = ".png");
        static GLuint loadHdrCubeMap(const std::string& baseName);
        static unsigned char* loadPixels(const std::string& fName, int& w, int& h, bool flip = true);
        static unsigned char* LoadPixel(const char* fName, int& w, int& h, int a, bool flip = true);
        static void deletePixels(unsigned char*);
        static GLuint LoadTexFromMem(GLubyte* data, int w, int h);
        static void SaveImage(std::string path, int w, int h, int n, unsigned char* data);
    };


}
#endif // !COMMOM_UTILS_H
