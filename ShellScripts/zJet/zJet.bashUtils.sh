#!/bin/bash

source ./ShellScripts/bashUtils.sh
## global variables
IFS=$'\n' # very important. force to use only '\n' to split the elements of the array
productionVersion="20160704"
outputSkimDir="/export/d00/scratch/tatar/EWJTA-out/"
outputSkimDirLOG="./"
outputHistDir="./"
outputHistDirLOG="./"
inputMinBiasJetSkim="/mnt/hadoop/cms/store/user/katatar/skims/minBiasJetSkim_HIMinimumBias2_HIRun2015_PromptReco_v1_Run263233_263284.root"
inputMinBiasJetSkimHydjet="/export/d00/scratch/tatar/EWJTA-out/minBiasJetSkim_Hydjet_Quenched_MinBias_5020GeV_750_HINPbPbWinter16DR.root"
# path to configuration files
config_HI_ZEE_DATA="Configurations/zJet/zJet.conf"
config_HI_ZMM_DATA="Configurations/zJet/zJet.diMuon.conf"
config_PP_ZEE_DATA="Configurations/zJet/zJet.PP.conf"
config_PP_ZMM_DATA="Configurations/zJet/zJet.PP.diMuon.conf"
config_HI_ZEE_MC="Configurations/zJet/zJet.HIMC.conf"
config_HI_ZMM_MC="Configurations/zJet/zJet.HIMC.diMuon.conf"
config_PP_ZEE_MC="Configurations/zJet/zJet.PPMC.conf"
config_PP_ZMM_MC="Configurations/zJet/zJet.PPMC.diMuon.conf"

config_HI_ZEE_MC_Pythia=$config_HI_ZEE_MC
config_HI_ZMM_MC_Pythia=$config_HI_ZMM_MC
config_PP_ZEE_MC_Pythia=$config_PP_ZEE_MC
config_PP_ZMM_MC_Pythia=$config_PP_ZMM_MC

config_HI_ZEE_MC_Pyquen=$config_HI_ZEE_MC
config_HI_ZMM_MC_Pyquen=$config_HI_ZMM_MC

