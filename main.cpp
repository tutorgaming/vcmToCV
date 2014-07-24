////////////////////////////////////////////////////
// UPENNALIZERS's MANAGED SHARED MEMORY READER
// READ = YUYV IMAGE TO OPENCV
// by CU-EIC team
// Linker option :  -lopencv_core
//                  -lopencv_highgui
//                  -lopencv_imgproc
//                  -lrt
//                  -lpthread
////////////////////////////////////////////////////
#include <iostream>
#include <opencv/cv.h> //for cvtColor
#include <opencv2/opencv.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace cv;
using namespace boost::interprocess;
typedef double  value_t;


void ProcessColor(cv::Mat &source, cv::Mat &output)
	{
        output.create(source.rows, source.cols, CV_8UC3);
        int size = source.rows * source.cols;

        cv::Vec4b *ptr_input  = (cv::Vec4b *)source.ptr(0); //SOURCE(vec4b) POINTER
        cv::Vec3b *ptr_output = (cv::Vec3b *)output.ptr(0); //OUTPUT(vec3b) POINTER

        for(int i = 0; i < size; i+=2) //YUYV 1 INPUT = 2 PIXEL/ROUND
        {
            //PIXEL1
            (*ptr_output)[0] = (*ptr_input)[0];//Y1
            (*ptr_output)[1] = (*ptr_input)[1];//U
            (*ptr_output)[2] = (*ptr_input)[3];//V
            ptr_output++;//GO TO NEXT PIXEL
            //PIXEL2
            (*ptr_output)[0] = (*ptr_input)[2];//Y2
            (*ptr_output)[1] = (*ptr_input)[1];//U
            (*ptr_output)[2] = (*ptr_input)[3];//V
            ptr_input++; // PREPARE NEXT SOURCE OF Y1-U-Y2-V FOR NEXT ROUND
            ptr_output++; //PREPARE NEXT PIXEL FOR NEXT ROUND
        }

	}


void openShmAndCopyToMat(const char *key , unsigned char *dst){

    //Create Shared Memory manager Segment (Files in /dev/shm)
    managed_shared_memory segment(open_only,"vcmImage181tutorgaming");

    //Create Data Type Pair<*double,site_t> for Receiving Pair From Segment
    std::pair<value_t*, std::size_t> ret;

    //Find The Pair that relavalant with the key
    ret = segment.find<value_t>(key);

    //Copy To Pointer Of Char "dst" (destination , from , sizeToMalloc)
    memcpy(dst, (unsigned char*)ret.first, ret.second*sizeof(value_t));
}



int main()
{
    int width = 640;
    int height = 480;
    cv::Mat M(height,width, CV_8UC2);
    cv::Mat yuvImg(height,width, CV_8UC3);

    //Mat For extended Type "RGB"
    cv::Mat rgbImg(height,width, CV_8UC3);
    std::cout << "showing Yuyv from /dev/shm/vcmImage181tutorgaming" << std::endl;
    while(1){
        // Pass Byte Vector and Key To Access SHM
            openShmAndCopyToMat("yuyv", M.data);
        // Change Byte Combination [YUYVYUYV] to [ YUV + YUV ]
            ProcessColor(M,yuvImg);
            cv::cvtColor(yuvImg, rgbImg, CV_YUV2RGB);
        // OpenCV's Script
            // Create a window for display.
            namedWindow( "RGB", WINDOW_AUTOSIZE );
            // Show our image inside it.
            imshow( "RGB", rgbImg );
        waitKey(1);
    }
    return 0;
}
