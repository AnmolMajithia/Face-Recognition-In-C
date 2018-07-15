#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include "SerialPort.h"
#include<conio.h>
#include<direct.h>
#include "windows.h"
#include "opencv2/face.hpp"
#include <stdlib.h>

using namespace std;
using namespace cv;
using namespace cv::face;

Ptr<LBPHFaceRecognizer> model1 = createLBPHFaceRecognizer(1, 8,  8,  8,  DBL_MAX);
//Ptr<FaceRecognizer> model2 = createEigenFaceRecognizer();
//Ptr<BasicFaceRecognizer> model3 = createFisherFaceRecognizer();

// Function Headers
Mat detectAndDisplay(Mat, vector<string>&);
Mat detectAndsave(Mat, string);
bool dirExists(string);
void display();
void display2();
void display3();
void display4();
static void csv(const string& , vector<Mat>& , vector<int>&);

// global variables
const string csvFile = "./csv_databaseFile.csv";
String face_cascade_name = "./haarcascades/haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;
String window_name = "Capture - Face detection";
char *port_name = "\\\\.\\COM3";
char incomingData[MAX_DATA_LENGTH];
SerialPort arduino(port_name);
String wn = "camera window";

// @function main 
int main()
{
	system("color F0");
	int lll=0;
	vector<string> namearray2;
	std::ifstream file1("images.txt", ifstream::in);
	string line1;
	while (getline(file1, line1))
	{
		namearray2.push_back(line1);
	}
	for (int i = 0; i < namearray2.size()-3; i++)
	{
		cout <<"| "<< namearray2[i] <<" |"<< endl;
		Sleep(200);
	}
	cout << "\n      ---------------------------------------------------------------------------------" << endl;
	cout << "     |  press any key to continue ...                                                  |" << endl;
	cout << "      ---------------------------------------------------------------------------------" << endl;
	getch();
	string imagedirectory = "./images";
	dirExists(imagedirectory);
	int n = 1;
	char choice[100];
	string whitespace="" ;
	// loading previsouly saved models ,if present
	cout << "loading models" << endl;
	model1->load("LBPHFaceRecognizer.yml");
	cout << "lbph loaded" << endl;
	//model2->load("EigenFaceRecognizer.yml");
	//cout << "eigen loaded" << endl;
	//model3->load("FisherFaceRecgnizer.yml");
	//cout << "fisher loaded" << endl;

	while (n != 0)
	{
		//menu
		system("CLS");
		system("python create_csv.py f:/ema/att > csv_databaseFile.csv");
	    vector<string> namearray1;
		std::ifstream file("NameList.txt", ifstream::in);
		string line;
		while(getline(file, line))
		{
			namearray1.push_back(line);
		}
		cout << "\n\n\nPRESS -" << endl;
		cout << " ---------------------------------------------------------------------------------\n" << endl;
		cout << "|           1  .  FACE DETECTION/RECOGNITION                                      |\n" << endl;
		cout << "|           2  .  ENTER FACE DATA                                                 |\n" << endl;
		cout << "|           3  .  TRAIN RECOGIZER                                                 |\n" << endl;
		cout << "|           4  .  ABOUT?                                                          |\n" << endl;
		cout << "|           5  .  HOW TO..?                                                       |\n" << endl;
		cout << "|           6  .  EXIT                                                            |\n" << endl;
		cout << " ---------------------------------------------------------------------------------\n" << endl;
		cout << "\n\nENTER CHOICE ::  ";
		scanf("%s", choice);

		//call detectAndDisplay
		if (strcmp(choice, "1") == 0)
		{
			system("CLS");
			
			VideoCapture capture;
			Mat frame;
			//-- 1. Load the cascades
			if (!face_cascade.load(face_cascade_name))
			{
				printf("--(!)Error loading face cascade\n");
				return -1;
			}
			//-- 2. Read the video stream
			capture.open(0);
			if (!capture.isOpened())
			{
				printf("--(!)Error opening video capture\n"); return -1;
			}
			while (capture.read(frame))
			{
				if (frame.empty())
				{
					printf(" --(!) No captured frame -- Break!");
					break;
				}
				//-- 3. Apply the classifier to the frame
				detectAndDisplay(frame,namearray1);
				int c = waitKey(10);
				if (c == 27)
				{
					destroyWindow(wn);
					break;
				}
			}
			n = 1;
		}

		//call detectAndsave
		if (strcmp(choice, "2") == 0)
		{	
			system("CLS");
			int sampleNumber = 0;
			string username;
			string persondirectory;
			bool k =false,temp;			
			while (k == false)
			{   
				redirect :
				system("CLS");
				cout << " ---------------------------------------------------------------------------------" << endl;
				cout << "| to go back to the main menu ENTER b/B                                           |" << endl;
				cout << " ---------------------------------------------------------------------------------" << endl;
				cout << "\n\n\n\nenter username    ::  ";
				cin >> username;
				if (username.compare("b") == 0 || username.compare("B") == 0)
				{
					k = true;
					break;
				}
				for (int i = 0; i < username.length(); i++)
				{
					if (username[i] == 32 || (username[i] <= 90 && username[i] >= 65) || (username[i] <= 122 && username[i] >= 97) || (username[i] <= 57 && username[i] >= 48))
					{
						continue;
					}
					else
					{ 
						display4();
						goto redirect;
					}
				}
				persondirectory = "./images/" + username;
				k = dirExists(persondirectory);
				if (k == false)
				{
					display3();
				}
				else
				{ 
					k = true;
				}
			}	
			VideoCapture capture;
			Mat frame;
			//-- 1. Load the cascades
			if (username.compare("b") == 0 || username.compare("B") == 0)
			{
				continue;
			}
			else
			{
				if (!face_cascade.load(face_cascade_name))
				{
					printf("--(!)Error loading face cascade\n");
					return -1;
				}
				//-- 2. Read the video stream
				capture.open(0);
				if (!capture.isOpened())
				{
					printf("--(!)Error opening video capture\n"); return -1;
				}

				while (capture.read(frame))
				{
					if (frame.empty())
					{
						printf(" --(!) No captured frame -- Break!");
						break;
					}
					//-- 3. Apply the classifier to the frame
					Mat temp = detectAndsave(frame, username);
					int c = waitKey(1);
					if ((char)c == ' ')
					{
						if (!temp.empty())
						{	
							sampleNumber++;
							imwrite(persondirectory + "/" + std::to_string(sampleNumber) + ".png", temp);
						}
					}
					if (c == 27)
					{
						if (sampleNumber == 0)
						{
							_rmdir(persondirectory.c_str());
						}
						destroyWindow(wn);
						break;
					}// escape
				}
			}
			n = 1;
		}

		//train recognizer
		if (strcmp(choice, "3") == 0)
		{
			system("CLS");
			cout << "\n\n ---------------------------------------------------------------------------------" << endl;
			cout << "|  training.....................                                                  |" << endl;
			cout << " ---------------------------------------------------------------------------------" << endl;
			vector<Mat> Images;
			vector<int> labels;
			csv(csvFile, Images, labels);
			Images.pop_back();
			labels.pop_back();
			model1->train(Images, labels);
			//model2->train(Images, labels);
			//model3->train(Images, labels);
			cout << "      ---------------------------------------------------------------------------------" << endl;
			cout << "     |  training complete                                                              |" << endl;
			cout << "      ---------------------------------------------------------------------------------" << endl;
			model1->save("LBPHFaceRecognizer.yml");
			//model2->save("EigenFaceRecognizer.yml");
			//model3->save("FisherFaceRecgnizer.yml");
			cout << "           ---------------------------------------------------------------------------------" << endl;
			cout << "          |  press any key to continue ...                                                  |" << endl;
			cout << "           ---------------------------------------------------------------------------------" << endl;
			getch();
		}

		//call about
		if (strcmp(choice, "4") == 0)
		{
			system("CLS");
			display();
			getch();
			n = 1;
		}

		//howto
		if (strcmp(choice, "5") == 0)
		{
			system("CLS");
			display2();
			getch();
			n = 1;
		}

		//  exit
		if (strcmp(choice, "6") == 0)
		{
			system("CLS");
			cout << "\n\n\n\n\n\n\nGOODBYE....................." << endl;
			break;
			destroyAllWindows();
		}
	}
	return 0;
}

