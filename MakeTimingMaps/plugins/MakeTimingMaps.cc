// -*- C++ -*-
//
// Package:    HBHETimingValidation/MakeTimingMaps
// Class:      MakeTimingMaps
// 
/**\class MakeTimingMaps MakeTimingMaps.cc HBHETimingValidation/MakeTimingMaps/plugins/MakeTimingMaps.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Stephanie Brandt
//         Created:  Fri, 21 Aug 2015 11:42:17 GMT
//
//


// system include files
#include <memory>
#include <string>
#include <map>
#include <iostream>
using namespace std;

// user include files
#include "TTree.h"
#include "TFile.h"
#include "TProfile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TProfile2D.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"

#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"

#include "SimCalorimetry/HcalSimAlgos/interface/HcalSimParameterMap.h"
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class MakeTimingMaps : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit MakeTimingMaps(const edm::ParameterSet&);
      ~MakeTimingMaps();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      
      std::string int2string(int i);
      
      void ClearVariables();
      void SaveHistToFile();
      
      double RecHitEnergy;
      double RecHitTime;
      double iEta;
      double iPhi; 
      int depth;
      int RunNumber;
      int EvtNumber;
      
      bool FillHBHE;
      
      string sHBHERecHitCollection;      
      edm::Service<TFileService> FileService;
      
      TTree    *OutputTree; 

      TProfile *hHBHETiming_SmallStats;
      
      TProfile2D *hHBHETiming_Depth1;
      TProfile2D *hHBHETiming_Depth2;
      TProfile2D *hHBHETiming_Depth3;
      
      TProfile2D *hHBHETiming_wide_Depth1;
      TProfile2D *hHBHETiming_wide_Depth2;
      TProfile2D *hHBHETiming_wide_Depth3;
      
      TH2F *hHBHEEnergy_Depth1;
      TH2F *hHBHEEnergy_Depth2;
      TH2F *hHBHEEnergy_Depth3;
      
      // pointer to pointer to pointers to make a 2-d array of histograms
      TH1F ***hTiming_Depth1 = new TH1F**[59];
      TH1F ***hTiming_Depth2 = new TH1F**[59];
      TH1F ***hTiming_Depth3 = new TH1F**[6];
      
      int runNumber_;
      
      TH1F *hPart1;
      TH1F *hPart3;
      TH1F *hPart2;
      
      TH2F *occupancy_d1;
      TH2F *occupancy_d2;
      TH2F *occupancy_d3;
      
      const double nBinsX =  100;
      const double nBinsY =  100;
      const double minValX =   0;
      const double maxValX = 100;
      const double minValY = -25;
      const double maxValY =  25;

      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
MakeTimingMaps::MakeTimingMaps(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
   usesResource("TFileService");
    
  for (int i = 0; i < 59; ++i){
    hTiming_Depth1[i] = new TH1F*[72];
    hTiming_Depth2[i] = new TH1F*[72];
  }
  
  for(int i = 0; i < 6; ++i){
    hTiming_Depth3[i] = new TH1F*[72]; 
  }
  
  sHBHERecHitCollection = iConfig.getUntrackedParameter<string>("HBHERecHits","hbhereco");
  runNumber_ = iConfig.getParameter<int>("runNumber");
   
  // Make the output tree
  OutputTree = FileService->make<TTree>("testTree","testTree");
  OutputTree->Branch("RunNumber",&RunNumber,"RunNumber/I");
  OutputTree->Branch("EvtNumber",&EvtNumber,"EvtNumber/I");
  OutputTree->Branch("RecHitEnergy",&RecHitEnergy,"RecHitEnergy/D");
  
  OutputTree->Branch("RecHitTime",&RecHitTime,"RecHitTime/D");
  OutputTree->Branch("iEta",&iEta,"iEta/D");
  OutputTree->Branch("iPhi",&iPhi,"iPhi/D");
  OutputTree->Branch("depth",&depth,"depth/I");
  
  
  for(int i = 0; i < 72; ++i){
    for(int j = 0; j < 59; ++j){
       hTiming_Depth1[j][i] = FileService->make<TH1F>(("Depth1_ieta"+int2string(j-29)+"_iphi"+int2string(i+1)).c_str(),("Depth1_ieta"+int2string(j-29)+"_iphi"+int2string(i+1)).c_str(),75,-37.5,37.5);
       hTiming_Depth2[j][i] = FileService->make<TH1F>(("Depth2_ieta"+int2string(j-29)+"_iphi"+int2string(i+1)).c_str(),("Depth2_ieta"+int2string(j-29)+"_iphi"+int2string(i+1)).c_str(),75,-37.5,37.5);
    } 
    for(int j = 0; j < 6; ++j){
      int eta = 0;
      if(j==0) eta = -28;
      if(j==1) eta = -27;
      if(j==2) eta = -16;
      if(j==3) eta =  16;
      if(j==4) eta =  27;
      if(j==5) eta =  28;
      hTiming_Depth3[j][i] = FileService->make<TH1F>(("Depth3_ieta"+int2string(eta)+"_iphi"+int2string(i+1)).c_str(),("Depth3_ieta"+int2string(eta)+"_iphi"+int2string(i+1)).c_str(),75,-37.5,37.5);
    }
  }
  
  
  // run 251244
  hHBHETiming_wide_Depth1 = FileService->make<TProfile2D>("hHBHETiming_wide_Depth1","hHBHETiming_wide_Depth1",59,-29.5,29.5,72,0.5,72.5, -37.5, 37.5,"s");
  hHBHETiming_wide_Depth2 = FileService->make<TProfile2D>("hHBHETiming_wide_Depth2","hHBHETiming_wide_Depth2",59,-29.5,29.5,72,0.5,72.5, -37.5, 37.5,"s");
  hHBHETiming_wide_Depth3 = FileService->make<TProfile2D>("hHBHETiming_wide_Depth3","hHBHETiming_wide_Depth3",59,-29.5,29.5,72,0.5,72.5, -37.5, 37.5,"s");
  
  hHBHETiming_Depth1 = FileService->make<TProfile2D>("hHBHETiming_Depth1","hHBHETiming_Depth1",59,-29.5,29.5,72,0.5,72.5, -12.5, 12.5,"s");
  hHBHETiming_Depth2 = FileService->make<TProfile2D>("hHBHETiming_Depth2","hHBHETiming_Depth2",59,-29.5,29.5,72,0.5,72.5, -12.5, 12.5,"s");
  hHBHETiming_Depth3 = FileService->make<TProfile2D>("hHBHETiming_Depth3","hHBHETiming_Depth3",59,-29.5,29.5,72,0.5,72.5, -12.5, 12.5,"s");
  
  hHBHETiming_SmallStats = FileService->make<TProfile>("hHBHETiming_SmallStats","hHBHETiming_SmallStats",3,0.5,3.5, -37.5, 37.5,"s");
  
//   hHBHEEnergy_Depth1 = FileService->make<TProfile2D>("hHBHEEnergy_Depth1","hHBHEEnergy_Depth1",59,-29.5,29.5,72,0.5,72.5, 5, 1000);
//   hHBHEEnergy_Depth2 = FileService->make<TProfile2D>("hHBHEEnergy_Depth2","hHBHEEnergy_Depth2",59,-29.5,29.5,72,0.5,72.5, 5, 1000);
//   hHBHEEnergy_Depth3 = FileService->make<TProfile2D>("hHBHEEnergy_Depth3","hHBHEEnergy_Depth3",59,-29.5,29.5,72,0.5,72.5, 5, 1000);
  
  hHBHEEnergy_Depth1 = FileService->make<TH2F>("hHBHEEnergy_Depth1","hHBHEEnergy_Depth1",59,-29.5,29.5,72,0.5,72.5);
  hHBHEEnergy_Depth2 = FileService->make<TH2F>("hHBHEEnergy_Depth2","hHBHEEnergy_Depth2",59,-29.5,29.5,72,0.5,72.5);
  hHBHEEnergy_Depth3 = FileService->make<TH2F>("hHBHEEnergy_Depth3","hHBHEEnergy_Depth3",59,-29.5,29.5,72,0.5,72.5);
  
  occupancy_d1 = FileService->make<TH2F>("occupancy_d1","occupancy_depth1",59,-29.5,29.5,72,0.5,72.5);
  occupancy_d2 = FileService->make<TH2F>("occupancy_d2","occupancy_depth2",59,-29.5,29.5,72,0.5,72.5);
  occupancy_d3 = FileService->make<TH2F>("occupancy_d3","occupancy_depth3",59,-29.5,29.5,72,0.5,72.5);
  
  hPart1 = FileService->make<TH1F>("hPart1","average avg time partition 1", 25, -12.5, 12.5);
  hPart2 = FileService->make<TH1F>("hPart2","average avg time partition 2", 25, -12.5, 12.5);
  hPart3 = FileService->make<TH1F>("hPart3","average avg time partition 3", 25, -12.5, 12.5);

}


MakeTimingMaps::~MakeTimingMaps()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

std::string MakeTimingMaps::int2string(int i) {
  stringstream ss;
  string ret;
  ss << i;
  ss >> ret;
  return ret;
}

// ------------ method called for each event  ------------
void
MakeTimingMaps::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

Handle<HBHERecHitCollection> hRecHits;
   Handle<PCaloHitContainer> hSimHits;
   iEvent.getByLabel(InputTag(sHBHERecHitCollection),hRecHits);
   
   // get the run number of the event 
   RunNumber = iEvent.id().run();
   
  double nrh2 = 0;
  double nrh3 = 0;
  double nrh1 = 0;
  double np1 = 0;
  double np2 = 0;
  double np3 = 0;
  for(int i = 0; i < (int)hRecHits->size(); i++)
  {
    ClearVariables();
    RunNumber = iEvent.id().run();
    EvtNumber = iEvent.id().event();
    if(RunNumber != runNumber_ /*&& RunNumber!=251244 && RunNumber!=251252 && RunNumber!=251562 && RunNumber!=251561 && RunNumber!=251643*/) continue;
    
    //if((*hRecHits)[i].energy() < 1) continue;
    HcalDetId detID_rh = (*hRecHits)[i].id().rawId();
    depth = (*hRecHits)[i].id().depth();
    RecHitEnergy = (*hRecHits)[i].energy();
    RecHitTime = (*hRecHits)[i].time();
    iEta = detID_rh.ieta();
    iPhi = detID_rh.iphi();
   
    if(RecHitEnergy> 1.0){
//       ++nrh;
      if(detID_rh.iphi()>=3 && detID_rh.iphi() < 27){
        np1+=RecHitTime;
        ++nrh1;
//           hHBHETiming_SmallStats->Fill(1,RecHitTime);
//           hPart1->Fill(RecHitTime);
        }
        if(detID_rh.iphi()>=27 && detID_rh.iphi() < 51){
          np2+=RecHitTime;
          ++nrh2;
//           hHBHETiming_SmallStats->Fill(2,RecHitTime);
//           hPart2->Fill(RecHitTime);
        }
        if(detID_rh.iphi()>=51 || detID_rh.iphi() < 3){
          np3+=RecHitTime;
          ++nrh3;
//           hHBHETiming_SmallStats->Fill(3,RecHitTime);
//           hPart3->Fill(RecHitTime);
        }
    }
    
    if(RecHitEnergy > 5.0) {
//       std::cout << "ieta = " << detID_rh.ieta() << " and index = " << detID_rh.ieta()+29 << std::endl;
//       std::cout << "iphi = " << detID_rh.iphi() << " and index = " << detID_rh.iphi()-1 << std::endl;
      
      if(depth==1){
        hHBHETiming_Depth1->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitTime);
        if(RecHitEnergy > 10.0) hHBHETiming_wide_Depth1->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitTime);
        hHBHEEnergy_Depth1->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitEnergy);
        occupancy_d1->Fill(detID_rh.ieta(), detID_rh.iphi(),1);
        hTiming_Depth1[detID_rh.ieta()+29][detID_rh.iphi()-1]->Fill(RecHitTime);
      }
      if(depth==2){
        hHBHETiming_Depth2->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitTime);
        if(RecHitEnergy > 10.0) hHBHETiming_wide_Depth2->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitTime);
        hHBHEEnergy_Depth2->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitEnergy);
        occupancy_d2->Fill(detID_rh.ieta(), detID_rh.iphi(),1);
        hTiming_Depth2[detID_rh.ieta()+29][detID_rh.iphi()-1]->Fill(RecHitTime);
      }
      if(depth==3){
        hHBHETiming_Depth3->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitTime);
        if(RecHitEnergy > 10.0) hHBHETiming_wide_Depth3->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitTime);
        hHBHEEnergy_Depth3->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitEnergy);
        occupancy_d3->Fill(detID_rh.ieta(), detID_rh.iphi(),1);
        
        int bin = 0;
        if(detID_rh.ieta() == -28) bin = 0;
        if(detID_rh.ieta() == -27) bin = 1;
        if(detID_rh.ieta() == -16) bin = 2;
        if(detID_rh.ieta() ==  16) bin = 3;
        if(detID_rh.ieta() ==  27) bin = 4;
        if(detID_rh.ieta() ==  28) bin = 5;
        hTiming_Depth3[bin][detID_rh.iphi()-1]->Fill(RecHitTime);
      }
    }
    
    
//      OutputTree->Fill(); // comment in when looking at a few events
  }
  
  
   
     if(nrh1 > 0){
          hHBHETiming_SmallStats->Fill(1,np1/nrh1);
          hPart1->Fill(np1/nrh1);
     }
     if(nrh2> 0){
          hHBHETiming_SmallStats->Fill(2,np2/nrh2);
          hPart2->Fill(np2/nrh2);
     }
     if(nrh3 > 0) {
          hHBHETiming_SmallStats->Fill(3,np3/nrh3);
          hPart3->Fill(np3/nrh3);
     }
   

   
   
   
#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
MakeTimingMaps::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MakeTimingMaps::endJob() 
{
}

void MakeTimingMaps::ClearVariables(){
 RecHitEnergy = 0;
 RunNumber = 0;
 depth=0;
 iEta = 0;
 iPhi = 0;
 RecHitTime = 0;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MakeTimingMaps::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MakeTimingMaps);
