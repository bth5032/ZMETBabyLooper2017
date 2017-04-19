#include "TChain.h"
#include "TString.h"
#include "ConfigHelper.C"

void addToChain(TChain *ch, TString set, bool hadoop=false, bool skimmed=true) {


//====================================
// DY MC
//====================================
  //---------------------------
  // DY Skims
  //---------------------------
  if (set == "ZMC-inclusive-Skimmed"){
    //Inclusive HT
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";

    cout<<"Adding ZMC-inclusive-Skimmed"<<endl;
    
    ch->Add(dir+"dy_m50_mgmlm_ext1*");
  }
  else if (set == "ZMC-100200-Skimmed"){
    //HT 100-200 
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";

    cout<<"Adding ZMC-100200-Skimmed"<<endl;
    
    ch->Add(dir+"dy_m50_mgmlm_ht100_ext1*");
  }
  else if (set == "ZMC-200400-Skimmed"){
    //HT 200-400 
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";
    
    cout<<"Adding ZMC-200400-Skimmed"<<endl;
    
    ch->Add(dir+"dy_m50_mgmlm_ht200_ext1*");
  }
  else if (set == "ZMC-400600-Skimmed"){
    //HT 400-600 
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";

    cout<<"Adding ZMC-400600-Skimmed"<<endl;
    
    ch->Add(dir+"dy_m50_mgmlm_ht400_ext1*");
  }
  else if (set == "ZMC-600Inf-Skimmed"){
    //HT 600-Inf 
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";

    cout<<"Adding ZMC-600Inf-Skimmed"<<endl;
    
    ch->Add(dir+"dy_m50_mgmlm_ht600_nonext*");
    ch->Add(dir+"dy_m50_mgmlm_ht800_nonext*");
    ch->Add(dir+"dy_m50_mgmlm_ht1200_nonext*");
    ch->Add(dir+"dy_m50_mgmlm_ht2500_nonext*");
  }
  else if (set == "ZMC-fullmc-Skimmed"){
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";

    cout<<"Adding ZMC-fullmc-Skimmed"<<endl;
    
    //ZZ Sample
    ch->Add(dir+"zz_2l2q_powheg*" );
    //WZ Sample
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/wz_2l2q_amcnlo*.root");
  }
  else if (set == "ZMC-low-mass-Skimmed"){
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";

    cout<<"Adding ZMC Low Mass Skimmed sample"<<endl;
    
    ch->Add(dir+"dy_m1050_mgmlm*");
  }
  else if (set == "ZMC-V08-22-08-Skimmed"){
    cout<<"Adding ZMC V08-22-08 Skimmed sample"<<endl;
    
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/dy_m50_mgmlm_ext1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/dy_m50_mgmlm_ht100_nonext*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/dy_m50_mgmlm_ht200_ext1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/dy_m50_mgmlm_ht400_ext1*"); 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/dy_m50_mgmlm_ht600_nonext*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/dy_m50_mgmlm_ht800_nonext*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/dy_m50_mgmlm_ht1200_nonext*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/dy_m50_mgmlm_ht2500_nonext*");

    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/dy_m1050_mgmlm*.root");

    //ZZ Sample
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/zz_2l2q_amcnlo*.root" );
    //WZ Sample
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/wz_2l2q_amcnlo*.root" );
  }
  else if (set == "ZMC-V08-22-13-Skimmed"){
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-11/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-13/skims/";

    cout<<"Adding ZMC V08-22-13 Skimmed sample"<<endl;
    
    ch->Add(dir+"dy_m50_mgmlm_ext1*");
    ch->Add(dir+"dy_m50_mgmlm_ht100_ext1*");
    ch->Add(dir+"dy_m50_mgmlm_ht200_ext1*");
    ch->Add(dir+"dy_m50_mgmlm_ht400_ext1*"); 
    ch->Add(dir+"dy_m50_mgmlm_ht600_nonext*");
    ch->Add(dir+"dy_m50_mgmlm_ht800_nonext*");
    ch->Add(dir+"dy_m50_mgmlm_ht1200_nonext*");
    ch->Add(dir+"dy_m50_mgmlm_ht2500_nonext*");

    ch->Add(dir+"dy_m1050_mgmlm*.root");

    //ZZ Sample
    ch->Add(dir+"zz_2l2q_amcnlo*.root" );
    //WZ Sample
    ch->Add(dir+"wz_2l2q_amcnlo*.root" );
  }
  else if (set == "ZMC-V08-22-16-Skimmed"){
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";

    cout<<"Adding ZMC V08-22-16 Skimmed sample"<<endl;
    
    ch->Add(dir+"dy_m50_mgmlm_ext1*");
    ch->Add(dir+"dy_m50_mgmlm_ht100_ext1*");
    ch->Add(dir+"dy_m50_mgmlm_ht200_ext1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-17/skims/dy_m50_mgmlm_ht400_ext1*"); 
    //ch->Add(dir+"dy_m50_mgmlm_ht400_ext1*"); 
    ch->Add(dir+"dy_m50_mgmlm_ht600_nonext*");
    ch->Add(dir+"dy_m50_mgmlm_ht800_nonext*");
    ch->Add(dir+"dy_m50_mgmlm_ht1200_nonext*");
    ch->Add(dir+"dy_m50_mgmlm_ht2500_nonext*");

    ch->Add(dir+"dy_m1050_mgmlm*.root");

    //ZZ Sample
    ch->Add(dir+"zz_2l2q_amcnlo*.root" );
    //WZ Sample
    ch->Add(dir+"wz_2l2q_amcnlo*.root" );
  }
  else if (set == "ZMC-DY-LeonoraSync"){
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-11/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-13/skims/";

    cout<<"Adding ZMC for Leonora Sync ()"<<endl;
    
    ch->Add(dir+"dy_m1050_amcnlo*");
    ch->Add(dir+"dy_m50_amcnlo*");
   
    //ZZ Sample
    ch->Add(dir+"zz_4l_powheg*" );
  }

