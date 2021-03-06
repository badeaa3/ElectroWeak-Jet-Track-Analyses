#include "TFile.h"
#include "TMath.h"
#include "TH1D.h"
#include "TF1.h"

#include <vector>
#include <string>
#include <iostream>

#include "interface/correlationHist.h"
#include "../TreeHeaders/CutConfigurationTree.h"
#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"

const std::vector<std::string> correlationHistNames {
  "xjg", "dphi", "ptJet"
};
const std::vector<std::string> correlationHistTitleX {
  "p^{Jet}_{T}/p^{#gamma}_{T}", "#Delta#phi_{J#gamma}", "p^{Jet}_{T}"
};
const std::vector<std::string> correlationHistTitleY_final_normalized {
  "#frac{1}{N_{#gamma}} #frac{dN_{J#gamma}}{dx_{J#gamma}}",
  "#frac{1}{N_{#gamma}} #frac{dN_{J#gamma}}{d#Delta#phi}",
  "#frac{1}{N_{#gamma}} #frac{dN_{J#gamma}}{dp^{Jet}_{T}}"
};
const std::vector<int>         nBinsx {16, 20,          30};
const std::vector<double>      xlow   {0,  0,           0};
const std::vector<double>      xup    {2,  TMath::Pi(), 300};

int gammaJetHistogramArithmetic(const TString configFile, const TString inputFile, const TString outputFile) {
  TH1::SetDefaultSumw2();

  std::cout << "running gammaJetHistogramArithmetic()" << std::endl;
  std::cout << "configFile  = " << configFile.Data() << std::endl;
  std::cout << "inputFile   = " << inputFile.Data() << std::endl;
  std::cout << "outputFile  = " << outputFile.Data() << std::endl;

  InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
  CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

  if (!configInput.isValid) {
    std::cout << "Invalid input configuration" << std::endl;
    return 1;
  } else if (!configCuts.isValid) {
    std::cout << "Invalid cut configuration" << std::endl;
    return 1;
  }

  // input configuration
  int collision = configInput.proc[INPUT::kHISTOGRAM].i[INPUT::k_collisionType];
  const char* collisionName = getCollisionTypeName((COLL::TYPE)collision).c_str();
  // verbose about input configuration
  std::cout << "Input Configuration :" << std::endl;
  std::cout << "collision = " << collisionName << std::endl;

  // bool isMC = collisionIsMC((COLL::TYPE)collision);
  bool isHI = collisionIsHI((COLL::TYPE)collision);

  // observable bins
  std::vector<float> bins_pt[2];          // array of vectors for eta bins, each array element is a vector.
  std::vector<int>   bins_hiBin[2];       // array of vectors for hiBin bins, each array element is a vector.

  bins_pt[0] = ConfigurationParser::ParseListFloat(
                 configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_gt]);
  bins_pt[1] = ConfigurationParser::ParseListFloat(
                 configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_lt]);
  bins_hiBin[0] = ConfigurationParser::ParseListInteger(
                    configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
  bins_hiBin[1] = ConfigurationParser::ParseListInteger(
                    configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);

  int nBins_pt = bins_pt[0].size();         // assume <myvector>[0] and <myvector>[1] have the same size.
  int nBins_hiBin = bins_hiBin[0].size();     // assume <myvector>[0] and <myvector>[1] have the same size.

  std::vector<float> purityVec = ConfigurationParser::ParseListFloat(
                                   configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_purity]);
  int puritySize = purityVec.size();
  if (puritySize != (nBins_pt * nBins_hiBin)) {
    std::cout << "Purity array malformed. Should be size: " << (nBins_pt * nBins_hiBin)
              << ", actual size: " << puritySize << ". Aborting." << std::endl;
    return 1;
  }

  double purity[nBins_pt][nBins_hiBin];
  for (int i = 0; i < nBins_pt; ++i) {
    for (int j = 0; j < nBins_hiBin; ++j) {
      purity[i][j] = purityVec[i * nBins_hiBin + j];
    }
  }

  // dphi width/pedestal plots as a function of photon pt, for different centrality bins
  std::vector<int> pt_bin_numbers = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_pt_bin_numbers]);
  const int n_pt_bins = pt_bin_numbers.size();

  // dphi width/pedestal plots as a function of centrality, for different photon pt bins
  std::vector<int> cent_bin_numbers = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_cent_bin_numbers]);
  int n_cent_bins = cent_bin_numbers.size();

  int nEventsToMix = configCuts.proc[CUTS::kSKIM].obj[CUTS::kGAMMAJET].i[CUTS::GJT::k_nEventsToMix];
  int nSmear = configCuts.proc[CUTS::kSKIM].obj[CUTS::kJET].i[CUTS::JET::k_nSmear];

  TTree* configTree = setupConfigurationTreeForWriting(configCuts);

  TFile* input = TFile::Open(inputFile);
  TFile* output = TFile::Open(outputFile, "RECREATE");

  // histograms will be put under a directory whose name is the type of the collision
  if (!output->GetKey(collisionName))
    output->mkdir(collisionName, Form("input file is %s", inputFile.Data()));
  output->cd(collisionName);
  std::cout << "histograms will be put under directory : " << collisionName << std::endl;

  int nCorrHist = correlationHistNames.size();
  correlationHist corrHists[nCorrHist][nBins_pt][nBins_hiBin];

  float xjg_mean[nBins_pt][nBins_hiBin][CORR::kN_CORRFNC][CORR::kN_CORRFNC] = {0};
  float xjg_mean_err[nBins_pt][nBins_hiBin][CORR::kN_CORRFNC][CORR::kN_CORRFNC] = {0};
  float xjg_weight[nBins_pt][nBins_hiBin][CORR::kN_CORRFNC][CORR::kN_CORRFNC] = {0};

  // nPho Histograms
  TH1D* h_nPho[nBins_pt][nBins_hiBin][2];
  for (int i = 0; i < nBins_pt; ++i) {
    for (int j = 0; j < nBins_hiBin; ++j) {
      std::string histNamePhoRAW = Form("h_nPho_ptBin%d_hiBin%d_%s", i, j, CORR::CORR_PHO_LABELS[CORR::kRAW].c_str());
      h_nPho[i][j][CORR::kRAW] = (TH1D*)input->Get(Form("%s/%s", collisionName, histNamePhoRAW.c_str()));

      std::string histNamePhoBKG = Form("h_nPho_ptBin%d_hiBin%d_%s", i, j, CORR::CORR_PHO_LABELS[CORR::kBKG].c_str());
      h_nPho[i][j][CORR::kBKG] = (TH1D*)input->Get(Form("%s/%s", collisionName, histNamePhoBKG.c_str()));
    }
  }

  // prepare histogram names for xjg, abs(dphi) and jet pt
  for (int iHist = 0; iHist < nCorrHist; ++iHist) {
    for (int i = 0; i < nBins_pt; ++i) {
      for (int j = 0; j < nBins_hiBin; ++j) {
        corrHists[iHist][i][j].name = Form("%s_ptBin%d_hiBin%d", correlationHistNames[iHist].c_str(), i, j);

        for (int iCorr = 0; iCorr < CORR::kN_CORRFNC; ++iCorr) {
          for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {
            std::string subHistName = Form("%s_ptBin%d_hiBin%d_%s_%s", correlationHistNames[iHist].c_str(), i, j,
                                           CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
            corrHists[iHist][i][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
            if (iCorr > 1 || jCorr > 1) {
              corrHists[iHist][i][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "",
                  nBinsx[iHist], xlow[iHist], xup[iHist]);
              continue;
            }

            corrHists[iHist][i][j].h1D[iCorr][jCorr] = (TH1D*)input->Get(Form("%s/h1D_%s", collisionName, subHistName.c_str()));

            if (!corrHists[iHist][i][j].h1D[iCorr][jCorr])
              std::cout << "Histogram not found: " << subHistName.c_str() << std::endl;

            corrHists[iHist][i][j].h1D_titleX[iCorr][jCorr] = correlationHistTitleX[iHist].c_str();
            corrHists[iHist][i][j].h1D_titleY_final_norm[iCorr][jCorr] = correlationHistTitleY_final_normalized[iHist].c_str();

            if (jCorr == CORR::kBKG && !isHI) continue;

            // FINAL
            std::string tmpHistName = corrHists[iHist][i][j].h1D[iCorr][jCorr]->GetName();
            corrHists[iHist][i][j].h1D_final[iCorr][jCorr] =
              (TH1D*)corrHists[iHist][i][j].h1D[iCorr][jCorr]->Clone(Form("%s_final", tmpHistName.c_str()));
            corrHists[iHist][i][j].h1D_final[iCorr][jCorr]->Write("", TObject::kOverwrite);

            // FINAL_NORM
            corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr] =
              (TH1D*)corrHists[iHist][i][j].h1D_final[iCorr][jCorr]->Clone(Form("%s_final_norm", tmpHistName.c_str()));
            if (jCorr == CORR::kBKG && isHI) {
              // normalize first by the number of mixed events
              corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Scale(1. / nEventsToMix);
            }

            if (nSmear > 0 && nSmear != 1) {
              // first correct actual bin value
              corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Scale(1. / nSmear);
              // then increase statistical bin error by 10 to account for 100 "fake" smearing
              for (int ibin = 1; ibin <= corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->GetNbinsX(); ibin++) {
                corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->SetBinError(ibin, TMath::Sqrt(nSmear)*corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->GetBinError(ibin));
              }
            }

            // normalization is done with photon events, not necessarily by photon-jet events
            // so the integral of the normalized histogram is R_jg.
            double tmpNEntriesPho = h_nPho[i][j][iCorr]->GetBinContent(1);
            corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Scale(1. / tmpNEntriesPho);

            if (iHist == 0 && iCorr != CORR::kSIG && jCorr != CORR::kSIG) {
              xjg_mean[i][j][iCorr][jCorr] = corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->GetMean();
              xjg_mean_err[i][j][iCorr][jCorr] = corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->GetMeanError();
              xjg_weight[i][j][iCorr][jCorr] = corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->GetSumOfWeights();
            }

            corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Scale(1, "width"); // distribution histogram : scale by bin width

            std::string tmpTitleY_final_norm = corrHists[iHist][i][j].h1D_titleY_final_norm[iCorr][jCorr].c_str();
            corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->GetYaxis()->SetTitle(tmpTitleY_final_norm.c_str());
            corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Write("", TObject::kOverwrite);
          }

          if (iHist == 0 && iCorr != CORR::kSIG) {
            double xjg_mean_RAW = xjg_mean[i][j][iCorr][CORR::kRAW];
            double xjg_mean_BKG = xjg_mean[i][j][iCorr][CORR::kBKG];
            double xjg_mean_err_RAW = xjg_mean_err[i][j][iCorr][CORR::kRAW];
            double xjg_mean_err_BKG = xjg_mean_err[i][j][iCorr][CORR::kBKG];
            double xjg_weight_RAW = xjg_weight[i][j][iCorr][CORR::kRAW];
            double xjg_weight_BKG = xjg_weight[i][j][iCorr][CORR::kBKG];

            xjg_weight[i][j][iCorr][CORR::kSIG] = xjg_weight_RAW - xjg_weight_BKG;
            xjg_mean[i][j][iCorr][CORR::kSIG] = (xjg_mean_RAW * xjg_weight_RAW - xjg_mean_BKG * xjg_weight_BKG) / (xjg_weight_RAW - xjg_weight_BKG);
            xjg_mean_err[i][j][iCorr][CORR::kSIG] = TMath::Sqrt(xjg_weight_RAW * xjg_weight_RAW * xjg_mean_err_RAW * xjg_mean_err_RAW - xjg_weight_BKG * xjg_weight_BKG * xjg_mean_err_BKG * xjg_mean_err_BKG) / (xjg_weight_RAW - xjg_weight_BKG);
          }
        }

        if (iHist == 0) {
          double xjg_mean_RAW = xjg_mean[i][j][CORR::kRAW][CORR::kSIG];
          double xjg_mean_BKG = xjg_mean[i][j][CORR::kBKG][CORR::kSIG];
          double xjg_mean_err_RAW = xjg_mean_err[i][j][CORR::kRAW][CORR::kSIG];
          double xjg_mean_err_BKG = xjg_mean_err[i][j][CORR::kBKG][CORR::kSIG];
          double xjg_weight_RAW = xjg_weight[i][j][CORR::kRAW][CORR::kSIG];
          double xjg_weight_BKG = xjg_weight[i][j][CORR::kBKG][CORR::kSIG];

          xjg_weight_BKG *= (1 - purity[i][j]);

          xjg_weight[i][j][CORR::kSIG][CORR::kSIG] = xjg_weight_RAW - xjg_weight_BKG;
          xjg_mean[i][j][CORR::kSIG][CORR::kSIG] = (xjg_mean_RAW * xjg_weight_RAW - xjg_mean_BKG * xjg_weight_BKG) / (xjg_weight_RAW - xjg_weight_BKG);
          xjg_mean_err[i][j][CORR::kSIG][CORR::kSIG] = TMath::Sqrt(xjg_weight_RAW * xjg_weight_RAW * xjg_mean_err_RAW * xjg_mean_err_RAW - xjg_weight_BKG * xjg_weight_BKG * xjg_mean_err_BKG * xjg_mean_err_BKG) / (xjg_weight_RAW - xjg_weight_BKG);
        }
      }
    }
  }

  output->cd(collisionName);

  // histograms with pt bins on x-axis
  std::vector<std::string> correlationHistNames_ptBinAll = {"rjg", "xjg_mean"};
  int nCorrHist_ptBinAll = correlationHistNames_ptBinAll.size();
  correlationHist corrHists_ptBinAll[nCorrHist_ptBinAll][nBins_hiBin];

  double pt_binning[n_pt_bins + 1];
  for (int i = 0; i < n_pt_bins; ++i) {
    if (i > 0 && bins_pt[0][pt_bin_numbers[i]] != bins_pt[1][pt_bin_numbers[i - 1]]) {
      std::cout << "ERROR: pt bins are not arranged correctly." << std::endl;
      return 1;
    }
    pt_binning[i] = bins_pt[0][pt_bin_numbers[i]];
  }
  pt_binning[n_pt_bins] = bins_pt[0][pt_bin_numbers[n_pt_bins - 1]] + 20;

  // prepare histogram names for rjg and <xjg>
  for (int j = 0; j < nBins_hiBin; ++j) {
    for (int iCorr = 0; iCorr < CORR::kN_CORRFNC; ++iCorr) {
      for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {
        // rjg
        std::string subHistName;
        subHistName = Form("%s_ptBinAll_hiBin%d_%s_%s", correlationHistNames_ptBinAll[0].c_str(), j,
                           CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
        corrHists_ptBinAll[0][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
        corrHists_ptBinAll[0][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "", n_pt_bins, pt_binning);

        // x_jg_mean
        subHistName = Form("%s_ptBinAll_hiBin%d_%s_%s", correlationHistNames_ptBinAll[1].c_str(), j,
                           CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
        corrHists_ptBinAll[1][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
        corrHists_ptBinAll[1][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "", n_pt_bins, pt_binning);
      }
    }
  }

  // histograms with centrality on the x-axis
  std::vector<std::string> correlationHistNames_centBinAll = {"rjg", "xjg_mean"};
  int nCorrHist_centBinAll = correlationHistNames_centBinAll.size();
  correlationHist corrHists_centBinAll[nCorrHist_centBinAll][nBins_pt];

  double centrality_binning[n_cent_bins + 1];
  for (int i = 0; i < n_cent_bins; ++i) {
    if (i > 0 && bins_hiBin[0][cent_bin_numbers[i]] != bins_hiBin[1][cent_bin_numbers[i - 1]]) {
      std::cout << "ERROR: cent bins are not arranged correctly." << std::endl;
      return 1;
    }
    centrality_binning[i] = bins_hiBin[0][cent_bin_numbers[i]];
  }
  centrality_binning[n_cent_bins] = bins_hiBin[1][cent_bin_numbers[n_cent_bins - 1]];

  double npart_binning[n_cent_bins + 1];
  for (int i = 0; i < n_cent_bins + 1; ++i)
    npart_binning[i] = findNpart(centrality_binning[n_cent_bins - i]);

  // prepare histogram names for rjg and <xjg>
  for (int j = 0; j < nBins_pt; ++j) {
    for (int iCorr = 0; iCorr < CORR::kN_CORRFNC; ++iCorr) {
      for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {
        // rjg
        std::string subHistName;
        subHistName = Form("%s_centBinAll_ptBin%d_%s_%s", correlationHistNames_centBinAll[0].c_str(), j,
                           CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
        corrHists_centBinAll[0][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
        corrHists_centBinAll[0][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "", n_cent_bins, npart_binning);

        // x_jg_mean
        subHistName = Form("%s_centBinAll_ptBin%d_%s_%s", correlationHistNames_centBinAll[1].c_str(), j,
                           CORR::CORR_PHO_LABELS[iCorr].c_str(), CORR::CORR_JET_LABELS[jCorr].c_str());
        corrHists_centBinAll[1][j].h1D_name[iCorr][jCorr] = subHistName.c_str();
        corrHists_centBinAll[1][j].h1D[iCorr][jCorr] = new TH1D(Form("h1D_%s", subHistName.c_str()), "", n_cent_bins, npart_binning);
      }
    }
  }

  for (int iHist = 0; iHist < nCorrHist; iHist++) {
    for (int i = 0; i < nBins_pt; ++i) {
      for (int j = 0; j < nBins_hiBin; ++j) {
        // histograms for RAW and BKG regions
        for (int iCorr = 0; iCorr < CORR::kN_CORRFNC - 1; ++iCorr) {
          for (int jCorr = 0; jCorr < CORR::kN_CORRFNC - 1; ++jCorr) {
            if (jCorr == CORR::kBKG && !isHI) continue; // no jet background for non-HI

            corrHists[iHist][i][j].h1D[iCorr][jCorr]->Write("", TObject::kOverwrite);
            corrHists[iHist][i][j].h1D_final[iCorr][jCorr]->Write("", TObject::kOverwrite);
            corrHists[iHist][i][j].h1D_final_norm[iCorr][jCorr]->Write("", TObject::kOverwrite);
          }
        }

        // calculate SIGSIG histogram,
        // these histograms are ignored : SIGRAW, SIGBKG

        // no background for PP and PA
        if (!isHI) {
          // for non-HI, reset jet BKG histograms
          // in that case BKG histograms are not used, but set to empty histograms.
          // so it becomes SIG = RAW
          std::cout << "collision is " << collisionName << std::endl;
          std::cout << "contribution from BKG region is set to zero." << std::endl;

          std::string tmpHistName;

          // reset RAWBKG
          tmpHistName = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kRAW][CORR::kBKG]->GetName());
          corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kBKG] =
            (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kRAW]->Clone(tmpHistName.c_str());
          corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kBKG]->Reset();

          // reset BKGBKG
          tmpHistName = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kBKG][CORR::kBKG]->GetName());
          corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kBKG] =
            (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kRAW]->Clone(tmpHistName.c_str());
          corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kBKG]->Reset();
        }

        // first subtract jet BKG, then subtract photon BKG
        // subtract jet BKG
        // RAWSIG = RAWRAW - RAWBKG
        std::string tmpHistNameRAWSIG = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kRAW][CORR::kSIG]->GetName());
        corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG] =
          (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kRAW]->Clone(tmpHistNameRAWSIG.c_str());
        // do arithmetic if histograms are not empty
        if (corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kBKG]->GetEntries() > 0) {
          corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG]->Add(corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kBKG], -1);
        }

        // BKGSIG = BKGRAW - BKGBKG
        std::string tmpHistNameBKGSIG = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kBKG][CORR::kSIG]->GetName());
        corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG] =
          (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kRAW]->Clone(tmpHistNameBKGSIG.c_str());
        // do arithmetic if histograms are not empty
        if (corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kBKG]->GetEntries() > 0) {
          corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG]->Add(corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kBKG], -1);
        }

        // subtract photon BKG
        // purity * SIGSIG + (1 - purity) * BKGSIG = RAWSIG
        // SIGSIG = 1/purity * (RAWSIG - (1-purity) * BKGSIG)
        std::string tmpHistNameSIGSIG = Form("%s_final_norm", corrHists[iHist][i][j].h1D[CORR::kSIG][CORR::kSIG]->GetName());
        corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG] =
          (TH1D*)corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG]->Clone(tmpHistNameSIGSIG.c_str());
        // do arithmetic if histograms are not empty
        if (corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG]->GetEntries() > 0) {
          corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Add(corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG], -1 * (1 - purity[i][j]));
          corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Scale(1. / purity[i][j]);
        }

        // correct dphi normalization
        if (iHist == 1) {
          corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Scale(1. / corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Integral(), "width");
        }

        corrHists[iHist][i][j].h1D_final_norm[CORR::kRAW][CORR::kSIG]->Write("", TObject::kOverwrite);
        corrHists[iHist][i][j].h1D_final_norm[CORR::kBKG][CORR::kSIG]->Write("", TObject::kOverwrite);
        corrHists[iHist][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Write("", TObject::kOverwrite);

        std::cout << Form("histogramming END : ptBin%d HiBin%d", i, j) << std::endl;
      }
    }
    std::cout << "histogramming END : " << correlationHistNames[iHist].c_str() << std::endl;
    std::cout << "####################" << std::endl;
  }

  // fit dphi
  TF1* dphi_fit_func = new TF1("dphi_fit_func", "[0]+[1]*exp((x-3.14159265358979323)/[2])", 0, TMath::Pi());
  dphi_fit_func->SetParameters(0.01, 0.5, 0.3);

  TF1* fit_dphi[nBins_pt][nBins_hiBin];
  for (int i = 0; i < nBins_pt; ++i) {
    for (int j = 0; j < nBins_hiBin; ++j) {
      corrHists[1][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->Fit("dphi_fit_func", "QREM0", "", TMath::Pi() * 2 / 3, TMath::Pi());
      fit_dphi[i][j] = corrHists[1][i][j].h1D_final_norm[CORR::kSIG][CORR::kSIG]->GetFunction("dphi_fit_func");
      fit_dphi[i][j]->Write(Form("fit_dphi_ptBin%i_hiBin%i", i, j), TObject::kOverwrite);
    }
  }

  TH1D* h_dphi_width_pt[nBins_hiBin];
  TH1D* h_dphi_pedestal_pt[nBins_hiBin];
  for (int i = 0; i < nBins_hiBin; ++i) {
    h_dphi_width_pt[i] = new TH1D(Form("h1D_dphi_width_ptBinAll_hiBin%i", i), "", n_pt_bins, pt_binning);
    h_dphi_pedestal_pt[i] = new TH1D(Form("h1D_dphi_pedestal_ptBinAll_hiBin%i", i), "", n_pt_bins, pt_binning);
    h_dphi_width_pt[i]->SetTitle(";p^{#gamma}_{T} (GeV/c);#Delta#phi width");
    h_dphi_pedestal_pt[i]->SetTitle(";p^{#gamma}_{T} (GeV/c);#Delta#phi pedestal");

    for (int j = 0; j < n_pt_bins; ++j) {
      h_dphi_width_pt[i]->SetBinContent(j + 1, fit_dphi[pt_bin_numbers[j]][i]->GetParameter(2));
      h_dphi_width_pt[i]->SetBinError(j + 1, fit_dphi[pt_bin_numbers[j]][i]->GetParError(2));
      h_dphi_pedestal_pt[i]->SetBinContent(j + 1, fit_dphi[pt_bin_numbers[j]][i]->GetParameter(0));
      h_dphi_pedestal_pt[i]->SetBinError(j + 1, fit_dphi[pt_bin_numbers[j]][i]->GetParError(0));
    }
  }

  TH1D* h_dphi_width_cent[nBins_pt];
  TH1D* h_dphi_pedestal_cent[nBins_pt];
  for (int i = 0; i < nBins_pt; ++i) {
    h_dphi_width_cent[i] = new TH1D(Form("h1D_dphi_width_centBinAll_ptBin%i", i), "", n_cent_bins, npart_binning);
    h_dphi_pedestal_cent[i] = new TH1D(Form("h1D_dphi_pedestal_centBinAll_ptBin%i", i), "", n_cent_bins, npart_binning);
    h_dphi_width_cent[i]->SetTitle(";N_{part};#Delta#phi width");
    h_dphi_pedestal_cent[i]->SetTitle(";N_{part};#Delta#phi pedestal");

    for (int j = 0; j < n_cent_bins; ++j) {
      h_dphi_width_cent[i]->SetBinContent(n_cent_bins - j, fit_dphi[i][cent_bin_numbers[j]]->GetParameter(2));
      h_dphi_width_cent[i]->SetBinError(n_cent_bins - j, fit_dphi[i][cent_bin_numbers[j]]->GetParError(2));
      h_dphi_pedestal_cent[i]->SetBinContent(n_cent_bins - j, fit_dphi[i][cent_bin_numbers[j]]->GetParameter(0));
      h_dphi_pedestal_cent[i]->SetBinError(n_cent_bins - j, fit_dphi[i][cent_bin_numbers[j]]->GetParError(0));
    }
  }

  // histograms with pt bins on x-axis
  // corrHists_ptBinAll[0][]  = R_jg
  // corrHists_ptBinAll[1][]  = <X_jg>
  std::cout << "####################" << std::endl;
  for (int j = 0; j < nBins_hiBin; ++j) {
    for (int iCorr = 0; iCorr < CORR::kN_CORRFNC; ++iCorr) {
      for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {
        // ignore SIGRAW, SIGBKG histograms
        if ((iCorr == CORR::kSIG && (jCorr == CORR::kRAW || jCorr == CORR::kBKG))) continue;

        // rjg block
        for (int i = 0; i < n_pt_bins; ++i) {
          double err;
          double val = corrHists[0][pt_bin_numbers[i]][j].h1D_final_norm[iCorr][jCorr]->IntegralAndError(1, corrHists[0][pt_bin_numbers[i]][j].h1D_final_norm[iCorr][jCorr]->GetNbinsX(), err, "width");

          corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetBinContent(i + 1, val);
          corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetBinError(i + 1, err);
        }

        std::string histoTitle = Form("%s , %d-%d %%", collisionName, bins_hiBin[0][j] / 2, bins_hiBin[1][j] / 2);

        corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;p^{#gamma}_{T} (GeV/c); R_{J#gamma}", histoTitle.c_str()));
        corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
        corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->SetMarkerColor(kBlack);

        corrHists_ptBinAll[0][j].h1D[iCorr][jCorr]->Write("", TObject::kOverwrite);

        // xjg_mean block
        for (int i = 0; i < n_pt_bins; ++i) {
          // double val = corrHists[0][pt_bin_numbers[i]][j].h1D_final_norm[iCorr][jCorr]->GetMean();
          // double err = corrHists[0][pt_bin_numbers[i]][j].h1D_final_norm[iCorr][jCorr]->GetMeanError();
          double val = xjg_mean[pt_bin_numbers[i]][j][iCorr][jCorr];
          double err = xjg_mean_err[pt_bin_numbers[i]][j][iCorr][jCorr];

          corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetBinContent(i + 1, val);
          corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetBinError(i + 1, err);
        }

        corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;p^{#gamma}_{T} (GeV/c); <x_{J#gamma}>", histoTitle.c_str()));
        corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
        corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->SetMarkerColor(kBlack);

        corrHists_ptBinAll[1][j].h1D[iCorr][jCorr]->Write("", TObject::kOverwrite);
      }
    }
  }
  std::cout << "####################" << std::endl;

  // histograms with centrality bins on x-axis
  // corrHists_centBinAll[0][]  = R_jg
  // corrHists_centBinAll[1][]  = <X_jg>
  std::cout << "####################" << std::endl;
  for (int j = 0; j < nBins_pt; ++j) {
    for (int iCorr = 0; iCorr < CORR::kN_CORRFNC; ++iCorr) {
      for (int jCorr = 0; jCorr < CORR::kN_CORRFNC; ++jCorr) {
        // ignore SIGRAW, SIGBKG histograms
        if ((iCorr == CORR::kSIG && (jCorr == CORR::kRAW || jCorr == CORR::kBKG))) continue;

        // rjg block
        for (int i = 0; i < n_cent_bins; ++i) {
          double err;
          double val = corrHists[0][j][cent_bin_numbers[i]].h1D_final_norm[iCorr][jCorr]->IntegralAndError(1, corrHists[0][j][cent_bin_numbers[i]].h1D_final_norm[iCorr][jCorr]->GetNbinsX(), err, "width");

          corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->SetBinContent(n_cent_bins - i, val);
          corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->SetBinError(n_cent_bins - i, err);
        }

        std::string histoTitle = Form("%s , %.0f < p^{#gamma}_{T} < %.0f GeV/c", collisionName, bins_pt[0][j], bins_pt[1][j]);

        corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;N_{part}; R_{J#gamma}", histoTitle.c_str()));
        corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
        corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->SetMarkerColor(kBlack);

        corrHists_centBinAll[0][j].h1D[iCorr][jCorr]->Write("", TObject::kOverwrite);

        // xjg_mean block
        for (int i = 0; i < n_cent_bins; ++i) {
          // double val = corrHists[0][j][cent_bin_numbers[i]].h1D_final_norm[iCorr][jCorr]->GetMean();
          // double err = corrHists[0][j][cent_bin_numbers[i]].h1D_final_norm[iCorr][jCorr]->GetMeanError();
          double val = xjg_mean[j][cent_bin_numbers[i]][iCorr][jCorr];
          double err = xjg_mean_err[j][cent_bin_numbers[i]][iCorr][jCorr];

          corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->SetBinContent(n_cent_bins - i, val);
          corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->SetBinError(n_cent_bins - i, err);
        }

        corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->SetTitle(Form("%s;N_{part}; <x_{J#gamma}>", histoTitle.c_str()));
        corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->SetMarkerStyle(kFullCircle);
        corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->SetMarkerColor(kBlack);

        corrHists_centBinAll[1][j].h1D[iCorr][jCorr]->Write("", TObject::kOverwrite);
      }
    }
  }
  std::cout << "####################" << std::endl;

  configTree->Write("", TObject::kOverwrite);

  output->Write("", TObject::kOverwrite);
  input->Close();
  output->Close();

  return 0;
}

int main(int argc, char** argv) {
  if (argc == 4)
    return gammaJetHistogramArithmetic(argv[1], argv[2], argv[3]);
  else
    printf("Usage : \n"
           "./gammaJetHistogramArithmetic.exe <configFile> <inputFile> <outputFile>\n");

  return 1;
}
