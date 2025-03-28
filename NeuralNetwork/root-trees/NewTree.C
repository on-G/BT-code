#if !defined(__CINT__) || defined(__MAKECINT__)

#include <Riostream.h>
#include <TFile.h>
#include <TROOT.h>
#include <TTree.h>

#endif

void NewTree() {
    TFile *fileS = new TFile("./signalNew_1_2.root");
    TFile *fileB = new TFile("./background_1_2.root");
    TFile *newfile = new TFile("SB_simul.root", "recreate");

    fileS->ls();
    TTree *treeS = (TTree*)fileS->Get("treeList_0_24_0_24_Sgn");
    newfile->cd();
    treeS->SetName("treeS");
    treeS->CloneTree()->Write("", TObject::kWriteDelete);

    fileB->ls();
    TTree *treeB = (TTree*)fileB->Get("treeList_0_24_0_24_Sgn");
    newfile->cd();
    treeB->SetName("treeB");
    treeB->CloneTree()->Write("", TObject::kWriteDelete);

    delete fileS;
    delete fileB;
    delete newfile;
}