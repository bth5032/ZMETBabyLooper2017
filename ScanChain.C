#include "ScanChain.h"

//=============================
// Variable Computation
//=============================

pair<int, int> getMostBlike(){
  /* returns two most B-like jet indicies */
  int first_index = 0;
  int second_index = 1;

  if (g_jets_csv.at(first_index) < g_jets_csv.at(second_index)){
    first_index = 1;
    second_index = 0;
  }

  for (int i = 2; i < (int) g_jets_p4.size(); i++){
    if (g_jets_csv.at(first_index) < g_jets_csv.at(i)){
      second_index = first_index;
      first_index = i;
    }
    else if (g_jets_csv.at(second_index) < g_jets_csv.at(i)){
      second_index = i;
    }
  }

  return make_pair(first_index, second_index);
}

pair<int,int> getClosestBPairToHiggsMass(){
  int first = 0;
  int second = 1;
  
  if (g_jets_p4.size()<2){
    cout<<"Going to throw error finding closest B pair: Less than two jets!"<<endl;
    std::stringstream message;
    message<<"Can not find closest B pair to Higgs for event: "<<phys.evt()<<" run: "<<phys.run()<<" lumi: "<<phys.lumi()<<" the event only has "<<g_jets_p4.size()<<" jets";
    throw std::underflow_error(message.str());
  }

  double dist = abs((g_jets_p4.at(0)+g_jets_p4.at(1)).M() - 125.0);

  for (int i = 0; i < (int) g_jets_p4.size(); i++) {
    for (int j = i+1; j < (int) g_jets_p4.size(); j++) {
      if (abs((g_jets_p4.at(i)+g_jets_p4.at(j)).M() - 125 ) < dist){
        first = i;
        second = j;
      }
    }
  }

  return make_pair(first,second);
}

double getMT2B(){
  /*Builds MT2b from two highest CSV jets*/

  pair<int, int> b_index = getMostBlike();

  double mt2_1=MT2(g_met, g_met_phi, g_jets_p4.at(b_index.first)+phys.lep_p4().at(0), g_jets_p4.at(b_index.second)+phys.lep_p4().at(1), 0, 0);
  double mt2_2=MT2(g_met, g_met_phi, g_jets_p4.at(b_index.first)+phys.lep_p4().at(1), g_jets_p4.at(b_index.second)+phys.lep_p4().at(0), 0, 0);

  if (mt2_1 > mt2_2){
    return mt2_2;
  }
  else{
    return mt2_1;
  }
}

double getMbb(){
  /*Builds Mbb from two highest CSV jets*/
  
  pair<int, int> b_index = getMostBlike();

  return (g_jets_p4.at(b_index.first)+g_jets_p4.at(b_index.second)).M();
}

double getMT2ForBjets(bool select_highest_csv/*=false*/){
  /*This function gets the MT2 built out of the two Bjets in an event, no guarentee is made about selecting the highest csv jets*/
  double mt2;
  //cout<<__LINE__<<endl;
  if (select_highest_csv){
    pair<int, int> b_index = getMostBlike();
    //cout<<__LINE__<<endl;
    //make sure first index points to the higher csv of the first two jets
    mt2=MT2(g_met, g_met_phi, g_jets_p4.at(b_index.first), g_jets_p4.at(b_index.second), 0, 0);
    //cout<<__LINE__<<endl;
  }
  else{
    // MT2( MET_MAGNITUDE, MET_PHI, P4_LEPTON_1, P4_LEPTON_2, MASS_INVISIBLE_PARTICLE, Bool Verbose)
    //cout<<__LINE__<<endl;
    mt2=MT2(g_met, g_met_phi, g_jets_medb_p4.at(0), g_jets_medb_p4.at(1), 0, 0);
  }
  //cout<<__LINE__<<endl;
  return mt2;
}

double getMT2HiggsZ(bool select_highest_closest_higgs_mass/*=false*/){

  double mt2; 

  if(select_highest_closest_higgs_mass){
    pair<int, int> jet_indexes = getClosestBPairToHiggsMass();
    // MT2( MET_MAGNITUDE, MET_PHI, P4_LEPTON_1, P4_LEPTON_2, MASS_INVISIBLE_PARTICLE, Bool Verbose)
    mt2=MT2(g_met, g_met_phi, g_jets_p4.at(jet_indexes.first)+g_jets_p4.at(jet_indexes.second), phys.lep_p4().at(0)+phys.lep_p4().at(1), 0, 0);
  }
  else{
    // MT2( MET_MAGNITUDE, MET_PHI, P4_LEPTON_1, P4_LEPTON_2, MASS_INVISIBLE_PARTICLE, Bool Verbose)
    mt2=MT2(g_met, g_met_phi, g_jets_medb_p4.at(0)+g_jets_medb_p4.at(1), phys.lep_p4().at(0)+phys.lep_p4().at(1), 0, 0);
  }

  return mt2;
}

double bosonPt(){
  // Returns boson Pt, determines whether sample is gjets or zjets first.
  if (conf->get("event_type") == "dilepton") {
    return phys.dilpt();
  }
  else{
    if (phys.evt_type() == 2 && phys.ngamma() > 0){
      return phys.gamma_pt().at(0);
    }
    else
      return 0;
  }
}

double getMTLepMET(short id/*=0*/){
  /* Builds the MT from the lepton at index id and the MET vector (assumes massless particles)*/
  return sqrt(g_met*phys.lep_p4().at(id).pt()*(1 - cos(g_met_phi - phys.lep_p4().at(id).phi())));

  /* Massive Case
    ET1sq = m_1^2 + pt1^2 
    ET2sq = m_2^2 + pt2^2
    MT^2 = m_1^2  + m_2^2 + 2(sqrt(ET1sq*ET2sq) - pt1*pt2*cos(phi1 - phi2)) 
  */
}

double getdRGammaLep(short id/*=0*/){
  /* Builds the delta R (sqrt(dPhi^2 + dEta^2)) between the lepton at index id and the leading photon*/
  double dPhi=acos(cos(phys.gamma_p4().at(0).phi() - phys.lep_p4().at(id).phi()));
  double dEta=phys.gamma_p4().at(0).eta() - phys.lep_p4().at(id).eta();

  return sqrt(pow(dPhi, 2) + pow(dEta, 2));
}

//=============================
// Triggers
//=============================

bool passPhotonEmulatedTrigger() {
  if( phys.gamma_r9()            .at(0) < 0.92 ) return false;
  if( phys.gamma_hOverE()        .at(0) > 0.2  ) return false;
  if( phys.gamma_hollowtkiso03() .at(0) > 5    ) return false;
  if( abs(phys.gamma_eta().at(0)) < 1.4 && phys.gamma_ecpfclusiso().at(0) > 4 + phys.gamma_pt().at(0) * 0.0053  ) return false;
  if( abs(phys.gamma_eta().at(0)) < 1.4 && phys.gamma_hcpfclusiso().at(0) > 8 + phys.gamma_pt().at(0) * 0.014   ) return false;
  if( abs(phys.gamma_eta().at(0)) > 1.6 && phys.gamma_ecpfclusiso().at(0) > 4 + phys.gamma_pt().at(0) * 0.0034  ) return false;
  if( abs(phys.gamma_eta().at(0)) > 1.6 && phys.gamma_hcpfclusiso().at(0) > 8 + phys.gamma_pt().at(0) * 0.0139  ) return false;

  return true;
}

bool passPhotonTriggers(){
  if ( (! MCTriggerEmulation) && (! phys.isData()) ){
    return true;
  }
  else{
    if( ((phys.HLT_Photon165_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon165_R9Id90_HE10_IsoM() > 0) || (phys.HLT_Photon165_HE10_matchedtophoton() && phys.HLT_Photon165_HE10() > 0)) && phys.gamma_pt().at(0) > 180 ) return true;
    else if( phys.HLT_Photon120_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon120_R9Id90_HE10_IsoM() > 0 && phys.gamma_pt().at(0) > 135 && phys.gamma_pt().at(0) < 180 ) return true;
    else if( phys.HLT_Photon90_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon90_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 105 && phys.gamma_pt().at(0) < 135  ) return true;
    else if( phys.HLT_Photon75_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon75_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 85 && phys.gamma_pt().at(0) < 105   ) return true;
    else if( phys.HLT_Photon50_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon50_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 55 && phys.gamma_pt().at(0) < 85    ) return true;
    else if( phys.HLT_Photon36_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon36_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 40 && phys.gamma_pt().at(0) < 55    ) return true;
    else if( phys.HLT_Photon30_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon30_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 33 && phys.gamma_pt().at(0) < 40    ) return true;
    else if( phys.HLT_Photon22_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon22_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) < 33 ) return true;
    return false;
  }
}

bool passMuonTriggers(){
  if ( (! MCTriggerEmulation) && (! phys.isData()) ){
    return true;
  }
  else{
    //cout<<__LINE__<<endl;
    if ( conf->get("use_muon_DZ_triggers") == "true" ){
      //cout<<"Using DZ triggers"<<endl;
      //cout<<__LINE__<<endl;
      //if (printStats) { cout<<"HLT_DoubleMu: "<<phys.HLT_DoubleMu()<<" HLT_DoubleMu_tk: "<<phys.HLT_DoubleMu_tk()<<" "<<" HLT_DoubleMu_noiso: "<<phys.HLT_DoubleMu_noiso()<<" "; }
      return (phys.HLT_DoubleMu() || phys.HLT_DoubleMu_tk() || phys.HLT_DoubleMu_noiso());
    }
    else{
      //cout<<"Using Non DZ triggers"<<endl;
      //cout<<__LINE__<<endl;
      //if (printStats) { cout<<"HLT_DoubleMu_nonDZ: "<<phys.HLT_DoubleMu_nonDZ()<<" HLT_DoubleMu_tk_nonDZ: "<<phys.HLT_DoubleMu_tk_nonDZ()<<" "<<" HLT_DoubleMu_noiso: "<<phys.HLT_DoubleMu_noiso()<<" "; }
      if(conf->get("signal_region") == "LeonoraEvtLists"){
        return (phys.HLT_DoubleMu() || phys.HLT_DoubleMu_tk() /*|| phys.HLT_DoubleMu_dbltk()*/ || phys.HLT_DoubleMu_nonDZ() || phys.HLT_DoubleMu_tk_nonDZ() || phys.HLT_DoubleMu_noiso());
      }
      else{
        return (phys.HLT_DoubleMu() || phys.HLT_DoubleMu_tk() /*|| phys.HLT_DoubleMu_dbltk()*/ || phys.HLT_DoubleMu_nonDZ() || phys.HLT_DoubleMu_tk_nonDZ() || phys.HLT_DoubleMu_noiso());
      }
    } 
  }
}

bool passElectronTriggers(){
  if ( (! MCTriggerEmulation) && (! phys.isData()) ){
    return true;
  }
  else{
    //cout<<__LINE__<<endl;
    //if (printStats) { cout<<"HLT_DoubleEl_DZ_2: "<<phys.HLT_DoubleEl_DZ_2()<<" HLT_DoubleEl_noiso: "<<phys.HLT_DoubleEl_noiso()<<" HLT_DoubleEl_DZ(): "<<phys.HLT_DoubleEl_DZ()<<endl; }
    return (phys.HLT_DoubleEl_DZ_2() || phys.HLT_DoubleEl_noiso() || phys.HLT_DoubleEl_DZ() );
  }
}

bool passEMuTriggers(){
  if ( (! MCTriggerEmulation) && (! phys.isData()) ){
    return true;
  }
  else{
    return (phys.HLT_MuEG() || phys.HLT_MuEG_2() || phys.HLT_MuEG_noiso() || phys.HLT_MuEG_noiso_2() || phys.HLT_Mu8_EG23_DZ() || phys.HLT_Mu23_EG12_DZ() || phys.HLT_Mu23_EG8_DZ()); //updated for Moriond 2017
  }
}

bool passSingleMuTriggers(){
  if ( (! MCTriggerEmulation) && (! phys.isData()) ){
    return true;
  }
  else{
    return (phys.HLT_singleMu());
  }
}

bool passLeptonHLTs(){
  if (conf->get("dil_flavor") == "emu"){
    return passEMuTriggers();
  }
  else{
    //cout<<__LINE__<<endl;
    if ( phys.hyp_type() == 1 ){ //Muon Event
      return passMuonTriggers();
    }
    else if ( phys.hyp_type() == 0 ){ //Electron Event
      return passElectronTriggers();
    }
    else{ //Emu event, should never happen since this is only called from hasGoodZ()
      return false; 
    }
  }
}

//=============================
// Has Good Event Functions
//=============================

bool hasGoodZ(){
  if( phys.nlep() < 2         ){ 
    numEvents->Fill(10);
    if (printFail) cout<<phys.evt()<<" :Failed 2 lepton Z cut"<<endl;
    return false; // require at least 2 good leptons
  }
  //if (printStats) { cout<<"Number of Leptons: "<<phys.nlep()<<" "; }
  
  //cout<<__LINE__<<endl;

  if( phys.lep_pt().at(0) < 25        ) {
    numEvents->Fill(11); 
    if (printFail) cout<<phys.evt()<<" :Failed lep1 pt < 25 Z cut"<<endl;
    return false; // leading lep pT > 25 GeV
  }
  //if (printStats) { cout<<"lep1 pt: "<<phys.lep_pt().at(0)<<" "; }

  //cout<<__LINE__<<endl;

  if( phys.lep_pt().at(1) < 20        ) {
    numEvents->Fill(12); 
    if (printFail) cout<<phys.evt()<<" :Failed lep2 pt < 20 Z cut"<<endl;
    return false; // tailing lep pT > 20 GeV      
  }
  //if (printStats) { cout<<"lep2 pt: "<<phys.lep_pt().at(1)<<" "; }
  
  //cout<<__LINE__<<endl;

  if( abs(phys.lep_p4().at(0).eta())     > 2.4       ) {
    numEvents->Fill(13); 
    if (printFail) cout<<phys.evt()<<" :Failed lep1 eta > 2.4 Z cut"<<endl;
    return false; // eta < 2.4
  }    
  //if (printStats) { cout<<"lep1 eta: "<<phys.lep_p4().at(0).eta()<<" "; }

  //cout<<__LINE__<<endl;
  
  if( abs(phys.lep_p4().at(1).eta())     > 2.4       ) {
    numEvents->Fill(14); 
    if (printFail) cout<<phys.evt()<<" :Failed lep2 eta > 2.4 Z cut"<<endl;
    return false; // eta < 2.4
  }
  //if (printStats) { cout<<"lep2 eta: "<<phys.lep_p4().at(1).eta()<<" "; }

  //cout<<__LINE__<<endl;

  if (conf->get("dil_flavor") == "emu"){ //only true for ttbar estimate
    if (! (phys.hyp_type() == 2) ){ //require explicit emu event
      numEvents->Fill(15); 
      if (printFail) cout<<phys.evt()<<" :Failed not explicit e/mu Z cut, for ttbar only"<<endl;
      return false; // require explicit opposite flavor event
    }
    //if (printStats) { cout<<"hyp_type: "<<phys.hyp_type()<<" "; }
  }
  else{
    //require explicit hypothesis type
    if( !( phys.hyp_type() == 0 || phys.hyp_type() == 1 ) ) {
        numEvents->Fill(15); 
        if (printFail) cout<<phys.evt()<<" :Failed explicit mu/mu or e/e Z cut"<<endl;
        return false; // require explicit same flavor event
    }
    //if (printStats) { cout<<"hyp_type: "<<phys.hyp_type()<<" "; } 
  }


  //cout<<__LINE__<<endl;

  //This is the original cut selection
  if( abs(phys.lep_p4().at(0).eta()) > 1.4 && abs(phys.lep_p4().at(0).eta()) < 1.6 ){
    numEvents->Fill(17);
    if (printFail) cout<<phys.evt()<<" :Failed lep1 in xition region Z cut"<<endl;
    return false;
  }

  //cout<<__LINE__<<endl;

  if( phys.dilpt() <25 ){
    numEvents->Fill(26);
    if (printFail) cout<<phys.evt()<<" :Failed Z pt cut"<<endl;
    return false;
  }
  //cout<<__LINE__<<endl;

  if( abs(phys.lep_p4().at(1).eta()) > 1.4 && abs(phys.lep_p4().at(1).eta()) < 1.6 ) {
    numEvents->Fill(18); 
    if (printFail) cout<<phys.evt()<<" :Failed lep2 in xition region Z cut"<<endl;
    return false; // veto xition region
  }

  //cout<<__LINE__<<endl;
  
  if( phys.dRll() < 0.1 ) {
    numEvents->Fill(19); 
    if (printFail) cout<<phys.evt()<<" :Failed deltaR Z cut"<<endl;
    return false;
  }
  //if (printStats) { cout<<"DeltaR_ll: "<<phys.dRll()<<" "; }

  //cout<<__LINE__<<endl;

  if (! passLeptonHLTs()){
    numEvents->Fill(20);
    if (printFail) cout<<phys.evt()<<" :Failed HLT Z cut"<<endl;
    return false;
  }

  //cout<<__LINE__<<endl;

  if( !(phys.evt_type() == 0 ) ) {
    numEvents->Fill(21); 
    if (printFail) cout<<phys.evt()<<" :Failed evt_type=0 Z cut"<<endl;
    return false; // require opposite sign
  }
  //if (printStats) { cout<<"evt_type: "<<phys.evt_type()<<" "; }

  //cout<<__LINE__<<endl;
  //Set up Dilepton Masses
  double dilmass_low, dilmass_high;

  if (conf->get("dilmass_low") == "") {
    dilmass_low = 86;
  }
  else{
    dilmass_low = stod(conf->get("dilmass_low"));
  }

  if (conf->get("dilmass_high") == ""){
    dilmass_high = 96;
  }
  else{
    dilmass_high = stod(conf->get("dilmass_high"));
  }

  if( phys.dilmass() < dilmass_low ) {
    numEvents->Fill(22); 
    if (printFail) cout<<phys.evt()<<" :Failed Z mass window Z cut"<<endl;
    return false; // on-Z
  }
  if( phys.dilmass() > dilmass_high && dilmass_high != -1 ) {
    numEvents->Fill(22); 
    if (printFail) cout<<phys.evt()<<" :Failed Z mass window Z cut"<<endl;
    return false; // on-Z
  }
  //if (printStats) { cout<<"mass_ll: "<<phys.dilmass()<<" "; }

  //cout<<__LINE__<<endl;
  
  //if (printPass) cout<<phys.evt()<<": Passes good Z Cuts"<<endl;
  return true;
}