//====================================
// SUSY MC
//====================================

  else if (set == "TChiHZ"){
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-17/skims/";
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";

    //TChiHZ Signal
    cout<<"Adding TChiHZ Monte Carlo"<<endl;
    ch->Add(dir+"tchihz_80x_v2*");
  }
  else if (set == "TChiWZ"){
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    
    //TChiHZ Signal
    cout<<"Adding TChiWZ Monte Carlo"<<endl;
    ch->Add(dir+"tchiwz_80x_v2*");
  }
  else if (set == "TChiZZ"){
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-17/skims/";
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    
    //TChiHZ Signal
    cout<<"Adding TChiZZ Monte Carlo"<<endl;
    ch->Add(dir+"tchizz_80x_v2*");
  }
  else if (set == "T5ZZ"){
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    
    //T5ZZ Signal
    cout<<"Adding T5ZZ Monte Carlo"<<endl;
    ch->Add(dir+"t5zz_orig_80x_v2_skim_*");
    //ch->Add("/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-11/skim/t5zz_orig_80x_v2_skim_3.root");
    ch->Add(dir+"t5zz_mg1850_80x_v2_skim_*"); 
  }

//====================================
// Gamma Jets MC
//====================================

  else if (set == "GammaMC-40100"){
    //HT 40-100
    cout<<"Adding GammaMC-40100"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/gjetsht40_dr0p05_mgmlm*");
  }
  else if (set == "GammaMC-100200"){
    //HT 100-200 
    cout<<"Adding GammaMC-100200"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/gjetsht100_dr0p05_mgmlm*");
  }
  else if (set == "GammaMC-200400"){
    //HT 200-400
    cout<<"Adding GammaMC-200400"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/gjetsht200_dr0p05_mgmlm*");
  }
  else if (set == "GammaMC-400600"){
    //HT 400-600 
    cout<<"Adding GammaMC-400600"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/gjetsht400_dr0p05_mgmlm*");
  }
  else if (set == "GammaMC-600Inf"){
    //HT 600-Inf 
    cout<<"Adding GammaMC-600+"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/gjetsht600_dr0p05_mgmlm*");
  }
  //---------------------------
  // Gamma Jets HT Binned Skims
  //---------------------------
  else if (set == "GammaMC-40100-Skimmed"){
    //HT 40-100
    cout<<"Adding Skimmed GammaMC-40100"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/gjetsht40_dr0p05_mgmlm*");
  }
  else if (set == "GammaMC-100200-Skimmed"){
    //HT 100-200 
    cout<<"Adding Skimmed GammaMC-100200"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/gjetsht100_dr0p05_mgmlm*");
  }
  else if (set == "GammaMC-200400-Skimmed"){
    //HT 200-400
    cout<<"Adding Skimmed GammaMC-200400"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/gjetsht200_dr0p05_mgmlm*");
  }
  else if (set == "GammaMC-400600-Skimmed"){
    //HT 400-600 
    cout<<"Adding Skimmed GammaMC-400600"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/gjetsht400_dr0p05_mgmlm*");
  }
  else if (set == "GammaMC-600Inf-Skimmed"){
    //HT 600-Inf 
    cout<<"Adding Skimmed GammaMC-600+"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/gjetsht600_dr0p05_mgmlm*");
  }
  //---------------------------
  // dr0p4
  //---------------------------
  else if (set == "GammaMC-40100-dr0p4-Skimmed"){
    //HT 40-100
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";

    cout<<"Adding Skimmed GammaMC-40100"<<endl; 
    
    ch->Add(dir+"gjetsht40_dr0p4_mgmlm*");
  }
  else if (set == "GammaMC-100200-dr0p4-Skimmed"){
    //HT 100-200 
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";

    cout<<"Adding Skimmed GammaMC-100200"<<endl; 
    
    ch->Add(dir+"gjetsht100_dr0p4_mgmlm*");
  }
  else if (set == "GammaMC-200400-dr0p4-Skimmed"){
    //HT 200-400
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";

    cout<<"Adding Skimmed GammaMC-200400"<<endl; 
    
    ch->Add(dir+"gjetsht200_dr0p4_mgmlm*");
  }
  else if (set == "GammaMC-400600-dr0p4-Skimmed"){
    //HT 400-600 
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";

    cout<<"Adding Skimmed GammaMC-400600"<<endl; 
    
    ch->Add(dir+"gjetsht400_dr0p4_mgmlm*");
  }
  else if (set == "GammaMC-600Inf-dr0p4-Skimmed"){
    //HT 600-Inf 
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";
    
    cout<<"Adding Skimmed GammaMC-600+"<<endl; 
    
    ch->Add(dir+"gjetsht600_dr0p4_mgmlm*");
  }
  //---------------------------
  // Other Gamma MC
  //---------------------------
  else if (set == "GammaMC-justWJets"){
    cout<<"Adding GammaMC-wjets-inclusive"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_incl_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht100_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht200_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht400_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht600_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht800_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht1200_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht2500_mgmlm*");
  }
  else if (set == "GammaMC-WGamma"){
    //This set combines WJets with WGJets and is intended to take only prompt photons from WGjets and only non-prompt photons from WJets

    cout<<"Adding GammaMC-WGamma"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_incl_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht100_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht200_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht400_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht600_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht800_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht1200_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht2500_mgmlm*");

    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wgjets_incl_mgmlm*");
  }
  else if (set == "GammaMC-WGamma-Skimmed"){
    //This set combines WJets with WGJets and is intended to take only prompt photons from WGjets and only non-prompt photons from WJets
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";

    cout<<"Adding Skimmed GammaMC-WGamma"<<endl; 
    
    ch->Add(dir+"wjets_incl_mgmlm*");
    ch->Add(dir+"wjets_ht100_mgmlm*");
    ch->Add(dir+"wjets_ht200_mgmlm*");
    ch->Add(dir+"wjets_ht400_mgmlm*");
    ch->Add(dir+"wjets_ht600_mgmlm*");
    ch->Add(dir+"wjets_ht800_mgmlm*");
    ch->Add(dir+"wjets_ht1200_mgmlm*");
    ch->Add(dir+"wjets_ht2500_mgmlm*");

    ch->Add(dir+"wgjets_incl_mgmlm*");
    ch->Add(dir+"wgjets_ptg40_mgmlm*");
    ch->Add(dir+"wgjets_ptg130_mgmlm*");
  }
  else if (set == "GammaMC-gammaZtoNuNu"){
    cout<<"Adding GammaMC-gammaZtoNuNu"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/znunugamma*.root");
  }
  else if (set == "GammaMC-gammaZtoNuNu-Skimmed"){
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";

    cout<<"Adding Skimmed GammaMC-gammaZtoNuNu"<<endl; 
    
    ch->Add(dir+"znunugamma_ptg40*");
    ch->Add(dir+"znunugamma_ptg130*");
  }
  else if (set == "GammaMC-wgjets-inclusive-amc"){
    cout<<"Adding GammaMC WGJets AMC@NLO Sample"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wgjets_incl_amcnlo*.root");
  }
  else if (set == "GammaMC-wgjets-inclusive-mgmlm"){
    cout<<"Adding GammaMC WGJets Madgraph Sample"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wgjets_incl_mgmlm*.root");
  }
  else if (set == "GammaMC-GammaRealMET"){
    cout<<"Adding GammaMC-GammaRealMET"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltbr_mgmlm.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltop_mgmlm.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wgjets_incl_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht100_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht1200_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht200_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht2500_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht400_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht600_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht800_amcnlo.root");
  }

