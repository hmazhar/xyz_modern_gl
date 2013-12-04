#include "rendering/include.h"
#include <armadillo>
//#include <omp.h>
#include <pcl/pcl_base.h>
#include <pcl/common/common_headers.h>
#include <pcl/common/centroid.h>
#include <pcl/common/pca.h>
#include <pcl/common/impl/pca.hpp>
#include <pcl/point_types.h>
#include <pcl/filters/crop_box.h>

using namespace glm;
using namespace std;
using namespace pcl;
class Surface {

public:
	Surface() {
	}
	void Load(string filename) {
		int point_num;

		xyz_header_read(filename, &point_num);
		cloud.resize(point_num);
		data.resize(point_num);

		std::ifstream t(filename.c_str());
		std::string str;

		t.seekg(0, std::ios::end);
		str.reserve(uint(t.tellg()));
		t.seekg(0, std::ios::beg);
		//assign the data to a string
		str.assign((std::istreambuf_iterator<char>(t)),
				std::istreambuf_iterator<char>());
		//create a string stream and push all data
		stringstream ss;
		ss << str;
		string text;
		int i = 0;
		while (1) {
			getline(ss, text);
			if (ss.eof()) {
				break;
			}

			if (text[0] == '#') {
				continue;
			}
			stringstream temp(text);
			PointXYZ value;
			temp >> value.x >> value.z >> value.y;
			cloud[i] = value;
			data[i] = vec3(value.x, value.z, value.y);
			i++;
		}

		cloud_ptr.reset(new pcl::PointCloud<pcl::PointXYZ>(cloud));

	}
	void Flatten() {
		vec3 centroid = Centroid();
		mat3 covariance = Covariance(centroid);
		mat4 rotation = Eigen(covariance);
		RotateData(rotation);
		vec3 min_p(0), max_p(0);
		Bounds(min_p, max_p);
		centroid = Centroid();
		//cout<<centroid.x<<" "<<centroid.y<<" "<<centroid.z<<endl;
		ToOrigin((min_p + max_p) * .5f);
	}

	void Cubeify() {
		vec3 min_p(0), max_p(0);
		Bounds(min_p, max_p);

		vec3 v1 = vec3(min_p.x, min_p.y - 10, min_p.z);
		vec3 v2 = vec3(min_p.x, min_p.y - 10, max_p.z);
		vec3 v3 = vec3(max_p.x, min_p.y - 10, min_p.z);

		vec3 ab = v2 - v1;
		vec3 ac = v3 - v1;
		vec3 norm = normalize(cross(ab, ac));

		cout << norm.x << " " << norm.y << " " << norm.z << endl;
	}

	void Center() {
		vec3 min_p(0), max_p(0);
		Bounds(min_p, max_p);
		ToOrigin((min_p + max_p) * .5f);
	}
	vec3 Centroid() {
		vec3 centroid = vec3(0);
		for (int i = 0; i < data.size(); i++) {
			centroid = centroid + data[i];
		}
		centroid = centroid / float(data.size());
		return centroid;
	}

	mat3 Covariance(vec3 mu) {
		mat3 covariance = mat3(0);
		for (int i = 0; i < data.size(); i++) {
			covariance[0].x += (mu.x - data[i].x) * (mu.x - data[i].x);
			covariance[1].x += (mu.y - data[i].y) * (mu.x - data[i].x);
			covariance[2].x += (mu.z - data[i].z) * (mu.x - data[i].x);

			covariance[0].y += (mu.x - data[i].x) * (mu.y - data[i].y);
			covariance[1].y += (mu.y - data[i].y) * (mu.y - data[i].y);
			covariance[2].y += (mu.z - data[i].z) * (mu.y - data[i].y);

			covariance[0].z += (mu.x - data[i].x) * (mu.z - data[i].z);
			covariance[1].z += (mu.y - data[i].y) * (mu.z - data[i].z);
			covariance[2].z += (mu.z - data[i].z) * (mu.z - data[i].z);
		}
		return covariance / (float(data.size() - 1.0f));
	}

