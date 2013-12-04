#include "rendering/include.h"
#include "rendering/cloud.h"
#include "rendering/camera.h"
using namespace std;
using namespace glm;
#include "xyz_io/xyz_io.hpp"
#include "surface.h"
//#include <omp.h>
Cloud point_cloud;
Shader cloud_shader;
Camera camera;

mat4 bbox_rot;
vec3 bbox_scale;
vec3 bbox_translate;
class Window {
public:
	Window() {
		this->interval = 1000 / 60;
		this->window_handle = -1;
	}
	int window_handle;
	int interval;
	ivec2 size;
	float window_aspect;
	vector<string> instructions;
} window;

void CloseFunc() {
	window.window_handle = -1;

}

void ReshapeFunc(int w, int h) {
	// Question for reader: Why is this 'if' statement here?
	if (h > 0) {
		window.size = ivec2(w, h);
		window.window_aspect = float(w) / float(h);
	}
	camera.SetViewport(0, 0, window.size.x, window.size.y);
}

void KeyboardFunc(unsigned char c, int x, int y) {
	switch (c) {
	case 'w':
		camera.Move(FORWARD);
		break;
	case 'a':
		camera.Move(LEFT);
		;
		break;
	case 's':
		camera.Move(BACK);
		break;
	case 'd':
		camera.Move(RIGHT);
		break;
	case 'q':
		camera.Move(DOWN);
		break;
	case 'e':
		camera.Move(UP);
		break;
	case 'x':
	case 27:
		exit(0);
		return;
	default:
		break;
	}
}

void SpecialFunc(int c, int x, int y) {
}
void CallBackMouseFunc(int button, int state, int x, int y) {
	camera.SetPos(button, state, x, y);
}

void CallBackMotionFunc(int x, int y) {
	camera.Move2D(x, y);
}
void CallBackPassiveFunc(int x, int y) {
}
void DisplayFunc() {
	glEnable(GL_CULL_FACE);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window.size.x, window.size.y);

	glm::mat4 model, view, projection;
	camera.Update();
	camera.GetMatricies(projection, view, model);

	glm::mat4 modelview = view * model;

	point_cloud.Draw(projection, modelview);
	mat4 rot = mat4(1);
	rot = glm::scale(rot, bbox_scale);
	glMultMatrixf(glm::value_ptr(rot));

	glutWireCube(1);

	//DisplayInstructions();
	glutSwapBuffers();
}

void TimerFunc(int value) {
	// Question for reader: Why is this 'if' statement here?
	if (window.window_handle != -1) {
		glutTimerFunc(window.interval, TimerFunc, value);
		glutPostRedisplay();
	}
}

void Histogram_(vector<vec3> &data, vector<int> &hist, vec3 min_p, vec3 max_p) {
	int BIN_COUNT = 256;
	hist.clear();
	hist.resize(BIN_COUNT, 0);
	float h = (max_p.y - min_p.y) / float(BIN_COUNT);
	//float h = (0.230561f + 0.230561f) / float(BIN_COUNT);

	unsigned int bin;
	for (int i = 0; i < data.size(); i++) {
		float sample = data[i].y;
		bin = BIN_COUNT;
		for (unsigned int binNum = 0; binNum < BIN_COUNT; ++binNum) {
			float rightEdge = min_p.y + binNum * h;
			if (sample <= rightEdge) {
				bin = binNum;
				break;
			}
		}
		hist[bin]++;
	}
}
void DumpHist(vector<vector<vec3> > all_data, string fname, vec3 min_p, vec3 max_p) {
	vector<vector<int> > histograms(all_data.size());

	for (int i = 0; i < all_data.size(); i++) {
		Histogram_(all_data[i], histograms[i], min_p, max_p);
	}

	ofstream ofile(fname);
	for (int j = 0; j < histograms[0].size(); j++) {
		for (int i = 0; i < histograms.size(); i++) {
			ofile << histograms[i][j] << " ";
		}
		ofile << endl;
	}
}

