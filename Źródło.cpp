#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<conio.h>          

using namespace std;
using namespace cv;

const Scalar blackColour = Scalar(0.0, 0.0, 0.0);
const Scalar whiteColour = Scalar(255.0, 255.0, 255.0);

class defineObject {

public:
	vector<cv::Point> contour;

	defineObject::defineObject(vector<cv::Point> objContour) {
		contour = objContour;
		}
};

int main(void) {

	char escKey = 0;
	VideoCapture VideoIn;
	Mat frame1;
	Mat frame2;

	VideoIn.open("cars.avi");

	VideoIn.read(frame1);
	VideoIn.read(frame2);


	while (VideoIn.isOpened() && escKey != 27) {



	vector<defineObject> items;

		Mat frame1Cpy = frame1.clone();
		Mat frame2Cpy = frame2.clone();

		imshow("original", frame1);

		Mat framesDiff;
		Mat frameThresh;

		cvtColor(frame1Cpy, frame1Cpy, CV_BGR2GRAY);
		cvtColor(frame2Cpy, frame2Cpy, CV_BGR2GRAY);

		GaussianBlur(frame1Cpy, frame1Cpy, cv::Size(5, 5), 0);
		GaussianBlur(frame2Cpy, frame2Cpy, cv::Size(5, 5), 0);

		absdiff(frame1Cpy, frame2Cpy, framesDiff);

		threshold(framesDiff, frameThresh, 45, 255.0, CV_THRESH_BINARY);

		//imshow("imgThresh", frameThresh);

		dilate(frameThresh, frameThresh, getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
		dilate(frameThresh, frameThresh, getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
		erode(frameThresh, frameThresh, getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));

		Mat frameThreshCpy = frameThresh.clone();

		vector<vector<cv::Point> > contours;

		findContours(frameThreshCpy, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		Mat carContours(frameThresh.size(), CV_8UC3, blackColour);

		drawContours(carContours, contours, -1, whiteColour, -1);
		dilate(carContours, carContours, getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));

		imshow("imgContours", carContours);

		vector<vector<cv::Point> > convexHulls(contours.size());

		for (unsigned int i = 0; i < contours.size(); i++) {
			convexHull(contours[i], convexHulls[i]);
		}

		Mat frameConvexHull(frameThresh.size(), CV_8UC3, blackColour);

		convexHulls.clear();

		for (auto &newItem : items) {
			convexHulls.push_back(newItem.contour);
		}

		drawContours(frameConvexHull, convexHulls, -1, whiteColour, -1);
		//imshow("imgConvexHulls", imgConvexHulls);

		frame1 = frame2.clone();           

		if ((VideoIn.get(CV_CAP_PROP_POS_FRAMES) + 1) < VideoIn.get(CV_CAP_PROP_FRAME_COUNT)) {      
			VideoIn.read(frame2);                            
		}
		else {                                                 
			cout << "end\n";                     
			break;                                              
		}
		escKey = waitKey(2);      
	}
	if (escKey != 27) {              
		waitKey(0);                       
	}
	return(0);
}






