//====================================
// Flavor Symmetric MC
//====================================
  else if (set == "FSMC-dilep-mgmlm"){
    cout<<"Adding FSMC-dilep-mgmlm (smaller stats sample)"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_dilep_mgmlm*.root");
  }
  else if (set == "FSMC-dilep-mgmlm-Skimmed"){
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";

    cout<<"Adding FSMC-dilep-mgmlm-Skimmed (smaller stats sample)"<<endl; 
    
    ch->Add(dir+"ttbar_dilep_mgmlm_ext1_*");
  }
  else if (set == "FSMC-ttbar-dilep-mgmlm-LeonoraSync"){
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";

    cout<<"Adding TTBar to dilep madgraph sample for Leonora Sync (has ext1 and nonext)"<<endl; 
    
    ch->Add(dir+"ttbar_dilep_mgmlm_ext1_*");
    ch->Add(dir+"ttbar_dilep_mgmlm_nonext_*");
  }
  else if (set == "FSMC-dilep-powheg"){
    cout<<"Adding FSMC-dilep-powheg (large stats sample)"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_dilep_powheg*.root"); //larger stats
  }
  else if (set == "FSMC-dilep-powheg-Skimmed"){
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";

    cout<<"Adding Skimmed FSMC-dilep-powheg (large stats sample)"<<endl; 
    
    ch->Add(dir+"ttbar_dilep_powheg*"); //larger stats
  }
  else if (set == "FSMC-TTBar-NoPromptGamma"){
    cout<<"Adding FSMC-TTBar-NoPromptGamma (large stats sample)"<<endl; 
    //Take non-prompt photon events
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-09/skims/ttbar_dilep_powheg*.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-09/skims/ttbar_1ltbr_mgmlm*.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-09/skims/ttbar_1ltop_mgmlm*.root");
  }
  else if (set == "FSMC-TTGamma-NoNonPromptGamma"){
    cout<<"Adding FSMC-TTGamma-NoNonPromptGamma"<<endl; 
    //Take prompt photon events
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-09/skims/ttg_incl_amcnlo*.root");
  }
  else if (set == "FSMC-1lep"){
    TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/output/";
    
    cout<<"Adding FSMC-1lep"<<endl; 
    
    ch->Add("/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/output/ttbar_1ltbr_mgmlm_ext1*");
    ch->Add("/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/output/ttbar_1ltop_mgmlm_ext1*");
  }
  else if (set == "FSMC-1lep-Skimmed"){
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";

    cout<<"Adding skimmed FSMC-1lep"<<endl; 
    
    ch->Add(dir+"ttbar_1ltbr_mgmlm_ext1*");
    ch->Add(dir+"ttbar_1ltop_mgmlm_ext1*");
  }
  else if (set == "FSMC-TTto1Lep-LeonoraSync"){
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";

    cout<<"Adding TTBar to 1lep sample for Leonora Sync (combo of ext1 and nonext)"<<endl; 
    
    ch->Add(dir+"ttbar_1ltbr_mgmlm_ext1*");
    ch->Add(dir+"ttbar_1ltbr_mgmlm_nonext*");

    ch->Add(dir+"ttbar_1ltop_mgmlm_ext1*");
    ch->Add(dir+"ttbar_1ltop_mgmlm_nonext*");
  }
  else if (set == "FSMC-singletop-inclusive"){
    cout<<"Adding FSMC-singletop"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/sttw_antitop_powheg*.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/sttw_top_powheg*.root");
  }
  else if (set == "FSMC-singletop-nofullhaddecay"){
    cout<<"Adding FSMC-singletop with no fully hadronic decay"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/sttw_antitop_nofullhaddecay_powheg.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/sttw_top_nofullhaddecay_powheg.root");
  }
  else if (set == "FSMC-singletop-nofullhaddecay-Skimmed"){
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";

    cout<<"Adding Skimmed FSMC-singletop with no fully hadronic decay"<<endl; 
    
    ch->Add(dir+"sttw_antitop_nofullhaddecay_powheg*");
    ch->Add(dir+"sttw_top_nofullhaddecay_powheg*");
  }
  //cout<<"Entries: "<<ch_fs->GetEntries()<<endl;
  else if (set == "FSMC-ww"){
    TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/output/";

    cout<<"Adding FSMC-ww"<<endl; 
    
    ch->Add(dir+"ww_2l2nu_powheg*.root");
    ch->Add(dir+"www_incl_amcnlo*.root");
  }
  else if (set == "FSMC-ww-Skimmed"){
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";

    cout<<"Adding FSMC-ww-Skimmed"<<endl; 
    
    ch->Add(dir+"ww_2l2nu_powheg*");
    ch->Add(dir+"www_incl_amcnlo*");
  }
  else if (set == "FSMC-ttw"){
    cout<<"Adding FSMC-ttw"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttw_ln_amcnlo*.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttw_qq_amcnlo*.root");
  }
  else if (set == "FSMC-ttw-Skimmed"){
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";
    
    cout<<"Adding FSMC-ttw-Skimmed"<<endl; 
    
    ch->Add(dir+"ttw_ln_amcnlo*");
    ch->Add(dir+"ttw_qq_amcnlo*");
  }

