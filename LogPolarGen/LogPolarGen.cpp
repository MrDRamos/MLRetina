
#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <algorithm>
const double pi = 3.14159265358979323846;

struct Point
{
	float x;
	float y;
};

struct GridPtSource
{
	int16_t x;
	int16_t y;
	float weight;
};

class WedgeRoi
{
public:

	static Point Rad2Xy(float R, float A)
	{
		Point xy = { R * cos(A), R * sin(A) };
		return xy;
	}

	static std::vector<GridPtSource> GetGridPt(float Rmin, float Rmax, int nR, float Amin, float Amax, int nA)
	{
		nR = std::max(nR, 1);
		nA = std::max(nA, 1);
		float nW = (nA + 1) * (nR + 1);
		std::map<std::tuple<int16_t, int16_t>, uint16_t> CountS;

		float dR = (Rmax - Rmin) / nR;
		float dA = (Amax - Amin) / nA;
		float A = Amin;
		for (int ia = 0; ia <= nA; ++ia, A += dA)
		{
			float R = Rmin;
			for (int ir = 0; ir <= nR; ++ir, R += dR)
			{
				int16_t x = std::round(R * cos(A));
				int16_t y = std::round(R * sin(A));
				auto xy = std::make_tuple(x, y);
				++CountS[xy];
			}
		}

		std::vector<GridPtSource> retval;
		retval.reserve(CountS.size());
		for (auto kvp : CountS)
		{
			auto pt = kvp.first;
			float weight = kvp.second / nW;
			GridPtSource gp = {
				std::get<0>(pt),
				std::get<1>(pt),
				weight };
			retval.emplace_back(gp);
		}
		return retval;
	}

};


int main()
{
	int Rn = 400; // Radia
	int An = 360; // angles
	float Arc = 2.0 * pi / An;
	std::vector<GridPtSource> GridPointS;
	GridPointS.reserve(Rn * An * 10);

	for (int ri = 0; ri < Rn; ri++)
	{
		float Rmin = ri;
		float Rmax = ri + 1;
		int nR = 3;
		int nA = std::round(Arc * Rmax * 3);
		for (int ai = 0; ai < An; ai++)
		{
			float Amin = ai;
			float Amax = Amin + Arc;
			auto gpS = WedgeRoi::GetGridPt(Rmin, Rmax, nR, Amin, Amax, nA);
			GridPointS.insert(GridPointS.end(), gpS.begin(), gpS.end());
		}
	}
	GridPointS.shrink_to_fit();
}