	mat4 Eigen(mat3 covariance) {
		arma::mat A(3, 3);
		A(0, 0) = covariance[0].x;
		A(1, 0) = covariance[0].y;
		A(2, 0) = covariance[0].z;

		A(0, 1) = covariance[1].x;
		A(1, 1) = covariance[1].y;
		A(2, 1) = covariance[1].z;

		A(0, 2) = covariance[2].x;
		A(1, 2) = covariance[2].y;
		A(2, 2) = covariance[2].z;

		arma::vec eigval;
		arma::mat eigvec;

		eig_sym(eigval, eigvec, A);

		vec3 eigenvalues = normalize(vec3(eigval[0], eigval[1], eigval[2]));
//cout<<eigvec<<endl;
		mat4 rot = mat4(1);

		rot[1].x = eigvec(0, 0);
		rot[1].y = eigvec(1, 0);
		rot[1].z = eigvec(2, 0);

		rot[2].x = eigvec(0, 1);
		rot[2].y = eigvec(1, 1);
		rot[2].z = eigvec(2, 1);

		rot[0].x = eigvec(0, 2);
		rot[0].y = eigvec(1, 2);
		rot[0].z = eigvec(2, 2);

		float angle_1 = atan2(rot[2].y, rot[2].z) * 180.0f / H_PI;
		float angle_2 = atan2(-rot[2].x,
				sqrt(rot[2].y * rot[2].y + rot[2].z * rot[2].z)) * 180.0f / H_PI;
		float angle_3 = atan2(rot[1].x, rot[0].x) * 180.0f / H_PI;
		//cout<<angle_1<<" "<<angle_2<<" "<<angle_3<<endl;
		rot = mat4(1);
		angle_3 = angle_3 > 90 ? angle_3 - 180 : angle_3;
		//rot = glm::rotate(rot, -(angle_1), vec3(1, 0, 0));
		//rot = glm::rotate(rot, -(angle_2), vec3(0, 1, 0));
		rot = glm::rotate(rot, -(angle_3), vec3(0, 0, 1));

		return rot;
	}

	void ToOrigin(vec3 mu) {
		for (int i = 0; i < data.size(); i++) {
			data[i] = data[i] - mu;
		}
	}

	void RotateData(mat4 rot) {
		for (int i = 0; i < data.size(); i++) {
			data[i] = vec3(rot * vec4(data[i], 1.0f));
		}
	}

	vec3 Bounds(vec3 & min_p, vec3 & max_p) {
		min_p = vec3(FLT_MAX);
		max_p = vec3(-FLT_MAX);
		for (int i = 0; i < data.size(); i++) {
			min_p.x = fmin(min_p.x, data[i].x);
			min_p.y = fmin(min_p.y, data[i].y);
			min_p.z = fmin(min_p.z, data[i].z);

			max_p.x = fmax(max_p.x, data[i].x);
			max_p.y = fmax(max_p.y, data[i].y);
			max_p.z = fmax(max_p.z, data[i].z);
		}
		return (max_p - min_p);
	}

	void Slice(vec3 min_p, vec3 max_p) {
		vector<vec3> dat_new;

		for (int i = 0; i < data.size(); i++) {

			if (data[i].x > min_p.x && data[i].y > min_p.y
					&& data[i].z > min_p.z) {
				if (data[i].x < max_p.x && data[i].y < max_p.y
						&& data[i].z < max_p.z) {
					dat_new.push_back(data[i]);
				}
			}

		}
		data.clear();
		data = dat_new;
	}

