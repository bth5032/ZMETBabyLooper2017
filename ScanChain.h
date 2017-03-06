// Usage:
// > root -b doAll.C

//
// 2016 MET study looper. Written by Bobak Hashemi May 13 2016
//

// C++
#include <iostream>
#include <vector>
#include <set>
#include <tuple>
#include <utility>
#include <fstream>


// ROOT
#include "TBenchmark.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTreeCache.h"
#include "TEfficiency.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"

// Analysis Specific
#include "ZMET2016.cc"

// CORE
//You can not include headers!!! This is not compiled code.
#include "CoreTools/dorky.cc"
#include "CoreTools/goodrun.cc"
#include "/home/users/bhashemi/Projects/GIT/CORE/Tools/MT2/MT2Utility.cc"
#include "/home/users/bhashemi/Projects/GIT/CORE/Tools/MT2/MT2.cc"
// Configuration parsing
#include "ConfigParser.C"
#include "ConfigHelper.C"

using namespace std;
//using namespace zmet;
using namespace duplicate_removal;

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

//Global Vars
ConfigParser *conf;
int nDuplicates=0;
int num_events_veto_ttbar=0;
int num_events_veto_ttgamma=0;
bool MCTriggerEmulation = true;

vector<pair <TH1D*, TString> > g_reweight_pairs;
TDirectory *rootdir = gDirectory->GetDirectory("Rint:");
TH1D *g_pileup_hist, *g_l1prescale_hist22, *g_l1prescale_hist30, *g_l1prescale_hist36; 

//Btag and ISR Scale Factor overall normalization
TH1D *g_btagsf_norm, *g_btagsf_light_norm_up, *g_btagsf_heavy_norm_up;
TH1D *g_isr_norm, *g_isr_norm_up;
TFile *g_SUSYsf_norm_file;

TEfficiency *g_pt_eff_barrel, *g_pt_eff_endcap; 
TFile *g_weight_hist_file, *g_pileup_hist_file, *g_l1prescale_file;
TString g_sample_name;
TFile* currentFile = 0;
double g_scale_factor=1; //Holds scale factors for sample.

TH1I *numEvents; //Holds the number of events in the whole script and the number that pass various cuts 

// TTBar -> Dilepton MET > 250
// /* above Z */ make_tuple(4012971,1,4980),make_tuple(21272149,1,26397),make_tuple(13030838,1,16171),make_tuple(13374903,1,16598),make_tuple(11120815,1,13800),make_tuple(16216842,1,20124),make_tuple(13045829,1,16189),make_tuple(4769923,1,5919),make_tuple(10024171,1,12440),make_tuple(19397864,1,24071),make_tuple(4721588,1,5860),make_tuple(7407557,1,9193),make_tuple(7074954,1,8780),make_tuple(22713668,1,28186),make_tuple(19705397,1,24453),make_tuple(13331830,1,16544),make_tuple(1181517,1,1467),make_tuple(11125348,1,13806),make_tuple(21541320,1,26731),make_tuple(25536762,1,31689),make_tuple(20980431,1,26035),make_tuple(20925522,1,25967),make_tuple(7302650,1,9062),make_tuple(5514720,1,6844),make_tuple(21154548,1,26251),make_tuple(1028190,1,1276),make_tuple(10904087,1,13532),make_tuple(3991364,1,4953),make_tuple(11930090,1,14804),make_tuple(1567847,1,1946),make_tuple(3849571,1,4777),make_tuple(25691560,1,31881),make_tuple(20538858,1,25487),make_tuple(25048692,1,31084),make_tuple(7370159,1,9146),make_tuple(24899778,1,30899),make_tuple(6629105,1,8227),make_tuple(7598861,1,9430),make_tuple(21686557,1,26912),make_tuple(7764619,1,9636),make_tuple(7824024,1,9709)};
// /* below Z */ set<tuple<long,long,long>> inspection_set_erl = {make_tuple(19992517,1,24809),make_tuple(7298591,1,9057),make_tuple(14959462,1,18564),make_tuple(7443400,1,9237),make_tuple(13001022,1,16133),make_tuple(20599412,1,25562),make_tuple(18873336,1,23420),make_tuple(13563152,1,16831),make_tuple(15628860,1,19394),make_tuple(23182617,1,28768),make_tuple(21748489,1,26988),make_tuple(21917332,1,27198),make_tuple(19857525,1,24642),make_tuple(14995613,1,18609),make_tuple(23049463,1,28603),make_tuple(7013505,1,8704),make_tuple(14430095,1,17907),make_tuple(1582055,1,1964),make_tuple(13146972,1,16315),make_tuple(12589216,1,15622),make_tuple(10051412,1,12473),make_tuple(1981452,1,2459),make_tuple(25412175,1,31535),make_tuple(3411621,1,4234),make_tuple(15055516,1,18683),make_tuple(13935646,1,17293),make_tuple(21447372,1,26615),make_tuple(8745617,1,10853)};

