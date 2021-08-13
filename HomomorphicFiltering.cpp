#include<iostream> 
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;

int main()
{
	float gammaL = 0.4, gammaH = 1.0;
	Mat Origin_img = imread("D://Workspace_School//HomomorphicFiltering//homo1.jpg", 0);//원본
	Mat f = imread("D://Workspace_School//HomomorphicFiltering//homo1.jpg", 0);//1. graysclae
	f.convertTo(f, CV_32F, 1 / 255.f);//2. 원본 영상을 float (0~1범위)로 convert 합니다 (원래는 0~255)
	//image += 0.1;//밝기 조절 +0.1
	f += 0.1;
	log(f, f);//3. convert 된 영상에 0.1을 더한 후 log를 취합니다. 
	
	Mat F;
	dft(f, F, DFT_COMPLEX_OUTPUT);

	//4.가우시안의 highpass filter
	Mat filter = Mat::zeros(F.size(), CV_32FC2);
	for (int y = 0; y < filter.rows; y++)for (int x = 0; x < filter.cols; x++) {
		int xx = x > filter.cols / 2 ? x - filter.cols : x;
		int yy = y > filter.rows / 2 ? y - filter.rows : y;

		float Duv = sqrtf(xx * xx + yy * yy);
		float D0 = 3;
		int C = 1;

		float Huv = (gammaH - gammaL) * (1 - exp(-C * (Duv * Duv / (D0 + D0)))) + gammaL;// gaussian을 이용한 highpass filter 생성

		filter.at<Vec2f>(y, x)[0] = Huv;
		filter.at<Vec2f>(y, x)[1] = Huv;
	}
	multiply(F, filter, F);//5.log를 취한 입력 영상과 filter를 곱(multiply)합니다.

	Mat g;
	idft(F, g, DFT_SCALE | DFT_REAL_OUTPUT);
	exp(g,g);
	g -= 0.1;

	std::vector<Mat> channels;
	split(filter, channels);
	imshow("Filter", channels[0]);//커널


	std::vector<Mat>channels1;
	split(g, channels1);
	imshow("Res",channels1[0]);//결과
	imshow("Before", Origin_img);//원본
	waitKey();
	return 0;
}