bool hasGoodPhoton(){
  if( phys.ngamma() <  1 ) {
    numEvents->Fill(24);
    if (printFail) cout<<phys.evt()<<" :Failed at least 1 photon cut"<<endl;
    return false; // require at least 1 good photon
  }
  
  if( phys.evt_type() != 2 ) {
    numEvents->Fill(25);
    if (printFail) cout<<phys.evt()<<" :Failed explicitly evt_type2 photon cut"<<endl;
    return false; // photon + jets events
  }
  
  if( phys.gamma_pt().at(0) < 25 ) {
    numEvents->Fill(26);
    if (printFail) cout<<phys.evt()<<" :Failed pt < 22 photon cut"<<endl;
    return false; // photon pt > 22 GeV
  }
  
  if( abs(phys.gamma_p4().at(0).eta()) > 1.4 && abs(phys.gamma_p4().at(0).eta()) < 1.6 ) {
    numEvents->Fill(27);
    if (printFail) cout<<phys.evt()<<" :Failed gamm ain xition region photon cut"<<endl;
    return false; // veto xition region
  }
  
  if( abs(phys.gamma_p4().at(0).eta()) > 2.4 ) {
    numEvents->Fill(28);
    if (printFail) cout<<phys.evt()<<" :Failed gamma eta > 2.4 photon cut"<<endl;
    return false; // photon in EC or EB
  }
  
  if( phys.gamma_hOverE().at(0) > 0.1 ) {
    numEvents->Fill(29);
    if (printFail) cout<<phys.evt()<<" :Failed gamma hOverE photon cut"<<endl;
    return false; // H/E < 0.1
  }
  
  // if( phys.matched_neutralemf()          < 0.7   ) return false; // jet neutral EM fraction cut
  
  if( phys.matched_emf() < 0.7 ) {
    numEvents->Fill(30);
    if (printFail) cout<<phys.evt()<<" :Failed matched_emf photon cut"<<endl;
    return false; // jet neutral EM fraction cut
  }
  
  if (conf->get("ECalTest") == ""){
    if( acos( cos( phys.gamma_phi().at(0) - g_met_phi ) ) < 0.4 ) {
      numEvents->Fill(31);
      if (printFail) cout<<phys.evt()<<" :Failed photons aligned with MET photon cut"<<endl;
      return false; // kill photons aligned with MET
    }
  }
  
  if( phys.elveto() ) {
    numEvents->Fill(32);
    if (printFail) cout<<phys.evt()<<" :Failed electron pixel veto photon cut"<<endl;
    return false; // veto pixel match
  }

  if(conf->get("event_type") == "photon"){
    if (phys.isData() && (! passPhotonTriggers()) ){
      numEvents->Fill(52);
      if (printFail) cout<<phys.evt()<<" :Failed Photon trigger cut"<<endl;
      return false;
    }
  }

  if (/*(! phys.isData()) &&*/ (! passPhotonEmulatedTrigger()) ){
    numEvents->Fill(53);
    if (printFail) cout<<phys.evt()<<" :Failed emulated photon trigger"<<endl;
    return false;
  }

  if (conf->get("nisoTrack_5gev_max") != ""){
    if( phys.nisoTrack_5gev() > stoi(conf->get("nisoTrack_5gev_max")) ){
      numEvents->Fill(62);
      if (printFail) cout<<phys.evt()<<" :Failed nisotrack_5gev cut"<<endl;
      return false;
    }
  }
  
  //if (printPass) cout<<phys.evt()<<": Passes good gamma Cuts"<<endl;
  return true;  
}

bool hasGoodGammaMu(){
  /*Method for testing whether the event has a good gamma mu pair trigger requirements are on the photon.
  It just checks muon quality stuff and then calls hasGoodPhoton()*/
  
  if( phys.nlep() < 1         ){ 
    numEvents->Fill(10);
    if (printFail) cout<<phys.evt()<<" :Failed 1 lepton GammaMu cut"<<endl;
    return false; // require at least 1 good leptons
  }
  //if (printStats) { cout<<"Number of Leptons: "<<phys.nlep()<<" "; }
  
  //cout<<__LINE__<<endl;

  if (abs(phys.lep_pdgId().at(0)) != 13){
    numEvents->Fill(61);
    if (printFail) cout<<phys.evt()<<" :Failed GammaMu muon check"<<endl;
    return false;
  }

  //cout<<__LINE__<<endl;

  if( phys.lep_pt().at(0) < 25        ) {
    numEvents->Fill(11); 
    if (printFail) cout<<phys.evt()<<" :Failed lep1 pt < 25 Z cut"<<endl;
    return false; // leading lep pT > 25 GeV
  }
  //if (printStats) { cout<<"lep1 pt: "<<phys.lep_pt().at(0)<<" "; }

  //cout<<__LINE__<<endl;

  if( abs(phys.lep_p4().at(0).eta())     > 2.4       ) {
    numEvents->Fill(13); 
    if (printFail) cout<<phys.evt()<<" :Failed lep1 eta > 2.4 Z cut"<<endl;
    return false; // eta < 2.4
  }    
  //if (printStats) { cout<<"lep1 eta: "<<phys.lep_p4().at(0).eta()<<" "; }

  //cout<<__LINE__<<endl;

  //This is the original cut selection
  if( abs(phys.lep_p4().at(0).eta()) > 1.4 && abs(phys.lep_p4().at(0).eta()) < 1.6 ){
    numEvents->Fill(17);
    if (printFail) cout<<phys.evt()<<" :Failed lep1 in xition region Z cut"<<endl;
    return false;
  }

  //cout<<__LINE__<<endl;

  if (conf->get("trigger_type") == "singleMu"){
    if(/*phys.isData() &&*/ (! passSingleMuTriggers()) ){
      numEvents->Fill(65);
      if (printFail) cout<<phys.evt()<<" :Failed Single Muon trigger cut"<<endl;
      return false;
    }
  }
  else if (conf->get("trigger_type") == "singleGamma"){
    if (/*phys.isData() &&*/ (! passPhotonTriggers()) ){
      numEvents->Fill(52);
      if (printFail) cout<<phys.evt()<<" :Failed Photon trigger cut"<<endl;
      return false;
    }
  }
  else{
    cout<<"No Trigger Type Set"<<endl;
    std::stringstream message;
    message<<"Invalid or missing trigger type set in config. Please check config variable \"trigger_type\".";
    throw std::invalid_argument(message.str());
    return false;
  }

  //cout<<__LINE__<<endl;

  //if (printPass) cout<<phys.evt()<<": Passes good muon cuts"<<endl;
  return hasGoodPhoton();  
}

bool hasGoodEvent() {
  if ( conf->get("event_type") == "photon") {
    return hasGoodPhoton();
  }
  else if (conf->get("event_type") == "photon_muon" ){
    return (hasGoodGammaMu());
  }
  else {
    return hasGoodZ();
  }
}

//=============================
// Event Weight Assignment
//=============================

void readyReweightHists(){
  TString conf_name = conf->get("Name");

  //cout<<"FINDFIND Adding "<<conf->get("Name");

  cout<<"Reweighting with "<<TString(conf->get("histo_output_dir")+"ct_"+conf->get("rwt_var")+"_"+conf->get("signal_region")+"_rwt.root")<<endl;
  TString rwt_hist_name = "h_"+conf->get("rwt_var")+"_ratio";
  TFile *reweight_file = TFile::Open( TString(conf->get("histo_output_dir")+"ct_"+conf->get("rwt_var")+"_"+conf->get("signal_region")+"_rwt.root"), "READ");
  g_reweight_pairs.push_back(make_pair( (TH1D*) reweight_file->Get(rwt_hist_name)->Clone(TString("reweight_hist_")+conf->get("rwt_var")),conf->get("rwt_var")));
  g_reweight_pairs.back().first->SetDirectory(rootdir);
  reweight_file->Close();

  while (conf->get("weight_from") != "" ){
    conf->loadConfig(conf->get("weight_from"));
    //cout<<"FINDFIND Adding "<<conf->get("Name");
    cout<<"Reweighting with "<<TString(conf->get("histo_output_dir")+"ct_"+conf->get("rwt_var")+"_"+conf->get("signal_region")+"_rwt.root")<<endl;
    rwt_hist_name = "h_"+conf->get("rwt_var")+"_ratio";
    reweight_file = TFile::Open( TString(conf->get("histo_output_dir")+"ct_"+conf->get("rwt_var")+"_"+conf->get("signal_region")+"_rwt.root"), "READ");
    g_reweight_pairs.push_back(make_pair( (TH1D*) reweight_file->Get(rwt_hist_name)->Clone(TString("reweight_hist_")+conf->get("rwt_var")),conf->get("rwt_var")));
    g_reweight_pairs.back().first->SetDirectory(rootdir);
    reweight_file->Close();      
  }

  conf->loadConfig(conf_name.Data());
  cout<<"Reweight hists loaded, proceeding with conf "<<conf->get("Name")<<endl;
}

void readyVPTReweight(TString save_path){
  /* Adds the vpt reweighting histogram to the g_reweight_pairs vector */

  TString vpt_weight_path = save_path+conf->get("Name")+"_vpt_rwt.root";
  TString rwt_hist_name = "vpt_ratio";

  cout<<"Reweighting with "<<vpt_weight_path<<endl;

  TFile *reweight_file = TFile::Open(vpt_weight_path, "READ");
  
  //Add pair (vpt_weight, "vpt") to g_reweight_pairs
  g_reweight_pairs.push_back(make_pair( (TH1D*) reweight_file->Get(rwt_hist_name)->Clone(TString("vpt_reweight_hist")),"vpt"));
  g_reweight_pairs.back().first->SetDirectory(rootdir);
  
  reweight_file->Close();
}

double getEff(const double &pt, const double &eta){
  /* Returns the trigger efficiency from g_pt_eff */
  if (abs(eta) < 1.4){
    return g_pt_eff_barrel->GetEfficiency(g_pt_eff_barrel->FindFixBin(pt));
  }
  else{
    return g_pt_eff_endcap->GetEfficiency(g_pt_eff_endcap->FindFixBin(pt)); 
  }
}

double getReweight(){
  double weight = 1;
  
  TH1D* rwt_hist;
  TString rwt_var;
  //cout<<"Size: "<<g_reweight_pairs.size()<<endl;
  for (int i=0; i<(int)g_reweight_pairs.size(); i++){
    rwt_hist = g_reweight_pairs.at(i).first;
    rwt_var = g_reweight_pairs.at(i).second;
    //cout<<rwt_var<<endl;

    if (rwt_var == "vpt"){
      //cout<<"Applying vpt reweight -- pt: "<<bosonPt()<<" weight: "<<rwt_hist->GetBinContent(rwt_hist->FindBin(bosonPt()))<<endl;
      weight *= rwt_hist->GetBinContent(rwt_hist->FindBin(bosonPt()));
    }
    else if (rwt_var == "ht_wide"){
      //cout<<"Adding HT weight: "<<rwt_hist->GetBinContent(rwt_hist->FindBin(ht))<<endl;
      weight *= rwt_hist->GetBinContent(rwt_hist->FindBin(g_ht)); 
    }
    else if (rwt_var == "muonPT_endcap"){
      //cout<<"Adding muon pt trigger efficiency weight in endcap: "<<rwt_hist->GetBinContent(rwt_hist->FindBin(phys.lep_pt().at(0)))<<endl;
      if (phys.lep_p4().at(0).eta() > 1.6){
        weight *= rwt_hist->GetBinContent(rwt_hist->FindBin(phys.lep_pt().at(0))); 
      }
    }
    else if (rwt_var == "muonPT_barrel"){
      //cout<<"Adding muon pt trigger efficiency weight in barrel: "<<rwt_hist->GetBinContent(rwt_hist->FindBin(phys.lep_pt().at(0)))<<endl;
      if (phys.lep_p4().at(0).eta() < 1.4){
        weight *= rwt_hist->GetBinContent(rwt_hist->FindBin(phys.lep_pt().at(0))); 
      }
    }
    else{
      std::stringstream message;
      message<<"Reweight varible is not a valid option, please choose vpt, or ht_wide, got: "<<rwt_var<<".";
      throw std::invalid_argument(message.str());
    }
  }

  return weight;
}

double scale1fbFix(){
  /*This method stores fixes to the evt_scale1fb in the event of file corruptions. It's basically just a lookup table*/

  if (TString(currentFile->GetTitle()).Contains("sttw_antitop_nofullhaddecay_powheg")){
    //cout<<"Scale 1fb fixed for "<<TString(currentFile->GetTitle())<<endl;
    return 1.03;
  }
  else{
    //cout<<"Scale 1fb is good for "<<TString(currentFile->GetTitle())<<endl;
    return 1;
  }
}

double getWeight(){
  /*Gets the proper weight for the sample. */
  double weight=1;
  //cout<<__LINE__<<endl;
  if ( ! ( phys.isData() ) ){
    weight *= phys.evt_scale1fb();
    
    //Weight to some other lumi
    if ( conf->get("scaleTofb") != "" ){
      weight *= stod(conf->get("scaleTofb"));
    }
    
    //cout<<__LINE__<<endl;

    if (conf->get("pileup_reweight") == "true"){
      weight*=g_pileup_hist->GetBinContent(g_pileup_hist->FindBin(phys.nTrueInt()));
    }
    
    //cout<<__LINE__<<endl;
    
    if (TString(conf->get("data_set")).Contains("GammaData-EWKSub")){
      weight *= -1; //EWK Subtraction
    }
  }
  //cout<<__LINE__<<endl;

  weight *= g_scale_factor;

  if ( conf->get("reweight") == "true" || conf->get("vpt_reweight") == "true") {
    weight *= getReweight();
  }

  //cout<<__LINE__<<endl;

  if (conf->get("rwt_photon_eff") == "true" ){
    weight *= getEff(phys.gamma_pt().at(0), phys.gamma_eta().at(0));
  }

  //cout<<__LINE__<<endl;

  if (conf->get("rwt_muon_eff") == "true"){
    weight *= getEff(phys.lep_pt().at(0), phys.lep_eta().at(0));
  }

  //cout<<__LINE__<<endl;

 if ((! phys.isData()) ){    
    if ( conf->get("event_type") == "dilepton" && (! MCTriggerEmulation)){
      //simulated trigger efficiencies
      if (phys.hyp_type() == 0) weight *= 0.969; //ee
      if (phys.hyp_type() == 1) weight *= 0.980; //mumu
      if (phys.hyp_type() == 2) weight *= 0.932; //emu
    }

    //cout<<__LINE__<<endl;

    for (int i = 0; i < phys.nlep(); i++){
      //cout<<__LINE__<<endl;
      weight*=phys.weightsf_lepid().at(i);
      //cout<<__LINE__<<endl;
      weight*=phys.weightsf_lepiso().at(i);
      //cout<<__LINE__<<endl;
      weight*=phys.weightsf_lepip().at(i);
      //cout<<__LINE__<<endl;
      weight*=phys.weightsf_lepreco().at(i);
      //cout<<__LINE__<<endl;
      weight*=phys.weightsf_lepconv().at(i);
      //cout<<__LINE__<<endl;
    }

    //cout<<__LINE__<<endl;

    if (conf->get("no_btag_sf") != "true"){
      //cout<<"Applying Btag Scale Factors"<<endl;
      weight *= phys.weight_btagsf();
    }

    if (conf->get("susy_mc") == "true"){
      double ISR_norm, btag_norm;
      if(conf->get("SUSY_Glu_LSP_scan") == "true"){
        ISR_norm=1./g_isr_norm->GetBinContent(g_isr_norm->GetXaxis()->FindBin(phys.mass_gluino()), g_isr_norm->GetYaxis()->FindBin(phys.mass_LSP()));
        //cout<<__LINE__<<endl;
        btag_norm=1./g_btagsf_norm->GetBinContent(g_btagsf_norm->GetXaxis()->FindBin(phys.mass_gluino()), g_btagsf_norm->GetYaxis()->FindBin(phys.mass_LSP()));
      }
      else if (conf->get("SUSY_chi_scan") == "true"){
        //JUST READ FROM BIN 1 IN THE LSP ROW
        ISR_norm=1./g_isr_norm->GetBinContent(g_isr_norm->GetXaxis()->FindBin(phys.mass_chi()), 1);
        //cout<<__LINE__<<endl;
        btag_norm=1./g_btagsf_norm->GetBinContent(g_btagsf_norm->GetXaxis()->FindBin(phys.mass_chi()), 1);
      }
      else{
        std::stringstream message;
        message<<"Can not get ISR or Btag normalization if SUSY_chi_scan or SUSY_Glu_LSP_scan are not set.";
        throw std::invalid_argument(message.str());
      }
      //cout<<__LINE__<<endl;
      weight *= phys.isr_weight(); //ISR scale factor
      weight *= ISR_norm;
      
      //cout<<__LINE__<<endl;

      weight *= btag_norm;

      //cout<<__LINE__<<endl;
      
      for (int i = 0; i < phys.nlep(); i++){
        weight *= phys.weightsf_lepid_FS().at(i); //Fast Sim Lepton ID
        weight *= phys.weightsf_lepiso_FS().at(i); //Fast Sim Lepton isolation
        weight *= phys.weightsf_lepip_FS().at(i); //Fast Sim Lepton impact parameter
      } 
      //cout<<__LINE__<<endl;
    }
  }
  //cout<<__LINE__<<endl;

  if (phys.isData() && phys.ngamma() > 0 && TString(currentFile->GetTitle()).Contains("data") && TString(currentFile->GetTitle()).Contains("_ph")){
    weight *= getPrescaleWeight();
  }

  if (conf->get("tchihz_tchizz_weightfix") == "true"){
    if (TString(currentFile->GetTitle()).Contains("tchihz_80x_v2")){
      weight *= 0.5;
    }
    else if (TString(currentFile->GetTitle()).Contains("tchizz_80x_v2")){
      weight *= 0.25;
    }
  }

  //cout<<__LINE__<<endl;

  /*if (weight < 0){
    cout<<"Negative Weight: "<<weight<<" "<<phys.evt()<<endl;
  }*/

  //weight *= scale1fbFix();

  return weight;
}

