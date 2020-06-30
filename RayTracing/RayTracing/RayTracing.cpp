#include <fstream>
#include <stdlib.h>
#include <windows.h>
#include <string>

#include "RayTracing.h"
#include "Material.h"

RayTracing::RayTracing()
	: nx(400)
	, ny(200)
	, ns(100)
	, CalculatePixelCount(0)
	, GeneratePixelCount(0)
	, forceThreadNum(0)
{
	AllocateMemory();
	totalPixel = nx * ny;
}

RayTracing::RayTracing(int nx_, int ny_, int ns_, Hitable *world_)
	: nx(nx_)
	, ny(ny_)
	, ns(ns_)
	, CalculatePixelCount(0)
	, GeneratePixelCount(0)
	, forceThreadNum(0)
{
	world = world_;
	AllocateMemory();
	totalPixel = nx * ny;
}

RayTracing::~RayTracing()
{
	for (int i = 0; i < ny; ++i) {
		delete[]pixelArray[i];
	}
	delete[]pixelArray;

	if (camera) {
		delete camera;
	}	
}

float RayTracing::GetCalculatePercentage()
{
	return (float(CalculatePixelCount) / float(totalPixel)) * 100.0f;
}

float RayTracing::GetGeneraatePercentage()
{
	return (float(GeneratePixelCount) / float(totalPixel)) * 100.0f;
}

void RayTracing::BeginMutiThreadCalc()
{
	int CoreNum = (forceThreadNum > 0) ? forceThreadNum : (thread::hardware_concurrency() > 1 ? thread::hardware_concurrency() - 1 : 1);	

	for (int i = 0; i < CoreNum; ++i) {
		int beginNy = CurCalcRow++;
		thread calcThread(&RayTracing::Calculate, this, beginNy);
		calcThread.detach();
	}
}

void RayTracing::GenerateImage(string path, ImgType imgType)
{
	thread generateThread(&RayTracing::Generate, this, path, imgType);
	generateThread.detach();
}

Vec3 RayTracing::CalculateColor(const Ray & r, int depth)
{
	HitRecord rec;
	if (world->Hit(r, 0.001f, FLT_MAX, rec)) {
		Ray scattered;
		Vec3 attenuation;
		Vec3 emitted = rec.pMat->Emitted(rec.u, rec.v, rec.p);
		if (depth < MaxDepth && rec.pMat->Scatter(r, rec, attenuation, scattered)) {
			return emitted + attenuation * CalculateColor(scattered, depth + 1);
		}
		else {
			return emitted;
		}
	}
	else {
		/*Vec3 unitDirection = UnitVector(r.Direction());
		float t = 0.5f *(unitDirection.Y() + 1.f);
		return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);*/
		return Vec3(0, 0, 0);
	}
}

void RayTracing::AllocateMemory()
{
	pixelArray = new Pixel *[ny];
	for (int i = 0; i < ny; ++i) {
		pixelArray[i] = new Pixel[nx];
		memset(pixelArray[i], 0, sizeof(pixelArray[i]));
	}	
}