//====================================
// Rare MC
//====================================

  else if (set == "RareMC-wz"){
    cout<<"Adding RareMC-wz"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wz_3lnu_powheg*.root");
  }
  else if (set == "RareMC-zz"){
    cout<<"Adding RareMC-zz"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/zz_2l2n_powheg*.root");
  }
  else if (set == "RareMC-vvv"){
    cout<<"Adding RareMC-vvv"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wwz_incl_amcnlo*.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wzz_incl_amcnlo*.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/zzz_incl_amcnlo*.root");
  }
  else if (set == "RareMC-ttz"){
    cout<<"Adding RareMC-ttz"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttz_2l2n_amcnlo_ext*.root");
  } 
  else if (set == "RareMC-wz-Skimmed"){
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";
    cout<<"Adding RareMC-wz skim"<<endl; 
    ch->Add(dir+"wz_3lnu_powheg*");
  }
  else if (set == "RareMC-zz-Skimmed"){
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";
    cout<<"Adding RareMC-zz skim"<<endl; 
    ch->Add(dir+"zz_2l2n_powheg*");
  }
  else if (set == "RareMC-vvv-Skimmed"){
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";
    cout<<"Adding RareMC-vvv skim"<<endl; 
    ch->Add(dir+"wwz_incl_amcnlo*");
    ch->Add(dir+"wzz_incl_amcnlo*");
    ch->Add(dir+"zzz_incl_amcnlo*");
    ch->Add(dir+"tzq_ll_amcnlo*");
    ch->Add(dir+"sttwll_madgraph*");
  }
  else if (set == "RareMC-ttz-Skimmed"){
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";
    cout<<"Adding RareMC-ttz skim"<<endl; 
    ch->Add(dir+"ttz_2l2n_amcnlo*");
    //ch->Add(dir+"ttz_incl_mgmlm_skim*");
  } 

