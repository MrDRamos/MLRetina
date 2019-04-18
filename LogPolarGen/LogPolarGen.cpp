
#include <iostream>
#include <vector>
//#include <unordered_map>
#include <map>
#include <tuple>
#include <algorithm>

struct GridPtSource
{
  int16_t x;
  int16_t y;
  float weight;
};

class WedgeRoi
{
protected:

  static std::vector<GridPtSource> GetGridPt(float Rmin, float Rmax, int nR, float Amin, float Amax, int nA)
  {
    nR = std::max(nR, 1);
    nA = std::max(nA, 1);
    float nW = (float)nA*nR;
    std::map<std::tuple<int16_t, int16_t>, uint16_t> CountS;
    //std::unordered_map<std::tuple<int16_t, int16_t>, uint16_t> CountS;
    //CountS.reserve(nR * nA);

    float dA = 0;
    if (1 == nA)
    {
      Amin = (Amax + Amin) / 2;
    }
    else
    {
      dA = (Amax - Amin) / (nA - 1);
    }

    float dR = 0;
    if (1 == nR)
    {
      Rmin = (Rmax + Rmin) / 2;
    }
    else
    {
      dR = (Rmax - Rmin) / (nR - 1);
    }

    float A = Amin;
    for (int ia = 0; ia < nA; ++ia, A += dA)
    {
      float cosA = cos(A);
      float sinA = sin(A);
      float R = Rmin;
      for (int ir = 0; ir < nR; ++ir, R += dR)
      {
        int16_t x = (int16_t)std::round(R * cosA);
        int16_t y = (int16_t)std::round(R * sinA);
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
      GridPtSource gp = {std::get<0>(pt), std::get<1>(pt), weight };
      retval.emplace_back(gp);
    }
    return retval;
  }


public:

  static std::vector<GridPtSource> NewTemplate(uint16_t Rxy, uint16_t Rn, uint16_t An)
  {
    const double pi = 3.14159265358979323846;
    const float nAdP = 2.0; // Arc probing density/grid-point
    const float nRdP = 2.0; // Radial probing density/grid-point
    float Arc = 2.0f * (float)pi / An;
    std::vector<GridPtSource> Retval;
    Retval.reserve((size_t)(nRdP * Rn * nAdP * An));

    double logBase = log(Rxy)/Rn;

    float Rmin = 0;
    for (uint16_t ri = 1; ri <= Rn; ri++)
    {
      float Rmax = (float)exp(ri * logBase);
      uint16_t nR = (uint16_t)std::round((Rmax - Rmin) * nRdP);
      uint16_t nA = (uint16_t)std::round(Arc * Rmax * nAdP);
      for (uint16_t ai = 0; ai < An; ai++)
      {
        float Amin = Arc * ai;
        float Amax = Amin + Arc;
        auto gpS = GetGridPt(Rmin, Rmax, nR, Amin, Amax, nA);
        Retval.insert(Retval.end(), gpS.begin(), gpS.end());
      }
      Rmin = Rmax;
    }
    Retval.shrink_to_fit();
    return Retval;
  }


  static std::vector<GridPtSource> NewLogPolarMap(std::vector<GridPtSource>& LpTempate, uint16_t xCenter, uint16_t yCenter, uint16_t Rmax)
  {
    std::vector<GridPtSource> Retval;
    Retval.reserve(LpTempate.size());
    for (auto kvp : LpTempate)
    {

    }
    Retval.shrink_to_fit();
    return Retval;
  }
};


int main()
{
  uint16_t Rxy = 400; // Grid points
  uint16_t Rn = 400;  // Radia
  uint16_t An = 360;  // Angles
  auto LogPlolarTemplate = WedgeRoi::NewTemplate(Rxy, Rn, An);

  uint16_t xCenter = (uint16_t)Rn / 2;
  uint16_t yCenter = xCenter;
  uint16_t Rmax = Rn;
  auto LogPlolarMap = WedgeRoi::NewLogPolarMap(LogPlolarTemplate, xCenter, yCenter, Rmax);

}