//Function to read csv file
static void csv(const string& filename, vector<Mat>& images, vector<int>& labels)
{
	vector<string> namearray;
	ofstream myfile("NameList.txt");
	int index = 0;
	char separator = ';';
	std::ifstream file(filename.c_str(), ifstream::in);
	if (!file)
	{
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(Error::StsBadArg, error_message);
	}
	string line, path, classlabel;
	while (getline(file, line))
	{
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel);
		if (!path.empty() && !classlabel.empty())
		{
			images.push_back(imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
		}
		char x = '/';
		for (int i = 0; i < line.length(); i++) 
		{
			if (line[i] == x)
			{
				index = i;
			}
		}
		string labelName = line.substr(9, index - 9);
		namearray.push_back(labelName);
	}
	for (int i = 0; i < namearray.size()-1 ; i++)
	{
		if (namearray[i] != namearray[i + 1])
		{
			myfile << namearray[i] << endl;
		}
	}
	myfile << namearray[namearray.size() - 1] << endl;
	myfile.close();	
}

//function detectAndDisplay 
Mat detectAndDisplay(Mat frame, vector<string>& namearray1)
{
	int sampleNumber = 1;
	char *k = "1 ";
	std::vector<Rect> faces;
	Mat frame_gray, frame_gray1;
	//cvtColor( frame, frame_rgb, COLOR_BGR2RGB );
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	resize(frame_gray, frame_gray, Size(), 1, 1, INTER_LINEAR);
	equalizeHist(frame_gray, frame_gray1);
	//-- Detect faces
	face_cascade.detectMultiScale(frame_gray1, faces, 1.7, 2.8, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
	if (!faces.empty())
	{
		k = "1 ";
		arduino.writeSerialPort(k, MAX_DATA_LENGTH);
		//cout << "1 " << endl;
	}
	else if (faces.empty())
	{
		k = "0 ";
		arduino.writeSerialPort(k, MAX_DATA_LENGTH);
		//cout << "0 " << endl;
	}
	for (size_t i = 0; i < faces.size(); i++)
	{
		int startX = faces[i].x, startY = faces[i].y, width = faces[i].width, height = faces[i].height;
		Mat ROI(frame_gray1, Rect(startX, startY, width, height));
		Mat croppedImage;
		// Copy the data into new matrix
		ROI.copyTo(croppedImage);
		resize(croppedImage, croppedImage, Size(100,100), 0, 0, INTER_LINEAR);

		int predictedLabel1 = -1;
		double confidence1 = 0.0;
		model1->predict(croppedImage, predictedLabel1, confidence1);
		
		/*
		int predictedLabel2 = -1;
		double confidence2 = 0.0;
		model2->predict(croppedImage, predictedLabel2, confidence2);
		*/
		/*int predictedLabel3 = -1;
		double confidence3 = 0.0;
		model3->predict(croppedImage, predictedLabel3, confidence3);
		*/
		//terminal display
		cout << predictedLabel1 <<"    "<< confidence1 <<"   "<< namearray1[predictedLabel1] <<endl;
		rectangle(frame, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar(0, 255, 0), 2, 4, 0);
		//camera window display
		if (predictedLabel1>=0 && confidence1<=90)
		{
			putText(frame, namearray1[predictedLabel1], Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), FONT_HERSHEY_PLAIN, 1, Scalar(255, 0, 0), 2, 5, false);
		}
		else
		{
			putText(frame, "", Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), FONT_HERSHEY_PLAIN, 1, Scalar(255, 0, 0), 2, 5, false);
		}
	}
	//-- Display the coloured image with the detect box and text
	imshow(wn, frame);
	return frame;
}

