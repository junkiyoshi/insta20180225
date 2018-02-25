#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofBackground(0);
	ofSetWindowTitle("Insta");

	this->cap.open(1);
	this->cap_size = cv::Size(1280 * 0.5, 720 * 0.5);
	ofSetFrameRate(this->cap.get(CV_CAP_PROP_FPS));

	this->eye_cascade.load("opencv-3.3.1\\build\\install\\etc\\haarcascades\\haarcascade_eye.xml");

	this->image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
	this->frame = cv::Mat(this->image.getHeight(), this->image.getWidth(), CV_MAKETYPE(CV_8UC3, this->image.getPixels().getNumChannels()), this->image.getPixels().getData(), 0);

}

//--------------------------------------------------------------
void ofApp::update() {

	cv::Mat cap_frame, src, blend_frame;
	this->cap >> cap_frame;
	if (cap_frame.empty()) {

		cap.set(CV_CAP_PROP_POS_FRAMES, 1);
		return;
	}
	cv::resize(cap_frame, src, this->cap_size);
	cv::flip(src, src, 1);
	cv::cvtColor(src, src, CV_RGB2BGR);

	this->frame = this->frame.zeros(this->frame.size(), this->frame.type());

	vector<cv::Rect> eyes;
	this->eye_cascade.detectMultiScale(src, eyes);

	for (int x = 0; x < src.cols; x++) {

		for (int y = src.rows - 1; y > -1; y--) {

			ofVec2f point(x, y);

			for (cv::Rect eye : eyes) {
				
				ofVec2f eye_point = ofVec2f(eye.x + eye.size().width / 2, eye.y + eye.size().height / 2);
				if (eye_point.distance(point) < 30) {

					ofVec2f center = ofVec2f(src.cols / 2, src.rows / 2);
					point = point + ofVec2f(0, 100 * ofNoise((eye.x, ofGetFrameNum() * 0.05)));
				}

			}

			int tmp_x = (int)point.x;
			int tmp_y = (int)point.y;

			if (tmp_x >= 0 && tmp_x < src.cols && tmp_y >= 0 && tmp_y < src.rows) {

				this->frame.at<cv::Vec3b>(tmp_y, tmp_x) = src.at<cv::Vec3b>(y, x);
			}
		}
	}

	this->image.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->image.draw(0, 0);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(1280 * 0.5, 720 * 0.5, OF_WINDOW);
	ofRunApp(new ofApp());
}