config_PP_ZEE_MC_Madgraph=$config_PP_ZEE_MC
config_PP_ZMM_MC_Madgraph=$config_PP_ZMM_MC
# flags for running systematic variation
DO_SYSVAR_HI_ZEE_DATA=0
DO_SYSVAR_HI_ZMM_DATA=0
DO_SYSVAR_PP_ZEE_DATA=0
DO_SYSVAR_PP_ZMM_DATA=0
DO_SYSVAR_HI_ZEE_MC_Pythia=0
DO_SYSVAR_HI_ZMM_MC_Pythia=0
DO_SYSVAR_PP_ZEE_MC_Pythia=1
DO_SYSVAR_PP_ZMM_MC_Pythia=1
DO_SYSVAR_HI_ZEE_MC_Pyquen=1
DO_SYSVAR_HI_ZMM_MC_Pyquen=1
DO_SYSVAR_PP_ZEE_MC_Madgraph=0
DO_SYSVAR_PP_ZEE_MC_Madgraph=0
### SAMPLES - HIFOREST
## DATA
HI_ZEE_DATA="/mnt/hadoop/cms/store/user/rbi/azsigmon-HIRun2015E-PromptReco-AOD-DielectronSkim-ElePt8-v3_forest_csjet_v1_3/0.root"
HI_ZMM_DATA="/mnt/hadoop/cms/store/user/rbi/merged/azsigmon-HIRun2015E-PromptReco-AOD-DimuonSkim-Mass40-v3_forest_csjet_v1_2/0.root"
PP_ZEE_DATA="/mnt/hadoop/cms/store/user/tatar/HighPtPhoton30AndZ/HIRun2015E-PromptReco-AOD-dielectron-skim-FOREST/0.root"
PP_ZMM_DATA="/mnt/hadoop/cms/store/user/tatar/SingleMuHighPt/HIRun2015E-PromptReco-AOD-dimuon-skim-FOREST/0.root"
## MC - Pythia8
HI_ZEE_MC_Pythia="/mnt/hadoop/cms/store/user/tatar/official/Pythia8_Z30eeJet_Hydjet_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13_ext1-FOREST/0.root"
HI_ZMM_MC_Pythia="/mnt/hadoop/cms/store/user/tatar/official/Pythia8_Z30mumuJet_Hydjet_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13_ext1-FOREST/0.root"
PP_ZEE_MC_Pythia="/mnt/hadoop/cms/store/user/tatar/official/Pythia8_Z30eeJet/HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3_ext1-v1-FOREST/0.root"
PP_ZMM_MC_Pythia="/mnt/hadoop/cms/store/user/tatar/official/Pythia8_Z30mumuJet/HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3_ext1-v1-FOREST/0.root"
## MC - PyquenFW
HI_ZEE_MC_Pyquen="/mnt/hadoop/cms/store/user/tatar/official/PyquenFW_Z30eeJet_Hydjet_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1-FOREST/0.root"
HI_ZMM_MC_Pyquen="/mnt/hadoop/cms/store/user/tatar/official/PyquenFW_Z30mumuJet_Hydjet_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1-FOREST/0.root"
## MC - Madgraph (same sample for ZEE and ZMM)
PP_MC_Madgraph="/mnt/hadoop/cms/store/user/tatar/official/DYJetsToLL_TuneCUETP8M1_5020GeV-amcatnloFXFX-pythia8/HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3-v1-FOREST/0.root"
PP_ZEE_MC_Madgraph=$PP_MC_Madgraph
PP_ZMM_MC_Madgraph=$PP_MC_Madgraph
## old/obsolete Samples
HI_ZEE_MC_Pythia_OLD="/mnt/hadoop/cms/store/user/tatar/official/Pythia8_Z30eeJet_Hydjet_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1-FOREST/0.root"
HI_ZMM_MC_Pythia_OLD="/mnt/hadoop/cms/store/user/tatar/official/Pythia8_Z30mumuJet_Hydjet_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1-FOREST/0.root"
PP_ZEE_MC_Pythia_OLD="/mnt/hadoop/cms/store/user/rbi/merged/Pythia8_Z30eeJet-HINppWinter16DR-75X_mcRun2_forest_v1/0.root"
PP_ZMM_MC_Pythia_OLD="/mnt/hadoop/cms/store/user/rbi/merged/Pythia8_Z30mumuJet-HINppWinter16DR-75X_mcRun2_forest_v1/0.root"
PP_ZEE_MC_Madgraph_OLD="/mnt/hadoop/cms/store/user/rbi/merged/DYJetsToLL_TuneCUETP8M1_5020GeV-amcatnloFXFX-pythia8_HINppWinter16DR-75X_mcRun2_forest_v1/0.root"
PP_ZMM_MC_Madgraph_OLD="/mnt/hadoop/cms/store/user/rbi/merged/DYJetsToLL_TuneCUETP8M1_5020GeV-amcatnloFXFX-pythia8_HINppWinter16DR-75X_mcRun2_forest_v1/0.root"
### substrings used in output file names
substr_HI_ZEE_DATA="HIRun2015E_PromptReco_AOD_HI_Zee"
substr_HI_ZMM_DATA="HIRun2015E_PromptReco_AOD_HI_Zmm"
substr_PP_ZEE_DATA="HIRun2015E_PromptReco_AOD_PP_Zee"
substr_PP_ZMM_DATA="HIRun2015E_PromptReco_AOD_PP_Zmm"

substr_HI_ZEE_MC_Pythia="Pythia8_Z30eeJet_Hydjet_MB_HI_Zee"
substr_HI_ZMM_MC_Pythia="Pythia8_Z30mumuJet_Hydjet_MB_HI_Zmm"
substr_PP_ZEE_MC_Pythia="Pythia8_Z30eeJet_PP_Zee"
substr_PP_ZMM_MC_Pythia="Pythia8_Z30mumuJet_PP_Zmm"

substr_HI_ZEE_MC_Pyquen="PyquenFW_Z30eeJet_Hydjet_MB_HI_Zee"
substr_HI_ZMM_MC_Pyquen="PyquenFW_Z30mumuJet_Hydjet_MB_HI_Zmm"

substr_PP_ZEE_MC_Madgraph="DYJetsToLL_TuneCUETP8M1_5020GeV_PP_Zee"
substr_PP_ZMM_MC_Madgraph="DYJetsToLL_TuneCUETP8M1_5020GeV_PP_Zmm"
### prefix for skim file names
skim_HI_ZEE_DATA_Prefix="zJetSkim_"$substr_HI_ZEE_DATA"_"$productionVersion
skim_HI_ZMM_DATA_Prefix="zJetSkim_"$substr_HI_ZMM_DATA"_"$productionVersion
skim_PP_ZEE_DATA_Prefix="zJetSkim_"$substr_PP_ZEE_DATA"_"$productionVersion
skim_PP_ZMM_DATA_Prefix="zJetSkim_"$substr_PP_ZMM_DATA"_"$productionVersion

