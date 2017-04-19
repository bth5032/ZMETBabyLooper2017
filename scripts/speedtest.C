void check(){
  TChain *ch = new TChain("t");

  ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-07/t5zz_80x_v2*");

  int n_entries = ch->GetEntries();
  int i_entry, a, b;
  double nb;
  for (int j_entry = 0; j_entry < n_entries; j_entry++){
    i_entry = ch->LoadTree(j_entry);
    if (i_entry < 0) break;
    ch->SetBranchAddress("mass_gluino",&a);
    ch->SetBranchAddress("mass_LSP",&b);

    nb = ch->GetEntry(j_entry);
    if (nb <= 0) continue;
    if (j_entry % 10000 == 0){
      cout<<"Processing entry "<<j_entry<<" of "<<n_entries<<endl;
    }
  }
}

void speedtest(){
  check();
}