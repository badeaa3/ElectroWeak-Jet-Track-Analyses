#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.SysVar.sh
####################
echo "### HISTOGRAMSUM STEP"
echo "## zJetHistogramSum PP MC"
####################
configFile=$config_histSum_PP_MC_Madgraph

inputHistDir="/home/kaya/Desktop/EWJTA-out/zJet20160715/"
inputHistPrefixZEE=$inputHistDir$hist_PP_ZEE_MC_Madgraph_Prefix
inputHistPrefixZMM=$inputHistDir$hist_PP_ZMM_MC_Madgraph_Prefix
inputZEE=$inputHistPrefixZEE".root"
inputZMM=$inputHistPrefixZMM".root"

outputPrefix=$outputHistSumDir$histSum_PP_MC_Madgraph_Prefix
output=$outputPrefix".root"
outputLOG=$outputPrefix".log"

echo "./Histogramming/zBosons/zJetHistogramSum.exe $configFile $inputZEE $inputZMM $output &> $outputLOG &"
./Histogramming/zBosons/zJetHistogramSum.exe $configFile $inputZEE $inputZMM $output &> $outputLOG &

####################
configSuffixListZEE=("${configSuffixList_histSum_PP_ZEE_MC[@]}")

configSuffixListZMM=("${configSuffixList_histSum_PP_ZMM_MC[@]}")

DO_SYS_ZEE=$DO_SYSVAR_PP_ZEE_MC_Madgraph
DO_SYS_ZMM=$DO_SYSVAR_PP_ZMM_MC_Madgraph
if [[ $DO_SYS_ZEE -eq 1 ]] && [[ $DO_SYS_ZMM -eq 1 ]]; then 
  echo "################# SYSTEMATICS #######################"
  echo "### HISTOGRAM STEP"
  echo "## zJetHistogramSum PP MC"

  runZJetHistogramSum $configFile $outputPrefix $inputHistPrefixZEE $inputHistPrefixZMM $configSuffixListZEE $configSuffixListZMM
  echo "################# SYSTEMATICS - END #######################"
fi

