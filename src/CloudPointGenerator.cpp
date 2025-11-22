#include "CloudPointGenerator.hpp"

int pcr::CloudPointGenerator::density = 0;
std::unique_ptr<pcr::CloudPointMetaData> pcr::CloudPointGenerator::fileData = 0;

glm::vec3 pcr::CloudPointGenerator::Baycentric(const glm::vec3& A, const glm::vec3& B, const glm::vec3& C) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    float u = dist(gen);
    float v = dist(gen);

    if (u + v > 1.0f) {
        u = 1.0f - u;
        v = 1.0f - v;
    }

    float w = 1.0f - u - v;

    return A * u + B * v + C * w;
}


void pcr::CloudPointGenerator::WorkerInsertIntoVector(
    std::vector<glm::vec3>& faces, 
    std::vector<glm::vec3>& points, 
    std::vector<glm::vec3>& dest,
    int start, 
    int end,
    int density
) {
    int index = start * density;

    for (int i = start; i < end; ++i) {
        const glm::vec3& face = faces[i];

        const glm::vec3& A = points[static_cast<int>(face.x - 1)];
        const glm::vec3& B = points[static_cast<int>(face.y - 1)];
        const glm::vec3& C = points[static_cast<int>(face.z - 1)];

        for (int j = 0; j < density; ++j) {
            glm::vec3 point = CloudPointGenerator::Baycentric(A, B, C);
            const float pointKey = point.x + point.y + point.z;

            dest[index] = std::move(point);
            index += 1;
        }
    }
}


pcr::CloudPointMetaData& pcr::CloudPointGenerator::GetCloudPointsFromFile(const char* filename) {
    if (CloudPointGenerator::fileData) {
        return *CloudPointGenerator::fileData.get();
    }

    std::vector<glm::vec3> points;
    std::vector<glm::vec3> faces;

    std::string line{};
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file... pcr::CloudPointGenerator::GenerateCloudPoints\n");
    }

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        
        iss >> prefix;
        if (prefix == "v") {
            float x, y, z;
            iss >> x >> z >> y;

            points.emplace_back(-x, y, -z);
        }

        if (prefix == "f") {
            float f1, f2, f3;
            auto ErasePreviousTextTillDelimiter = [&line, &iss]() -> void {
                line = line.erase(0, line.find(" ") + 1);
                iss.str(line);
            };

            ErasePreviousTextTillDelimiter();
            iss >> f1;

            ErasePreviousTextTillDelimiter();
            iss >> f2;
               
            ErasePreviousTextTillDelimiter();
            iss >> f3;

            faces.emplace_back(f1, f2, f3);
        }
    }

    CloudPointGenerator::fileData = std::make_unique<CloudPointMetaData>(points, faces);
    return *CloudPointGenerator::fileData.get();
}


std::future<void> pcr::CloudPointGenerator::AsyncGetCloudPointsFromFile(const char* filename) {
    return std::async(std::launch::async, [this, filename] {
        this->GetCloudPointsFromFile(filename);
    });
}


void pcr::CloudPointGenerator::SetDensity(const int& density) {
    CloudPointGenerator::density = density;
}


pcr::CloudPointData pcr::CloudPointGenerator::GenerateCloudPoints(const char* filename) {
    // obj file faces's are 1 indexed not 0 .... ?

    CloudPointMetaData rawData = this->GetCloudPointsFromFile(filename);

    std::vector<glm::vec3> rawFaces = rawData.faces;
    std::vector<glm::vec3> rawPoints = rawData.vertices;
    
    const size_t numPoints = rawPoints.size();
    
    // lower density based on num of points 
    //   - if its < 320 -> stick with 200
    if(CloudPointGenerator::density <= 0) { 
        if (numPoints > 6000) {
            CloudPointGenerator::density = 1; // already an extremely detailed .object
        } else if (numPoints > 5000) {
            CloudPointGenerator::density = 25;
        } else if (numPoints > 2000) {
            CloudPointGenerator::density = 35;
        } else if(numPoints > 850) {
            CloudPointGenerator::density = 55;
        } else if(numPoints > 320) {
            CloudPointGenerator::density = 120;
        } else {
            CloudPointGenerator::density = 200;
        }
    }

    std::vector<glm::vec3> cloud(density * rawFaces.size());
    
    // TODO: Thread Pool
    //   - rn has a manual threads but not ideal
    std::vector<std::thread> workers;
    int division = static_cast<int>(rawFaces.size() / 4);
    
    for (int i = 0; i < 4; ++i) {
        const int start = i * division;
        const int end   = (i+1) * division;

        workers.emplace_back(
            CloudPointGenerator::WorkerInsertIntoVector, 
            std::ref(rawFaces), 
            std::ref(rawPoints), 
            std::ref(cloud), 
            start, 
            end, 
            density
       );
    }

    for (std::thread& worker : workers) {
        worker.join();
    }

    return { cloud, cloud.size() };
}


pcr::CloudPointData pcr::CloudPointGenerator::GenerateBasicCloudPoint(const char* filename) {
    // Least detailed, just render each vertex as a point
    std::vector<glm::vec3> points = this->GetCloudPointsFromFile(filename).vertices;
    return { points, points.size() };
}


pcr::CloudPointData pcr::CloudPointGenerator::GenerateTestTrianglePoints(const char* filename) {
    std::vector<glm::vec3> basicTriangle 
        = { glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.5f, -0.5f, 0.0f) };
    return { basicTriangle, basicTriangle.size()};
}


void pcr::CloudPointGenerator::FreeStaticFileData() {
    if (this->fileData) {
        this->fileData.release();
    }
}
