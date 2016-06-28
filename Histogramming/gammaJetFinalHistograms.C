#include "TFile.h"
#include "TString.h"
#include "TH1.h"
#include "TCanvas.h"

#include "../Utilities/interface/CutConfigurationParser.h"
#include "../Utilities/interface/InputConfigurationParser.h"

int gammaJetFinalHistograms(const TString configFile,
                            const TString PbPb_Data_Filename, const TString PbPb_MC_Filename,
                            const TString pp_Data_Filename,
                            const TString outputFile) {
    TH1::SetDefaultSumw2();

    TFile* PbPb_Data_file = new TFile(PbPb_Data_Filename.Data(), "read");
    TFile* PbPb_MC_file = new TFile(PbPb_MC_Filename.Data(), "read");
    TFile* pp_Data_file = new TFile(pp_Data_Filename.Data(), "read");

    InputConfiguration configInput = InputConfigurationParser::Parse(configFile.Data());
    CutConfiguration configCuts = CutConfigurationParser::Parse(configFile.Data());

    std::vector<float> ptBins[2];
    ptBins[0] = ConfigurationParser::ParseListFloat(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_gt]);
    ptBins[1] = ConfigurationParser::ParseListFloat(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kPHOTON].s[CUTS::PHO::k_bins_pt_lt]);
    int nPtBins = ptBins[0].size();

    std::vector<int> centBins[2];
    centBins[0] = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_gt]);
    centBins[1] = ConfigurationParser::ParseListInteger(configCuts.proc[CUTS::kHISTOGRAM].obj[CUTS::kEVENT].s[CUTS::EVT::k_bins_hiBin_lt]);
    int nCentBins = centBins[0].size();

    TFile* output_file = new TFile(outputFile.Data(), "recreate");

    TH1D* h_xjg_PbPb_Data[nPtBins][nCentBins];
    TH1D* h_xjg_PbPb_MC[nPtBins][nCentBins];
    TH1D* h_xjg_pp_Data[nPtBins][nCentBins];

    TH1D* h_dphi_PbPb_Data[nPtBins][nCentBins];
    TH1D* h_dphi_PbPb_MC[nPtBins][nCentBins];
    TH1D* h_dphi_pp_Data[nPtBins][nCentBins];

    TH1D* h_ptJet_PbPb_Data[nPtBins][nCentBins];
    TH1D* h_ptJet_PbPb_MC[nPtBins][nCentBins];
    TH1D* h_ptJet_pp_Data[nPtBins][nCentBins];

    TH1D* h_IAA[nPtBins][nCentBins];

    for (int i=0; i<nPtBins; ++i) {
        for (int j=0; j<nCentBins; ++j) {
            h_xjg_PbPb_Data[i][j] = (TH1D*)PbPb_Data_file->Get(Form("HI/h1D_xjg_ptBin%i_hiBin%i_phoSIG_jetSIG_final_norm", i, j));
            h_xjg_PbPb_MC[i][j] = (TH1D*)PbPb_MC_file->Get(Form("HIMC/h1D_xjg_ptBin%i_hiBin%i_phoSIG_jetSIG_final_norm", i, j));
            h_xjg_pp_Data[i][j] = (TH1D*)pp_Data_file->Get(Form("PP/h1D_xjg_ptBin%i_hiBin%i_phoSIG_jetSIG_final_norm", i, j));

            h_xjg_PbPb_Data[i][j]->Write(Form("h1D_xjg_ptBin%i_hiBin%i_PbPb_Data", i, j), TObject::kOverwrite);
            h_xjg_PbPb_MC[i][j]->Write(Form("h1D_xjg_ptBin%i_hiBin%i_PbPb_MC", i, j), TObject::kOverwrite);
            h_xjg_pp_Data[i][j]->Write(Form("h1D_xjg_ptBin%i_hiBin%i_pp_Data", i, j), TObject::kOverwrite);

            h_dphi_PbPb_Data[i][j] = (TH1D*)PbPb_Data_file->Get(Form("HI/h1D_dphi_ptBin%i_hiBin%i_phoSIG_jetSIG_final_norm", i, j));
            h_dphi_PbPb_MC[i][j] = (TH1D*)PbPb_MC_file->Get(Form("HIMC/h1D_dphi_ptBin%i_hiBin%i_phoSIG_jetSIG_final_norm", i, j));
            h_dphi_pp_Data[i][j] = (TH1D*)pp_Data_file->Get(Form("PP/h1D_dphi_ptBin%i_hiBin%i_phoSIG_jetSIG_final_norm", i, j));

            h_dphi_PbPb_Data[i][j]->Write(Form("h1D_dphi_ptBin%i_hiBin%i_PbPb_Data", i, j), TObject::kOverwrite);
            h_dphi_PbPb_MC[i][j]->Write(Form("h1D_dphi_ptBin%i_hiBin%i_PbPb_MC", i, j), TObject::kOverwrite);
            h_dphi_pp_Data[i][j]->Write(Form("h1D_dphi_ptBin%i_hiBin%i_pp_Data", i, j), TObject::kOverwrite);

            h_ptJet_PbPb_Data[i][j] = (TH1D*)PbPb_Data_file->Get(Form("HI/h1D_ptJet_ptBin%i_hiBin%i_phoSIG_jetSIG_final_norm", i, j));
            h_ptJet_PbPb_MC[i][j] = (TH1D*)PbPb_MC_file->Get(Form("HIMC/h1D_ptJet_ptBin%i_hiBin%i_phoSIG_jetSIG_final_norm", i, j));
            h_ptJet_pp_Data[i][j] = (TH1D*)pp_Data_file->Get(Form("PP/h1D_ptJet_ptBin%i_hiBin%i_phoSIG_jetSIG_final_norm", i, j));

            h_ptJet_PbPb_Data[i][j]->Write(Form("h1D_ptJet_ptBin%i_hiBin%i_PbPb_Data", i, j), TObject::kOverwrite);
            h_ptJet_PbPb_MC[i][j]->Write(Form("h1D_ptJet_ptBin%i_hiBin%i_PbPb_MC", i, j), TObject::kOverwrite);
            h_ptJet_pp_Data[i][j]->Write(Form("h1D_ptJet_ptBin%i_hiBin%i_pp_Data", i, j), TObject::kOverwrite);

            h_IAA[i][j] = (TH1D*)h_ptJet_PbPb_Data[i][j]->Clone(Form("h1D_iaa_ptBin%i_hiBin%i", i, j));
            h_IAA[i][j]->SetTitle(Form("%.0f < p^{#gamma}_{T} < %.0f GeV/c, %d-%d %%", ptBins[0][i], ptBins[1][i], centBins[0][j]/2, centBins[1][j]/2));
            h_IAA[i][j]->Divide(h_ptJet_pp_Data[i][j]);

            h_IAA[i][j]->Write("", TObject::kOverwrite);
        }
    }

    TH1D* h_rjg_centBinAll_PbPb_Data[nPtBins];
    TH1D* h_rjg_centBinAll_PbPb_MC[nPtBins];
    TH1D* h_rjg_centBinAll_pp_Data[nPtBins];

    TH1D* h_xjg_mean_centBinAll_PbPb_Data[nPtBins];
    TH1D* h_xjg_mean_centBinAll_PbPb_MC[nPtBins];
    TH1D* h_xjg_mean_centBinAll_pp_Data[nPtBins];

    TH1D* h_dphi_width_cent_PbPb_Data[nPtBins];
    TH1D* h_dphi_width_cent_PbPb_MC[nPtBins];
    TH1D* h_dphi_width_cent_pp_Data[nPtBins];

    TH1D* h_dphi_pedestal_cent_PbPb_Data[nPtBins];
    TH1D* h_dphi_pedestal_cent_PbPb_MC[nPtBins];
    TH1D* h_dphi_pedestal_cent_pp_Data[nPtBins];

    for (int i=0; i<nPtBins; ++i) {
        h_rjg_centBinAll_PbPb_Data[i] = (TH1D*)PbPb_Data_file->Get(Form("HI/h1D_rjg_centBinAll_ptBin%i_phoSIG_jetSIG", i));
        h_rjg_centBinAll_PbPb_MC[i] = (TH1D*)PbPb_MC_file->Get(Form("HIMC/h1D_rjg_centBinAll_ptBin%i_phoSIG_jetSIG", i));
        h_rjg_centBinAll_pp_Data[i] = (TH1D*)pp_Data_file->Get(Form("PP/h1D_rjg_centBinAll_ptBin%i_phoSIG_jetSIG", i));

        h_rjg_centBinAll_PbPb_Data[i]->Write(Form("h1D_rjg_centBinAll_ptBin%i_PbPb_Data", i), TObject::kOverwrite);
        h_rjg_centBinAll_PbPb_MC[i]->Write(Form("h1D_rjg_centBinAll_ptBin%i_PbPb_MC", i), TObject::kOverwrite);
        h_rjg_centBinAll_pp_Data[i]->Write(Form("h1D_rjg_centBinAll_ptBin%i_pp_Data", i), TObject::kOverwrite);

        h_xjg_mean_centBinAll_PbPb_Data[i] = (TH1D*)PbPb_Data_file->Get(Form("HI/h1D_xjg_mean_centBinAll_ptBin%i_phoSIG_jetSIG", i));
        h_xjg_mean_centBinAll_PbPb_MC[i] = (TH1D*)PbPb_MC_file->Get(Form("HIMC/h1D_xjg_mean_centBinAll_ptBin%i_phoSIG_jetSIG", i));
        h_xjg_mean_centBinAll_pp_Data[i] = (TH1D*)pp_Data_file->Get(Form("PP/h1D_xjg_mean_centBinAll_ptBin%i_phoSIG_jetSIG", i));

        h_xjg_mean_centBinAll_PbPb_Data[i]->Write(Form("h1D_xjg_mean_centBinAll_ptBin%i_PbPb_Data", i), TObject::kOverwrite);
        h_xjg_mean_centBinAll_PbPb_MC[i]->Write(Form("h1D_xjg_mean_centBinAll_ptBin%i_PbPb_MC", i), TObject::kOverwrite);
        h_xjg_mean_centBinAll_pp_Data[i]->Write(Form("h1D_xjg_mean_centBinAll_ptBin%i_pp_Data", i), TObject::kOverwrite);

        h_dphi_width_cent_PbPb_Data[i] = (TH1D*)PbPb_Data_file->Get(Form("HI/h1D_dphi_width_centBinAll_ptBin%i", i));
        h_dphi_width_cent_PbPb_MC[i] = (TH1D*)PbPb_MC_file->Get(Form("HIMC/h1D_dphi_width_centBinAll_ptBin%i", i));
        h_dphi_width_cent_pp_Data[i] = (TH1D*)pp_Data_file->Get(Form("PP/h1D_dphi_width_centBinAll_ptBin%i", i));

        h_dphi_width_cent_PbPb_Data[i]->Write(Form("h1D_dphi_width_centBinAll_ptBin%i_PbPb_Data", i), TObject::kOverwrite);
        h_dphi_width_cent_PbPb_MC[i]->Write(Form("h1D_dphi_width_centBinAll_ptBin%i_PbPb_MC", i), TObject::kOverwrite);
        h_dphi_width_cent_pp_Data[i]->Write(Form("h1D_dphi_width_centBinAll_ptBin%i_pp_Data", i), TObject::kOverwrite);

        h_dphi_pedestal_cent_PbPb_Data[i] = (TH1D*)PbPb_Data_file->Get(Form("HI/h1D_dphi_pedestal_centBinAll_ptBin%i", i));
        h_dphi_pedestal_cent_PbPb_MC[i] = (TH1D*)PbPb_MC_file->Get(Form("HIMC/h1D_dphi_pedestal_centBinAll_ptBin%i", i));
        h_dphi_pedestal_cent_pp_Data[i] = (TH1D*)pp_Data_file->Get(Form("PP/h1D_dphi_pedestal_centBinAll_ptBin%i", i));

        h_dphi_pedestal_cent_PbPb_Data[i]->Write(Form("h1D_dphi_pedestal_centBinAll_ptBin%i_PbPb_Data", i), TObject::kOverwrite);
        h_dphi_pedestal_cent_PbPb_MC[i]->Write(Form("h1D_dphi_pedestal_centBinAll_ptBin%i_PbPb_MC", i), TObject::kOverwrite);
        h_dphi_pedestal_cent_pp_Data[i]->Write(Form("h1D_dphi_pedestal_centBinAll_ptBin%i_pp_Data", i), TObject::kOverwrite);
    }

    TH1D* h_rjg_ptBinAll_PbPb_Data[nPtBins];
    TH1D* h_rjg_ptBinAll_PbPb_MC[nPtBins];
    TH1D* h_rjg_ptBinAll_pp_Data[nPtBins];

    TH1D* h_xjg_mean_ptBinAll_PbPb_Data[nPtBins];
    TH1D* h_xjg_mean_ptBinAll_PbPb_MC[nPtBins];
    TH1D* h_xjg_mean_ptBinAll_pp_Data[nPtBins];

    TH1D* h_dphi_width_pt_PbPb_Data[nCentBins];
    TH1D* h_dphi_width_pt_PbPb_MC[nCentBins];
    TH1D* h_dphi_width_pt_pp_Data[nCentBins];

    TH1D* h_dphi_pedestal_pt_PbPb_Data[nCentBins];
    TH1D* h_dphi_pedestal_pt_PbPb_MC[nCentBins];
    TH1D* h_dphi_pedestal_pt_pp_Data[nCentBins];

    for (int i=0; i<nCentBins; ++i) {
        h_rjg_ptBinAll_PbPb_Data[i] = (TH1D*)PbPb_Data_file->Get(Form("HI/h1D_rjg_ptBinAll_hiBin%i_phoSIG_jetSIG", i));
        h_rjg_ptBinAll_PbPb_MC[i] = (TH1D*)PbPb_MC_file->Get(Form("HIMC/h1D_rjg_ptBinAll_hiBin%i_phoSIG_jetSIG", i));
        h_rjg_ptBinAll_pp_Data[i] = (TH1D*)pp_Data_file->Get(Form("PP/h1D_rjg_ptBinAll_hiBin%i_phoSIG_jetSIG", i));

        h_rjg_ptBinAll_PbPb_Data[i]->Write(Form("h1D_rjg_ptBinAll_hiBin%i_PbPb_Data", i), TObject::kOverwrite);
        h_rjg_ptBinAll_PbPb_MC[i]->Write(Form("h1D_rjg_ptBinAll_hiBin%i_PbPb_MC", i), TObject::kOverwrite);
        h_rjg_ptBinAll_pp_Data[i]->Write(Form("h1D_rjg_ptBinAll_hiBin%i_pp_Data", i), TObject::kOverwrite);

        h_xjg_mean_ptBinAll_PbPb_Data[i] = (TH1D*)PbPb_Data_file->Get(Form("HI/h1D_xjg_mean_ptBinAll_hiBin%i_phoSIG_jetSIG", i));
        h_xjg_mean_ptBinAll_PbPb_MC[i] = (TH1D*)PbPb_MC_file->Get(Form("HIMC/h1D_xjg_mean_ptBinAll_hiBin%i_phoSIG_jetSIG", i));
        h_xjg_mean_ptBinAll_pp_Data[i] = (TH1D*)pp_Data_file->Get(Form("PP/h1D_xjg_mean_ptBinAll_hiBin%i_phoSIG_jetSIG", i));

        h_xjg_mean_ptBinAll_PbPb_Data[i]->Write(Form("h1D_xjg_mean_ptBinAll_hiBin%i_PbPb_Data", i), TObject::kOverwrite);
        h_xjg_mean_ptBinAll_PbPb_MC[i]->Write(Form("h1D_xjg_mean_ptBinAll_hiBin%i_PbPb_MC", i), TObject::kOverwrite);
        h_xjg_mean_ptBinAll_pp_Data[i]->Write(Form("h1D_xjg_mean_ptBinAll_hiBin%i_pp_Data", i), TObject::kOverwrite);

        h_dphi_width_pt_PbPb_Data[i] = (TH1D*)PbPb_Data_file->Get(Form("HI/h1D_dphi_width_ptBinAll_hiBin%i", i));
        h_dphi_width_pt_PbPb_MC[i] = (TH1D*)PbPb_MC_file->Get(Form("HIMC/h1D_dphi_width_ptBinAll_hiBin%i", i));
        h_dphi_width_pt_pp_Data[i] = (TH1D*)pp_Data_file->Get(Form("PP/h1D_dphi_width_ptBinAll_hiBin%i", i));

        h_dphi_width_pt_PbPb_Data[i]->Write(Form("h1D_dphi_width_ptBinAll_hiBin%i_PbPb_Data", i), TObject::kOverwrite);
        h_dphi_width_pt_PbPb_MC[i]->Write(Form("h1D_dphi_width_ptBinAll_hiBin%i_PbPb_MC", i), TObject::kOverwrite);
        h_dphi_width_pt_pp_Data[i]->Write(Form("h1D_dphi_width_ptBinAll_hiBin%i_pp_Data", i), TObject::kOverwrite);

        h_dphi_pedestal_pt_PbPb_Data[i] = (TH1D*)PbPb_Data_file->Get(Form("HI/h1D_dphi_pedestal_ptBinAll_hiBin%i", i));
        h_dphi_pedestal_pt_PbPb_MC[i] = (TH1D*)PbPb_MC_file->Get(Form("HIMC/h1D_dphi_pedestal_ptBinAll_hiBin%i", i));
        h_dphi_pedestal_pt_pp_Data[i] = (TH1D*)pp_Data_file->Get(Form("PP/h1D_dphi_pedestal_ptBinAll_hiBin%i", i));

        h_dphi_pedestal_pt_PbPb_Data[i]->Write(Form("h1D_dphi_pedestal_ptBinAll_hiBin%i_PbPb_Data", i), TObject::kOverwrite);
        h_dphi_pedestal_pt_PbPb_MC[i]->Write(Form("h1D_dphi_pedestal_ptBinAll_hiBin%i_PbPb_MC", i), TObject::kOverwrite);
        h_dphi_pedestal_pt_pp_Data[i]->Write(Form("h1D_dphi_pedestal_ptBinAll_hiBin%i_pp_Data", i), TObject::kOverwrite);
    }

    PbPb_Data_file->Close();
    PbPb_MC_file->Close();
    pp_Data_file->Close();

    output_file->Write("", TObject::kOverwrite);
    output_file->Close();

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 6)
        return gammaJetFinalHistograms(argv[1], argv[2], argv[3], argv[4], argv[5]);
    else
        printf("Usage: ./gammaJetFinalHistograms.exe <configFile> <PbPb_Data_file> <PbPb_MC_file> <pp_Data_file> <outputFile>\n");

    return 1;
}