// TZQ MET > 250
// /* above Z */ set<tuple<long,long,long>> inspection_set_erl = {make_tuple(10842568,1,78569),make_tuple(13812745,1,100093),make_tuple(4924476,1,35685),make_tuple(12045096,1,87283),make_tuple(8460735,1,61310),make_tuple(3377686,1,24476),make_tuple(5950050,1,43117),make_tuple(3692795,1,26759),make_tuple(3408913,1,24703),make_tuple(7477581,1,54185),make_tuple(1731804,1,12549),make_tuple(14859667,1,107679),make_tuple(11573628,1,83867)};
// /* below Z */ set<tuple<long,long,long>> inspection_set_erl = {make_tuple(2379543,1,17243),make_tuple(4914525,1,35613),make_tuple(5605189,1,40617),make_tuple(1474023,1,10681),make_tuple(13635529,1,98809),make_tuple(13345186,1,96705),make_tuple(10603383,1,76837),make_tuple(12476767,1,90411)};

// ZZ 2L 2nu
set<tuple<long,long,long>> inspection_set_erl = {make_tuple(1297271,1,6877),make_tuple(61516,1,326),make_tuple(2206707,1,11698),make_tuple(4266756,1,22619),make_tuple(7968266,1,42242),make_tuple(5882871,1,31187),make_tuple(8635556,1,45779),make_tuple(4661791,1,24713),make_tuple(3720823,1,19725),make_tuple(1878684,1,9960),make_tuple(2783642,1,14757),make_tuple(6388949,1,33870)};
// WW 2L 2Nu
// set<tuple<long,long,long>> inspection_set_erl = {make_tuple(849216,1,4280),make_tuple(1010161,1,5090),make_tuple(84837,1,428),make_tuple(39748,1,200),make_tuple(1907225,1,9610),make_tuple(1966907,1,9912),make_tuple(793018,1,3996),make_tuple(135652,1,684),make_tuple(1573069,1,7927),make_tuple(57707,1,291),make_tuple(1439012,1,7252),make_tuple(698932,1,3522),make_tuple(834708,1,4206),make_tuple(1706140,1,8597),make_tuple(1093702,1,5512),make_tuple(516830,1,2604),make_tuple(619262,1,3120),make_tuple(1199575,1,6045),make_tuple(1990371,1,10030),make_tuple(1535469,1,7737)};

