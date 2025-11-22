#ifndef PCR_CLOUD_POINT_GENERATOR_HPP
#define PCR_CLOUD_POINT_GENERATOR_HPP

#include <glm/vec3.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <vector>
#include <thread>
#include <future>
#include <string>
#include <memory>

namespace pcr {


	// Represents the full data and metadata of the cloud generation.
	// includes: the legnth and points
	struct CloudPointData {
		std::vector<glm::vec3> points{};
		unsigned long long length = 0; 
		// could just be an int but:
		//	may as well just utilize the remaining space rather 
		//    than just have it be blank and empty but still used
	};

	struct CloudPointMetaData {
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> faces;
		CloudPointMetaData(std::vector<glm::vec3>& verts, std::vector<glm::vec3> faces) {
			this->vertices = verts;
			this->faces = faces;
		}
	};

	class CloudPointGenerator {
	private:
		// there has to be a better way to load static data
		static std::unique_ptr<CloudPointMetaData> fileData;
		static int density;

		static void WorkerInsertIntoVector(
			std::vector<glm::vec3>& faces, 
			std::vector<glm::vec3>& points,
			std::vector<glm::vec3>& dest,
			int start, 
			int end,
			int density
		);
		
		CloudPointMetaData& GetCloudPointsFromFile(const char* filename);
		static glm::vec3 Baycentric(const glm::vec3& A, const glm::vec3& B, const glm::vec3& C);
		
		
	public:

		CloudPointData GenerateCloudPoints(const char* filename);
		CloudPointData GenerateBasicCloudPoint(const char* filename);
		CloudPointData GenerateTestTrianglePoints(const char* filename);
		std::future<void> AsyncGetCloudPointsFromFile(const char* filename);

		void SetDensity(const int& density);
		// Releases the static file data resources
		//  - Make sure you call this
		void FreeStaticFileData();
	};
};

#endif
