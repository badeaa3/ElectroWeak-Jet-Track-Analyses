#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/zJet/zJet.bashUtils.sh
###################### 04.07.2016 ####################
echo "### SKIM STEP"
echo "## zJetSkim HI DATA ZMM"
####################
configFile=$config_HI_ZMM_DATA
inputSkim=$HI_ZMM_DATA
outputSkimPrefix=$skim_HI_ZMM_DATA_Prefix

outputSkim=$outputSkimDir$outputSkimPrefix".root"
outputSkimLOG=$outputSkimDirLOG$outputSkimPrefix".log"
echo "inputSkim     : "$inputSkim; echo "outputSkim    : "$outputSkim;echo "outputSkimLOG : "$outputSkimLOG
echo "~/code/scripts/myRun.sh ForestSkimmers/Zbosons/zJetSkim.exe $configFile $inputSkim $outputSkim $inputMinBiasJetSkim &> $outputSkimLOG &"
~/code/scripts/myRun.sh ForestSkimmers/Zbosons/zJetSkim.exe $configFile $inputSkim $outputSkim $inputMinBiasJetSkim &> $outputSkimLOG &

####################
DO_SYSVAR=$DO_SYSVAR_HI_ZMM_DATA
if [ $DO_SYSVAR -eq 1 ]; then 
  echo "################# SYSTEMATICS #######################"
  echo "### SKIM STEP"
  echo "## zJetSkim HI DATA ZMM"

  configSuffixList=(
"SYS_jes106"
"SYS_jes096"
"SYS_noCorrJetRes"
"SYS_noL2L3"
"SYS_smearResJetPlus"
"SYS_smearResJetMinus"
  );

  cutList=(
"skim.jet.energyScale = 1.06"
"skim.jet.energyScale = 0.96"
"skim.jet.doCorrectionResidual = 0"
"skim.jet.doCorrectionL2L3 = 0"
"skim.jet.smearingRes = 1.15"
"skim.jet.smearingRes = 0.85"
  );

  runZJetSkim $configFile $inputSkim $outputSkimPrefix $configSuffixList $cutList $inputMinBiasJetSkim
  echo "################# SYSTEMATICS - END #######################"
fi
####################