//set<long> inspection_set = {100009692,100032487,10027757,1010744879,1015822881,101645567,101803627,1026912431,1029387594,1044013053,1046547114,104670004,1050588548,105338603,1054370950,105758909,1058388592,1059043352,1064658055,1064920437,1069338373,1072792265,1075741112,1085573085,108711721,1087396320,1088682598,1094779189,1102076480,1110673724,111242434,111383006,1116918236,1117345657,1117446591,111795763,1128564530,1133323793,1133449435,1133722320,113447501,113686500,113704325,114095835,1141199143,1141631494,1145342567,114920030,114968328,1152890586,115348344,11649408,117138699,1183380915,118392973,118612686,1202988269,1203019151,1204870199,1206150057,1207148927,120790282,1209409113,121081925,1211757139,121905298,1220054509,1222235426,1225811473,1228548503,1232573659,1233403995,123442712,1236187674,123985858,1241201441,1241950260,1248061084,1248127548,125011823,1253873513,1255693030,125629921,1259943982,126173960,1263239087,127587312,1277644982,1283027826,128977341,128981569,1292052857,1295656306,1301703500,1309280766,1309940922,1316735966,132203912,1322185446,1322842308,1323447641,1326015015,1328797524,1331982311,1343450987,1349354042,135154879,1351744974,135183011,1357072292,1360767456,1361525498,136295941,1364057683,1369240885,1370320586,137156600,137363653,1374244490,138079864,1384835826,1388221704,1388992744,1390350583,139193612,1392775668,1398973481,1402729947,1407617099,140868372,1413738984,1415811356,1417698475,1419892336,14203891,142716459,1427786549,1428108658,1429043412,1433513423,143422195,1436253945,1441761553,1442898969,144576423,1446130034,14468698,1457220658,1458190644,145980719,146254557,146400583,1465403829,1469045020,1469265912,1471053845,1474700878,1475357441,147658758,1478751688,1481862944,148431377,1485274043,148619238,1488068624,149054504,1494859230,1502419317,1506544210,1506904040,1507725434,1507852773,1510282584,1514852672,1517902846,152316487,1533615064,1534081,1536364729,1538911732,1541767560,154306972,154921517,154939362,1554238976,155721965,1566677186,1570931582,1572199636,1579157818,1581779614,1582294461,159674903,1597583140,159781882,159926429,1599287772,160895688,160960764,162276255,163206954,1635590919,163686738,163784701,1639406300,163954802,16450104,1646415750,164766327,1651324139,165237392,1657648735,1664635753,1668817177,16741914,168141310,1685752965,169508579,169519666,1698775233,1708469549,1710802004,1712716938,1726350174,1728469276,1730941342,173307472,1734756378,1739293168,17444928,1746082038,174669350,175400250,175781506,175899164,17662543,1774905933,177679464,177869362,1783285770,178479808,178680433,1787520526,1791035076,1792619546,180340418,1804614936,180810773,1808223665,181431881,1816852655,1831072490,1832415156,1835234049,1840657780,184340248,1850607504,1850981332,185183260,1853091483,1854840845,1857599901,1861338055,1872067858,1881551749,188928947,1895625702,190589499,1909410737,191303099,191874013,1930186694,193038182,1935233061,194344327,1946509655,1951383830,1952762047,1963474618,1970130156,1975310891,1978543901,1979554248,1983112936,1983235731,198825293,1991451360,199680177,199814663,200212777,200287523,2003942393,2007409303,201400256,2018924857,2019220191,20272593,2037943567,2044096177,204605833,2047833089,2049952425,205209355,2053046069,206296403,206717718,207270681,2073736288,2080635070,2085215400,208688687,209036256,209494241,2101414645,2106581461,212120844,2124587761,212512663,2136206109,213856231,2150408588,215577442,216881264,2175458430,2179114839,218918271,2196385166,220483327,221312510,2214580869,2223478516,222366391,2229377997,223044166,223138843,223735882,2237768807,2259751408,226436034,226887973,227074878,227484976,227616880,228545898,230938296,2314587174,2319609762,2353949132,236057182,2362030194,236414813,237864560,238142984,239948422,2408546412,242903283,242952528,2429724144,242998980,2433179560,2443324308,2473107,247315471,2487439121,2507068208,251130289,2523743485,25322312,2536673734,254603687,255203514,2558396747,2575883608,2576595101,2577667077,2598386102,260296294,2628499098,2630982821,263430898,2640190775,264181654,265630730,268373042,2690587329,269804740,271511425,2718470159,2719080190,271967709,2719792917,2734404918,27441029,2745459298,2758706989,2773123378,277827560,278334398,2788408487,2790814579,28017266,281857557,2829917529,283978089,2844763762,284647538,284691429,2853692344,2869977498,2887013613,289400670,293764192,2939008113,294674891,2950169168,2963610076,2963652321,296991947,2976809591,299382109,299386443,2995312450,301962141,3020985042,302953023,303802596,3063774308,3071423607,308264066,308386114,3126148110,3158118298,3176251865,3177489030,318411801,3207512677,3214380339,321444945,321459146,321796236,322418658,322951146,324109724,325760233,327973001,328620535,32884734,329180835,33168403,3318658610,333110803,334200984,33437993,33453121,3351899453,3353469001,33600849,337781268,339082581,341842692,3432844987,34534627,345988786,349116996,3493689747,352943047,353580600,354581502,354669484,35471238,35495309,358180709,358465002,359981818,360224097,3605005450,3609587459,361041572,36201823,362328404,363748381,364313534,364576876,3657447543,365926783,367467579,367743293,368723574,369359835,3699201302,370027188,370710626,3715152,371989162,373482955,373569585,373716150,375432780,37557290,37847650,379401119,379775715,38151309,383715357,38427376,38465689,385938027,389146799,391800362,392809489,39379785,396101529,398112557,399293418,400053779,402651581,4030909462,405055062,405077299,405526372,406994229,409119292,409730397,411131170,412183123,416324149,416580185,418651650,418816437,422388965,423584675,4247991726,426450292,426681361,4278358,429645602,431266223,431643384,433119744,433772682,434103961,4355994601,438463248,439543732,440033931,443167310,44378323,4441699321,44438398,446492516,44662129,446912670,4479888562,451581865,45296145,454321863,454682310,454683658,45620502,456902043,45830662,460134198,460432209,46046881,461335704,463325727,4661260,468645551,469852840,47166555,474493176,4754507878,47594247,476721764,478847765,479030330,480857476,481347909,48149618,481519066,482546897,483701418,484100714,48512973,485685969,487423829,487541688,489169544,48987779,490798625,491467528,494168886,49736077,498184440,499046387,500139826,500405170,500882436,5025132511,50414492,505308078,50536382,505416146,505543645,507351115,507367976,508140216,508638870,510179452,513029908,514367427,516768664,52304953,525257256,525290168,525496135,526305554,528794792,531494714,5316881,538493797,541356859,544844436,546298686,548884270,551255740,552381436,552763815,55531807,555799453,558198472,559880471,560647350,56295912,563150740,564820438,565768110,567304602,569422294,576643608,576996573,578294770,57966889,582040157,582130676,582584222,583416853,583454337,583626130,584228958,58456989,584636132,584959502,59104726,591512862,593025609,593270591,59548207,598678844,599524472,599583398,599880053,603741332,603984247,60522073,60606902,60709768,610963824,611100260,613133583,613244428,618780574,622130933,622699685,623203810,629476892,629540779,629579289,63039343,631184958,633214550,637756191,638974092,642775195,644742765,64556174,64635874,649780564,652151825,652181369,653176550,654001763,654959966,656247657,656467006,65763233,6581822,658314162,659411107,661594594,661930012,662919424,664734632,668544567,671297808,671620251,67489323,675510075,675938052,67681776,676843722,685967013,687330514,691096348,69282688,693189517,694210809,694633506,696590630,698374772,698806328,699682935,69974863,702555889,70264911,704288490,704438652,70846418,710617693,710960595,711974880,71224066,712787009,713632411,714008290,715066767,715090391,716981233,717572124,722603926,724249247,725179598,726655537,732779286,73294343,734054688,735092876,735555985,735690191,736684059,73708764,74265268,74443913,74541547,748236634,750120647,752307565,752397216,75273175,754724623,755478482,759167636,760122412,760672890,761226281,761254794,761852750,761910713,765867959,768472318,769620071,770296485,774471445,774527362,774807033,775594501,777114507,77792114,778781016,782146496,78414816,78470536,787071002,78816776,789146859,792240708,799372980,800894400,80617323,806573271,80735578,810750510,810916840,811978504,81217292,81367450,819305877,823772995,829973370,830039201,831402395,834697629,836799689,83767665,839542171,841789020,84665016,849015575,854399384,855568077,85857592,860362516,862016121,862419688,867571507,870822952,871186948,881052397,883968818,884865258,88501831,886304206,88705084,889847642,891240730,892945215,895181882,90007284,903391855,90634618,90742531,90816020,909353342,911791260,915491525,923564687,926889576,934833372,937335760,937472881,939385873,943648001,94542634,949717708,951223171,956528108,957484721,95844035,963756973,964145967,964380730,965387777,974983904,975744985,976509539,979425869,981620426,98194428,982373908,984093713,98711848,98718085,987922830,989993945,991783679,992485886,99795815,998615983,998751102,999957050};