int main(int argc, char * argv[]) {
	ifstream ifile(argv[1]);
	string text;
	//omp_set_num_threads(4);
	vec3 min_p(FLT_MAX), max_p(-FLT_MAX);
	Surface cloud;

	vector<string> filenames;
	vector<float> rms, skew, kurtosis, mean;
	vector<vector<vec3> > all_data;

	while (ifile.fail() == false) {
		getline(ifile, text);
		if (ifile.fail() == true) {
			break;
		}
		string filename = text;
		cout << filename << endl;

		cloud.Process(filename);

		vec3 min_t(FLT_MAX), max_t(-FLT_MAX);
		cloud.Bounds(min_t, max_t);

		filenames.push_back(filename);
		vec3 centroid = cloud.Centroid();
		rms.push_back(cloud.RMS());
		skew.push_back(cloud.Skewness(centroid));
		kurtosis.push_back(cloud.Kurtosis(centroid));
		mean.push_back(cloud.ArithmaticMean());

		min_p.x = fmin(min_p.x, min_t.x);
		min_p.y = fmin(min_p.y, min_t.y);
		min_p.z = fmin(min_p.z, min_t.z);

		max_p.x = fmax(max_p.x, max_t.x);
		max_p.y = fmax(max_p.y, max_t.y);
		max_p.z = fmax(max_p.z, max_t.z);

		//cout << min_t.x << " " << min_t.y << " " << min_t.z << endl;
		//cout << max_t.x << " " << max_t.y << " " << max_t.z << endl;

		cout << min_p.x << " " << min_p.y << " " << min_p.z << endl;
		cout << max_p.x << " " << max_p.y << " " << max_p.z << endl;

		cout << rms[rms.size() - 1] << endl;
		cout << skew[rms.size() - 1] << endl;
		cout << kurtosis[rms.size() - 1] << endl;
		cout << mean[rms.size() - 1] << endl;

		all_data.push_back(cloud.data);

//		bbox_scale = (max_t-min_t);
//
//
//		glutInit(&argc, argv);
//		glutInitWindowSize(1024, 512);
//		glutInitWindowPosition(0, 0);
//		glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
//
//		window.window_handle = glutCreateWindow("XYZ_Visualizer");
//		glutReshapeFunc(ReshapeFunc);
//		glutDisplayFunc(DisplayFunc);
//		glutKeyboardFunc(KeyboardFunc);
//		glutSpecialFunc(SpecialFunc);
//		glutMouseFunc(CallBackMouseFunc);
//		glutMotionFunc(CallBackMotionFunc);
//		glutPassiveMotionFunc(CallBackPassiveFunc);
//		glutTimerFunc(window.interval, TimerFunc, 0);
//
//		glEnable(GL_POINT_SMOOTH);
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//		glewExperimental = GL_TRUE;
//
//		if (glewInit() != GLEW_OK) {
//			cerr << "GLEW failed to initialize." << endl;
//			return 0;
//		}
//
//		camera.SetMode(FREE);
//		camera.SetPosition(glm::vec3(0, 0, 0));
//		camera.SetLookAt(cloud.data[0]);
//		camera.SetClipping(.1, 1000);
//		camera.SetFOV(45);
//
//		cloud_shader.Initialize("phong.vert", "phong.frag");
//		cloud_shader.SetTime(0);
//		point_cloud.Initialize(cloud.data);
//		//
//		point_cloud.AttachShader(&cloud_shader);
//		glutMainLoop();

	}

	ofstream ofile("full_out.txt");
	for (int i = 0; i < filenames.size(); i++) {
		ofile << filenames[i] << " " << rms[i] << " " << skew[i] << " " << kurtosis[i] << " " << mean[i] << endl;
	}
	ofile.close();

	DumpHist(all_data,"hists.txt", min_p, max_p);


	return 0;
}
