#if !defined(__CINT__) || defined(__MAKECINT__)

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TH1D.h"
#include "THnSparse.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TPaveText.h"
#include "TString.h"
#include "TROOT.h"
#include "TGaxis.h"
#include "TStyle.h"
#include "TGraph.h"
#include <TDatabasePDG.h>
#include <fstream>

#endif

void SetStyle();

Double_t mLcPDG  = TDatabasePDG::Instance()->GetParticle(4122)->Mass();
Float_t iMassMin = 3*0.0076;
Float_t iMassMax = 3*0.0076;
	     
Double_t mygaus(Double_t* x, Double_t* par)
{
  Double_t total = par[0] * (TMath::Gaus(x[0], par[1], par[2], 1));
  return total;
}

Double_t fitBackground(Double_t* x, Double_t* par)
{
  Double_t total = par[0] + par[1]*x[0] + par[2]*x[0]*x[0] /*+ par[3]*x[0]*x[0]*x[0]*/;
  if(x[0] > mLcPDG - iMassMin && x[0] < mLcPDG + iMassMax) total = 0;
  return total;
}

Double_t fitBackground_fullRange(Double_t* x, Double_t* par)
{
  Double_t total = par[0] + par[1]*x[0] + par[2]*x[0]*x[0] /*+ par[3]*x[0]*x[0]*x[0]*/;
  return total;
}

Double_t fitFunc(Double_t* x, Double_t* par)
{
  Double_t total = fitBackground_fullRange(x,par) + mygaus(x,&par[/*4*/3]);
  return total;
}