//About
void display()
{
	system("CLS");
	cout << "\n\n ---------------------------------------------------------------------------------" << endl;
	cout << "|  ABOUT  ::                                                                      |" << endl;
	cout << " ---------------------------------------------------------------------------------\n" << endl;
	cout << "      -----------------------------------------------------------------------------------" << endl;
	cout << "     |  OPENCV  :: OpenCV (Open Source Computer Vision Library) is released under a BSD  |" << endl;
	cout << "     |             license and hence it is free for both academic and commercial use.    |" << endl;
	cout << "     |             It has C++, Python and Java interfaces and supports Windows, Linux,   |" << endl;
	cout << "     |             Mac OS, iOS and Android. OpenCV was designed for computational        |" << endl;
	cout << "     |             efficiency and with a strong focus on real-time applications. Written |" << endl;
	cout << "     |             in optimized C/C++, the library can take advantage of multi-core      |" << endl;
	cout << "     |             processing. Enabled with OpenCL, it can take advantage of the hardware|" << endl;
	cout << "     |             acceleration of the underlying heterogeneous computer platforms.      |" << endl;
	cout << "     |             Adopted all around the world, OpenCV has more than 47 thousand people |" << endl;
	cout << "     |             of user community and estimated number of downloads exceeding 14      |" << endl;
	cout << "     |             million. Usage ranges from interactive art, to mines inspection,      |" << endl;
	cout << "     |             stitching maps on the web or through advanced robotics.               |" << endl;
	cout << "      -----------------------------------------------------------------------------------\n" << endl;
	cout << "           -------------------------------------------------------------------------------------" << endl;
	cout << "          |  FACE DETECTION  :: Face detection is a computer technology being used in a variety |" << endl;
	cout << "          |                     of applications that identifies human faces in  digital images. |" << endl;
	cout << "          |                     Face detection also refers to the psychological process by which|" << endl;
	cout << "          |                     humans locate and attend to  faces in a visual scene.           |" << endl;
	cout << "           -------------------------------------------------------------------------------------\n" << endl;
	cout << "                ------------------------------------------------------------------------------------" << endl;
	cout << "               |  FACE RECOGNITION  :: Facial recognition system is a technology capable of         |" << endl;
	cout << "               |                       identifying a face from a digital image or a video frame or  |" << endl;
	cout << "               |                       from a video source. There are multiples methods in which    |" << endl;
	cout << "               |                       facial recognition systems work, but in general, they work by|" << endl;
	cout << "               |                       comparing selected facial features from given image with     |" << endl;
	cout << "               |                       faces within a database.                                     |" << endl;
	cout << "                ------------------------------------------------------------------------------------\n" << endl;
	cout << "                    -----------------------------------------------------------------------------------" << endl;
	cout << "                    |  press any key to continue ...                                                    |" << endl;
	cout << "                     -----------------------------------------------------------------------------------\n" << endl;
}
 
