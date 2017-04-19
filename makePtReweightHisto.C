#include <iostream>
#include <stdexcept>

#include <TFile.h>
#include <TH1D.h>

#include "ConfigParser.C"
#include "ConfigHelper.C"
#include "HistTools.C"

using namespace std;

void makePtReweightHisto(ConfigParser * conf)
{
  TString hist_name = "vpt";
  TString output_dir = getOutputDir(conf, "hist"); //get output dir location for this histogram something like /nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/prediction/PhotonData_VPTRWT/TChiHZ/
  TString output_location = output_dir+conf->get("Name")+"_vpt_rwt.root";

  //-----------------------------------
  // Get Path to Histograms From Config
  //-----------------------------------
  TString primary_path = output_dir;
  primary_path.ReplaceAll("/PhotonData_VPTRWT/", "/DileptonData/");
  primary_path += (conf->get("vpt_rwt_primary_samplename") != "") ? conf->get("vpt_rwt_primary_samplename")+".root" : "DileptonData.root";

  TString secondary_path = output_dir;
  secondary_path.ReplaceAll("/PhotonData_VPTRWT/", "/PhotonData/");
  secondary_path += (conf->get("vpt_rwt_secondary_samplename") != "") ? conf->get("vpt_rwt_secondary_samplename")+".root" : conf->get("Name")+".root";

  cout<<"primary_histos: "<<primary_path<<endl;
  cout<<"output_dir: "<<output_dir<<endl;
  cout<<"hist_name: "<<hist_name<<endl;
  cout<<"secondary_path: "<<secondary_path<<endl;
  cout<<"output_location: "<<output_location<<endl;

  //....................
  //Subtraction Hists
  //...................
  vector<TString> subtractor_paths;
  vector<double> subtractor_scales;
  TString s_path;

  //FS----------------------------
  s_path = primary_path;
  s_path.ReplaceAll("/DileptonData/", "/FS/");
  s_path.ReplaceAll("DileptonData.root", "FS.root");
  subtractor_paths.push_back(s_path);
  if (conf->get("old_FS") == "true"){
    subtractor_scales.push_back(1); //RSFOF*kappa
  }
  else{
    subtractor_scales.push_back(0.073905); //RSFOF*kappa 
  }
  
  if (conf->get("signal_region") != "ICHEP"){
    //ZNu---------------------------
    s_path = primary_path;
    s_path.ReplaceAll("/DileptonData/", "/ZNu/");
    //ZNu-VVV
    s_path.ReplaceAll("DileptonData.root", "VVV.root");
    subtractor_paths.push_back(s_path);
    subtractor_scales.push_back(1);
    //ZNu-ttz
    s_path.ReplaceAll("VVV.root", "ttz.root");
    subtractor_paths.push_back(s_path);
    subtractor_scales.push_back(1.09);
    //ZNu-zz
    s_path.ReplaceAll("ttz.root", "zz.root");
    subtractor_paths.push_back(s_path);
    subtractor_scales.push_back(1.8);
    //ZNu-wz
    s_path.ReplaceAll("zz.root", "wz.root");
    subtractor_paths.push_back(s_path);
    subtractor_scales.push_back(0.94);
  }
  cout<<"subtractor_paths: "<<endl;
  for (int i=0; i < (int) subtractor_paths.size(); i++){
    cout<<subtractor_paths.at(i)<<endl;
  }

  TFile * f_primary = TFile::Open(primary_path , "READ"); //typically location to data hist
  TFile * f_secondary = TFile::Open(secondary_path, "READ"); //typically location to zjets hist
  vector<TFile *> f_subtractors;
  for (int i=0; i < (int) subtractor_paths.size(); i++){
    f_subtractors.push_back(TFile::Open(subtractor_paths.at(i), "READ")); //locations of flavor symmetric and ZNu counterparts.
  }
  
  cout<<"Found input files for reweighting"<<endl;
  TH1D * h_primary;
  TH1D * h_secondary;
  TH1D * h_subtractor;
  TH1D * h_primary_scaled;
  TH1D * h_secondary_scaled;

  TString primary_name = "DileptonData";
  TString secondary_name = "PhotonData";

  h_primary = (TH1D*)f_primary->Get(hist_name)->Clone(primary_name);
  h_secondary = (TH1D*)f_secondary->Get(hist_name)->Clone(secondary_name);
  
  if (conf->get("no_subtraction_vpt_rwt") != "true"){
    h_subtractor = (TH1D*)(f_subtractors.at(0))->Get(hist_name)->Clone("subtractor_"+primary_name);
    h_subtractor->Scale(subtractor_scales.at(0));
    for (int i=1; i < (int) subtractor_paths.size(); i++){
      h_subtractor->Add((TH1D*)(f_subtractors.at(i))->Get(hist_name), subtractor_scales.at(i));
    }
    cout<<"Retrived Histograms, subtracting other backgrounds"<<endl;
    h_primary->Add(h_subtractor, -1);
  }
  else{
    cout<<"Skipping background subtraction"<<endl;
  }
  cout<<"Zeroing negative bins"<<endl;

  zeroNegatives(h_primary);
  zeroNegatives(h_secondary);

  TH1D * h_ratio_unscaled = (TH1D*) h_primary->Clone(hist_name+"_ratio_unscaled");
  h_ratio_unscaled->Divide(h_secondary); 

  h_primary_scaled=(TH1D*) h_primary->Clone(primary_name+"_scaled");
  h_secondary_scaled=(TH1D*) h_secondary->Clone(secondary_name+"_scaled");
  h_secondary_scaled->Scale(1./h_secondary->GetSumOfWeights());
  h_primary_scaled->Scale(1./h_primary->GetSumOfWeights());

  TH1D * h_ratio = (TH1D*) h_primary_scaled->Clone(hist_name+"_ratio");
  h_ratio->Divide(h_secondary_scaled);

  TFile * file = TFile::Open(output_location,"RECREATE");
  file->cd();
  h_ratio->Write();
  h_ratio_unscaled->Write();
  h_primary->Write();
  h_secondary->Write();
  h_primary_scaled->Write();
  h_secondary_scaled->Write();
  file->Close();

  cout<<"Reweight histogram succesfully made at "<<output_location<<endl;
  return ;
}