void fitTest(Int_t iBin = 0, Int_t rebin = 1, Float_t iMin = -1, Float_t iMax = 1, Float_t massMin = 2.0865, Float_t massMax = 2.4865, Int_t nsigma = 3) {

  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetPalette(1);
  gStyle->SetCanvasColor(0);
  gStyle->SetFrameFillColor(0);
  gStyle->SetOptTitle(0);

  TDatime date;
  Int_t year = date.GetYear();
  Int_t month = date.GetMonth();
  Int_t day = date.GetDay();

  TGaxis::SetMaxDigits(3);
  gStyle->SetOptStat(1111111);
  gStyle->SetCanvasColor(0);
  gStyle->SetFrameFillColor(0);

  SetStyle();

  //Int_t ptmin = 1, ptmax = 2;

  Float_t sigmaMC[15] = {0.0076, 0.0076, 0.0076, 0.0079, 0.0077, 0.0084, 0.0090, 0.0087, 0.0096, 0.0101, 0.0098, 0.0108, 0.0118, 0.0111, 0.0136};
  Float_t errSigmaMC[15] = {0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0002, 0.0002, 0.0002, 0.0002, 0.0002, 0.0002, 0.0002};
  
  iMassMin = nsigma*sigmaMC[iBin];
  iMassMax = nsigma*sigmaMC[iBin];
  
  //TFile *fileIn = new TFile("applicationfile_20230919.root");
  /*TFile *fileIn = new TFile("applicationfile_20230923.root");
  TH2F *hData;
  hData = (TH2F*)fileIn->Get("MVA_vs_InvMass");
  hData->GetXaxis()->SetRangeUser(iMin, iMax);
  TH1D *histogram = (TH1D*)hData->ProjectionY("histogram");       //Penso sia l'istogramma delle masse */
  TH1D *histogram = new TH1D("histogram", "Mass distribution", 40, 2.0865, 2.4865);
  ifstream filein("Mass.txt");
  Double_t x;
  while (filein.good()){
    filein >> x;
    histogram->Fill(x);
  }
  TH1D *histogramToBeFitted = (TH1D*)histogram->Clone("histogramToBeFitted");
  
  histogramToBeFitted->Rebin(rebin);
  
  TCanvas *canvas = new TCanvas("FitResult", "FitResult", 10, 10, 1050, 550);
  canvas->Divide(2,1);
  canvas->cd(1);
  histogramToBeFitted->Draw("PE");
  histogramToBeFitted->GetXaxis()->SetRangeUser(massMin ,massMax);
  histogramToBeFitted->GetXaxis()->SetTitleFont(42);
  histogramToBeFitted->GetXaxis()->SetLabelFont(42);
  histogramToBeFitted->GetYaxis()->SetTitleFont(42);
  histogramToBeFitted->GetYaxis()->SetLabelFont(42);
  histogramToBeFitted->GetYaxis()->SetTitle(Form("Entries / %3.1f MeV/#it{c}^{2}", histogramToBeFitted->GetBinWidth(1)*1000.));
  histogramToBeFitted->GetXaxis()->SetTitle(Form("m_{inv}(pK_{S}^{0})[GeV/#it{c}^{2}]"));
  histogramToBeFitted->GetYaxis()->SetRangeUser(/*histogramToBeFitted->GetMinimum()*0.8*/ 150000, /*histogramToBeFitted->GetMaximum()*1.05*/168000);
  histogramToBeFitted->GetYaxis()->SetTitleOffset(2);
  histogramToBeFitted->GetXaxis()->SetTitleOffset(1.25);
  histogramToBeFitted->GetXaxis()->SetTitleSize(0.035);
  histogramToBeFitted->GetXaxis()->SetLabelSize(0.035);
  histogramToBeFitted->GetYaxis()->SetTitleSize(0.035);
  histogramToBeFitted->GetYaxis()->SetLabelSize(0.035);
  histogramToBeFitted->SetMarkerStyle(20);
  histogramToBeFitted->SetMarkerColor(kBlack);  
  histogramToBeFitted->SetLineColor(kBlack);  

  printf("Minimum: %0.4f, bin: %0.4f at mass: %0.4f", histogramToBeFitted->GetMinimum(), histogramToBeFitted->GetMinimumBin(), histogramToBeFitted->GetBinCenter(histogramToBeFitted->GetMinimumBin()));

  Double_t parB[/*4*/3];
  Double_t parS[3];
  Double_t par[/*7*/6];
  
  TF1 *f1 = new TF1("f1","pol2", 2.0865, 2.4865);
  histogramToBeFitted->Fit(f1, "0R+", "ep", massMin, massMax);
  f1->GetParameters(parB);

  TF1 *fitB = new TF1("fitB", fitBackground, 2.0865, 2.4865, /*4*/3);
  fitB->SetParameters(parB);
  histogramToBeFitted->Fit(fitB, "0R+", "ep", massMin, massMax);
  
  TF1 *fitB_fullRange = new TF1("fitB_fullRange", fitBackground_fullRange, 2.0865, 2.4865, /*4*/3);
  fitB->GetParameters(parB);
  fitB_fullRange->SetParameters(parB);
  fitB_fullRange->SetLineStyle(7);
  fitB_fullRange->Draw("same");

  TF1 *total = new TF1("total", fitFunc, 2.0865, 2.4865, /*7*/6);
  total->SetLineColor(2);
  total->SetLineWidth(3);
  total->SetParameter(0, parB[0]); 
  total->SetParameter(1, parB[1]); 
  total->SetParameter(2, parB[2]);
  //total->SetParameter(3, parB[3]); 
  total->SetParameter(/*5*/4, mLcPDG); 
  total->FixParameter(/*6*/5, 0.0076); 
  histogramToBeFitted->Fit(total, "R+", "ep", massMin, massMax);
  total->GetParameters(par);
 
  Float_t ymin = 0;
  Float_t ymax = 0;
  TH1F *hh2 = (TH1F*)histogramToBeFitted->Clone("hh2");
  for(Int_t i=0; i<hh2->GetNbinsX();i++) {
    hh2->SetBinContent(i+1, fitB_fullRange->Eval(hh2->GetBinCenter(i+1)));
    hh2->SetBinError(i+1, TMath::Sqrt(fitB_fullRange->Eval(hh2->GetBinCenter(i+1))));
  }
  
  canvas->cd(2);
  TH1F *hh = (TH1F*)histogramToBeFitted->Clone("hh");
  for(Int_t i=0; i<hh->GetNbinsX(); i++) {
    Float_t yy = histogramToBeFitted->GetBinContent(i+1) - hh2->GetBinContent(i+1);
    hh->SetBinContent(i+1, yy);
    if(hh->GetBinCenter(i+1) > massMin && hh->GetBinCenter(i+1) < massMax) {
      ymin = TMath::Min(ymin, yy);
      ymax = TMath::Max(ymax, yy);
    }
    hh->SetBinError(i+1, TMath::Sqrt(histogramToBeFitted->GetBinError(i+1)*histogramToBeFitted->GetBinError(i+1) + hh2->GetBinError(i+1)*hh2->GetBinError(i+1)));
  }
  hh->SetMaximum(3*ymax);
  hh->SetMinimum(3*ymin);

  TF1 *fitS = new TF1("fitS", mygaus, massMin, massMax, 3);
  fitS->SetParameters(&par[3]);
  fitS->SetName("fitS");
  fitS->SetParName(0, "const");
  fitS->SetParName(1, "Mass");
  fitS->SetParName(2, "Sigma");
  fitS->SetLineColor(kRed);

  fitS->SetLineWidth(3);
  hh->Draw();
  hh->GetYaxis()->SetRangeUser(hh->GetMinimum()*0.8, hh->GetMaximum()*1.05);
  hh->GetXaxis()->SetRangeUser(massMin, massMax);
  hh->SetMarkerStyle(20);
  hh->SetMarkerColor(kBlack);
  hh->SetLineColor(kBlack);
  fitS->Draw("same");
  
  Float_t binwidth = hh->GetBinWidth(1);
  Float_t signal = total->GetParameter(/*4*/3)/histogramToBeFitted->GetBinWidth(1);
  Float_t errSignal = total->GetParError(/*4*/3)/histogramToBeFitted->GetBinWidth(1);
  //Float_t background = fitB_fullRange->Integral(mLcPDG - iMassMin, mLcPDG + iMassMax)/histogramToBeFitted->GetBinWidth(1);
  Float_t background = fitB_fullRange->Integral(mLcPDG - 3*sigmaMC[iBin], mLcPDG + 3*sigmaMC[iBin])/histogramToBeFitted->GetBinWidth(1);
  //Float_t yield = total->Integral(mLcPDG - iMassMin, mLcPDG + iMassMax)/histogramToBeFitted->GetBinWidth(1) - background;
  Float_t yield = total->Integral(mLcPDG - 3*sigmaMC[iBin], mLcPDG + 3*sigmaMC[iBin])/histogramToBeFitted->GetBinWidth(1) - background;
  
  /* Bin counting --> SKIP for now
  Double_t cntErr[2];
  Double_t cntSig[2];
  Double_t errRyBC[2];

  Int_t minBinSum = histogramToBeFitted->FindBin(mLcPDG - 3*sigmaMC[iBin]);
  Int_t maxBinSum = histogramToBeFitted->FindBin(mLcPDG + 3*sigmaMC[iBin]);

  if(minBinSum<1){
    printf("Left range for bin counting smaller than allowed by histogram axis, setting it to the lower edge of the first histo bin\n");
    minBinSum=1;
  }
  if(maxBinSum>hh->GetNbinsX()){
    printf("Right range for bin counting larger than allowed by histogram axis, setting it to the upper edge of the last histo bin\n");
    maxBinSum=hh->GetNbinsX();
  }

  for(Int_t jb=minBinSum; jb<=maxBinSum; jb++){
    Double_t cntTot = hh->GetBinContent(jb);
    cntSig[0] += cntTot;
    cntErr[0] += hh->GetBinError(jb)*hh->GetBinError(jb);
  }
  errRyBC[0] = TMath::Sqrt(cntErr[0]);

  minBinSum = histogramToBeFitted->FindBin(mLcPDG - 5*sigmaMC[iBin]);
  maxBinSum = histogramToBeFitted->FindBin(mLcPDG + 5*sigmaMC[iBin]);

  if(minBinSum<1){
    printf("Left range for bin counting smaller than allowed by histogram axis, setting it to the lower edge of the first histo bin\n");
    minBinSum=1;
  }
  if(maxBinSum>hh->GetNbinsX()){
    printf("Right range for bin counting larger than allowed by histogram axis, setting it to the upper edge of the last histo bin\n");
    maxBinSum=hh->GetNbinsX();
  }

  for(Int_t jb=minBinSum; jb<=maxBinSum; jb++){
    Double_t cntTot = hh->GetBinContent(jb);
    cntSig[1] += cntTot;
    cntErr[1] += hh->GetBinError(jb)*hh->GetBinError(jb);
  }
  errRyBC[1] = TMath::Sqrt(cntErr[1]);
  */
  
  canvas->cd(1);
  TPaveText *pinfo = new TPaveText(0.2,0.2,0.7,0.4,"NDC");
  pinfo->SetBorderSize(0);
  pinfo->SetFillStyle(0);
  
  TString str = Form("Mean: %.4f #pm %.4f", total->GetParameter(/*5*/4), total->GetParError(/*5*/4));
  pinfo->AddText(str);
  str = Form("Sigma: %.4f Fixed", total->GetParameter(/*6*/5)/*, total->GetParError(5)*/);
  pinfo->AddText(str);
  str = Form("Signal (3#sigma): %.0f #pm %.0f (%.03f)", signal, errSignal, errSignal/signal);
  pinfo->AddText(str);
  str = Form("S/B: %.4f", yield/background);
  pinfo->AddText(str);   
  pinfo->Draw("same");

  printf("Significance = %0.2f \n", yield/(TMath::Sqrt(total->Integral(mLcPDG - 3*sigmaMC[iBin], mLcPDG + 3*sigmaMC[iBin])/histogramToBeFitted->GetBinWidth(1))));
  printf("S/sigma(S) = %0.2f \n", signal/errSignal);
  
}

void SetStyle() {
  cout << "Setting style!" << endl;
  
  gStyle->Reset("Plain");
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  gStyle->SetCanvasColor(10);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetFrameLineWidth(1);
  gStyle->SetFrameFillColor(kWhite);
  gStyle->SetPadColor(10);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetHistLineWidth(1);
  gStyle->SetHistLineColor(kRed);
  gStyle->SetFuncWidth(2);
  gStyle->SetFuncColor(kGreen);
  gStyle->SetLineWidth(2);
  gStyle->SetLabelSize(0.045,"xyz");
  gStyle->SetLabelOffset(0.01,"y");
  gStyle->SetLabelOffset(0.01,"x");
  gStyle->SetLabelColor(kBlack,"xyz");
  gStyle->SetTitleSize(0.05,"xyz");
  gStyle->SetTitleOffset(1.25,"y");
  gStyle->SetTitleOffset(1.2,"x");
  gStyle->SetTitleFillColor(kWhite);
  gStyle->SetTextSizePixels(26);
  gStyle->SetTextFont(42);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetLegendFillColor(kWhite);
  gStyle->SetLegendFont(42);
}