//foldername error 
void display3()
{
	cout << "\n\n -----------------------------------------------------------------------------------" << endl;
	cout << "|  ERROR  :: Face with a similar name exists in the database.                       |" << endl;
	cout << "|            Please try again with different name, or try adding characters/surname.|" << endl;
	cout << " -----------------------------------------------------------------------------------" << endl;
	cout << "      ---------------------------------------------------------------------------------" << endl;
	cout << "     |  press any key to continue ...                                                  |" << endl;
	cout << "      ---------------------------------------------------------------------------------" << endl;
	getch();
}

//special character error
void display4()
{	
	cout << "\n\n ----------------------------------------------------------------------------------------------------" << endl;
	cout << "|  ERROR  :: Username must not contain special characters other than whitespace ' '. Please try again.|" << endl;
	cout << " -----------------------------------------------------------------------------------------------------" << endl;
	cout << "      ---------------------------------------------------------------------------------" << endl;
	cout << "     |  press any key to continue ...                                                  |" << endl;
	cout << "      ---------------------------------------------------------------------------------" << endl;
	getch();
}

//HOW TO
void display2()
{
	system("CLS");
	cout << "\n\n ---------------------------------------------------------------------------------" << endl;
	cout << "|  HOW TO  ::                                                                     |" << endl;
	cout << " ---------------------------------------------------------------------------------\n" << endl;
	cout << "      ----------------------------------------------------------------------------------" << endl;
	cout << "     |  STEP 1  :: First time users press 2 to save facial data for recognition         |" << endl;
	cout << "     |             ->Press the 'spacebar' to grab images at least 10 times or for 1 sec |" << endl;
	cout << "     |             ->Press 'escape' to exit camera window                               |" << endl;
	cout << "      ----------------------------------------------------------------------------------\n" << endl;
	cout << "           ----------------------------------------------------------------------------------" << endl;
	cout << "          |  STEP 2  :: Press 3 to train the recognizer model                                |" << endl;
	cout << "          |  (If you have aready stored and trained the recognizer skip ,then step 1 and 2)  |" << endl;
	cout << "           ----------------------------------------------------------------------------------\n" << endl;
	cout << "                ----------------------------------------------------------------------------------" << endl;
	cout << "               |  STEP 3  :: Press 1 to detect and recognize faces.                               |" << endl;
	cout << "                ----------------------------------------------------------------------------------\n" << endl;
	cout << "                     ----------------------------------------------------------------------------------" << endl;
	cout << "                    | press any key to continue ...                                                    |" << endl;
	cout << "                     ----------------------------------------------------------------------------------\n" << endl;
}

