#!/bin/bash

make
#g++ -Wall -Werror -Wextra -O2 `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/photons/photonRecoAna.d Performance/photons/photonRecoAna.C -o Performance/photons/photonRecoAna.exe
#echo "g++ -Wall -Werror -Wextra -O2 `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/photons/photonRecoAna.d Performance/photons/photonRecoAna.C -o Performance/photons/photonRecoAna.exe"

runCmd="time -p"
runCmdStr="time -p"
if [[ $USER == "tatar" ]]; then
  runCmd="$HOME/code/scripts/myRun.sh"
  runCmdStr="myRun"
fi

$runCmd Performance/photons/photonRecoAna.exe Configurations/Performance/photons/photonRecoAna.eScale.AllQCD.conf /mnt/hadoop/cms/store/user/tatar/official/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/0.root /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_AllQCDPhoton30_Hydjet_Cymbal_MB.root &> /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_AllQCDPhoton30_Hydjet_Cymbal_MB.log &
echo "$runCmdStr Performance/photons/photonRecoAna.exe Configurations/Performance/photons/photonRecoAna.eScale.AllQCD.conf /mnt/hadoop/cms/store/user/tatar/official/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/0.root /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_AllQCDPhoton30_Hydjet_Cymbal_MB.root &> /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_AllQCDPhoton30_Hydjet_Cymbal_MB.log &"

$runCmd Performance/photons/photonRecoAna.exe Configurations/Performance/photons/photonRecoAna.eScale.AllQCD.GED.conf /mnt/hadoop/cms/store/user/tatar/official/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/0.root /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_GED_AllQCDPhoton30_Hydjet_Cymbal_MB.root &> /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_GED_AllQCDPhoton30_Hydjet_Cymbal_MB.log &
echo "$runCmdStr Performance/photons/photonRecoAna.exe Configurations/Performance/photons/photonRecoAna.eScale.AllQCD.GED.conf /mnt/hadoop/cms/store/user/tatar/official/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/0.root /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_GED_AllQCDPhoton30_Hydjet_Cymbal_MB.root &> /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_GED_AllQCDPhoton30_Hydjet_Cymbal_MB.log &"

## SIG
$runCmd Performance/photons/photonRecoAna.exe Configurations/Performance/photons/photonRecoAna.eScale.AllQCD.SIG.conf /mnt/hadoop/cms/store/user/tatar/official/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/0.root /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_SIG_AllQCDPhoton30_Hydjet_Cymbal_MB.root &> /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_SIG_AllQCDPhoton30_Hydjet_Cymbal_MB.log &
echo "$runCmdStr Performance/photons/photonRecoAna.exe Configurations/Performance/photons/photonRecoAna.exe Configurations/Performance/photons/photonRecoAna.eScale.AllQCD.SIG.conf /mnt/hadoop/cms/store/user/tatar/official/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/0.root /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_SIG_AllQCDPhoton30_Hydjet_Cymbal_MB.root &> /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_SIG_AllQCDPhoton30_Hydjet_Cymbal_MB.log &"

$runCmd Performance/photons/photonRecoAna.exe Configurations/Performance/photons/photonRecoAna.eScale.AllQCD.SIG.GED.conf /mnt/hadoop/cms/store/user/tatar/official/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/0.root /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_SIG_GED_AllQCDPhoton30_Hydjet_Cymbal_MB.root &> /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_SIG_GED_AllQCDPhoton30_Hydjet_Cymbal_MB.log &
echo "$runCmdStr Performance/photons/photonRecoAna.exe Configurations/Performance/photons/photonRecoAna.exe Configurations/Performance/photons/photonRecoAna.eScale.AllQCD.SIG.GED.conf /mnt/hadoop/cms/store/user/tatar/official/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/0.root /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_SIG_GED_AllQCDPhoton30_Hydjet_Cymbal_MB.root &> /export/d00/scratch/tatar/EWJTA-out/Performance/photons/photonRecoAna_eScale_SIG_GED_AllQCDPhoton30_Hydjet_Cymbal_MB.log &"

