#include<iostream> 
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;

void median(const cv::Mat& src, int window, cv::Mat& dst) {//1.Median Filter 함수
	std::vector<uchar> neighbors;

	for (auto y = 0; y < dst.rows; y++)for (auto x = 0; x < dst.cols; x++) {//dst의 모든 픽셀 순회
		neighbors.clear();
		uchar median = 0;
		for (auto s = -window / 2; s <= window / 2; s++)for (auto t = -window / 2; t <= window / 2; t++) {//window 기준으로 대상영역 지정
			neighbors.push_back(src.at<uchar>(min(dst.rows - 1, max(0, y + t)), min(dst.cols - 1, max(0, x + s))));//src대상의 픽셀값 추출 + replicate padding

			if (s == window / 2 || t == window / 2) {
				std::sort(neighbors.begin(), neighbors.end());//모은 값 정렬
				median = neighbors[neighbors.size() / 2];//중간값 찾기
			}
		}
		dst.at <uchar>(y, x) = median;//dst에 찾은 중간값 저장
	}
}

int main(int argc, const char* argv[]) {
	Mat f = imread("..//MedianFilterInput.png", 0);//grayscale로 이미지 읽어오기
	//f.convertTo(f, CV_32F, 1 / 255.f);
	Mat g(f.size(), CV_8UC1);
	Mat g_function(f.size(), CV_8UC1);

	imshow("image", f);

	//window 1일때
	median(f, 1, g);
	imshow("1 MyMedian", g);
	medianBlur(f, g_function, 1);
	imshow("1 FunctionMedian", g_function);

	//window 2일때 => 오류 why? window값이 짝수면 오류가 남 , 홀수여야 함
	//median(f, 2, g);
	//imshow("2 MyMedian", g);
	//medianBlur(f, g_function, 2);
	//imshow("2 FunctionMedian", g_function);

	//window 3일때
	median(f, 3, g);
	imshow("3 MyMedian", g);
	medianBlur(f, g_function, 3);
	imshow("3 FunctionMedian", g_function);

	//window 4일떄 => 오류
	// median(f, 4, g);
	//imshow("4 MyMedian", g);
	//medianBlur(f, g_function, 4);
	//imshow("4 FunctionMedian", g_function);

	//window 5일때 
	median(f, 5, g);
	imshow("5 MyMedian", g);

	medianBlur(f, g_function, 5);
	imshow("5 FunctionMedian", g_function);
	waitKey();

}