bool RayTracing::Generate(string path, ImgType type)
{
	path.append(type == ImgType::PPM ? ".PPM" : ".BMP");
	
	FILE* fp;
	fopen_s(&fp, path.c_str(), "wb");
	if (fp == NULL)
	{
		return false;
	}

	if (type == ImgType::PPM)
	{				
		fprintf(fp, "P6\n%d %d\n255\n", nx, ny);		

		for (int j = ny - 1; j >= 0; --j) {
			for (int i = 0; i < nx; ++i) {				
				fwrite(&pixelArray[j][i].R, 1, 1, fp);
				fwrite(&pixelArray[j][i].G, 1, 1, fp);
				fwrite(&pixelArray[j][i].B, 1, 1, fp);
				
				++GeneratePixelCount;
			}
		}				
	}
	else
	{	
		int bpp = 24;
		BITMAPFILEHEADER bmpheader;
		BITMAPINFOHEADER bmpinfo;		

		bmpheader.bfType = 0x4d42;
		bmpheader.bfReserved1 = 0;
		bmpheader.bfReserved2 = 0;
		bmpheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) - 2;
		bmpheader.bfSize = bmpheader.bfOffBits + nx * ny * bpp / 8;

		bmpinfo.biSize = sizeof(BITMAPINFOHEADER);
		bmpinfo.biWidth = nx;
		bmpinfo.biHeight = ny;
		bmpinfo.biPlanes = 1;
		bmpinfo.biBitCount = bpp;
		bmpinfo.biCompression = 0;//BI_RGB;
		bmpinfo.biSizeImage = nx * ny * bpp / 8;
		bmpinfo.biXPelsPerMeter = 100;
		bmpinfo.biYPelsPerMeter = 100;
		bmpinfo.biClrUsed = 0;
		bmpinfo.biClrImportant = 0;					
		
		fwrite(&bmpheader.bfType, sizeof(bmpheader.bfType), 1, fp);
		fwrite(&bmpheader.bfSize, sizeof(bmpheader.bfSize), 1, fp);
		fwrite(&bmpheader.bfReserved1, sizeof(bmpheader.bfReserved1), 1, fp);
		fwrite(&bmpheader.bfReserved2, sizeof(bmpheader.bfReserved2), 1, fp);
		fwrite(&bmpheader.bfOffBits, sizeof(bmpheader.bfOffBits), 1, fp);

		fwrite(&bmpinfo.biSize, sizeof(bmpinfo.biSize), 1, fp);
		fwrite(&bmpinfo.biWidth, sizeof(bmpinfo.biWidth), 1, fp);
		fwrite(&bmpinfo.biHeight, sizeof(bmpinfo.biHeight), 1, fp);
		fwrite(&bmpinfo.biPlanes, sizeof(bmpinfo.biPlanes), 1, fp);
		fwrite(&bmpinfo.biBitCount, sizeof(bmpinfo.biBitCount), 1, fp);
		fwrite(&bmpinfo.biCompression, sizeof(bmpinfo.biCompression), 1, fp);
		fwrite(&bmpinfo.biSizeImage, sizeof(bmpinfo.biSizeImage), 1, fp);
		fwrite(&bmpinfo.biXPelsPerMeter, sizeof(bmpinfo.biXPelsPerMeter), 1, fp);
		fwrite(&bmpinfo.biYPelsPerMeter, sizeof(bmpinfo.biYPelsPerMeter), 1, fp);
		fwrite(&bmpinfo.biClrUsed, sizeof(bmpinfo.biClrUsed), 1, fp);
		fwrite(&bmpinfo.biClrImportant, sizeof(bmpinfo.biClrImportant), 1, fp);		
		
		int skip = (nx * bpp / 8) % 4;
		char* pSkip = nullptr;
		if (skip > 0)
		{
			skip = 4 - skip;
			pSkip = new char[skip];
			memset(pSkip, 0, skip);
		}		
		for (int j = 0; j < ny; ++j) {
			for (int i = 0; i < nx; ++i) {
				fwrite(&pixelArray[j][i].B, 1, 1, fp);
				fwrite(&pixelArray[j][i].G, 1, 1, fp);
				fwrite(&pixelArray[j][i].R, 1, 1, fp);				
				++GeneratePixelCount;
			}
			if (skip > 0)
			{
				fwrite(&pSkip, skip, 1, fp);
			}
		}	
		if (pSkip != nullptr)
		{
			delete[] pSkip;
			pSkip = nullptr;
		}		
	}

	fclose(fp);
	fp = NULL;

	return true;
}

void RayTracing::Calculate(int beginNy)
{
	int calcNy = beginNy;
	while (CurCalcRow <= ny)
	{
		for (int i = 0; i < nx; ++i) {
			Vec3 col(0, 0, 0);
			for (int s = 0; s < ns; ++s) {
				float u = float(i + Random(0, 1)) / float(nx);
				float v = float(calcNy + Random(0, 1)) / float(ny);
				Ray r = camera->GetRay(u, v);				
				col += CalculateColor(r, 0);
			}

			col /= float(ns);
			col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

			Pixel pixel(int(255.99 * col[0]), int(255.99 * col[1]), int(255.99 * col[2]));

			mLock.lock();
			pixelArray[calcNy][i] = pixel;			
			++CalculatePixelCount;
			mLock.unlock();
		}
		mLock.lock();
		(calcNy != CurCalcRow) ? (calcNy = CurCalcRow++) : (calcNy = ++CurCalcRow);		
		mLock.unlock();
	}	
}

void RayTracing::Calculate_Range(int beginNy, int endNy)
{
	for (int j = beginNy; j < endNy; ++j) {
		for (int i = 0; i < nx; ++i) {
			Vec3 col(0, 0, 0);
			for (int s = 0; s < ns; ++s) {
				float u = float(i + Random(0, 1)) / float(nx);
				float v = float(j + Random(0, 1)) / float(ny);
				Ray r = camera->GetRay(u, v);				
				col += CalculateColor(r, 0);
			}

			col /= float(ns);
			col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

			Pixel pixel(int(255.99 * col[0]), int(255.99 * col[1]), int(255.99 * col[2]));

			mLock.lock();
			pixelArray[j][i] = pixel;
			++CalculatePixelCount;
			mLock.unlock();
		}
	}
}