bool printStats = false;
bool printFail = false;

//=======================================================
// Global variables used for uncertainty fluctuations
//=======================================================
double g_dphi_metj1;
double g_dphi_metj2;
int g_njets;
double g_mbb;
double g_mjj_mindphi;
int g_nBJetMedium;
double g_met;
double g_met_phi;
double g_mt2;
double g_mt2b;
double g_ht;

vector<float> g_jets_csv;
vector<LorentzVector> g_jets_p4;
vector<LorentzVector> g_jets_medb_p4;


/* returns two most B-like jet indicies */
pair<int, int> getMostBlike();

/*Finds the pair of B tagged jets (csv medium) with dijet mass closest to the mass of the higgs*/
pair<int,int> getClosestBPairToHiggsMass();

/*Builds MT2b from two highest CSV jets*/
double getMT2B();

/*Builds Mbb from two highest CSV jets*/
double getMbb();

/*This function gets the MT2 built out of the two Bjets in an event, no guarentee 
is made about selecting the highest csv jets*/
double getMT2ForBjets(bool select_highest_csv=false);

/*Builds MT2 for the two leading Bjets unless select_closest_higgs_mass is set, in which case it 
builds it out of the two bjets with dijet mass nearest the mass of the higgs.*/
double getMT2HiggsZ(bool select_highest_closest_higgs_mass=false);

