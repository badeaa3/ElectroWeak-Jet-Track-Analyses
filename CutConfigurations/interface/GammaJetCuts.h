#ifndef CUTCONFIGURATIONS_gjt
#define CUTCONFIGURATIONS_gjt

namespace CUTS {
  namespace GJT {
    enum F_CUTS{
      k_awayRange,      // default away range = 0.875, corresponding to 7/8*pi
      k_awayRange_lt,   // upper limit of away range, default value = 1 corresponding to pi
      k_dR,
      kN_F_CUTS
    };

    std::string F_CUTS_LABELS[kN_F_CUTS] = {
      "awayRange",
      "awayRange_lt",
      "dR"
    };

    enum I_CUTS{
      k_doMix,
      k_nMaxEvents_minBiasMixing,
      k_nCentralityBins,
      k_nVertexBins,
      k_nEventPlaneBins,
      k_nEventsToMix,
      kN_I_CUTS
    };

    std::string I_CUTS_LABELS[kN_I_CUTS] = {
      "doMix",
      "nMaxEvents_minBiasMixing",
      "nCentralityBins",
      "nVertexBins",
      "nEventPlaneBins",
      "nEventsToMix"
    };

    enum S_CUTS{
      kN_S_CUTS
    };

    std::string S_CUTS_LABELS[kN_S_CUTS] = {};

  };
};

#endif