skim_HI_ZEE_MC_Pythia_Prefix="zJetSkim_"$substr_HI_ZEE_MC_Pythia"_"$productionVersion
skim_HI_ZMM_MC_Pythia_Prefix="zJetSkim_"$substr_HI_ZMM_MC_Pythia"_"$productionVersion
skim_PP_ZEE_MC_Pythia_Prefix="zJetSkim_"$substr_PP_ZEE_MC_Pythia"_"$productionVersion
skim_PP_ZMM_MC_Pythia_Prefix="zJetSkim_"$substr_PP_ZMM_MC_Pythia"_"$productionVersion

skim_HI_ZEE_MC_Pyquen_Prefix="zJetSkim_"$substr_HI_ZEE_MC_Pyquen"_"$productionVersion
skim_HI_ZMM_MC_Pyquen_Prefix="zJetSkim_"$substr_HI_ZMM_MC_Pyquen"_"$productionVersion

skim_PP_ZEE_MC_Madgraph_Prefix="zJetSkim_"$substr_PP_ZEE_MC_Madgraph"_"$productionVersion
skim_PP_ZMM_MC_Madgraph_Prefix="zJetSkim_"$substr_PP_ZMM_MC_Madgraph"_"$productionVersion
# prefix for histogram file names
hist_HI_ZEE_DATA_Prefix="zJetHistogram_"$substr_HI_ZEE_DATA"_"$productionVersion
hist_HI_ZMM_DATA_Prefix="zJetHistogram_"$substr_HI_ZMM_DATA"_"$productionVersion
hist_PP_ZEE_DATA_Prefix="zJetHistogram_"$substr_PP_ZEE_DATA"_"$productionVersion
hist_PP_ZMM_DATA_Prefix="zJetHistogram_"$substr_PP_ZMM_DATA"_"$productionVersion

hist_HI_ZEE_MC_Pythia_Prefix="zJetHistogram_"$substr_HI_ZEE_MC_Pythia"_"$productionVersion
hist_HI_ZMM_MC_Pythia_Prefix="zJetHistogram_"$substr_HI_ZMM_MC_Pythia"_"$productionVersion
hist_PP_ZEE_MC_Pythia_Prefix="zJetHistogram_"$substr_PP_ZEE_MC_Pythia"_"$productionVersion
hist_PP_ZMM_MC_Pythia_Prefix="zJetHistogram_"$substr_PP_ZMM_MC_Pythia"_"$productionVersion

hist_HI_ZEE_MC_Pyquen_Prefix="zJetHistogram_"$substr_HI_ZEE_MC_Pyquen"_"$productionVersion
hist_HI_ZMM_MC_Pyquen_Prefix="zJetHistogram_"$substr_HI_ZMM_MC_Pyquen"_"$productionVersion

hist_PP_ZEE_MC_Madgraph_Prefix="zJetHistogram_"$substr_PP_ZEE_MC_Madgraph"_"$productionVersion
hist_PP_ZMM_MC_Madgraph_Prefix="zJetHistogram_"$substr_PP_ZMM_MC_Madgraph"_"$productionVersion
#### SAMPLES - AOD
## DATA
DAS_HI_ZEE_DATA="/HIPhoton40AndZ/azsigmon-HIRun2015E-PromptReco-AOD-DielectronSkim-ElePt8-v3-f8ca4a1d94c645c1a352f0d31009e079/USER"
DAS_HI_ZMM_DATA="/HIEWQExo/azsigmon-HIRun2015E-PromptReco-AOD-DimuonSkim-Mass40-v3-4f10cc0edb1df2aaccd97a62c3f32713/USER"
DAS_PP_ZEE_DATA="/HighPtPhoton30AndZ/azsigmon-Run2015E-PromptReco-AOD-DielectronSkim-v2-0d8b27ea5ddb998af72cec10f8b38257/USER"
DAS_PP_ZMM_DATA="/SingleMuHighPt/azsigmon-Run2015E-PromptReco-AOD-DimuonSkim-Mass40-v2-4f10cc0edb1df2aaccd97a62c3f32713/USER"
## MC
# https://twiki.cern.ch/twiki/bin/view/CMS/PbPb5TeVOfficialMC
# https://twiki.cern.ch/twiki/bin/view/CMS/PP5TeVOfficialMC
## MC - Pythia8
DAS_HI_ZEE_MC_Pythia="/Pythia8_Z30eeJet_Hydjet_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13_ext1-v1/AODSIM"
DAS_HI_ZMM_MC_Pythia="/Pythia8_Z30mumuJet_Hydjet_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13_ext1-v1/AODSIM"
DAS_PP_ZEE_MC_Pythia="/Pythia8_Z30eeJet/HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3_ext1-v1/AODSIM"
DAS_PP_ZMM_MC_Pythia="/Pythia8_Z30mumuJet/HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3_ext1-v1/AODSIM"
## MC - PyquenFW
DAS_HI_ZEE_MC_Pyquen="/PyquenFW_Z30eeJet_Hydjet_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1/AODSIM"
DAS_HI_ZMM_MC_Pyquen="/PyquenFW_Z30mumuJet_Hydjet_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1/AODSIM"
## MC - Madgraph (same sample for ZEE and ZMM)
DAS_PP_MC_Madgraph="/DYJetsToLL_TuneCUETP8M1_5020GeV-amcatnloFXFX-pythia8/HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3-v1/AODSIM"
DAS_PP_ZEE_MC_Madgraph=$DAS_PP_MC_Madgraph
DAS_PP_ZMM_MC_Madgraph=$DAS_PP_MC_Madgraph
## global variables - END

