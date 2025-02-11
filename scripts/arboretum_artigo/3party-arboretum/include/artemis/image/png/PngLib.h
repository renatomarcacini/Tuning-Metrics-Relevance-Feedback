/* Copyright 2003-2017 GBDI-ICMC-USP <caetano@icmc.usp.br>
* 
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*   http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
/**
* @file
*
* This file defines a Class to manipulate png images. 
* All methods required by ImageBase are implemented.
*
* @version 1.0
*/

#ifndef PNGLIB_HPP
#define PNGLIB_HPP

#include <opencv2/highgui.hpp>
//#include <opencv/cv.h>
//#include <opencv/highgui.h>

#include <artemis/image/ImageBase.h>

/**
* This is a base class to manipulate png files
*
* @brief Handler Class to BMP Images
* @see ImageBase
* @see pnglib.cpp
* @version 1.0
*/
class PNGImage : public Image{

    private:
        void loadPixelMatrix() throw (std::runtime_error);

    public:
        PNGImage();
        PNGImage(std::string filename);
        virtual ~PNGImage();

        void openImage(std::string filename) throw (std::runtime_error);
        void saveToFile(std::string filename) throw (std::runtime_error);
        
        PNGImage* clone();
};
#endif
