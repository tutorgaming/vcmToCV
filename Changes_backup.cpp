#include <iostream>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace cv;
using namespace boost::interprocess;
typedef double value_t;
typedef unsigned int uint32;

//void readbyte(double double_in , std::vector<int> &vec ){
//    //CAST WHOLE DOUBLE TO CHAR = > ACCESS BYTE BY BYTE
//    unsigned char * double_byte = (unsigned char *) & double_in;
//     for (int i = 0; i < sizeof (double); i++) {
//         vec.push_back((int)double_byte[i]); //PUSH 8 BYTE TO SEPERATE BYTE VECTOR ( Vector of Byte )
//    }
//}

void ProcessColor(cv::Mat &source, cv::Mat &output)
	{
        output.create(source.rows, source.cols, CV_8UC3);
        int size = source.rows * source.cols;

        cv::Vec4b *ptr = (cv::Vec4b *) source.ptr(0);
        cv::Vec3b *t = (cv::Vec3b *)output.ptr(0);

        for(int i = 0; i < size; i+=2)
        {

            (*t)[0] = (*ptr)[0];
            (*t)[1] = (*ptr)[1];
            (*t)[2] = (*ptr)[3];
            t++;
            (*t)[0] = (*ptr)[2];
            (*t)[1] = (*ptr)[1];
            (*t)[2] = (*ptr)[3];

            ptr++;
            t++;
        }

	}


void openShmAndPushByte(const char *key , unsigned char *dst){
    //Create Shared Memory manager Segment (Files in /dev/shm)
    managed_shared_memory segment(open_only,"vcmImage181tutorgaming");

    //Create Data Type Pair<*double,site_t> for Receiving Pair From Segment
    std::pair<value_t*, std::size_t> ret;

    //Find The Pair that relavalant with the key
    ret = segment.find<value_t>(key);
    memcpy(dst, (unsigned char*)ret.first, ret.second*sizeof(value_t));
}



int main()
{

    int height = 480;
    int width = 640;
    cv::Mat M(height,width, CV_8UC2);
    cv::Mat yuvImg(height,width, CV_8UC3);
    cv::Mat rgbImg(height,width, CV_8UC3);
    cv::Vec4b vec;

    while(1){
//    std::vector<int> ret; //614400 BYTES "WILL BE Pass By Reference "
    // 4 bytes Qty = 153600 = 320 * 240;  NOW WE USE 4 BYTES AS 1 BIT ( this resolution )
    // 2 bytes Qty = 307200

        openShmAndPushByte("yuyv", M.data); // Pass Byte Vector and Key To Access SHM
//        continue;
//        cout << "ret = " << ret.size() << " " << width << " " << height << std::endl;
//
//    int k = 0;
//    //PUSH ALL OF THE 4BYTE TO 1 GROUP
//        for(int i =0; i< M.rows ; i++){
//            cv::Vec2b *ptr = (cv::Vec2b *) M.ptr(i); //ROW i's Pointer(i*j*4)+(j*4)+p
//            for(int j =0 ; j < M.cols ; j++ ){
//
//                (*ptr)[0] = dst[k++];
//                (*ptr)[1] = dst[k++];
////                    (*ptr)[0] = ret[(i*M.cols*4)+(j*4)] ;
////                    (*ptr)[1] = ret[(i*M.cols*4)+(j*4)+1];
////                    (*ptr)[2] = ret[(i*M.cols*4)+(j*4)+2];
////                    (*ptr)[3] = ret[(i*M.cols*4)+(j*4)+3];
//                 ptr++;
//            }
//        }

        ProcessColor(M,yuvImg);
        cv::cvtColor(yuvImg, rgbImg, CV_YUV2RGB);



    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", rgbImg );                   // Show our image inside it.
    waitKey(1);
    }
    return 0;
}


/// OLD 


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@2

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace cv;
using namespace boost::interprocess;
typedef double value_t;
typedef unsigned int uint32;

void readbyte(double double_in , std::vector<int> &vec ){
    //CAST WHOLE DOUBLE TO CHAR = > ACCESS BYTE BY BYTE
    unsigned char * double_byte = (unsigned char *) & double_in;
     for (int i = 0; i < sizeof (double); i++) {
         vec.push_back((int)double_byte[i]); //PUSH 8 BYTE TO SEPERATE BYTE VECTOR ( Vector of Byte )
    }
}

void ProcessColor(cv::Mat &source, cv::Mat &output)
    {
        output.create(source.rows, source.cols, CV_8UC3);
        int size = source.rows * source.cols;

         cv::Vec3b *p = (cv::Vec3b *) source.ptr(0);
        cv::Vec4b *t = (cv::Vec4b *) output.ptr(0);
        //uchar *p = source.ptr(0);
        //uchar *t = output.ptr(0);

        for(int i = 0; i < size; i+=6)
        {
            
            *(t+0) = *(p+0); //Y1
            *(t+1) = *(p+1); //U
            *(t+2) = *(p+3); //V
            *(t+3) = *(p+2); //Y2
            *(t+4) = *(p+1); //U
            *(t+5) = *(p+3); //V
            p+=4;
        }

    }


void openShmAndPushByte(const char *key , std::vector<int> &vec){

    //Create Shared Memory manager Segment (Files in /dev/shm)
        managed_shared_memory segment(open_only,"vcmImage181tutorgaming");

    //Create Data Type Pair<*double,site_t> for Receiving Pair From Segment
    std::pair<value_t*, std::size_t> ret;

    //Find The Pair that relavalant with the key
        ret = segment.find<value_t>(key);
        value_t *pr = ret.first;

    //Show For Each Member in the Vector (that's come from Segment)
        for(int i = 0 ; i < ret.second ; i++) {
            readbyte(pr[i],vec);
        }

}



int main()
{

    int height = 480/2;
    int width = 640/2;
    cv::Mat M(height,width, CV_8UC4);
    cv::Vec4b vec;
    cv::Mat out(height,width , CV_8UC3);


    while(1){
    std::vector<int> ret; //614400 BYTES "WILL BE Pass By Reference "
    // 4 bytes Qty = 153600 = 320 * 240;  NOW WE USE 4 BYTES AS 1 BIT ( this resolution )
    // 2 bytes Qty = 307200


        openShmAndPushByte("yuyv",ret); // Pass Byte Vector and Key To Access SHM
        //cout << "ret = " << ret.size() << std::endl;

    //PUSH ALL OF THE 4BYTE TO 1 GROUP
        for(int i =0; i< M.rows ; i++){
            cv::Vec4b *ptr = (cv::Vec4b *) M.ptr(i); //ROW i's Pointer(i*j*4)+(j*4)+p
            for(int j =0 ; j < M.cols ; j++ ){
                    (*ptr)[0] = ret[(i*M.cols*4)+(j*4)] ;
                    (*ptr)[1] = ret[(i*M.cols*4)+(j*4)+1];
                    (*ptr)[2] = ret[(i*M.cols*4)+(j*4)+2];
                    (*ptr)[3] = ret[(i*M.cols*4)+(j*4)+3];
                 ptr++;
            }
        }

        ProcessColor(M,out);



    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", out );                   // Show our image inside it.
    waitKey(1);
    }
    return 0;
}
