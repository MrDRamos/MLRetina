
#include <iostream>
#include <vector>
#include <algorithm>
const double pi = 3.14159265358979323846;

struct Point
{
	/*
	Point(float x, float y)
	{
		this->x = y;
		this->y = y;
	}
	*/
	float x;
	float y;
};

struct GridPtSource
{
	int x;
	int y;
	float wight;
};

class WedgeRoi
{
public:
	const float Arc;		// const radians
	const int   GridDiv;	// Number of probing positions / gridPt

	float OrgR;		// Anchor radial distance to origin
	float OrgA;		// Anchor angle in radians
	float Scale;	// Size of shape

	Point Center;
	float OutR;		// Outer radius
	float S1A;		// Side 1 angle
	float S2A;		// Side 1 angle
	Point S1Ri;		// @ Side 1, inner radius
	Point S1Ro;		// @ Side 1, outer radius
	Point S2Ri;		// @ Side 2, inner radius
	Point S2Ro;		// @ Side 2, outer radius
	Point BB1;		// Bounding box Top, Left	Min x Max y
	Point BB2;		// Bounding box Btm, Right	Max x Min y
	float DivLen;
	float Area;

	Point Rad2Xy(float R, float A)
	{
		Point xy = { R * cos(S1A), R * sin(S1A) };
		return xy;
	}

	void InitBB()
	{
		OutR = OrgR + Scale;
		float CenterR = OrgR + Scale/2.0;
		Center = Rad2Xy(CenterR , OrgA);
		S1A = OrgA - Arc / 2.0;
		S1Ri = Rad2Xy(OrgR, S1A);
		S1Ro = Rad2Xy(OutR, S1A);
		S2A = OrgA + Arc / 2.0;
		S2Ri = Rad2Xy(OrgR, S2A);
		S2Ro = Rad2Xy(OutR, S2A);
		Point OutRp = Rad2Xy(OutR, OrgA);
		BB1.x = std::min(std::min(S1Ri.x, S2Ri.x), OutRp.x);
		BB1.y = std::max(std::max(S1Ri.y, S2Ri.y), OutRp.y);
		BB2.x = std::max(std::max(S1Ri.x, S2Ri.x), OutRp.x);
		BB2.y = std::min(std::min(S1Ri.y, S2Ri.y), OutRp.y);
		Area = Arc / 2 * ((OutR * OutR) - (OrgR * OrgR));
	}

	void SetLocation(float OrgR, float OrgA, float Scale)
	{
		this->OrgR = OrgR;
		this->OrgA = OrgA;
		this->Scale = Scale;
		InitBB();
	}

	WedgeRoi(float ArcRad, float OrgR, float OrgA, float Scale, int GridDiv) : Arc(Arc), GridDiv(GridDiv)
	{
		//this->Arc = Arc;
		//this->GridDiv = GridDiv;
		SetLocation(OrgR, OrgA, Scale);
	}

	std::vector<GridPtSource> GetGridPt()
	{
		int Probs = Area * (GridDiv * GridDiv) + 1;
		auto retval = std::vector<GridPtSource>(Probs);
		int Ny = GridDiv* (BB1.y - BB2.y)/2 +1;
		float dR = 1.0 / GridDiv;
		auto Yup = std::vector<int>(Ny);
		auto XupMin = std::vector<int>(Ny);
		auto XupMax = std::vector<int>(Ny);
		auto Ydn = std::vector<int>(Ny);
		auto XdnMin = std::vector<int>(Ny);
		auto XdnMax = std::vector<int>(Ny);

		// Find the min,max x positions along the shape
		for (int i = 0; i < Ny; ++i)
		{
			Yup[i] = Center.y + i * dR;
			XupMin[i] = BB1.x;	// TODO
			XupMax[i] = BB2.x;
		}
		for (int i = 0; i < Ny; ++i)
		{
			Ydn[i] = Center.y - (i+1) * dR;
			XdnMin[i] = BB1.x;	// TODO
			XdnMax[i] = BB2.x;
		}

		// Prob the grid
		for (int i = 0; i < Ny; Ny++)
		{
			for (int xi = XupMin[i]; i < XupMax[i]; ++xi)
			{
				// TODO
			}
		}


		return retval;
	}

};


int main()
{
	int Rn = 400; // Radia
	int An = 360; // angles
	float Arc = 2.0 * pi / An;
	WedgeRoi wr = WedgeRoi(Arc, 0, 0, 1, 10);

	
	for (int ri = 0; ri < Rn; ri++)
	{
		float Scale = 1;
		for (int ai = 0; ai < An; ai++)
		{
			wr.SetLocation(ri, ai, Scale);
			auto GridPts = wr.GetGridPt();
		}
	}	
}