//====================================
// EWK Subtraction MC
//====================================

  else if (set == "EWKSub-WJets"){
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";
    
    cout<<"Adding WJets EWK Subtraction Samples (no Photon Data)"<<endl;       
    //============
    // W+Jets
    //============
    //This is the Wjets sample, it is intended to have events with a prompt photon vetod
    ch->Add(dir+"wjets_incl_mgmlm*");
    ch->Add(dir+"wjets_ht100_mgmlm*");
    ch->Add(dir+"wjets_ht200_mgmlm*");
    ch->Add(dir+"wjets_ht400_mgmlm*");
    ch->Add(dir+"wjets_ht600_mgmlm*");
    ch->Add(dir+"wjets_ht800_mgmlm*");
    ch->Add(dir+"wjets_ht1200_mgmlm*");
    ch->Add(dir+"wjets_ht2500_mgmlm*");
  }
  else if (set == "EWKSub-WGJets"){
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";
    
    cout<<"Adding WGJets EWK Subtraction Samples (no Photon Data)"<<endl;       
    //This is the W+Gamma+Jets, it is inteded to have events with non-prompt photons vetod
    ch->Add(dir+"wgjets_incl_mgmlm*");
    ch->Add(dir+"wgjets_ptg40_mgmlm*");
    ch->Add(dir+"wgjets_ptg130_mgmlm*");
  }
  else if (set == "EWKSub-GZNuNu"){
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";
    
    cout<<"Adding Gamma Z -> NuNu EWK Subtraction Samples (no Photon Data)"<<endl;       
    //============
    // Gamma+Z->NuNu
    //============
    ch->Add(dir+"znunugamma_ptg40_mgmlm*");
    ch->Add(dir+"znunugamma_ptg130_mgmlm*");
  }
  else if (set == "EWKSub-TTBar-1lep"){ 
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";
    
    cout<<"Adding TTbar -> 1 lep EWK Subtraction Samples (no Photon Data)"<<endl;       
    //============
    // TTbar
    //============
    //1lep
    ch->Add(dir+"ttbar_1ltbr_mgmlm_ext1*");
    ch->Add(dir+"ttbar_1ltop_mgmlm_ext1*");
  }
  else if (set == "EWKSub-TTBar-Dilep"){
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";
    
    cout<<"Adding ttbar->dilep EWK Subtraction Samples (no Photon Data)"<<endl;       
    //dilep
    ch->Add(dir+"ttbar_dilep_mgmlm_ext1*");
  }
  else if (set == "EWKSub-singleTop"){
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";
    
    cout<<"Adding Single Top EWK Subtraction Samples (no Photon Data)"<<endl;       
    //============
    // Single Top
    //============
    ch->Add(dir+"sttw_antitop_nofullhaddecay_powheg*");
    ch->Add(dir+"sttw_top_nofullhaddecay_powheg*");
  }