	void Histogram() {
		vec3 min_p(0), max_p(0);
		Bounds(min_p, max_p);
		int BIN_COUNT = 256;
		hist.clear();
		hist.resize(BIN_COUNT, 0);
		//float h = (max_p.y - min_p.y) / float(BIN_COUNT);
		float h = (0.230561f + 0.230561f) / float(BIN_COUNT);

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
	void DumpHist(string fname) {
		Histogram();
		ofstream ofile(fname);

		for (int i = 0; i < hist.size(); i++) {
			ofile << hist[i] << endl;
		}
	}
	float RMS(vec3 center = vec3(0)) {
		float sq = 0;

		for (int i = 0; i < data.size(); i++) {
			sq += powf(data[i].y - center.y, 2);
		}
		sq = sqrt(sq / float(data.size()));
		return sq;
	}

	float Skewness(vec3 center) {
		float sq = RMS(center);
		float ssk = 0;

		for (int i = 0; i < data.size(); i++) {
			ssk += powf(data[i].y - center.y, 3);
		}
		ssk = ssk / (powf(sq, 3) * float(data.size() - 1));
		return ssk;
	}

	float Kurtosis(vec3 center) {
		float sq = RMS(center);
		float sku = 0;

		for (int i = 0; i < data.size(); i++) {
			sku += powf(data[i].y - center.y, 4);
		}

		sku = sku / (powf(sq, 4) * float(data.size() - 1));
		return sku;
	}
	float ArithmaticMean() {
		float mean = 0;

		for (int i = 0; i < data.size(); i++) {
			mean += data[i].y;

		}
		mean = mean / float(data.size());
		return mean;
	}
	void Write(string filename) {
		ofstream ofile(filename);

		ofile << "#  " << filename << "\n";
		ofile << "#  created by xyz_io::xyz_header_write.C\n";
		ofile << "#\n";
		ofile << "#  Number of points = " << data.size() << "\n";
		ofile << "#\n";

		for (int i = 0; i < data.size(); i++) {
			ofile << data[i].x << "  " << data[i].y << "  " << data[i].z
					<< "\n";
		}
		ofile.close();
	}

	void WriteStats(string filename) {
		ofstream ofile(filename);

		vec3 centroid = Centroid();
		ofile << RMS() << endl;
		ofile << Skewness(centroid) << endl;
		ofile << Kurtosis(centroid) << endl;
		ofile << ArithmaticMean() << endl;
		vec3 min_p(0), max_p(0);
		Bounds(min_p, max_p);
		ofile << min_p.x << " " << min_p.y << " " << min_p.z << endl;
		ofile << max_p.x << " " << max_p.y << " " << max_p.z << endl;

	}

	void SVD() {
		arma::mat svd_data(3, data.size());

		for (int i = 0; i < data.size(); i++) {
			svd_data(0, i) = data[i].x;
			svd_data(1, i) = data[i].y;
			svd_data(2, i) = data[i].z;

		}

		arma::mat U;
		arma::vec s;
		arma::mat V;
		arma::svd(U, s, V, svd_data);

		cout << U << endl;
		cout << s << endl;
		cout << V << endl;

	}

	void Process(string filename) {
		Load(filename);

		Eigen::Matrix3f covariance_matrix;
		Eigen::Vector4f xyz_centroid;

		compute3DCentroid(*cloud_ptr, xyz_centroid);
		computeCovarianceMatrixNormalized(*cloud_ptr, xyz_centroid,covariance_matrix);

		//Eigen::Vector4f min_p, max_p;
		//getMinMax3D(*cloud_ptr, min_p, max_p);

		//xyz_centroid = (max_p-min_p)*.5;

//		cout << covariance_matrix << endl;

		Eigen::SelfAdjointEigenSolver<Eigen::Matrix3f> eigen_solver(
				covariance_matrix, Eigen::ComputeEigenvectors);
		Eigen::Matrix3f eigDx = eigen_solver.eigenvectors();
		eigDx.col(1).swap(eigDx.col(0));
		eigDx.col(2) = eigDx.col(0).cross(eigDx.col(1));

		Eigen::Matrix4f p2w(Eigen::Matrix4f::Identity());
		p2w.block<3, 3>(0, 0) = eigDx.transpose();
		p2w.block<3, 1>(0, 3) = -1.f
				* (p2w.block<3, 3>(0, 0) * xyz_centroid.head<3>());
		pcl::transformPointCloud(*cloud_ptr, *cloud_ptr, p2w);

		cout << "in cloud ptr size:" << cloud_ptr->size() << "\n";
		cout << "in cloud size:" << cloud.size() << "\n";
		pcl::CropBox<PointXYZ> cropFilter;
		cropFilter.setInputCloud(cloud_ptr);
		Eigen::Vector4f minPoint(-.1 * 100.0f, -.1 * 100.0f, -.7 * 100.0f, 0);
		Eigen::Vector4f maxPoint(.1 * 100.0f, .1 * 100.0f, -.55 * 100.0f, 0);
		cropFilter.setMin(minPoint);
		cropFilter.setMax(maxPoint);
		cropFilter.filter(*cloud_ptr);

		cout << "cloud ptrsize:" << cloud_ptr->size() << "\n";
		cout << "cloud size:" << cloud.size() << "\n";

		{
			compute3DCentroid(*cloud_ptr, xyz_centroid);
			computeCovarianceMatrixNormalized(*cloud_ptr, xyz_centroid,
					covariance_matrix);
			Eigen::SelfAdjointEigenSolver<Eigen::Matrix3f> eigen_solver(
					covariance_matrix, Eigen::ComputeEigenvectors);
			Eigen::Matrix3f eigDx = eigen_solver.eigenvectors();
			eigDx.col(1).swap(eigDx.col(0));
			eigDx.col(2) = eigDx.col(0).cross(eigDx.col(1));

			Eigen::Matrix4f p2w(Eigen::Matrix4f::Identity());
			p2w.block<3, 3>(0, 0) = eigDx.transpose();
			p2w.block<3, 1>(0, 3) = -1.f
					* (p2w.block<3, 3>(0, 0) * xyz_centroid.head<3>());
			pcl::transformPointCloud(*cloud_ptr, *cloud_ptr, p2w);
		}

		mat4 trans = mat4(1);

		data.resize(cloud_ptr->size());
		for (int i = 0; i < cloud_ptr->size(); i++) {

			data[i] = vec3(cloud_ptr->points[i].x, cloud_ptr->points[i].y,
					cloud_ptr->points[i].z);
		}

		filename.pop_back();
		filename.pop_back();
		filename.pop_back();
		filename.pop_back();

		stringstream sa;
		sa << filename << "_info.txt";

		WriteStats(sa.str());

		stringstream sb;
		sb << filename << "_hist.txt";

		DumpHist(sb.str());
		stringstream sc;
		sc << filename << "_slice.txt";
		Write(sc.str());

	}
	vector<vec3> data;

	vector<int> hist;

	pcl::PointCloud<PointXYZ> cloud;
	pcl::PointCloud<PointXYZ>::Ptr cloud_ptr;
};