# take a list of configurations and run "zJetSkim" for each of them.
function runZJetSkim() {
    
    echo "runZJetSkim()"

    progPath="./ForestSkimmers/Zbosons/zJetSkim.exe"
    importedConfig=$1
    input=$2
    outputPrefix=$3
    configSuffixList=$4
    cutList=$5
    inputMBtmp=$6

#    LEN1=${#cutList[@]}
#    for i1 in {1..$LEN}
#    for (( i1=0; i1<$LEN1; i1++ ));		 
    arrayIndices=${!cutList[*]}
    for i1 in $arrayIndices	 
    do
        configsInput="##"   # do not enter empty string

        output=$outputSkimDir$outputPrefix"_"${configSuffixList[i1]}".root"
        outputLOG=$outputSkimDirLOG$outputPrefix"_"${configSuffixList[i1]}".log"

        echo "running ${cutList[$i1]} with suffix ${configSuffixList[$i1]}"
        echo "input     : "$input; echo "output    : "$output; echo "outputLOG : "$outputLOG

        # run the command
        if (( ${#inputMBtmp} > 0 )); then
          # minBiasJetSkim file is provided
          runGeneratedConf2 $progPath $importedConfig ${cutList[$i1]} $configsInput $input $output $inputMBtmp $outputLOG
        else
          # minBiasJetSkim file is NOT provided
          runGeneratedConf  $progPath $importedConfig ${cutList[$i1]} $configsInput $input $output $outputLOG
        fi
    done

    echo "runZJetSkim() - END"
}

# take a list of configurations and run "zJetHistogram" for each of them.
function runZJetHistogram() {

    echo "runZJetHistogram()"

    progPath="./Histogramming/zBosons/zJetHistogram.exe"
    importedConfig=$1
    skimPrefix=$2
    configSuffixList=$3
    cutList=$4
    configSuffixListSkim=$5     # suffix for skim files, zJetHistogram and zJetSkim file do not necessarily have the same suffix. 

#    for i1 in {1..$LEN}
#    LEN1=${#cutList[@]}
#    for (( i1=0; i1<$LEN1; i1++ ));		 
    arrayIndices=${!cutList[*]}
    for i1 in $arrayIndices
    do    
        configsInput="##"   # do not enter empty string

        skimFile=$outputSkimDir$skimPrefix".root"
        if (( ${#configSuffixListSkim[$i1]} > 0 )); then
          skimFile=$outputSkimDir$skimPrefix"_"${configSuffixListSkim[$i1]}".root"
        fi
        input=$skimFile
        outputPrefix="${skimPrefix/zJetSkim/zJetHistogram}"
        output=$outputHistDir$outputPrefix"_"${configSuffixList[i1]}".root"
        outputLOG=$outputHistDirLOG$outputPrefix"_"${configSuffixList[i1]}".log"

        echo "running ${cutList[$i1]} with suffix ${configSuffixList[$i1]}"
        echo "input     : "$input; echo "output    : "$output; echo "outputLOG : "$outputLOG

        # run the command    
        runGeneratedConf $progPath $importedConfig ${cutList[$i1]} $configsInput $input $output $outputLOG
    done

    echo "runZJetHistogram() - END"
}