//====================================
// Photon Data
//====================================

  //Single Photon Trigger
  else if (set == "GammaData-SinglePhoton"){
    cout<<"Adding GammaData-SinglePhoton"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016B_23sep2016rereco_ph_v3*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016C_23sep2016rereco_ph_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016D_23sep2016rereco_ph_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016E_23sep2016rereco_ph_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016F_23sep2016rereco_ph_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016G_23sep2016rereco_ph_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016H_Prompt_ph_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016H_Prompt_ph_v3*");
    
    //EWK Subtraction
  }
  else if (set == "GammaData-EWKSub"){
    cout<<"Adding EWK Subtraction Samples"<<endl;       
    //============
    // W+Gamma+Jets
    //============
    //This is the Wjets sample, it is intended to have events with a prompt photon vetod
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_incl_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht100_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht200_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht400_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht600_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht800_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht1200_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht2500_mgmlm*");

    //This is the W+Gamma+Jets, it is inteded to have events with non-prompt photons vetod
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wgjets_incl_mgmlm*");
    //============
    // Gamma+Z->NuNu
    //============
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/znunugamma_ptg130_mgmlm*.root");
    //============
    // TTbar
    //============
    //1lep
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltbr_mgmlm*.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltop_mgmlm*.root");
    //dilep
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_dilep_mgmlm*.root");
    //============
    // Single Top
    //============
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/sttw_antitop_nofullhaddecay_powheg.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/sttw_top_nofullhaddecay_powheg.root");
  }
  else if (set == "GammaData-GammaRealMET"){
    cout<<"Adding GammaData-GammaRealMET"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltbr_mgmlm.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltop_mgmlm.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wgjets_incl_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht100_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht1200_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht200_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht2500_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht400_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht600_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht800_amcnlo.root");
  }
  else if (set == "GammaData-JustWjetRealMET"){
    cout<<"Adding GammaData-JustWjetRealMET"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht100_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht1200_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht200_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht2500_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht400_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht600_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht800_amcnlo.root");
  }
  else if (set == "GammaData-JustWGjetRealMET"){
    cout<<"Adding GammaData-JustWGjetRealMET"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wgjets_incl_amcnlo.root");
  }
  else if (set == "GammaData-JustTTBarRealMET"){
    cout<<"Adding GammaData-JustTTBarRealMET"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltbr_mgmlm.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltop_mgmlm.root");
  }

//====================================
// Leptonic Data
//====================================

  else if (set == "DileptonData-ee"){
    TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-13/output/";
    
    cout<<"Adding EE Trigger Data"<<endl;

    ch->Add(dir+"data_Run2016B_23sep2016rereco_ee_v3*");
    ch->Add(dir+"data_Run2016C_23sep2016rereco_ee_v1*");
    ch->Add(dir+"data_Run2016D_23sep2016rereco_ee_v1*");
    ch->Add(dir+"data_Run2016E_23sep2016rereco_ee_v1*");
    ch->Add(dir+"data_Run2016F_23sep2016rereco_ee_v1*");
    ch->Add(dir+"data_Run2016G_23sep2016rereco_ee_v1*");
    ch->Add(dir+"data_Run2016H_Prompt_ee_v2*");
    ch->Add(dir+"data_Run2016H_Prompt_ee_v3*");
  }
  else if (set == "DileptonData-em"){
    TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-13/output/";
    
    cout<<"Adding EMu Trigger Data"<<endl;
    
    ch->Add(dir+"data_Run2016B_23sep2016rereco_em_v3*");
    ch->Add(dir+"data_Run2016C_23sep2016rereco_em_v1*");
    ch->Add(dir+"data_Run2016D_23sep2016rereco_em_v1*");
    ch->Add(dir+"data_Run2016E_23sep2016rereco_em_v1*");
    ch->Add(dir+"data_Run2016F_23sep2016rereco_em_v1*");
    ch->Add(dir+"data_Run2016G_23sep2016rereco_em_v1*");
    ch->Add(dir+"data_Run2016H_Prompt_em_v2*");
    ch->Add(dir+"data_Run2016H_Prompt_em_v3*");
  }
  else if (set == "DileptonData-mm"){
    TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-13/output/";
    
    cout<<"Adding MuMu Trigger Data"<<endl;
    
    ch->Add(dir+"data_Run2016B_23sep2016rereco_mm_v3*");
    ch->Add(dir+"data_Run2016C_23sep2016rereco_mm_v1*");
    ch->Add(dir+"data_Run2016D_23sep2016rereco_mm_v1*");
    ch->Add(dir+"data_Run2016E_23sep2016rereco_mm_v1*");
    ch->Add(dir+"data_Run2016F_23sep2016rereco_mm_v1*");
    ch->Add(dir+"data_Run2016G_23sep2016rereco_mm_v1*");
    ch->Add(dir+"data_Run2016H_Prompt_mm_v2*");
    ch->Add(dir+"data_Run2016H_Prompt_mm_v3*");
  }
  else if (set == "SingleLeptonData-SingleMu"){
    TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-13/output/";
    
    cout<<"Adding Single Muon Trigger Data"<<endl;
    
    ch->Add(dir+"data_Run2016B_23sep2016rereco_sm_v3*");
    ch->Add(dir+"data_Run2016C_23sep2016rereco_sm_v1*");
    ch->Add(dir+"data_Run2016D_23sep2016rereco_sm_v1*");
    ch->Add(dir+"data_Run2016E_23sep2016rereco_sm_v1*");
    ch->Add(dir+"data_Run2016F_23sep2016rereco_sm_v1*");
    ch->Add(dir+"data_Run2016G_23sep2016rereco_sm_v1*");
    ch->Add(dir+"data_Run2016H_Prompt_sm_v2*");
    ch->Add(dir+"data_Run2016H_Prompt_sm_v3*");
  }