/*Returns boson Pt, determines whether sample is gjets or zjets first*/
double bosonPt();

/* Builds the MT from the lepton at index id and the MET vector (assumes massless particles)*/
double getMTLepMET(short id=0);

/* Builds the delta R (sqrt(dPhi^2 + dEta^2)) between the lepton at index id and the leading photon*/
double getdRGammaLep(short id=0);

//=============================
// Triggers
//=============================
/*Checks that the event passes an "emulated photon trigger"*/
bool passPhotonEmulatedTrigger();

/*Ensures the event is within the efficiency plateu of the highest pt trigger it passed*/
bool passPhotonTriggers();

/*MC passes immediately, ensures data events were gathered from di-muon triggers*/
bool passMuonTriggers();

/*MC passes immediately, ensures data events were gathered from di-electron triggers*/
bool passElectronTriggers();

/*MC passes immediately, ensures data events were gathered from EMu triggers*/
bool passEMuTriggers();

/*MC passes immediately, ensures data events were gathered from SingleMu trigger*/
bool passSingleMuTriggers();

/*Helper method which chooses which above method to call. Calls EMu if the dil_flavor is emu, otherwise uses
the hyp_type to determine which to call. Events fail if they are hyp_type 2 and not tagged for emu*/
bool passLeptonHLTs();

//=============================
// Has Good Event Functions
//=============================
/*Lepton quality and Z mass cuts*/
bool hasGoodZ();

/*Photon quality cuts*/
bool hasGoodPhoton();

/*Method for testing whether the event has a good gamma mu pair trigger requirements are on the photon.
  It just checks muon quality stuff and then calls hasGoodPhoton()*/
bool hasGoodGammaMu();

/*Just a helper method that chooses which hasGood method to call based on the config event_type*/
bool hasGoodEvent();

//=============================
// Event Weight Assignment
//=============================

/*Goes through the chain of weight_from config options down to a config which does not have weight_from and
then adds a pair (config_name, hist_file) to the vector g_reweight_pairs.

For now this is depricated: NEEDS TO BE UPDATED WITH NEW CODE FIXES*/
void readyReweightHists();

/* Adds the vpt reweighting histogram to the g_reweight_pairs vector */
void readyVPTReweight(TString save_path);

/* Returns the trigger efficiency from g_pt_eff */
double getEff(const double &pt, const double &eta);

/*Loads the reweight hists from g_reweight_pairs and multiplies returns the weight associated with the proper
bin in the histogram*/
double getReweight();

/*This method stores fixes to the evt_scale1fb in the event of file corruptions. 
It's basically just a lookup table*/
double scale1fbFix();

/*Main function for determining the weights for each event*/
double getWeight();

/*Returns the weight associated with the photon prescales*/
double getPrescaleWeight();

//=============================
// Cuts
//=============================

/*Holds the cuts for all the signal regions, basically all the cuts that are turned on with a config option*/
bool passSignalRegionCuts();

/*Checks for a gen Neutrino (Real MET) and a gen Z (Real Z), only should be run when running
over samples tagged as "rares". This is only neccesary for the full prediction.*/
bool passRareCuts();

/*Checks for cuts that are spcific to SUSY samples like choosing a particular mass point*/
bool passSUSYSingalCuts();

/*Front end method to "Dorky" duplicate removal*/
bool isDuplicate();

/*Checks for MET filters*/
bool passMETFilters();

/*Holds baseline cuts*/
bool passBaseCut();

/*Ensures events from the ee/mumu/emu dataset pass the trigger for that type of event and for ee and emu ensures they don't pass other triggers.*/
bool passETHDileptonDataCleanse();

/*Method which holds all the file specific selections, for instance cutting out the
  events with genht > 100 in the DY inclusive samples*/
bool passFileSelections();

//=============================
// Setup
//=============================
/*Sets up global variables for the event which are the quantities that might be fluctuated in the process of computing uncertainty limits*/
void setupGlobals();

/*Obvi the event looper*/
int ScanChain( TChain* chain, ConfigParser *configuration, bool fast = true, int nEvents = -1);