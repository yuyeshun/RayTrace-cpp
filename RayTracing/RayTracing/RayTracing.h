#pragma once
#include <mutex>
#include "Camera.h"
#include "HitableList.h"
#include "Sphere.h"
#include "Material.h"

class RayTracing {

public:
	RayTracing();
	RayTracing(int nx_, int ny_, int ns_, Hitable *world_);
	~RayTracing();

public:
	void SetNx(int nx_) { nx = nx_; }
	void SetNy(int ny_) { ny = ny_; }
	void SetNs(int ns_) { ns = ns_; }
	void SetWorld(Hitable *list) { world = list; }
	void SetCamera(Camera *pCam) { camera = pCam; }
	void SetForceThreadNum(int num) { forceThreadNum = num; }

	int GetForceThreadNum() { return forceThreadNum; }

	float GetCalculatePercentage();
	float GetGeneraatePercentage();
public:
	void BeginMutiThreadCalc();

	/** full path with file name like: image*/
	void GenerateImage(string path, ImgType imgType = ImgType::BMP);

private:
	Vec3 CalculateColor(const Ray& r, int depth);

	void AllocateMemory();

	bool Generate(string path, ImgType type);

	void Calculate(int beginNy);

	void Calculate_Range(int beginNy, int endNy);

private:
	int nx;
	int ny;
	int ns;

	int forceThreadNum;

	int CalculatePixelCount;
	int GeneratePixelCount;
	int totalPixel;

	int CurCalcRow = 0;

	Pixel **pixelArray;

	Hitable *world;

	mutex mLock;

	Camera *camera;

};