//====================================
// Skimmed Leptonic Data
//====================================

  else if (set == "DileptonData-ee-Skimmed"){
    //TString dir="/home/users/cwelke/ZMetbabyskims/V08-22-05/";
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-11/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-15/skims/";

    cout<<"Adding Skimmed EE Trigger Data"<<endl;
    ch->Add(dir+"data_Run2016B_03feb2017rereco_ee_v2_skim*");
    ch->Add(dir+"data_Run2016C_03feb2017rereco_ee_v1_skim*");
    ch->Add(dir+"data_Run2016D_03feb2017rereco_ee_v1_skim*");
    ch->Add(dir+"data_Run2016E_03feb2017rereco_ee_v1_skim*");
    ch->Add(dir+"data_Run2016F_03feb2017rereco_ee_v1_skim*");
    ch->Add(dir+"data_Run2016G_03feb2017rereco_ee_v1_skim*");
    ch->Add(dir+"data_Run2016H_03feb2017rereco_ee_v2_skim*");
    ch->Add(dir+"data_Run2016H_03feb2017rereco_ee_v3_skim*");
  }
  else if (set == "DileptonData-em-Skimmed"){
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-11/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-15/skims/";
    
    cout<<"Adding Skimmed EMu Trigger Data"<<endl;
    
    ch->Add(dir+"data_Run2016B_03feb2017rereco_em_v2_skim*");
    ch->Add(dir+"data_Run2016C_03feb2017rereco_em_v1_skim*");
    ch->Add(dir+"data_Run2016D_03feb2017rereco_em_v1_skim*");
    ch->Add(dir+"data_Run2016E_03feb2017rereco_em_v1_skim*");
    ch->Add(dir+"data_Run2016F_03feb2017rereco_em_v1_skim*");
    ch->Add(dir+"data_Run2016G_03feb2017rereco_em_v1_skim*");
    ch->Add(dir+"data_Run2016H_03feb2017rereco_em_v2_skim*");
    ch->Add(dir+"data_Run2016H_03feb2017rereco_em_v3_skim*");
  }
  else if (set == "DileptonData-mm-Skimmed"){
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-11/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-15/skims/";

    cout<<"Adding Skimmed MuMu Trigger Data"<<endl;
    
    ch->Add(dir+"data_Run2016B_03feb2017rereco_mm_v2_skim*");
    ch->Add(dir+"data_Run2016C_03feb2017rereco_mm_v1_skim*");
    ch->Add(dir+"data_Run2016D_03feb2017rereco_mm_v1_skim*");
    ch->Add(dir+"data_Run2016E_03feb2017rereco_mm_v1_skim*");
    ch->Add(dir+"data_Run2016F_03feb2017rereco_mm_v1_skim*");
    ch->Add(dir+"data_Run2016G_03feb2017rereco_mm_v1_skim*");
    ch->Add(dir+"data_Run2016H_03feb2017rereco_mm_v2_skim*");
    ch->Add(dir+"data_Run2016H_03feb2017rereco_mm_v3_skim*");
  }
  else if (set == "SingleLeptonData-SingleMu-Skimmed"){
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-11/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-15/skims/";
    
    cout<<"Adding Skimmed Single Muon Trigger Data"<<endl;
    
    ch->Add(dir+"data_Run2016B_03feb2017rereco_sm_v2_skim*");
    ch->Add(dir+"data_Run2016C_03feb2017rereco_sm_v1_skim*");
    ch->Add(dir+"data_Run2016D_03feb2017rereco_sm_v1_skim*");
    ch->Add(dir+"data_Run2016E_03feb2017rereco_sm_v1_skim*");
    ch->Add(dir+"data_Run2016F_03feb2017rereco_sm_v1_skim*");
    ch->Add(dir+"data_Run2016G_03feb2017rereco_sm_v1_skim*");
    ch->Add(dir+"data_Run2016H_03feb2017rereco_sm_v2_skim*");
    ch->Add(dir+"data_Run2016H_03feb2017rereco_sm_v3_skim*");
  }