double getPrescaleWeight(){
  //cout<<__LINE__<<endl;
  //cout<<"Getting Prescale Weights"<<endl;
  if( ((phys.HLT_Photon165_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon165_R9Id90_HE10_IsoM() > 0) || (phys.HLT_Photon165_HE10_matchedtophoton() && phys.HLT_Photon165_HE10() > 0)) && phys.gamma_pt().at(0) > 180 ) return 1;
  else if( phys.HLT_Photon120_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon120_R9Id90_HE10_IsoM() > 0 && phys.gamma_pt().at(0) > 135 && phys.gamma_pt().at(0) < 180 ) return phys.HLT_Photon120_R9Id90_HE10_IsoM();
  else if( phys.HLT_Photon90_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon90_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 105 && phys.gamma_pt().at(0) < 135  ) return phys.HLT_Photon90_R9Id90_HE10_IsoM();
  else if( phys.HLT_Photon75_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon75_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 85 && phys.gamma_pt().at(0) < 105   ) return phys.HLT_Photon75_R9Id90_HE10_IsoM();
  else if( phys.HLT_Photon50_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon50_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 55 && phys.gamma_pt().at(0) < 85    ) return phys.HLT_Photon50_R9Id90_HE10_IsoM();
  else if( phys.HLT_Photon36_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon36_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 40 && phys.gamma_pt().at(0) < 55    ) return /*166*/ 168.309;
  else if( phys.HLT_Photon30_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon30_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 33 && phys.gamma_pt().at(0) < 40    ) return /*354*/ 379.058;
  else if( phys.HLT_Photon22_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon22_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) < 33 ) return /*1871*/ 1985.08;
  return 0;

  //if( (phys.HLT_Photon165_R9Id90_HE10_IsoM() > 0 || phys.HLT_Photon165_HE10() > 0) && phys.gamma_pt().at(0) > 180. ) return 1;
  //else if( phys.HLT_Photon120_R9Id90_HE10_IsoM() > 0 && phys.gamma_pt().at(0) > 135. ) return phys.HLT_Photon120_R9Id90_HE10_IsoM();
  //else if( phys.HLT_Photon90_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 105. ) return phys.HLT_Photon90_R9Id90_HE10_IsoM();
  //else if( phys.HLT_Photon75_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 85. ) return phys.HLT_Photon75_R9Id90_HE10_IsoM();
  //else if( phys.HLT_Photon50_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 55. ) return phys.HLT_Photon50_R9Id90_HE10_IsoM();
  //else if( phys.HLT_Photon36_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) < 55. && phys.gamma_pt().at(0) > 40. ) {
  //  return /*g_l1prescale_hist36->GetBinContent(g_l1prescale_hist36->FindBin(phys.nVert())) */ 166 /*134*/;
  //}
  //else if( phys.HLT_Photon30_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) < 40. && phys.gamma_pt().at(0) > 33. ){
  //  return /*g_l1prescale_hist30->GetBinContent(g_l1prescale_hist30->FindBin(phys.nVert())) */ 354 /*269*/;
  //}
  //else if( phys.HLT_Photon22_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) < 33. ) {
  //  return /*g_l1prescale_hist22->GetBinContent(g_l1prescale_hist22->FindBin(phys.nVert())) */ 1871 /*1667*/;
  //}
  //cout<<__LINE__<<endl;
  //return 1; // should not get here
}

//=============================
// Cuts
//=============================

