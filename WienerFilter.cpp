#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
float k = 0.001;



void WienerFilter( InputArray src, InputArray krn, OutputArray dst, float k ) {
	cv::Mat g = src.getMat().clone(), h = krn.getMat();
	if( g.depth()!=CV_32F )
		g.convertTo( g, CV_32F, 1/255.f );

	cv::Mat G, H, mag;
	cv::dft(g,G,DFT_COMPLEX_OUTPUT);
	cv::dft(h,H,DFT_COMPLEX_OUTPUT);


	std::vector<cv::Mat> chs;
	cv::split(H,chs);
	cv::magnitude(chs[0], chs[1], mag);
	cv::multiply(mag,mag,mag);
	mag+=k;
	chs = {mag,mag};
	merge(chs,mag);

	cv::mulSpectrums(G, H, G, 0, true);
	cv::divide(G,mag,G);

	cv::Mat& out = dst.getMatRef();
	idft(G,out,DFT_SCALE|DFT_REAL_OUTPUT);
}


int main()
{
	
	
	
	Mat g = imread("D:/Mygit_SourceTree/Image-Processing/Wiener_Input2.png", 0);
	Mat h = imread("D:/Mygit_SourceTree/Image-Processing//Wiener_Kernel.png", 0);
	Mat f;
	g.convertTo( g, CV_32F, 1/255.f );
	h.convertTo( h, CV_32F, 1/255.f );
	h /= sum(h);
	
	WienerFilter( g, h, f, 0.001 );
	imshow("res", f);
	waitKey();
	return 0;
}