//====================================
// Skimmed Photon Data
//====================================

  //Single Photon Trigger
  else if (set == "GammaData-SinglePhoton-Skimmed"){
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-11/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-15/skims/";

    cout<<"Adding GammaData-SinglePhoton-Skimmed"<<endl; 
    
    ch->Add(dir+"data_Run2016B_03feb2017rereco_ph_v2_skim*");
    ch->Add(dir+"data_Run2016C_03feb2017rereco_ph_v1_skim*");
    ch->Add(dir+"data_Run2016D_03feb2017rereco_ph_v1_skim*");
    ch->Add(dir+"data_Run2016E_03feb2017rereco_ph_v1_skim*");
    ch->Add(dir+"data_Run2016F_03feb2017rereco_ph_v1_skim*");
    ch->Add(dir+"data_Run2016G_03feb2017rereco_ph_v1_skim*");
    ch->Add(dir+"data_Run2016H_03feb2017rereco_ph_v2_skim*");
    ch->Add(dir+"data_Run2016H_03feb2017rereco_ph_v3_skim*");
  }
  //EWK Subtraction
  else if (set == "GammaData-EWKSub-Skimmed"){
    //TString dir="/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-16/skim/";
    TString dir="/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/";
    
    cout<<"Adding Skimmed EWK Subtraction Samples"<<endl;       
    
    //============
    // W+Gamma+Jets
    //============
    //This is the Wjets sample, it is intended to have events with a prompt photon vetod
    ch->Add(dir+"wjets_incl_mgmlm*");
    ch->Add(dir+"wjets_ht100_mgmlm*");
    ch->Add(dir+"wjets_ht200_mgmlm*");
    ch->Add(dir+"wjets_ht400_mgmlm*");
    ch->Add(dir+"wjets_ht600_mgmlm*");
    ch->Add(dir+"wjets_ht800_mgmlm*");
    ch->Add(dir+"wjets_ht1200_mgmlm*");
    ch->Add(dir+"wjets_ht2500_mgmlm*");

    //This is the W+Gamma+Jets, it is inteded to have events with non-prompt photons vetod
    ch->Add(dir+"wgjets_incl_mgmlm*");
    ch->Add(dir+"wgjets_ptg40_mgmlm*");
    ch->Add(dir+"wgjets_ptg130_mgmlm*");
    //============
    // Gamma+Z->NuNu
    //============
    ch->Add(dir+"znunugamma_ptg40_mgmlm*");
    ch->Add(dir+"znunugamma_ptg130_mgmlm*");
    //============
    // TTbar
    //============
    //1lep
    ch->Add(dir+"ttbar_1ltbr_mgmlm_ext1*");
    ch->Add(dir+"ttbar_1ltop_mgmlm_ext1*");
    //dilep
    ch->Add(dir+"ttbar_dilep_mgmlm_ext1*");
    //============
    // Single Top
    //============
    ch->Add(dir+"sttw_antitop_nofullhaddecay_powheg*");
    ch->Add(dir+"sttw_top_nofullhaddecay_powheg*");
  }

//====================================
// End Building TChain
//====================================

  return;
} 


TChain * getTChain(TString data_set) {
  TChain *ch = new TChain("t");

  vector<TString> sets;

  if (data_set[0]=='['){
     sets = sParseVector(data_set);
  }
  else{
    TString token;
    Ssiz_t from=0;
    //cout<<"got vector in string form: "<<opt<<endl;
    while(data_set.Tokenize(token, from, "_")){
      token.ReplaceAll("_", "");
      sets.push_back(TString(token.Data()));
    }
  }

  cout<<"Datasets Incoming: "<<endl;
  cout<<"===================================="<<endl;
  for (std::vector<TString>::iterator i=sets.begin(); i != sets.end(); i++){
    addToChain(ch, *i);
  }
  cout<<"===================================="<<endl;
  
  return ch;
}