bool passSignalRegionCuts(){
  
  //Njets Min Cut
  if (conf->get("Njets_min") != ""){
    if (g_njets < stod(conf->get("Njets_min"))){
      numEvents->Fill(34);
      if (printFail) cout<<phys.evt()<<" :Failed min jets cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  //Njets Max Cut
  if (conf->get("Njets_max") != ""){
    if (g_njets > stod(conf->get("Njets_max"))){
      numEvents->Fill(35);
      if (printFail) cout<<phys.evt()<<" :Failed max jets cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;
  //if (printStats) { cout<<"NbjetsMed: "<<g_nBJetMedium<<" "; }

  //Num Bottom jets Min Cut
  if (conf->get("NBjets_min") != ""){
    if (g_nBJetMedium < stod(conf->get("NBjets_min"))){
      numEvents->Fill(36);
      if (printFail) cout<<phys.evt()<<" :Failed min bjet cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  //Num Bottom jets Max Cut
  if (conf->get("NBjets_max") != ""){
    if (g_nBJetMedium > stod(conf->get("NBjets_max"))){
      numEvents->Fill(37);
      if (printFail) cout<<phys.evt()<<" :Failed max bjet cut"<<endl;
      return false;
    }
  }

  //Num Bottom jets Max Cut
  if (conf->get("NBjets_loose_max") != ""){
    if (phys.nBJetLoose() > stod(conf->get("NBjets_loose_max"))){
      numEvents->Fill(37);
      if (printFail) cout<<phys.evt()<<" :Failed max bjet cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  //Num Bottom jets Min Cut
  if (conf->get("NBjets_loose_min") != ""){
    if (phys.nBJetLoose() < stod(conf->get("NBjets_loose_min"))){
      numEvents->Fill(36);
      if (printFail) cout<<phys.evt()<<" :Failed min bjet cut"<<endl;
      return false;
    }
  }


  //cout<<__LINE__<<endl;
  //if (printStats) { cout<<"g_dphi_metj1: "<<g_dphi_metj1<<" "; }
  //Leading Jet/MET Phi min
  if (conf->get("dPhi_MET_j1") != ""){
    if (g_dphi_metj1 < stod(conf->get("dPhi_MET_j1"))){
      numEvents->Fill(38);
      if (printFail) cout<<phys.evt()<<" :Failed dPhi MET with jet 1 cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;
  //if (printStats) { cout<<"g_dphi_metj2: "<<g_dphi_metj2<<" "; }
  //Trailing Jet/MET Phi min
  if (conf->get("dPhi_MET_j2") != ""){
    if (g_dphi_metj2 < stod(conf->get("dPhi_MET_j2"))){
      numEvents->Fill(39);
      if (printFail) cout<<phys.evt()<<" :Failed dPhi MET with jet 2 cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;
  //if (printStats) { cout<<"mt2b: "<<g_mt2b<<" "; }
  //MT2b min
  if (conf->get("MT2b_min") != "" && conf->get("event_type") != "photon"){
    if (g_mt2b < stod(conf->get("MT2b_min"))){
      numEvents->Fill(40);
      if (printFail) cout<<phys.evt()<<" :Failed MT2b cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  //MT2b min
  if (conf->get("MT2b_loose_min") != "" && conf->get("event_type") != "photon"){
    if (getMT2B() < stod(conf->get("MT2b_loose_min"))){
      numEvents->Fill(40);
      if (printFail) cout<<phys.evt()<<" :Failed MT2b cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;
  //if (printStats) { cout<<"mt2b: "<<g_mt2b<<" "; }
  //MT2 min
  if (conf->get("MT2_min") != ""){
    if (g_mt2 < stod(conf->get("MT2_min"))){
      numEvents->Fill(59);
      if (printFail) cout<<phys.evt()<<" :Failed MT2 cut"<<endl;
      return false;
    }
  }


  //cout<<__LINE__<<endl;
  //HT min
  if (conf->get("HT_min") != ""){
  //if (printStats) { cout<<"ht: "<<g_ht<<" "; }
    if (g_ht < stod(conf->get("HT_min"))){
      numEvents->Fill(41);
      if (printFail) cout<<phys.evt()<<" :Failed sum HT min cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;
  //DiBottom mass difference from Higgs Mass
  if (conf->get("mbb_mh_diff") != ""){
  //if (printStats) { cout<<"mbb_mh_diff: "<<abs(mbb - 125)<<" "; }
    if (abs(g_mbb - 125) < stod(conf->get("mbb_mh_diff"))){
      numEvents->Fill(42);
      if (printFail) cout<<phys.evt()<<" :Failed sum mbb_mh diff cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  //Wierd ATLAS SR cut
  if (conf->get("sum_HT_pt_pt") != ""){
    double pt;
    //cout<<__LINE__<<endl;
    if (phys.evt_type() == 2 && phys.ngamma() > 0){
      pt = phys.gamma_pt().at(0);
    }
    else{
      pt = phys.lep_pt().at(0) + phys.lep_pt().at(1);
    }
    //cout<<__LINE__<<endl;
    //if (printStats) { cout<<"sum_HT_pt_pt: "<<abs(ht + pt )<<" "; }
    if ( abs(g_ht + pt ) < stod(conf->get("sum_HT_pt_pt") ) ){
      numEvents->Fill(43);
      if (printFail) cout<<phys.evt()<<" :Failed sum HT pt pt cut"<<endl;
      return false;
    }
  }
  
  //cout<<__LINE__<<endl;

  if (conf->get("lep1_pt_min") != "" && conf->get("event_type") != "photon" ){
    if ( phys.lep_pt().at(0) < stod( conf->get("lep1_pt_min") )){
      numEvents->Fill(45);
      if (printFail) cout<<phys.evt()<<" :Failed lep1 min pt"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  if (conf->get("lep2_pt_min") != "" && conf->get("event_type") != "photon" ){
    if ( phys.lep_pt().at(1) < stod( conf->get("lep2_pt_min") )){
      numEvents->Fill(46);
      if (printFail) cout<<phys.evt()<<" :Failed lep2 min pt cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  if (conf->get("MET_min") != ""){
    if ( g_met < stod( conf->get("MET_min") )){
      numEvents->Fill(56);
      if (printFail) cout<<phys.evt()<<" :Failed lep2 min pt cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  if (conf->get("MET_max") != ""){
    if ( g_met > stod( conf->get("MET_max") )){
      numEvents->Fill(57);
      if (printFail) cout<<phys.evt()<<" :Failed lep2 min pt cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  if (conf->get("Mbb_max") != ""){
    if ( g_mbb > stod( conf->get("Mbb_max") )){
      numEvents->Fill(58);
      if (printFail) cout<<phys.evt()<<" :Failed lep2 min pt cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  if (conf->get("Mbb_loose_max") != ""){
    if ( getMbb() > stod( conf->get("Mbb_loose_max") )){
      numEvents->Fill(58);
      if (printFail) cout<<phys.evt()<<" :Failed lep2 min pt cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  if (conf->get("MT_LepMET_min") != ""){
    if ( getMTLepMET() < stod( conf->get("MT_LepMET_min") ) ){
      numEvents->Fill(63);
      if (printFail) cout<<phys.evt()<<" :Failed MT from Lepton and MET min cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  if (conf->get("Mjj_dphi_max") != ""){
    if ( g_mjj_mindphi > stod( conf->get("Mjj_dphi_max") ) ){
      numEvents->Fill(68);
      if (printFail) cout<<phys.evt()<<" :Failed Mjj cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  if (conf->get("flavor") == "dimuon"){
    if ( phys.hyp_type() != 1){
      numEvents->Fill(73);
      if (printFail) cout<<phys.evt()<<" :Failed dimuon cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  if (conf->get("flavor") == "dielectron"){
    if ( phys.hyp_type() != 0){
      numEvents->Fill(73);
      if (printFail) cout<<phys.evt()<<" :Failed dielectron cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  if (conf->get("force_true_bjets") != ""){
    pair<int, int> b_index = getMostBlike();

    if ( abs(phys.jets_mcFlavour().at(b_index.first)) != 5 ){
      numEvents->Fill(67);
      if (printFail) cout<<phys.evt()<<" :Failed truth level bjet cut"<<endl;
      return false;
    }

    if ( abs(phys.jets_mcFlavour().at(b_index.second)) != 5 ){
      numEvents->Fill(67);
      if (printFail) cout<<phys.evt()<<" :Failed truth level bjet cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  if (conf->get("force_fake_bjets") != ""){
    pair<int, int> b_index = getMostBlike();

    if ( (abs(phys.jets_mcFlavour().at(b_index.first)) == 5) && (abs(phys.jets_mcFlavour().at(b_index.second)) == 5)){
      numEvents->Fill(67);
      if (printFail) cout<<phys.evt()<<" :Failed truth level bjet cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;
  //if (printPass) cout<<phys.evt()<<": Passes Signal Region Cuts"<<endl;
  return true;
}

bool passRareCuts(){
  
  bool hasrealmet = true;
  bool realzpair  = true;

  if( TString(conf->get("data_set")).Contains("RareMC-vvv") || TString(conf->get("data_set")).Contains("RareMC-ttz")){
    //cout<<"Checking for rare cuts"<<endl;
    hasrealmet = false;
    realzpair  = false;
    
    for( size_t genind = 0; genind < phys.genPart_motherId().size(); genind++ ){
      if( (abs(phys.genPart_motherId().at(genind)) == 24 || phys.genPart_motherId().at(genind) == 23) &&
        (phys.genPart_status().at(genind) == 23 || phys.genPart_status().at(genind) == 1 ) &&
        (abs(phys.genPart_pdgId().at(genind))==12 ||
         abs(phys.genPart_pdgId().at(genind))==14 ||
         abs(phys.genPart_pdgId().at(genind))==16) ){
        // cout<<"mom "<<phys.genPart_motherId().at(genind) << " | stat "<< phys.genPart_status().at(genind) <<  " | id "<< phys.genPart_pdgId().at(genind) << endl;
        hasrealmet = true;
      }
      
      if( (phys.genPart_motherId().at(genind) == 23 || phys.genPart_grandmaId().at(genind) == 23) &&
        (phys.genPart_status().at(genind) == 23 || phys.genPart_status().at(genind) == 1) &&
        (abs(phys.genPart_pdgId().at(genind))==11 ||
         abs(phys.genPart_pdgId().at(genind))==13) ){
        realzpair = true;
      }
    }
  }
  //if (printStats) { cout<<"HasGenZ: "<<realzpair<<" "; }
  //if (printStats) { cout<<"HasGenMET: "<<hasrealmet<<" "; }
  if ( ! hasrealmet ){
    numEvents->Fill(47);
    if (printFail) cout<<phys.evt()<<" :Failed Has Real MET Rare Cut"<<endl;    
    return false;
  }
  else if ( ! realzpair ){
    numEvents->Fill(48);
    if (printFail) cout<<phys.evt()<<" :Failed Has Real Z Rare Cut"<<endl;    
    return false;
  }
  
  //if (printPass) cout<<phys.evt()<<": Passes Has Real MET Rare Cut"<<endl;
  //if (printPass) cout<<phys.evt()<<": Passes Has Real Z Pair Rare Cut"<<endl;
  return true;
} 

bool passSUSYSingalCuts(){
  //cout<<__LINE__<<endl;
  if (conf->get("mass_chi") != ""){
    if (phys.mass_chi() != stod(conf->get("mass_chi"))){
      numEvents->Fill(55);
      if (printFail) cout<<phys.evt()<<" :Failed mass chi cut"<<endl;
      return false;
    }
  }
  
  //cout<<__LINE__<<endl;

  if (conf->get("mass_gluino") != ""){
    if (phys.mass_gluino() != stod(conf->get("mass_gluino"))){
      numEvents->Fill(55);
      if (printFail) cout<<phys.evt()<<" :Failed mass gluino cut"<<endl;
      return false;
    }
  }
  
  //cout<<__LINE__<<endl;

  if (conf->get("mass_LSP") != ""){
    if (phys.mass_LSP() != stod(conf->get("mass_LSP"))){
      numEvents->Fill(55);
      if (printFail) cout<<phys.evt()<<" :Failed mass LSP cut"<<endl;
      return false;
    }
  } 
  //cout<<__LINE__<<endl;
  return true; 
}

bool isDuplicate(){
  //cout<<__LINE__<<endl;
  if( phys.isData() ) {
    DorkyEventIdentifier id(phys.run(), phys.evt(), phys.lumi());
    //cout<<__LINE__<<endl;
    if (is_duplicate(id) ){
      ++nDuplicates;
      //cout<<__LINE__<<endl;
      if (printFail) cout<<phys.evt()<<" :Is a duplicate"<<endl;
      return true;
    }
    //cout<<__LINE__<<endl;
  }
  //cout<<__LINE__<<endl;
  //if (printPass) cout<<phys.evt()<<": Passes not a duplicate"<<endl;
  return false;
}

bool passMETFilters(){
  //updated for Moriond 2017
  if (!phys.Flag_EcalDeadCellTriggerPrimitiveFilter()      ) { 
    numEvents->Fill(5);
    if (printFail) cout<<phys.evt()<<" :Failed EcalDeadCellTriggerPrimativeFilter cut"<<endl;
    return false;
  }
  if (!phys.Flag_HBHENoiseFilter                   ()      ){ 
    numEvents->Fill(2);
    if (printFail) cout<<phys.evt()<<" :Failed HBHENoiseFilter cut"<<endl;
    return false;
  }
  if (!phys.Flag_HBHEIsoNoiseFilter                ()      ){ 
    numEvents->Fill(3);
    if (printFail) cout<<phys.evt()<<" :Failed HBHEIsoNoiseFilter cut"<<endl;
    return false;
  } 
  if (!phys.Flag_goodVertices                      ()      ) { 
    numEvents->Fill(6);
      if (printFail) cout<<phys.evt()<<" :Failed goodVerticies cut"<<endl;
    return false;
  }
  if(conf->get("signal_region") != "LeonoraEvtLists"){
    if (phys.nJet200MuFrac50DphiMet() > 0){
      numEvents->Fill(70);
      if (printFail) cout<<phys.evt()<<" :Failed nJet200MuFrac50DphiMet cut"<<endl;
      return false;
    }
    if ((g_met / phys.met_calo_pt()) > 5){
      numEvents->Fill(71);
      if (printFail) cout<<phys.evt()<<" :Failed T1MET/CaloMET cut"<<endl;
      return false;
    }
  }
  if (conf->get("susy_mc") != "true"){
    if (!phys.Flag_globalTightHalo2016            ()      ){ 
      numEvents->Fill(4);
      if (printFail) cout<<phys.evt()<<" :Failed CSCTightHalo2015Filter cut"<<endl;
      return false;
    }
  }
  if ( phys.isData() ) {
    if (!phys.Flag_eeBadScFilter                     ()      ) { 
      numEvents->Fill(7);
      if (printFail) cout<<phys.evt()<<" :Failed eeBadScFilter cut"<<endl;
      return false;
    }
  }
  
  //if (printPass) cout<<phys.evt()<<": Passes MET Filters"<<endl;
  return true;
}

bool passBaseCut(){
  //if (printStats) { cout<<"goodrun : "<<goodrun(phys.evt(), phys.lumi())<<" "; }
  //if (printStats) { cout<<"njets : "<<g_njets<<" "; }
  
  //bool pass=true;

  if (phys.isData()){
    if (! (goodrun(phys.run(), phys.lumi()))){ 
      numEvents->Fill(8);
      if (printFail) cout<<phys.evt()<<" :Failed golden JSON cut"<<endl;
      return false; //golden json
    }
  }
  //Old Method, using branch
  /*if (! (phys.evt_passgoodrunlist() > 0)){ 
    pass=false; //golden json
    if (printFail) cout<<phys.evt()<<" :Failed golden JSON cut"<<endl;
    numEvents->Fill(8);
  } */

  /*if (! (g_njets >= 2) ){ 
    numEvents->Fill(9);
    if (printFail) cout<<phys.evt()<<" :Failed 2 Jets cut"<<endl;
    return false; //2 jet cut
    //pass=false;
  }*/

  if(conf->get("n_lep_veto") != ""){
    if( (phys.nisoTrack_mt2() + phys.nlep()) >= stod(conf->get("n_lep_veto"))){
        numEvents->Fill(54);
        if (printFail) cout<<phys.evt()<<" :Failed extra lepton veto"<<endl;
        return false; //third lepton veto
        //pass=false;
    }
    if (phys.nveto_leptons() >= 1){
      numEvents->Fill(66);
      if (printFail) cout<<phys.evt()<<" :Failed multi-lepton analysis lepton veto"<<endl;
      return false;
    }
    if (phys.nTaus20() >= 1){
      numEvents->Fill(75);
      if (printFail) cout<<phys.evt()<<" :Failed Tau veto"<<endl;
      return false;
    }
  }

  if(conf->get("multi_lep_veto") != ""){
    if (phys.nveto_leptons() >= 1){
      numEvents->Fill(66);
      if (printFail) cout<<phys.evt()<<" :Failed multi-lepton analysis lepton veto"<<endl;
      return false;
    }
    if (phys.nTaus20() >= 1){
      numEvents->Fill(75);
      if (printFail) cout<<phys.evt()<<" :Failed Tau veto"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;
  /*//if (printStats) { cout<<"g_dphi_metj1: "<<g_dphi_metj1<<" "; }
  //Leading Jet/MET Phi min
  if (g_dphi_metj1 < 0.4){
    numEvents->Fill(38);
    if (printFail) cout<<phys.evt()<<" :Failed dPhi MET with jet 1 cut"<<endl;
    return false;
  }
  //cout<<__LINE__<<endl;
  //if (printStats) { cout<<"g_dphi_metj2: "<<g_dphi_metj2<<" "; }
  //Trailing Jet/MET Phi min
  if (g_dphi_metj2 < 0.4){
    numEvents->Fill(39);
    if (printFail) cout<<phys.evt()<<" :Failed dPhi MET with jet 2 cut"<<endl;
    return false;
  }*/

  //return pass;
  return true;
  //if (printPass) cout<<phys.evt()<<": Passes Base Cuts"<<endl;
}

bool passETHDileptonDataCleanse(){
  /*Ensures events from the ee/mumu/emu dataset pass the trigger for that type of event and for ee and emu ensures they don't pass other triggers.*/
  //Dilepton Data samples
  if ( (phys.isData()) && TString(conf->get("data_set")).Contains("DileptonData")){
    //cout<<"Dilepton Data Event"<<endl;

    //ETH Trigger Cleansing
    if( TString(currentFile->GetTitle()).Contains("data_Run2016")){
      if (TString(currentFile->GetTitle()).Contains("_mm_") ){
        if( ! passMuonTriggers() ) {
          //cout<<"skipped"<<endl;
          if (printFail) cout<<"ETH Trigger Cleansing: double muon dataset didn't pass double muon trigger"<<endl;
          numEvents->Fill(74);
          return false;
        }
      }
      else if (TString(currentFile->GetTitle()).Contains("_ee_") ){
        if(! passElectronTriggers() ) {
          //cout<<"skipped"<<endl;
          if (printFail) cout<<"ETH Trigger Cleansing: double electron dataset didn't pass double electron trigger"<<endl;
          numEvents->Fill(74);
          return false;
        }
        if(passMuonTriggers()) {
          //cout<<"skipped"<<endl;
          if (printFail) cout<<"ETH Trigger Cleansing: double electron dataset passed double muon trigger"<<endl;
          numEvents->Fill(74);
          return false;
        }
      }
      else if (TString(currentFile->GetTitle()).Contains("_em_") ){
        if(! passEMuTriggers() ) {
          //cout<<"skipped"<<endl;
          if (printFail) cout<<"ETH Trigger Cleansing: EMu dataset didn't pass EMu trigger"<<endl;
          numEvents->Fill(74);
          return false;
        }
        if( passElectronTriggers() ) {
          //cout<<"skipped"<<endl;
          if (printFail) cout<<"ETH Trigger Cleansing: EMu dataset dataset passed double electron trigger"<<endl;
          numEvents->Fill(74);
          return false;
        }
        if(passMuonTriggers()) {
          //cout<<"skipped"<<endl;
          if (printFail) cout<<"ETH Trigger Cleansing: double electron dataset passed double muon trigger"<<endl;
          numEvents->Fill(74);
          return false;
        }
      }
    }
  }

  return true;
}

bool passFileSelections(){
  /* Method which holds all the file specific selections, for instance cutting out the
  events with genht > 100 in the DY inclusive samples */


  //Zjets Monte Carlo samples
  if ( (! phys.isData()) && TString(conf->get("data_set")).Contains("ZMC")){
    //cout<<"Zjets MC event"<<endl;
    if( TString(currentFile->GetTitle()).Contains("dy_m50_mgmlm") && (! TString(currentFile->GetTitle()).Contains("_ht")) ){
      //cout<<"File: "<<currentFile->GetTitle()<<" with gen_ht: "<<phys.gen_ht()<<endl;
      if( phys.gen_ht() > 100 ) {
        //cout<<"skipped"<<endl;
        numEvents->Fill(44);
        return false;
      }
      if(phys.evt_scale1fb() > 30){
        numEvents->Fill(60);
        return false;
      }
      if( abs(phys.gen_ht() - g_ht) > 300 ) {
        //cout<<"skipped"<<endl;
        numEvents->Fill(69);
        return false;
      }
    }
    if( TString(currentFile->GetTitle()).Contains("dy_m50_mgmlm_ht100")){
      if( abs(phys.gen_ht() - g_ht) > 300 ) {
        //cout<<"skipped"<<endl;
        numEvents->Fill(69);
        return false;
      }
      /*if (conf->get("signal_region") == "TChiHZ" && phys.evt() == 24645544 && TString(conf->get("conf_path")).Contains("TemplatesClosure") ){
        cout<<"Hand removed the one TChiHZ event"<<endl;
        return false;
      }*/
    }
  }

  //Photon MC samples
  if ( (! phys.isData()) && TString(conf->get("data_set")).Contains("GammaMC")){
    //cout<<"Photon MC event"<<endl;
    if( TString(currentFile->GetTitle()).Contains("gjetsht40") ||  TString(currentFile->GetTitle()).Contains("gjetsht100") ){
      if( abs(phys.gen_ht() - g_ht) > 300 ) {
        //cout<<"skipped"<<endl;
        numEvents->Fill(69);
        return false;
      }
    }
  }

  //WJets cocktail for inclusive photon sample and Electroweak Subtraction
  if ( TString(conf->get("data_set")).Contains("GammaMC-WGamma") || TString(conf->get("data_set")).Contains("GammaData-EWKSub")){
    
    //Inclusive GenHT Cut
    if( TString(currentFile->GetTitle()).Contains("wjets_incl_mgmlm") ){
      //cout<<"File: "<<currentFile->GetTitle()<<" with gen_ht: "<<phys.gen_ht()<<endl;
      if( phys.gen_ht() > 100 ) {
        //cout<<"skipped"<<endl;
        numEvents->Fill(44);
        return false;
      }
    }

    //Remove overlap between WGammaJets and WJets
    if( TString(currentFile->GetTitle()).Contains("wjets") ){ //WJets
      if( phys.ngamma() > 0 && phys.gamma_genIsPromptFinalState().at(0) == 1 ) {
        //cout<<"skipped"<<endl;
        numEvents->Fill(64);
        return false;
      }
    }
    else if ( TString(currentFile->GetTitle()).Contains("wgjets_") ){ //WGammaJets
      if( phys.ngamma() > 0 && phys.gamma_genIsPromptFinalState().at(0) != 1 ) {
        //cout<<"skipped"<<endl;
        numEvents->Fill(64);
        return false;
      }
    }

    //remove the events with less than 40 gen pt
    if ( TString(currentFile->GetTitle()).Contains("wgjets_incl_mgmlm") && phys.ngamma() > 0){
      //First get best photon match
      int bestMatch = -1;
      float bestDR = 0.1;
      double eta = phys.gamma_eta().at(0);
      double phi = phys.gamma_phi().at(0);
      for(unsigned int iGen = 0; iGen < phys.genPart_pdgId().size(); iGen++){
        if ((phys.genPart_pdgId().at(iGen) != 22) /*&& (abs(phys.genPart_pdgId().at(iGen)) != 11)*/ ) continue; // accept gen photons
        if ((fabs(phys.genPart_motherId().at(iGen)) > 24) && (fabs (phys.genPart_motherId().at(iGen)) != 2212) ) continue; // don't want stuff from pions etc 
        if (phys.genPart_status().at(iGen) != 1  ) continue; 
        if (fabs(eta - phys.genPart_eta().at(iGen)) > 0.1 ) continue;
        float thisDR = sqrt(pow(eta-phys.genPart_eta().at(iGen), 2) + pow(phi-phys.genPart_phi().at(iGen), 2)); //DeltaR( cms3.genps_p4() .at(iGen).eta(), eta, cms3.genps_p4().at(iGen).phi(), phi);
        if (thisDR < bestDR) {
          bestMatch = iGen;
          bestDR=thisDR;
        }
      }
      if (bestMatch < 0){
        numEvents->Fill(72);
        return false;
      }
      else if (phys.genPart_pt().at(bestMatch) > 40){
        numEvents->Fill(72);
        return false; 
      }
    } 
  }

  if ( TString(conf->get("data_set")).Contains("FSMC-TTBar-NoPromptGamma") ){
    
    //Remove prompt photons from TTBar
    if( TString(currentFile->GetTitle()).Contains("ttbar") ){
      if( phys.ngamma() > 0 && (phys.gamma_genIsPromptFinalState().at(0) == 1 && phys.gamma_mcMatchId().at(0) == 22)) {
        //cout<<"skipped"<<endl;
        numEvents->Fill(64);
        num_events_veto_ttbar++;
        return false;
      }
    }   
  }

  if ( TString(conf->get("data_set")).Contains("FSMC-TTGamma-NoNonPromptGamma") ){
    
    //Remove Non-prompt from TTGamma
    if ( TString(currentFile->GetTitle()).Contains("ttgamma_incl_amcnlo") ){
      if( phys.ngamma() > 0 && (phys.gamma_genIsPromptFinalState().at(0) != 1 || phys.gamma_mcMatchId().at(0) != 22 ) ) {
        //cout<<"skipped"<<endl;
        numEvents->Fill(64);
        num_events_veto_ttgamma++;
        return false;
      }
    }   
  }

  return true;
}

//=============================
// Setup
//=============================

void setupGlobals(){
  if ( conf->get("uncertainty_mode") == "JES_up" ){
    g_dphi_metj1 = phys.dphi_metj1_up();
    g_dphi_metj2 = phys.dphi_metj2_up();
    g_njets = phys.njets_up();
    g_mbb = phys.mbb_csv_up();
    g_mjj_mindphi = phys.mjj_mindphi_up();
    g_nBJetMedium = phys.nBJetMedium_up();
    g_met = phys.met_T1CHS_miniAOD_CORE_up_pt();
    g_met_phi = phys.met_T1CHS_miniAOD_CORE_up_phi();
    g_mt2 = phys.mt2_up();
    g_mt2b = phys.mt2b_up();
    g_ht = phys.ht_up();

    g_jets_p4 = phys.jets_up_p4();
    g_jets_medb_p4 = phys.jets_medb_up_p4();
    g_jets_csv = phys.jets_up_csv();
  }
  else if (conf->get("uncertainty_mode") == "JES_dn"){
    g_dphi_metj1 = phys.dphi_metj1_dn();
    g_dphi_metj2 = phys.dphi_metj2_dn();
    g_njets = phys.njets_dn();
    g_mbb = phys.mbb_csv_dn();
    g_mjj_mindphi = phys.mjj_mindphi_dn();
    g_nBJetMedium = phys.nBJetMedium_dn();
    g_met = phys.met_T1CHS_miniAOD_CORE_dn_pt();
    g_met_phi = phys.met_T1CHS_miniAOD_CORE_dn_phi();
    g_mt2 = phys.mt2_dn();
    g_mt2b = phys.mt2b_dn();
    g_ht = phys.ht_dn();

    g_jets_p4 = phys.jets_dn_p4();
    g_jets_medb_p4 = phys.jets_medb_dn_p4();
    g_jets_csv = phys.jets_dn_csv();
  }
  else if (conf->get("uncertainty_mode") == "GenMet"){
    g_dphi_metj1 = phys.dphi_genmetj1();
    g_dphi_metj2 = phys.dphi_genmetj2();
    g_met = phys.met_genPt();
    g_met_phi = phys.met_genPhi();
    g_mt2 = phys.mt2_genmet();
    g_mt2b = phys.mt2b_genmet();
    
    g_mjj_mindphi = phys.mjj_mindphi();
    g_mbb = phys.mbb_csv();
    g_nBJetMedium = phys.nBJetMedium();
    g_njets = phys.njets();
    g_ht = phys.ht();
    g_jets_p4 = phys.jets_p4();
    g_jets_medb_p4 = phys.jets_medb_p4();
    g_jets_csv = phys.jets_csv();
  }
  else{
    g_dphi_metj1 = phys.dphi_metj1();
    g_dphi_metj2 = phys.dphi_metj2();
    g_njets = phys.njets();
    g_mbb = phys.mbb_csv();
    g_mjj_mindphi = phys.mjj_mindphi();
    g_nBJetMedium = phys.nBJetMedium();
    g_met = phys.met_T1CHS_miniAOD_CORE_pt();
    g_met_phi = phys.met_T1CHS_miniAOD_CORE_phi();
    g_mt2 = phys.mt2();
    g_mt2b = phys.mt2b();
    g_ht = phys.ht();
    g_jets_p4 = phys.jets_p4();
    g_jets_medb_p4 = phys.jets_medb_p4();
    g_jets_csv = phys.jets_csv();
  }
}

int ScanChain( TChain* chain, ConfigParser *configuration, bool fast/* = true*/, int nEvents/* = -1*/) {
  /* Runs through baby files and makes histogram files. 
  
  Inputs:
  chain -- contains the files to make the histograms from, 
  configuration -- pointer to the configuration object
  */  
  //cout<<__LINE__<<endl;
  //Set Global Vars
  conf=configuration;
  //cout<<__LINE__<<endl;
  g_sample_name=conf->get("Name");

  if (conf->get("MCTriggerEmulation") != ""){
    if (conf->get("MCTriggerEmulation") == "true"){
      cout<<"Manually setting MC trigger emulation to true"<<endl;
      MCTriggerEmulation=true;
    }
    else if (conf->get("MCTriggerEmulation") == "false"){
      cout<<"Manually setting MC trigger emulation to false"<<endl;
      MCTriggerEmulation=false;
    }
  }

  TString savePath = getOutputDir(conf, "hist");
  ofstream files_log;
  files_log.open((savePath+TString(g_sample_name+"_files.log")).Data());
  //cout<<__LINE__<<endl;
  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  if (conf->get("scale_factor") != ""){
    g_scale_factor*=stod(conf->get("scale_factor"));
  }

//===========================================
// Define Histograms
//===========================================

  clear_list(); //Event duplicate removal clear list

  cout<<"Opening file "<<TString(savePath+conf->get("Name")+".root")<<endl;
  TFile * output = new TFile(TString(savePath+conf->get("Name")+".root"), "recreate");
  output->cd();

  numEvents = new TH1I("numEvents", "Number of events in "+g_sample_name, 80, 0, 80);
  numEvents->SetDirectory(rootdir);

  const int n_weight_log_bins = 54;
  const double weight_log_bins[n_weight_log_bins+1] = {-5, -4.5, -4, -3.5, -3, -2.5, -2, -1.5, -1, -.9, -.8, -.7, -.6, -.5, -.4, -.3, -.2, -.1, -.09, -.08, -.07, -.06, -.05, -.04, -.03, -.02, -.01, 0, .01, .02, .03, .04, .05, .06, .07, .08, .09, .1, .2, .3, .4, .5, .6, .7, .8, .9, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5};

  TH1D* weight_log = new TH1D("weight_log", "Event weights in "+g_sample_name, n_weight_log_bins , weight_log_bins);
  weight_log->SetDirectory(rootdir);

  TH1D* weight_log_flat = new TH1D("weight_log_flat", "Event weights in "+g_sample_name, 101 , 0, 1.01);
  weight_log_flat->SetDirectory(rootdir);

  //MET Histos
  TH1D *t1met = new TH1D("type1MET", "Type 1 MET for "+g_sample_name, 6000,0,6000);
  t1met->SetDirectory(rootdir);
  t1met->Sumw2();

  const int n_metbins_wide_std = 6;
  const double metbins_wide_std[n_metbins_wide_std+1] = {0, 50, 100, 150, 225, 300, 500};

  TH1D *t1met_widebin = new TH1D("type1MET_widebin", "Type 1 MET for "+g_sample_name, n_metbins_wide_std, metbins_wide_std);
  t1met_widebin->SetDirectory(rootdir);
  t1met_widebin->Sumw2();

  //MET Histos
  TH1D *nVert = new TH1D("nVert", "Number of verticies in "+g_sample_name, 150,0,150);
  nVert->SetDirectory(rootdir);
  nVert->Sumw2();

  TH1D *rawmet = new TH1D("rawMET", "Raw MET for "+g_sample_name, 6000,0,6000);
  rawmet->SetDirectory(rootdir);
  rawmet->Sumw2();

  TH1D *mt2 = new TH1D("mt2", "MT2 for "+g_sample_name, 500,0,500);
  mt2->SetDirectory(rootdir);
  mt2->Sumw2();

  TH1D *dilmass;
  
  if (conf->get("wide_dilmass") == "true"){
    dilmass = new TH1D("dilmass", "Dilepton Mass for "+g_sample_name, 500,0,500);
  }
  else{
    dilmass = new TH1D("dilmass", "Dilepton Mass for "+g_sample_name, 20,81,101);
  }  
  
  dilmass->SetDirectory(rootdir);
  dilmass->Sumw2();

  TH1D *mt2b = new TH1D("mt2b", "MT2b for "+g_sample_name, 6000,0,6000);
  mt2b->SetDirectory(rootdir);
  mt2b->Sumw2();

  TH1D *nlep = new TH1D("nlep", "Number of Leptons for "+g_sample_name, 20,0,20);
  nlep->SetDirectory(rootdir);
  nlep->Sumw2();

  TH1D *nisotrack = new TH1D("nisotrack", "Number of Iso Track Leptons (MT2 style) for "+g_sample_name, 20,0,20);
  nisotrack->SetDirectory(rootdir);
  nisotrack->Sumw2();

  TH1D *dphi_jet1_met = new TH1D("dphi_jet1_met", "#Delta#Phi(jet_{1}, E^{miss}_{T}) for "+g_sample_name, 100,0,3.15);
  dphi_jet1_met->SetDirectory(rootdir);
  dphi_jet1_met->Sumw2();

  TH1D *dphi_jet2_met = new TH1D("dphi_jet2_met", "#Delta#Phi(jet_{2}, E^{miss}_{T}) for "+g_sample_name, 100,0,3.15);
  dphi_jet2_met->SetDirectory(rootdir);
  dphi_jet2_met->Sumw2();

  TH1D *ht = new TH1D("ht", "Scalar sum of hadronic pt (HT) for "+g_sample_name, 6000,0,6000);
  ht->SetDirectory(rootdir);
  ht->Sumw2();

  TH1D *ht_wide = new TH1D("ht_wide", "Scalar sum of hadronic pt (HT) for "+g_sample_name, 60,0,6000);
  ht_wide->SetDirectory(rootdir);
  ht_wide->Sumw2();

  TH1D *gen_ht = new TH1D("genht", "Scalar sum of generated hadronic pt (Gen HT) for "+g_sample_name, 6000,0,6000);
  gen_ht->SetDirectory(rootdir);
  gen_ht->Sumw2();

  TH1D *numMETFilters = new TH1D("numMETFilters", "Number of MET Filters passed for events in "+g_sample_name, 50,0,50);
  numMETFilters->SetDirectory(rootdir);
  numMETFilters->Sumw2();

  const int n_ptbins_std = 10;
  const double ptbins_std[n_ptbins_std+1] = {0, 22, 33, 40, 55, 85, 105, 135, 180, 250, 6000};

  const int n_ptbins_fine = 51;
  const double ptbins_fine[n_ptbins_fine+1] = {0, 22, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195, 200, 205, 210, 215, 220, 225, 230, 235, 240, 245, 250, 300, 350, 400, 6001};

  TH1D *vpt;
  if (conf->get("signal_region") == "2j"){
    vpt = new TH1D("vpt", "Boson Pt for events in "+g_sample_name, n_ptbins_fine, ptbins_fine);
  }  
  else{
    vpt = new TH1D("vpt", "Boson Pt for events in "+g_sample_name, n_ptbins_std, ptbins_std);
  }

  vpt->SetDirectory(rootdir);
  vpt->Sumw2();

  TH1D *vpt_flat = new TH1D("vpt_flat", "Boson P_{T} for events in "+g_sample_name, 6000,0,6000);
  vpt_flat->SetDirectory(rootdir);
  vpt_flat->Sumw2();

  TH1D *njets = new TH1D("njets", "Number of jets for events in "+g_sample_name, 50,0,50);
  njets->SetDirectory(rootdir);
  njets->Sumw2();

  TH1D *nbtags_m = new TH1D("nbtags_m", "Number of \"medium\" B-tagged jets for events in "+g_sample_name, 50,0,50);
  nbtags_m->SetDirectory(rootdir);
  nbtags_m->Sumw2();

  TH1D *nbtags_l = new TH1D("nbtags_l", "Number of \"loose\" B-tagged jets for events in "+g_sample_name, 50,0,50);
  nbtags_l->SetDirectory(rootdir);
  nbtags_l->Sumw2();

  TH1D *nbtags_t = new TH1D("nbtags_t", "Number of \"tight\" B-tagged jets for events in "+g_sample_name, 50,0,50);
  nbtags_t->SetDirectory(rootdir);
  nbtags_t->Sumw2();
  
  TH1D *MT_MuMET, *dR_GammaMu, *mu_pt;
  if (conf->get("GammaMuStudy") == "true"){
    MT_MuMET = new TH1D("MT_MuMET", "M_{T} Made With Muon and E^{miss}_{T} (If from W, #leq W mass) for "+g_sample_name, 6000,0,6000);
    MT_MuMET->SetDirectory(rootdir);
    MT_MuMET->Sumw2();

    dR_GammaMu = new TH1D("dR_GammaMu", "#Delta R Between Photon and Muon for "+g_sample_name, 200,0,5.8);
    dR_GammaMu->SetDirectory(rootdir);
    dR_GammaMu->Sumw2();

    mu_pt = new TH1D("mu_pt", "Muon P_{T} for "+g_sample_name, 6000,0,6000);
    mu_pt->SetDirectory(rootdir);
    mu_pt->Sumw2();
  }

  TH1D *sum_mlb, *m_bb_csv, *m_bb_bpt, *mt2j, *sum_pt_z_bb, *mt2_fromb, *mt2_hz;
  TH2D *MT2_MT2B, *MT2_MT2_fromb, *MT2_MT2_HZ;

  if(conf->get("signal_region") == "TChiHZ"){
    sum_mlb = new TH1D("sum_mlb", "#Sigma M_{lb} for "+g_sample_name, 6000,0,6000);
    sum_mlb->SetDirectory(rootdir);
    sum_mlb->Sumw2();

    m_bb_csv = new TH1D("m_bb_csv", "M_{bb} by CSV for "+g_sample_name, 6000,0,6000);
    m_bb_csv->SetDirectory(rootdir);
    m_bb_csv->Sumw2();

    m_bb_bpt = new TH1D("m_bb_bpt", "M_{bb} by B P_{T} for "+g_sample_name, 6000,0,6000);
    m_bb_bpt->SetDirectory(rootdir);
    m_bb_bpt->Sumw2();

    mt2j = new TH1D("mt2j", "MT2j for "+g_sample_name, 6000,0,6000);
    mt2j->SetDirectory(rootdir);
    mt2j->Sumw2();

    mt2_fromb = new TH1D("mt2_fromb", "MT2 From Bjets for "+g_sample_name, 6000,0,6000);
    mt2_fromb->SetDirectory(rootdir);
    mt2_fromb->Sumw2();

    mt2_hz = new TH1D("mt2_hz", "MT2 From Higgs and Z for "+g_sample_name, 6000,0,6000);
    mt2_hz->SetDirectory(rootdir);
    mt2_hz->Sumw2();

    sum_pt_z_bb = new TH1D("sum_pt_z_bb", "P_{T}(Z) + P_{T}(BB) for "+g_sample_name, 6000,0,6000);
    sum_pt_z_bb->SetDirectory(rootdir);
    sum_pt_z_bb->Sumw2();

    MT2_MT2B = new TH2D("MT2_MT2B", "MT2 vs. MT2b for "+g_sample_name, 6000, 0, 6000, 6000, 0, 6000);
    MT2_MT2B->SetDirectory(rootdir);
    MT2_MT2B->Sumw2();

    MT2_MT2_fromb = new TH2D("MT2_MT2_fromb", "MT2 vs. MT2(made from b-tagged jets) for "+g_sample_name, 6000, 0, 6000, 6000, 0, 6000);
    MT2_MT2_fromb->SetDirectory(rootdir);
    MT2_MT2_fromb->Sumw2();

    MT2_MT2_HZ = new TH2D("MT2_MT2_HZ", "MT2(from leptons) vs. MT2(from Higgs and Z) for "+g_sample_name, 6000, 0, 6000, 6000, 0, 6000);
    MT2_MT2_HZ->SetDirectory(rootdir);
    MT2_MT2_HZ->Sumw2();
  }

  TH1D* mjj_min_dphi;

  if(conf->get("signal_region") == "TChiWZ"){
    mjj_min_dphi = new TH1D("mjj_min_dphi", "M_{jj} (for minimum #Delta #Phi Jets) in "+g_sample_name, 6000,0,6000);
    mjj_min_dphi->SetDirectory(rootdir);
    mjj_min_dphi->Sumw2();
  }

  TH1D *lep1_eta,*lep2_eta,*lep1_eta_ee,*lep2_eta_ee,*lep1_eta_mm,*lep2_eta_mm,*dilmass_ee,*dilmass_mm;
  if(conf->get("signal_region") == "LeonoraXsecStudy"){

    dilmass_ee = new TH1D("dilmass_ee", "Dilepton Mass for just electrons"+g_sample_name, 500,0,500);
    dilmass_mm = new TH1D("dilmass_mm", "Dilepton Mass for just muons"+g_sample_name, 500,0,500);
    
    lep1_eta = new TH1D("lep1_eta", "#eta for leading lepton"+g_sample_name, 100,-3,3);
    lep2_eta = new TH1D("lep2_eta", "#eta for subleading lepton"+g_sample_name, 100,-3,3);
    
    lep1_eta_ee = new TH1D("lep1_eta_ee", "#eta for leading electron"+g_sample_name, 100,-3,3);
    lep2_eta_ee = new TH1D("lep2_eta_ee", "#eta for subleading electron"+g_sample_name, 100,-3,3);
    
    lep1_eta_mm = new TH1D("lep1_eta_mm", "#eta for leading muon"+g_sample_name, 100,-3,3);
    lep2_eta_mm = new TH1D("lep2_eta_mm", "#eta for subleading muon"+g_sample_name, 100,-3,3);
  }

  if(conf->get("signal_region") == "TChiWZ"){
    mjj_min_dphi = new TH1D("mjj_min_dphi", "M_{jj} (for minimum #Delta #Phi Jets) in "+g_sample_name, 6000,0,6000);
    mjj_min_dphi->SetDirectory(rootdir);
    mjj_min_dphi->Sumw2();
  }

  //==============================
  //T5ZZ model natural binning
  //==============================
  const int n_gluino_bins_t5zznat = 24;
  const double gluino_bins_t5zznat[n_gluino_bins_t5zznat+1]={800.000000,900.000000,1000.000000,1050.000000,1100.000000,1150.000000,1200.000000,1250.000000,1300.000000,1350.000000,1400.000000,1450.000000,1500.000000,1550.000000,1600.000000,1650.000000,1700.000000,1750.000000,1800.000000,1850.000000,1900.000000,1950.000000,2000.000000,2050.000000,2100.000000};

  const int n_lsp_bins_t5zznat = 66;
  const double lsp_bins_t5zznat[n_lsp_bins_t5zznat+1]={25.000000,50.000000,100.000000,150.000000,200.000000,250.000000,300.000000,350.000000,400.000000,450.000000,500.000000,550.000000,600.000000,650.000000,700.000000,750.000000,790.000000,800.000000,850.000000,890.000000,900.000000,950.000000,990.000000,1000.000000,1040.000000,1050.000000,1090.000000,1100.000000,1140.000000,1150.000000,1190.000000,1200.000000,1240.000000,1250.000000,1290.000000,1300.000000,1340.000000,1350.000000,1390.000000,1400.000000,1440.000000,1450.000000,1490.000000,1500.000000,1540.000000,1550.000000,1590.000000,1600.000000,1640.000000,1650.000000,1690.000000,1700.000000,1740.000000,1750.000000,1790.000000,1800.000000,1840.000000,1850.000000,1890.000000,1900.000000,1940.000000,1950.000000,1990.000000,2000.000000,2040.000000,2050.000000,2090.000000};

  const int n_met_bins_t5zznat = 5;
  const double met_bins_t5zznat[n_met_bins_t5zznat+1] = {50, 100, 150, 250, 350, 6000};

  //==============================
  //T5ZZ contrived binning 
  //==============================

  const int n_gluino_bins_t5zzcontrived = 24;
  const double gluino_bins_t5zzcontrived[n_gluino_bins_t5zzcontrived+1]={800.000000,900.000000,1000.000000,1050.000000,1100.000000,1150.000000,1200.000000,1250.000000,1300.000000,1350.000000,1400.000000,1450.000000,1500.000000,1550.000000,1600.000000,1650.000000,1700.000000,1750.000000,1800.000000,1850.000000,1900.000000,1950.000000,2000.000000,2050.000000,2100.000000};

  const int n_lsp_bins_t5zzcontrived = 39;
  const double lsp_bins_t5zzcontrived[n_lsp_bins_t5zzcontrived+1]={100.000000,150.000000,200.000000,250.000000,300.000000,350.000000,400.000000,450.000000,500.000000,550.000000,600.000000,650.000000,700.000000,750.000000,800.000000,850.000000,900.000000,950.000000,1000.000000,1050.000000,1100.000000,1150.000000,1200.000000,1250.000000,1300.000000,1350.000000,1400.000000,1450.000000,1500.000000,1550.000000,1600.000000,1650.000000,1700.000000,1750.000000,1800.000000,1850.000000,1900.000000,1950.000000,2000.000000,2050.000000};

  const int n_met_bins_t5zzcontrived = 5;
  const double met_bins_t5zzcontrived[n_met_bins_t5zzcontrived+1] = {50, 100, 150, 250, 350, 6000};
  
  //==============================
  //TChiWZ model natural binning
  //==============================
  const int n_gluino_bins_tchiwz = 24;
  const double gluino_bins_tchiwz[n_gluino_bins_tchiwz+1]={100.000000,125.000000,150.000000,175.000000,200.000000,225.000000,250.000000,275.000000,300.000000,325.000000,350.000000,375.000000,400.000000,425.000000,450.000000,475.000000,500.000000,525.000000,550.000000,575.000000,600.000000,625.000000,650.000000,675.000000,700.000000};

  const int n_lsp_bins_tchiwz = 69;
  const double lsp_bins_tchiwz[n_lsp_bins_tchiwz+1]={1.000000,5.000000,10.000000,15.000000,20.000000,25.000000,30.000000,35.000000,40.000000,45.000000,50.000000,55.000000,60.000000,65.000000,70.000000,75.000000,80.000000,85.000000,90.000000,93.000000,95.000000,100.000000,105.000000,110.000000,115.000000,118.000000,120.000000,125.000000,130.000000,135.000000,140.000000,143.000000,145.000000,150.000000,155.000000,160.000000,165.000000,168.000000,170.000000,175.000000,180.000000,185.000000,190.000000,193.000000,195.000000,200.000000,205.000000,210.000000,215.000000,218.000000,220.000000,225.000000,230.000000,235.000000,240.000000,243.000000,245.000000,250.000000,255.000000,260.000000,265.000000,268.000000,270.000000,275.000000,280.000000,285.000000,290.000000,293.000000,295.000000,300.000000};

  const int n_met_bins_tchiwz = 5;
  const double met_bins_tchiwz[n_met_bins_tchiwz+1] = {50, 100, 150, 250, 350, 6000};

  //==============================
  //TChiHZ model natural binning
  //==============================
  const int n_chi_bins_tchihz = 35;
  const double chi_bins_tchihz[n_chi_bins_tchihz+1] = {127,150,175,200,225,250,275,300,325,350,375,400,425,450,475,500,525,550,575,600,625,650,675,700,725,750,775,800,825,850,875,900,925,950,975,1000};

  const int n_met_bins_tchihz = 5;
  const double met_bins_tchihz[n_met_bins_tchihz+1] = {50, 100, 150, 250, 350, 6000};


  //==============================
  //TChiZZ model natural binning
  //==============================
  const int n_chi_bins_tchizz = 36;
  const double chi_bins_tchizz[n_chi_bins_tchizz+1] = {100,125,150,175,200,225,250,275,300,325,350,375,400,425,450,475,500,525,550,575,600,625,650,675,700,725,750,775,800,825,850,875,900,925,950,975,1000};

  const int n_met_bins_tchizz = 5;
  const double met_bins_tchizz[n_met_bins_tchizz+1] = {50, 100, 150, 250, 350, 6000};


  TH3D *susy_type1MET_btaglight_up, *susy_type1MET_btagheavy_up, *susy_type1MET_isr_up;

  TH3D *susy_type1MET_counts,*susy_type1MET_nowt;

  TH2D *susy_type1MET_btaglight_up_2d, *susy_type1MET_btagheavy_up_2d, *susy_type1MET_isr_up_2d;

  TH2D *susy_type1MET_counts_2d,*susy_type1MET_nowt_2d;

  if(conf->get("SUSY_Glu_LSP_scan") == "true"){

    cout<<"Setting up new hists for 2D mass gluino and mass LSP Scan"<<endl;

    const int *n_gluino_bins, *n_lsp_bins, *n_met_bins;
    const double *gluino_bins, *lsp_bins, *met_bins;

    if (conf->get("data_set") == "TChiWZ"){
      n_gluino_bins = &n_gluino_bins_tchiwz;
      n_lsp_bins = &n_lsp_bins_tchiwz;
      n_met_bins = &n_met_bins_tchiwz;

      gluino_bins = gluino_bins_tchiwz;
      lsp_bins = lsp_bins_tchiwz;
      met_bins = met_bins_tchiwz;
    }
    else if (conf->get("data_set") == "T5ZZ"){
      n_gluino_bins = &n_gluino_bins_t5zznat;
      n_lsp_bins = &n_lsp_bins_t5zznat;
      n_met_bins = &n_met_bins_t5zznat;

      gluino_bins = gluino_bins_t5zznat;
      lsp_bins = lsp_bins_t5zznat;
      met_bins = met_bins_t5zznat;
    }
    else{
      std::stringstream message;
      message<<"You have not chosen a valid dataset for the SUSY_Glu_LSP_scan, got: "<<conf->get("data_set")<<".";
      throw std::invalid_argument(message.str());
    }

    susy_type1MET_counts = new TH3D("susy_type1MET_counts", "(x,y,z) = (met, m_glu, m_lsp). Type1MET for"+g_sample_name, *n_met_bins, met_bins, *n_gluino_bins, gluino_bins, *n_lsp_bins, lsp_bins);
    susy_type1MET_counts->SetDirectory(rootdir);
    susy_type1MET_counts->Sumw2();

    susy_type1MET_nowt = new TH3D("susy_type1MET_nowt", "(x,y,z) = (met, m_glu, m_lsp). Type1MET with no event weights for"+g_sample_name, *n_met_bins, met_bins, *n_gluino_bins, gluino_bins, *n_lsp_bins, lsp_bins);
    susy_type1MET_nowt->SetDirectory(rootdir);
    susy_type1MET_nowt->Sumw2();

    susy_type1MET_btaglight_up = new TH3D("susy_type1MET_btaglight_up", " (x,y,z) = (met, m_glu, m_lsp). Type 1 MET with Light Btag SF fluctuated up for"+g_sample_name, *n_met_bins, met_bins, *n_gluino_bins, gluino_bins, *n_lsp_bins, lsp_bins);
    susy_type1MET_btaglight_up->SetDirectory(rootdir);
    susy_type1MET_btaglight_up->Sumw2();

    susy_type1MET_btagheavy_up = new TH3D("susy_type1MET_btagheavy_up", "(x,y,z) = (met, m_glu, m_lsp). Type 1 MET with Heavy Btag SF fluctuated up for"+g_sample_name, *n_met_bins, met_bins, *n_gluino_bins, gluino_bins, *n_lsp_bins, lsp_bins);
    susy_type1MET_btagheavy_up->SetDirectory(rootdir);
    susy_type1MET_btagheavy_up->Sumw2();

    susy_type1MET_isr_up = new TH3D("susy_type1MET_isr_up", "(x,y,z) = (met, m_glu, m_lsp). Type 1 MET with ISR SF fluctuated up for"+g_sample_name, *n_met_bins, met_bins, *n_gluino_bins, gluino_bins, *n_lsp_bins, lsp_bins);
    susy_type1MET_isr_up->SetDirectory(rootdir);
    susy_type1MET_isr_up->Sumw2();
  }

  else if(conf->get("SUSY_chi_scan") == "true"){

    cout<<"Setting up new hists for 1D mass Chi Scan"<<endl;

    const int *n_chi_bins, *n_met_bins;
    const double *chi_bins, *met_bins;

    if (conf->get("data_set") == "TChiHZ_TChiZZ" || conf->get("data_set") == "TChiHZ"){
      n_chi_bins = &n_chi_bins_tchihz;
      n_met_bins = &n_met_bins_tchihz;

      chi_bins = chi_bins_tchihz;
      met_bins = met_bins_tchihz;
    }
    else if (conf->get("data_set") == "TChiZZ"){
      n_chi_bins = &n_chi_bins_tchizz;
      n_met_bins = &n_met_bins_tchizz;

      chi_bins = chi_bins_tchizz;
      met_bins = met_bins_tchizz;
    }
    else{
      std::stringstream message;
      message<<"You have not chosen a valid dataset for the SUSY_chi_scan, got: "<<conf->get("data_set")<<".";
      throw std::invalid_argument(message.str());
    }

    susy_type1MET_counts_2d = new TH2D("susy_type1MET_counts", "(x,y) = (met, m_chi). Type1MET for"+g_sample_name, *n_met_bins, met_bins, *n_chi_bins, chi_bins);
    susy_type1MET_counts_2d->SetDirectory(rootdir);
    susy_type1MET_counts_2d->Sumw2();

    susy_type1MET_nowt_2d = new TH2D("susy_type1MET_nowt", "(x,y) = (met, m_chi). Type1MET with no event weights for"+g_sample_name, *n_met_bins, met_bins, *n_chi_bins, chi_bins);
    susy_type1MET_nowt_2d->SetDirectory(rootdir);
    susy_type1MET_nowt_2d->Sumw2();

    susy_type1MET_btaglight_up_2d = new TH2D("susy_type1MET_btaglight_up", "(x,y) = (met, m_chi). Type 1 MET with Light Btag SF fluctuated up for"+g_sample_name, *n_met_bins, met_bins, *n_chi_bins, chi_bins);
    susy_type1MET_btaglight_up_2d->SetDirectory(rootdir);
    susy_type1MET_btaglight_up_2d->Sumw2();

    susy_type1MET_btagheavy_up_2d = new TH2D("susy_type1MET_btagheavy_up", "(x,y) = (met, m_chi). Type 1 MET with Heavy Btag SF fluctuated up for"+g_sample_name, *n_met_bins, met_bins, *n_chi_bins, chi_bins);
    susy_type1MET_btagheavy_up_2d->SetDirectory(rootdir);
    susy_type1MET_btagheavy_up_2d->Sumw2();

    susy_type1MET_isr_up_2d = new TH2D("susy_type1MET_isr_up", "(x,y) = (met, m_chi). Type 1 MET with ISR SF fluctuated up for"+g_sample_name, *n_met_bins, met_bins, *n_chi_bins, chi_bins);
    susy_type1MET_isr_up_2d->SetDirectory(rootdir);
    susy_type1MET_isr_up_2d->Sumw2();

  }

  TH1D *dphi_gamma_MET, *dphi_gamma_MET100, *dphi_gamma_MET200, *dphi_gamma_MET300, *dphi_gamma_MET400, *dphi_gamma_MET500;
  TH1D *pt_gamma_MET100, *pt_gamma_MET200, *pt_gamma_MET300, *pt_gamma_MET400, *pt_gamma_MET500;

  if(conf->get("ECalTest") != ""){
    dphi_gamma_MET = new TH1D("dphi_gamma_met", "#Delta#Phi(#gamma, E^{miss}_{T}) for "+g_sample_name, 100,0,3.15);
    dphi_gamma_MET->SetDirectory(rootdir);
    dphi_gamma_MET->Sumw2();

    dphi_gamma_MET100 = new TH1D("dphi_gamma_met100", "#Delta#Phi(#gamma, E^{miss}_{T}) for "+g_sample_name+" with E^{miss}_{T} #geq 100", 100,0,3.15);
    dphi_gamma_MET100->SetDirectory(rootdir);
    dphi_gamma_MET100->Sumw2();

    dphi_gamma_MET200 = new TH1D("dphi_gamma_met200", "#Delta#Phi(#gamma, E^{miss}_{T}) for "+g_sample_name+" with E^{miss}_{T} #geq 200", 100,0,3.15);
    dphi_gamma_MET200->SetDirectory(rootdir);
    dphi_gamma_MET200->Sumw2();

    dphi_gamma_MET300 = new TH1D("dphi_gamma_met300", "#Delta#Phi(#gamma, E^{miss}_{T}) for "+g_sample_name+" with E^{miss}_{T} #geq 300", 100,0,3.15);
    dphi_gamma_MET300->SetDirectory(rootdir);
    dphi_gamma_MET300->Sumw2();

    dphi_gamma_MET400 = new TH1D("dphi_gamma_met400", "#Delta#Phi(#gamma, E^{miss}_{T}) for "+g_sample_name+" with E^{miss}_{T} #geq 400", 100,0,3.15);
    dphi_gamma_MET400->SetDirectory(rootdir);
    dphi_gamma_MET400->Sumw2();

    dphi_gamma_MET500 = new TH1D("dphi_gamma_met500", "#Delta#Phi(#gamma, E^{miss}_{T}) for "+g_sample_name+" with E^{miss}_{T} #geq 500", 100,0,3.15);
    dphi_gamma_MET500->SetDirectory(rootdir);
    dphi_gamma_MET500->Sumw2();

    pt_gamma_MET100 = new TH1D("pt_gamma_met100", "P_{T} for #gamma in "+g_sample_name+" with E^{miss}_{T} #geq 100", 6000,0,6000);
    pt_gamma_MET100->SetDirectory(rootdir);
    pt_gamma_MET100->Sumw2();

    pt_gamma_MET200 = new TH1D("pt_gamma_met200", "P_{T} for #gamma in "+g_sample_name+" with E^{miss}_{T} #geq 200", 6000,0,6000);
    pt_gamma_MET200->SetDirectory(rootdir);
    pt_gamma_MET200->Sumw2();

    pt_gamma_MET300 = new TH1D("pt_gamma_met300", "P_{T} for #gamma in "+g_sample_name+" with E^{miss}_{T} #geq 300", 6000,0,6000);
    pt_gamma_MET300->SetDirectory(rootdir);
    pt_gamma_MET300->Sumw2();

    pt_gamma_MET400 = new TH1D("pt_gamma_met400", "P_{T} for #gamma in "+g_sample_name+" with E^{miss}_{T} #geq 400", 6000,0,6000);
    pt_gamma_MET400->SetDirectory(rootdir);
    pt_gamma_MET400->Sumw2();

    pt_gamma_MET500 = new TH1D("pt_gamma_met500", "P_{T} for #gamma in "+g_sample_name+" with E^{miss}_{T} #geq 500", 6000,0,6000);
    pt_gamma_MET500->SetDirectory(rootdir);
    pt_gamma_MET500->Sumw2();
  }

  cout<<"Histograms initialized"<<endl;
  //cout<<__LINE__<<endl;
//===========================================
// Setup Stuff Pulled From External Files
//===========================================
  int eventsInFile;
  //Set up manual vertex reweighting.  
  if( conf->get("reweight") == "true" ){
    readyReweightHists();
  }
  if( conf->get("vpt_reweight") == "true" ){
    readyVPTReweight(savePath);
  }

  if(conf->get("pileup_reweight") == "true"){
    cout<<"Pileup reweighting with puWeight_Moriond2017.root"<<endl;
    g_pileup_hist_file = TFile::Open("auxFiles/puWeight_Moriond2017.root", "READ");
    //cout<<__LINE__<<endl;
    g_pileup_hist = (TH1D*)g_pileup_hist_file->Get("pileupWeight")->Clone("h_pileup_weight");
    //cout<<__LINE__<<endl;
    g_pileup_hist->SetDirectory(rootdir);
    //cout<<__LINE__<<endl;
    g_pileup_hist_file->Close();
  }

  //--------------------------------------------------------
  // 2D SUSY Scan ISR and BTag SF normalization Histograms
  //--------------------------------------------------------
  if(conf->get("susy_mc") == "true"){
    cout<<"Setting up normalization weights for ISR and Btag Scale Factors."<<endl;
    //cout<<__LINE__<<endl;
    if (conf->get("data_set") == "T5ZZ"){
      //cout<<__LINE__<<endl;
      g_SUSYsf_norm_file = TFile::Open("auxFiles/nsig_weights_t5zz.root", "READ");
    }
    else if (conf->get("data_set") == "TChiWZ"){
      //cout<<__LINE__<<endl;
      g_SUSYsf_norm_file = TFile::Open("auxFiles/nsig_weights_tchiwz.root", "READ");
    }
    else if (conf->get("data_set") == "TChiHZ"){
      //cout<<__LINE__<<endl;
      g_SUSYsf_norm_file = TFile::Open("auxFiles/nsig_weights_tchihz.root", "READ");
    }
    else if (conf->get("data_set") == "TChiHZ_TChiZZ"){
      //cout<<__LINE__<<endl;
      g_SUSYsf_norm_file = TFile::Open("auxFiles/nsig_weights_tchihz.root", "READ");
    }
    else if (conf->get("data_set") == "TChiZZ"){
      //cout<<__LINE__<<endl;
      g_SUSYsf_norm_file = TFile::Open("auxFiles/nsig_weights_tchizz.root", "READ");
    }
    else {
      //cout<<__LINE__<<endl;
      std::stringstream message;
      message<<"Can not pull normalization weights file for "<<conf->get("data_set")<<", no file configured for that dataset.";
      throw std::invalid_argument(message.str());
    }
    //cout<<__LINE__<<endl;

    g_isr_norm = (TH2D*)g_SUSYsf_norm_file->Get("h_avg_weight_isr")->Clone("h_isr_norm");
    //cout<<__LINE__<<endl;
    g_isr_norm_up = (TH2D*)g_SUSYsf_norm_file->Get("h_avg_weight_isr_UP")->Clone("h_isr_norm_up");
    //cout<<__LINE__<<endl;
    g_btagsf_norm = (TH2D*)g_SUSYsf_norm_file->Get("h_avg_weight_btagsf")->Clone("g_btagsf_norm");
    //cout<<__LINE__<<endl;
    g_btagsf_light_norm_up = (TH2D*)g_SUSYsf_norm_file->Get("h_avg_weight_btagsf_light_UP")->Clone("g_btagsf_light_norm_up");
    //cout<<__LINE__<<endl;
    g_btagsf_heavy_norm_up = (TH2D*)g_SUSYsf_norm_file->Get("h_avg_weight_btagsf_heavy_UP")->Clone("g_btagsf_heavy_norm_up");
    //cout<<__LINE__<<endl;
    

    g_isr_norm->SetDirectory(rootdir);
    //cout<<__LINE__<<endl;
    g_isr_norm_up->SetDirectory(rootdir);
    //cout<<__LINE__<<endl;
    g_btagsf_norm->SetDirectory(rootdir);
    //cout<<__LINE__<<endl;
    g_btagsf_light_norm_up->SetDirectory(rootdir);
    //cout<<__LINE__<<endl;
    g_btagsf_heavy_norm_up->SetDirectory(rootdir);
    //cout<<__LINE__<<endl;


    g_SUSYsf_norm_file->Close();
    //cout<<__LINE__<<endl;
  }

   
  //cout<<__LINE__<<endl;
  /*if( phys.isData() && conf->get("event_type")=="photon" ){
    cout<<"Pileup reweighting with "<<savePath+"L1PrescaleWeight_"+conf->get("signal_region")+".root"<<endl;
    g_l1prescale_file = TFile::Open(savePath+"L1PrescaleWeight_"+conf->get("signal_region")+".root", "READ");
    
    g_l1prescale_hist36 = (TH1D*)g_l1prescale_file->Get("rwt_nVert_HLT_Photon36_R9Id90_HE10_IsoM")->Clone("l1prescaleWeight36");
    g_l1prescale_hist36->SetDirectory(rootdir);

    g_l1prescale_hist30 = (TH1D*)g_l1prescale_file->Get("rwt_nVert_HLT_Photon30_R9Id90_HE10_IsoM")->Clone("l1prescaleWeight30");
    g_l1prescale_hist30->SetDirectory(rootdir);

    g_l1prescale_hist22 = (TH1D*)g_l1prescale_file->Get("rwt_nVert_HLT_Photon22_R9Id90_HE10_IsoM")->Clone("l1prescaleWeight22");
    g_l1prescale_hist22->SetDirectory(rootdir);

    g_l1prescale_file->Close();
  }*/
  //cout<<__LINE__<<endl;

  if( conf->get("rwt_photon_eff") == "true" ){
    cout<<"Reweighting for Effeciency with trigeff_Photon165_zmet2016.root"<<endl;
    TFile weight_eff_file("auxFiles/trigeff_Photon165_zmet2016.root", "READ");
    
    //barrel
    g_pt_eff_barrel = (TEfficiency*)weight_eff_file.Get("h_pt_eb_eff_jetht")->Clone("g_vpt_eff_barrel");
    g_pt_eff_barrel->SetDirectory(rootdir);

    //endcap
    g_pt_eff_endcap = (TEfficiency*)weight_eff_file.Get("h_pt_ee_eff_jetht")->Clone("g_vpt_eff_barrel");
    g_pt_eff_endcap->SetDirectory(rootdir);
    
    weight_eff_file.Close();
  }

  if( conf->get("rwt_muon_eff") == "true" ){
    cout<<"Reweighting for single muon trigger effeciency"<<endl;
    TFile weight_eff_file("/home/users/cwelke/analysis/CMSSW_8_0_22/V08-22-05/ZMET2015/makePlots/trigeff_Muon_pt_2016_withH.root", "READ");
    
    //barrel
    g_pt_eff_barrel = (TEfficiency*)weight_eff_file.Get("h_pt_denom_eb_ele27WPLoose_clone")->Clone("g_pt_eff_barrel");
    g_pt_eff_barrel->SetDirectory(rootdir);

    //endcap
    g_pt_eff_endcap = (TEfficiency*)weight_eff_file.Get("h_pt_denom_ee_ele27WPLoose_clone")->Clone("g_pt_eff_barrel");
    g_pt_eff_endcap->SetDirectory(rootdir);
    
    weight_eff_file.Close();
  }  

  //cout<<__LINE__<<endl;
  //set goodrun list
  if (conf->get("JSON") == "ICHEP"){
    const char* json_file = "auxFiles/golden_json_200716_12p9fb_snt.txt"; // ICHEP
    cout<<"Setting good run list: "<<json_file<<endl;
    set_goodrun_file(json_file);   
  }
  else if ((conf->get("JSON") == "18fb")){
    const char* json_file = "auxFiles/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON_unblind18_sorted_snt.txt"; // 18.1 fb
    cout<<"Setting good run list: "<<json_file<<endl;
    set_goodrun_file(json_file);
  }
  else{
    const char* json_file = "auxFiles/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON_snt.txt"; // 36.5 fb
    cout<<"Setting good run list: "<<json_file<<endl;
    set_goodrun_file(json_file);
  }

  //cout<<__LINE__<<endl;
  // Loop over events to Analyze
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();
  int eventCount=0;
  cout<<"DATASET: "<<conf->get("data_set")<<endl;
  if( nEvents >= 0 ) nEventsChain = nEvents;
  //cout<<__LINE__<<endl;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  //cout<<__LINE__<<endl;
  TIter fileIter(listOfFiles);
  //cout<<__LINE__<<endl;
//===========================================
// File Loop
//===========================================
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TFile file( currentFile->GetTitle() );
    //cout<<__LINE__<<endl;
    TTree *tree = (TTree*)file.Get("t");
    //cout<<__LINE__<<endl;
    if(fast) TTreeCache::SetLearnEntries(10); //What does this do?
    //cout<<__LINE__<<endl;
    if(fast) tree->SetCacheSize(128*1024*1024); //What does this do?
    //cout<<__LINE__<<endl;
    phys.Init(tree); //Loads in all the branches
    //cout<<__LINE__<<endl;
    eventsInFile = 0;
    //cout<<__LINE__<<endl;
    files_log<<"Running over new file: "<<currentFile->GetTitle()<<endl;
    cout<<"Running over new file: "<<currentFile->GetTitle()<<endl;
//===========================================
// Loop over Events in current file
//===========================================
    if( nEventsTotal >= nEventsChain ) continue;
    unsigned int nEventsTree = tree->GetEntriesFast();
    for( unsigned int event = 0; event < nEventsTree; ++event) {
      numEvents->Fill(0);
      // Get Event Content
      if( nEventsTotal >= nEventsChain ) continue;
      if(fast) tree->LoadTree(event);
      phys.GetEntry(event);
      ++nEventsTotal;
      //cout<<__LINE__<<endl;    
      // Progress
      ZMET2016::progress( nEventsTotal, nEventsChain );
      setupGlobals();
      //eventsInFile++;
      //if (eventsInFile > 100) continue;
      //cout<<__LINE__<<endl;
//===========================================
// Debugging And Odd Corrections Before Cuts
//===========================================
      
      // ----------------
      // DEBUG MODE
      // ----------------
      printStats = false;
      printFail = false;

      //if (inspection_set.count(phys.evt()) != 0){
      /*if ( inspection_set_erl.count(make_tuple(phys.evt(), phys.run(), phys.lumi())) != 0){
        cout<<"evt: "<<phys.evt()<<" run: "<<phys.run()<<" lumi: "<<phys.lumi()<<" scale1fb: "<<phys.evt_scale1fb()<<endl;
        inspection_copy.erase(make_tuple(phys.evt(), phys.run(), phys.lumi()));
        printStats=true;
        printFail=true;
      }*/
      /*else{ //Use if you don't want care about events in your list that are not in the other's
        continue;
      }*/
//===========================================
// Cuts
//===========================================
      //cout<<__LINE__<<endl;      
      //Set up event weight
      /*if (event % 10000 == 0){
        cout<<"Weight: "<<weight<<endl;
      }*/

      if (conf->get("ETH_Cleanse") != "false"){
          if (phys.isData() && conf->get("event_type") == "dilepton" && (! passETHDileptonDataCleanse()) ){
          //cout<<"Failed ETH Dilepton Data Cleanse"<<endl;
          continue;
        }
      }
      //cout<<__LINE__<<endl;

      if ( isDuplicate() ){
        //cout<<"Failed Duplicate"<<endl;
        numEvents->Fill(23);
        continue;
      } // check for duplicates
      //cout<<__LINE__<<endl;      

      if (! passFileSelections() ){
        //cout<<"Failed File Selections"<<endl;
        continue;
      }
      //cout<<__LINE__<<endl;

      if (! passBaseCut()){ 
        //cout<<"Failed Baseline"<<endl;
        continue; 
      }// Base Cut
      //cout<<__LINE__<<endl;      

      if (! hasGoodEvent()){
        //cout<<"Failed Good Event"<<endl;
        continue; // Event Type Specific Cuts
      }
      //cout<<__LINE__<<endl;      

      if (! passSignalRegionCuts()){ 
        //cout<<"Failed SR"<<endl;
        continue; // Signal Region Cuts
      }
      //cout<<__LINE__<<endl;

      if (conf->get("rare_real_MET_Z") == "true"){ 
        if ( ! passRareCuts() ){
          //cout<<"Failed Rare Cuts"<<endl;
          continue;
        } //Rare Sample Selections
      }

      if (conf->get("susy_mc") == "true"){
        if (! passSUSYSingalCuts()){
          //cout<<"Failed SUSY Signal"<<endl;
          continue;  
        }
      }
      
      if (conf->get("do_met_filters") != "false")
      {
        //cout<<"checking MET filters"<<endl;
        if (! passMETFilters()) continue; ///met filters
      }
      
      //double weight=1;
      double weight = getWeight();
      weight_log->Fill(log10(abs(weight)));
      weight_log_flat->Fill(abs(weight));

      if(conf->get("printEvtList") == "true"){
        // ----------------
        // DEBUG MODE
        // ----------------
        /*if (inspection_set_erl.count(make_tuple(phys.evt(), phys.run(), phys.lumi())) == 0){
          cout<<"NEW||evt: "<<phys.evt()<<" run: "<<phys.run()<<" lumi: "<<phys.lumi()<<" scale1fb: "<<phys.evt_scale1fb()<<" weight: "<<weight<<endl;
          cout<<"Inspection Set Count "<<inspection_set_erl.count(make_tuple(phys.evt(), phys.run(), phys.lumi()))<<endl;
        }*/
        //When Debug mode is off, you can turn this on:
        cout<<"evt: "<<phys.evt()<<" run: "<<phys.run()<<" lumi: "<<phys.lumi()<<" scale1fb: "<<phys.evt_scale1fb()<<" weight: "<<weight<<" extra_weight: "<< weight/phys.evt_scale1fb() <<endl;
      }
//===========================================
// Analysis Code
//===========================================
      //cout<<__LINE__<<endl;
      //cout<<"Event Weight "<<weight<<endl;      
      //Fill in Histos
      double sumMETFilters = phys.Flag_HBHENoiseFilter()+phys.Flag_HBHEIsoNoiseFilter()+phys.Flag_CSCTightHaloFilter()+phys.Flag_EcalDeadCellTriggerPrimitiveFilter()+phys.Flag_goodVertices()+phys.Flag_eeBadScFilter();
      //cout<<__LINE__<<endl;      
      numMETFilters->Fill(sumMETFilters);

      /*if (weight < 0){
         cout<<"Negative Weight2: "<<weight<<" "<<phys.evt()<<endl;
      }
      */
      if (g_met != 0) {
        t1met->Fill(g_met, weight);
        t1met_widebin->Fill(g_met, weight);
      }
      if (phys.met_rawPt() != 0) rawmet->Fill(phys.met_rawPt(), weight);
      if (g_ht != 0) {
        ht->Fill(g_ht, weight);
        ht_wide->Fill(g_ht, weight);
      }
      if (phys.gen_ht() != 0) gen_ht->Fill(phys.gen_ht(), weight);
      if (bosonPt() != 0){ 
        vpt->Fill(bosonPt(), weight); 
        vpt_flat->Fill(bosonPt(), weight); 
      }
      njets->Fill(g_njets, weight);
      nbtags_m->Fill(g_nBJetMedium, weight);
      nbtags_l->Fill(phys.nBJetLoose(), weight);
      nbtags_t->Fill(phys.nBJetTight(), weight);
      nVert->Fill(phys.nVert(), weight);
      nlep->Fill(phys.nlep(), weight);
      //cout<<"Filling nisotrack"<<endl;
      nisotrack->Fill(phys.nisoTrack_mt2(), weight);
      //cout<<__LINE__<<endl;
      if (g_mt2 != 0 ) mt2->Fill(g_mt2, weight);
      //cout<<__LINE__<<endl;
      if (g_mt2b != 0 ) mt2b->Fill(g_mt2b, weight);
      //cout<<__LINE__<<endl;
      if (g_njets > 0) dphi_jet1_met->Fill(acos(cos(g_met_phi - g_jets_p4.at(0).phi())), weight);
      //cout<<__LINE__<<endl;
      if (g_njets > 1) dphi_jet2_met->Fill(acos(cos(g_met_phi - g_jets_p4.at(1).phi())), weight);
      //cout<<__LINE__<<endl;
      if (conf->get("event_type") == "dilepton") dilmass->Fill(phys.dilmass(), weight);
//===========================================
// Signal Region Specific Histos
//===========================================
      double pt_bb;
      if (conf->get("signal_region") == "TChiHZ"){
        sum_mlb->Fill(phys.sum_mlb(), weight);

        double mt2_val_fromb;

        //cout<<__LINE__<<endl;
        if (conf->get("NBjets_loose_min") != "")
        {
          m_bb_csv->Fill(getMbb(), weight);
          m_bb_bpt->Fill(getMbb(), weight);  
          mt2_val_fromb = getMT2ForBjets(true);
          //cout<<__LINE__<<endl;
          mt2b->Fill(getMT2B(), weight);
          if (mt2_val_fromb != 0) mt2_fromb->Fill(mt2_val_fromb, weight);
        }
        else{
          m_bb_csv->Fill(g_mbb, weight);
          m_bb_bpt->Fill(phys.mbb_bpt(), weight);
          mt2_val_fromb = getMT2ForBjets();
          //cout<<__LINE__<<endl;
          if (mt2_val_fromb != 0) mt2_fromb->Fill(mt2_val_fromb, weight);
        }
    
        //cout<<__LINE__<<endl;

        if (phys.mt2j() != 0 ) mt2j->Fill(phys.mt2j(), weight);
        
        //cout<<__LINE__<<endl;

        double mt2_val_hz = 0;
        if (phys.nlep() > 1 && g_nBJetMedium >= 2) mt2_val_hz = getMT2HiggsZ();
        if (mt2_val_hz != 0) mt2_hz->Fill(mt2_val_hz, weight);

        //cout<<__LINE__<<endl;

        if (g_mt2 != 0 && g_mt2b != 0 ) MT2_MT2B->Fill(g_mt2, g_mt2b, weight);

        //cout<<__LINE__<<endl;

        if (g_mt2 != 0 && mt2_val_fromb != 0 ) MT2_MT2_fromb->Fill(g_mt2, mt2_val_fromb, weight);

        //cout<<__LINE__<<endl;

        if (g_mt2 != 0 && mt2_val_hz != 0 ) MT2_MT2_HZ->Fill(g_mt2, mt2_val_hz, weight);

        //cout<<__LINE__<<endl;

        if (g_nBJetMedium >= 2){
          pair<int,int> b_index = getMostBlike();
          
          //cout<<__LINE__<<endl;

          double bb_pt = (g_jets_p4.at(b_index.first) + g_jets_p4.at(b_index.second)).pt();
          sum_pt_z_bb->Fill(bb_pt+phys.dilpt(), weight);
        }
      }

      if (conf->get("signal_region") == "TChiWZ"){
        mjj_min_dphi->Fill(g_mjj_mindphi, weight);
      }
      //cout<<__LINE__<<endl;
      if(conf->get("signal_region") == "LeonoraXsecStudy"){
        
        if(phys.hyp_type() == 0){
          dilmass_ee->Fill(phys.dilmass(),weight);
          lep1_eta_ee->Fill(phys.lep_eta().at(0),weight);
          lep2_eta_ee->Fill(phys.lep_eta().at(1),weight);
        }
        else if (phys.hyp_type() == 1){
          dilmass_mm->Fill(phys.dilmass(),weight);
          lep1_eta_mm->Fill(phys.lep_eta().at(0),weight);
          lep2_eta_mm->Fill(phys.lep_eta().at(1),weight);
        }
    
        lep1_eta->Fill(phys.lep_eta().at(0),weight);
        lep2_eta->Fill(phys.lep_eta().at(1),weight);
    
      }

      //cout<<__LINE__<<endl;
      
      if (conf->get("GammaMuStudy") == "true"){
        MT_MuMET->Fill(getMTLepMET(),weight);
        dR_GammaMu->Fill(getdRGammaLep(),weight);
        mu_pt->Fill(phys.lep_pt().at(0), weight);
      }

      //cout<<__LINE__<<endl;

      if(conf->get("SUSY_Glu_LSP_scan") == "true"){
          //cout<<"mglu: "<<phys.mass_gluino()<<endl;
          //cout<<"mlsp: "<<phys.mass_LSP()<<endl;
          //cout<<"met: "<<g_met<<endl;
          //cout<<"weight: "<<weight<<endl;

          //cout<<__LINE__<<endl;

          susy_type1MET_counts->Fill(g_met, phys.mass_gluino(), phys.mass_LSP(), weight);
          susy_type1MET_nowt->Fill(g_met, phys.mass_gluino(), phys.mass_LSP(), 1);
          
          //cout<<__LINE__<<endl;

          double ISR_norm=1./g_isr_norm->GetBinContent(g_isr_norm->GetXaxis()->FindBin(phys.mass_gluino()), g_isr_norm->GetYaxis()->FindBin(phys.mass_LSP()));
          double btag_norm=1./g_btagsf_norm->GetBinContent(g_btagsf_norm->GetXaxis()->FindBin(phys.mass_gluino()), g_btagsf_norm->GetYaxis()->FindBin(phys.mass_LSP()));
         
          double ISR_norm_up=1./g_isr_norm_up->GetBinContent(g_isr_norm_up->GetXaxis()->FindBin(phys.mass_gluino()), g_isr_norm_up->GetYaxis()->FindBin(phys.mass_LSP()));
          
          double btag_heavy_norm_up=1./g_btagsf_heavy_norm_up->GetBinContent(g_btagsf_heavy_norm_up->GetXaxis()->FindBin(phys.mass_gluino()), g_btagsf_heavy_norm_up->GetYaxis()->FindBin(phys.mass_LSP()));
          double btag_light_norm_up=1./g_btagsf_light_norm_up->GetBinContent(g_btagsf_light_norm_up->GetXaxis()->FindBin(phys.mass_gluino()), g_btagsf_light_norm_up->GetYaxis()->FindBin(phys.mass_LSP()));
          
          susy_type1MET_btagheavy_up->Fill(g_met, phys.mass_gluino(), phys.mass_LSP(), (btag_heavy_norm_up/btag_norm)*weight*(phys.weight_btagsf_heavy_UP()/phys.weight_btagsf()));
          susy_type1MET_btaglight_up->Fill(g_met, phys.mass_gluino(), phys.mass_LSP(), (btag_light_norm_up/btag_norm)*weight*(phys.weight_btagsf_light_UP()/phys.weight_btagsf()));
          
          //cout<<__LINE__<<endl;
          
          if (conf->get("data_set") == "t5zz"){
            //isr_unc filled properly
            susy_type1MET_isr_up->Fill(g_met, phys.mass_gluino(), phys.mass_LSP(), (ISR_norm_up/ISR_norm)*(weight)*(1+(phys.isr_unc()/phys.isr_weight())) );
          }
          else{
            //isr_unc is just deviation from not using the scale factor
            susy_type1MET_isr_up->Fill(g_met, phys.mass_gluino(), phys.mass_LSP(), (1/ISR_norm)*(weight)*(1/phys.isr_weight()));
          }
      }
      else if(conf->get("SUSY_chi_scan") == "true"){
          //cout<<"mglu: "<<phys.mass_gluino()<<endl;
          //cout<<"mlsp: "<<phys.mass_LSP()<<endl;
          //cout<<"met: "<<g_met<<endl;
          //cout<<"weight: "<<weight<<endl;

          //cout<<__LINE__<<endl;

          susy_type1MET_counts_2d->Fill(g_met, phys.mass_chi(), weight);
          susy_type1MET_nowt_2d->Fill(g_met, phys.mass_chi(), 1);
          
          //cout<<__LINE__<<endl;

          double ISR_norm=1./g_isr_norm->GetBinContent(g_isr_norm->GetXaxis()->FindBin(phys.mass_chi()), 1);
          double btag_norm=1./g_btagsf_norm->GetBinContent(g_btagsf_norm->GetXaxis()->FindBin(phys.mass_chi()), 1);
         
          double ISR_norm_up=1./g_isr_norm_up->GetBinContent(g_isr_norm_up->GetXaxis()->FindBin(phys.mass_chi()), 1);
          
          double btag_heavy_norm_up=1./g_btagsf_heavy_norm_up->GetBinContent(g_btagsf_heavy_norm_up->GetXaxis()->FindBin(phys.mass_chi()), 1);
          double btag_light_norm_up=1./g_btagsf_light_norm_up->GetBinContent(g_btagsf_light_norm_up->GetXaxis()->FindBin(phys.mass_chi()), 1);
          
          susy_type1MET_btagheavy_up_2d->Fill(g_met, phys.mass_chi(), (btag_heavy_norm_up/btag_norm)*weight*(phys.weight_btagsf_heavy_UP()/phys.weight_btagsf()));
          susy_type1MET_btaglight_up_2d->Fill(g_met, phys.mass_chi(), (btag_light_norm_up/btag_norm)*weight*(phys.weight_btagsf_light_UP()/phys.weight_btagsf()));
          
          //cout<<__LINE__<<endl;
          //isr_unc is just deviation from not using the scale factor
          susy_type1MET_isr_up_2d->Fill(g_met, phys.mass_chi(),(1/ISR_norm)*(weight)*(1/phys.isr_weight()));
      }

      if(conf->get("ECalTest") != ""){
        double dphi_gm = acos(cos(g_met_phi - phys.gamma_p4().at(0).phi()));
        dphi_gamma_MET->Fill(dphi_gm, weight); 

          dphi_gamma_MET100->Fill(dphi_gm, weight);
        if (g_met >= 100){
          pt_gamma_MET100->Fill(bosonPt(), weight);  
        }
        if (g_met >= 200){
          dphi_gamma_MET200->Fill(dphi_gm, weight);
          pt_gamma_MET200->Fill(bosonPt(), weight);  
        }
        if (g_met >= 300){
          dphi_gamma_MET300->Fill(dphi_gm, weight);
          pt_gamma_MET300->Fill(bosonPt(), weight);  
        }
        if (g_met >= 400){
          dphi_gamma_MET400->Fill(dphi_gm, weight);
          pt_gamma_MET400->Fill(bosonPt(), weight);  
        }
        if (g_met >= 500){
          dphi_gamma_MET500->Fill(dphi_gm, weight);
          pt_gamma_MET500->Fill(bosonPt(), weight);  
        }
      }

      //cout<<__LINE__<<endl;
//===========================================
// Debugging And Odd Corrections After Cuts
//===========================================
      /*if (conf->get("rares") == "true"){
        //cout<<__LINE__<<endl;
        //cout<<"EVENT-LIST "<<eventCount<<" : "<<phys.evt()<<endl;
          //cout<<__LINE__<<endl;
        cout<<"EVENT-LIST "<<eventCount<<" : "<<phys.evt()<<" "<<g_met<<endl;
        eventCount++;
        if ( inVinceNotMine.count(phys.evt()) != 0){
          //printPass = true;
        }
        if ( inMineNotVince.count(phys.evt()) != 0){
          printFail = true;
        }
      }*/
      eventCount++;

      /*if ( inVinceNotMine.count(phys.evt()) != 0){
        printFail = true;
        cout<<"Event passed: "<<phys.evt()<<endl;
      }*/
      
      //if (printStats) {cout<<"Event: "<<phys.evt()<<endl;}
      //cout<<"Event_Run_Lumi: "<<phys.evt()<<" "<<phys.run()<<" "<<phys.lumi()<<endl;
      //if(g_met >= 300){
      //  cout<<"Event: "<<phys.evt()<<" MET: "<<g_met<<" g_njets: "<<g_njets<<" nbtags: "<<g_nBJetMedium<<" HT: "<<g_ht<<endl;
      //}
    }
    // Clean Up
    //cout<<__LINE__<<endl;
    delete tree;
    //cout<<__LINE__<<endl;
    file.Close();
  }

  // ----------------
  // DEBUG MODE
  // ----------------
  /*cout<<"Events that weren't in your babies:"<<endl;
  for (set<tuple<long,long,long>>::iterator it=inspection_copy.begin(); it!=inspection_copy.end(); ++it){
    cout<<"evt: "<<std::get<0>(*it)<<" run: "<<std::get<1>(*it)<<" lumi: "<<std::get<2>(*it)<<endl;
  }*/

  cout<<"Num events passed: "<<eventCount<<endl;
  files_log<<"Num events passed: "<<eventCount<<endl;
  if ( nEventsChain != nEventsTotal ) {
    cout << Form( "ERROR: number of events from files (%d) is not equal to total number of events (%d)", nEventsChain, nEventsTotal ) << endl;
  }  
  //cout<<__LINE__<<endl;
//=======================================
// Write Out Histos
//=======================================
  output->cd();

  //Write out histograms to file
  numEvents->Write();
  //cout<<__LINE__<<endl;
  weight_log->Write();
  //cout<<__LINE__<<endl;
  weight_log_flat->Write();
  //cout<<__LINE__<<endl;
  numMETFilters->Write();
  //cout<<__LINE__<<endl;
  t1met->Write();
  //cout<<__LINE__<<endl;
  t1met_widebin->Write();
  //cout<<__LINE__<<endl;
  rawmet->Write();
  //cout<<__LINE__<<endl;
  ht->Write();
  //cout<<__LINE__<<endl;
  ht_wide->Write();
  //cout<<__LINE__<<endl;
  gen_ht->Write();
  //cout<<__LINE__<<endl;
  vpt->Write();
  //cout<<__LINE__<<endl;
  vpt_flat->Write();
  //cout<<__LINE__<<endl;
  njets->Write();
  //cout<<__LINE__<<endl;
  nlep->Write();
  //cout<<__LINE__<<endl;
  nisotrack->Write();
  //cout<<__LINE__<<endl;
  nbtags_m->Write();
  //cout<<__LINE__<<endl;
  nbtags_l->Write();
  //cout<<__LINE__<<endl;
  nbtags_t->Write();
  //cout<<__LINE__<<endl;
  nVert->Write();
  //cout<<__LINE__<<endl;
  mt2->Write();
  //cout<<__LINE__<<endl;
  mt2b->Write();
  //cout<<__LINE__<<endl;
  dphi_jet1_met->Write();
  //cout<<__LINE__<<endl;
  dphi_jet2_met->Write();
  //cout<<__LINE__<<endl;
  dilmass->Write();
  //cout<<__LINE__<<endl;

  if (conf->get("signal_region") == "TChiHZ"){
    sum_mlb->Write();
    //cout<<__LINE__<<endl;
    m_bb_csv->Write();
    //cout<<__LINE__<<endl;
    m_bb_bpt->Write();
    //cout<<__LINE__<<endl;
    mt2j->Write();
    //cout<<__LINE__<<endl;
    mt2_fromb->Write();
    //cout<<__LINE__<<endl;
    mt2_hz->Write();
    //cout<<__LINE__<<endl;
    sum_pt_z_bb->Write();
    //cout<<__LINE__<<endl;

    //2D hists
    MT2_MT2B->Write();
    //cout<<__LINE__<<endl;
    MT2_MT2_fromb->Write();
    //cout<<__LINE__<<endl;
    MT2_MT2_HZ->Write();
    //cout<<__LINE__<<endl;
  }

  if (conf->get("signal_region") == "TChiWZ"){
    mjj_min_dphi->Write();
  }

  if(conf->get("signal_region") == "LeonoraXsecStudy"){
    dilmass_ee->Write();
    lep1_eta_ee->Write();
    lep2_eta_ee->Write();
    dilmass_mm->Write();
    lep1_eta_mm->Write();
    lep2_eta_mm->Write();
    lep1_eta->Write();
    lep2_eta->Write();
  }

  if (conf->get("GammaMuStudy") == "true"){
    MT_MuMET->Write();
    //cout<<__LINE__<<endl;
    dR_GammaMu->Write();
    //cout<<__LINE__<<endl;
    mu_pt->Write();
    //cout<<__LINE__<<endl;
  }
  if(conf->get("SUSY_Glu_LSP_scan") == "true"){
    susy_type1MET_counts->Write();
    susy_type1MET_nowt->Write();

    susy_type1MET_btagheavy_up->Write();
    susy_type1MET_btaglight_up->Write();

    susy_type1MET_isr_up->Write();
  }
  if(conf->get("SUSY_chi_scan") == "true"){
    susy_type1MET_counts_2d->Write();
    susy_type1MET_nowt_2d->Write();

    susy_type1MET_btagheavy_up_2d->Write();
    susy_type1MET_btaglight_up_2d->Write();

    susy_type1MET_isr_up_2d->Write();
  }
  if(conf->get("ECalTest") != ""){
    dphi_gamma_MET->Write();
    dphi_gamma_MET100->Write();
    dphi_gamma_MET200->Write();
    dphi_gamma_MET300->Write();
    dphi_gamma_MET400->Write();
    dphi_gamma_MET500->Write();

    pt_gamma_MET100->Write();
    pt_gamma_MET200->Write();
    pt_gamma_MET300->Write();
    pt_gamma_MET400->Write();
    pt_gamma_MET500->Write();
  }

  //close output file
  output->Write();
  output->Close();
  g_reweight_pairs.clear();
  files_log.close();
  //cout<<__LINE__<<endl;
  // return
  bmark->Stop("benchmark");
  cout << endl;
  cout << num_events_veto_ttbar << " Events Vetod from TTBar" << endl;
  cout << num_events_veto_ttgamma << " Events Vetod from TTGamma" << endl;
  cout << nEventsTotal << " Events Processed" << endl;
  cout << nDuplicates << " Duplicates" << endl;
  cout << "------------------------------" << endl;
  cout << "CPU  Time:	" << Form( "%.01f", bmark->GetCpuTime("benchmark")  ) << endl;
  cout << "Real Time:	" << Form( "%.01f", bmark->GetRealTime("benchmark") ) << endl;
  cout << endl;
  
  //====================
  // Clean Everything Up
  //====================
  delete bmark;
  /*delete output;
  delete numEvents;
  delete weight_log;
  delete weight_log_flat;
  delete numMETFilters;
  delete t1met;
  delete t1met_widebin;
  delete rawmet;
  delete ht;
  delete ht_wide;
  delete gen_ht;
  delete vpt;
  delete vpt_flat;
  delete njets;
  delete nlep;
  delete nisotrack;
  delete nbtags_m;
  delete nbtags_l;
  delete nbtags_t;
  delete nVert;
  delete mt2;
  delete mt2b;
  delete dphi_jet1_met;
  delete dphi_jet2_met;
  delete dilmass;
  if (conf->get("signal_region") == "TChiHZ"){
    delete sum_mlb;
    delete m_bb_csv;
    delete m_bb_bpt;
    delete mt2j;
    delete mt2_fromb;
    delete mt2_hz;
    delete sum_pt_z_bb;
    //2D hists
    delete MT2_MT2B;
    delete MT2_MT2_fromb;
    delete MT2_MT2_HZ;
  }
  if (conf->get("GammaMuStudy") == "true"){
    delete MT_MuMET;
    delete dR_GammaMu;
    delete mu_pt;
  }*/

  return 0;
}