//function detectAndsave  
Mat detectAndsave(Mat frame, string username)
{
	char *k = "1 ";
	std::vector<Rect> faces;
	Mat frame_gray, frame_gray1;
	//cvtColor( frame, frame_rgb, COLOR_BGR2RGB );
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	resize(frame_gray, frame_gray, Size(), 1, 1, INTER_LINEAR);
	equalizeHist(frame_gray, frame_gray);
	Mat croppedImage;
	//-- Detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.7, 2.8, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
	if (!faces.empty())
	{
		k = "1 ";
		arduino.writeSerialPort(k, MAX_DATA_LENGTH);
		cout << "1 " << endl;
	}
	else if (faces.empty())
	{
		k = "0 ";
		arduino.writeSerialPort(k, MAX_DATA_LENGTH);
		cout << "0 " << endl;
	}
	//-- Display the coloured image with the detect box and text
	for (size_t i = 0; i < faces.size(); i++)
	{
		int startX = faces[i].x, startY = faces[i].y, width = faces[i].width, height = faces[i].height;
		Mat ROI(frame_gray, Rect(startX, startY, width, height));
		// Copy the data into new matrix
		ROI.copyTo(croppedImage);
		resize(croppedImage, croppedImage, Size(100, 100), 0, 0, INTER_LINEAR);
		rectangle(frame, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar(0, 255, 0), 2, 4, 0);
		putText(frame, "DETECTED || hit spacebar to save", Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), FONT_HERSHEY_PLAIN, 1, Scalar(255, 0, 0), 2, 5, false);
	}
	imshow(wn, frame);
	return croppedImage;
}

//function checks existance of the image directory and the person directories
bool dirExists(string dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	//creating dataset folder if it already doesn't exists
	if (ftyp == INVALID_FILE_ATTRIBUTES)
	{
		_mkdir(dirName_in.c_str());
		return true;
	}
	else if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
	{
		return false;
	}
}

