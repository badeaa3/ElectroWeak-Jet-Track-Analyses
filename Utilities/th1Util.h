/*
 * utilities related to TH1 objects.
 */

#include <TH1.h>
#include <TH1D.h>
#include <TF1.h>
#include <TGraphAsymmErrors.h>
#include <TBox.h>
#include <TString.h>
#include <TMath.h>

#include "interface/InputConfigurationParser.h"

#ifndef TH1UTIL_H_
#define TH1UTIL_H_

float resetTH1axisMin4LogScale(float axisMin, std::string axis);
std::string  summaryTH1(TH1* h);
TH1* Graph2Histogram(TGraphAsymmErrors* graph);
void setTH1_energyScale(TH1* h, float titleOffsetX = 1.25, float titleOffsetY = 1.75);
void setTH1_energyWidth(TH1* h, float titleOffsetX = 1.25, float titleOffsetY = 1.75);
void setTH1_efficiency (TH1* h, float titleOffsetX = 1.25, float titleOffsetY = 1.75);
double getMinimumTH1s(TH1D* h[], int nHistos);
double getMaximumTH1s(TH1D* h[], int nHistos);
void scaleBinErrors(TH1* h, double scale);
void scaleBinContentErrors(TH1* h, double scaleContent, double scaleError);
std::vector<double> getTH1xBins(TH1* h);
// systematic uncertainty
void fillTH1fromTF1(TH1* h, TF1* f);
void calcTH1Ratio4SysUnc(TH1* h, TH1* hNominal, float scaleFactor = 1);
void calcTF1Ratio4SysUnc(TH1* h, TF1* fNominal, TF1* fVaried, float scaleFactor = 1);
void subtractIdentity4SysUnc(TH1* h);
void addSysUnc(TH1* hTot, TH1* h);
void setSysUncBox(TBox* box, TH1* h, TH1* hSys, int bin, double binWidth = -1, double binWidthScale = 1);
void drawSysUncBoxes(TBox* box, TH1* h, TH1* hSys, double binWidth = -1, double binWidthScale = 1);

/*
 * reset the lower limit of an axis in case the plot will be drawn log scale and the relevant lower limit is non-positive.
 */
float resetTH1axisMin4LogScale(float axisMin, std::string axis)
{
    float result = axisMin;
    if (ToLower(axis.c_str()).EqualTo("x")) {
        if (result <= 0)   result = INPUT_DEFAULT::xMin;
    }
    else if (ToLower(axis.c_str()).EqualTo("y")) {
        if (result <= 0)   result = INPUT_DEFAULT::yMin;
    }
    return result;
}

/*
 * print info about histogram
 */
std::string summaryTH1(TH1* h)
{
    int binFirst = h->GetXaxis()->GetFirst();
    int binLast  = h->GetXaxis()->GetLast();
    std::string result;

    result.append(Form("h->GetEntries() = %f \n", h->GetEntries()));
    result.append(Form("h->GetNbinsX() = %d \n", h->GetNbinsX()));
    result.append(Form("h->GetXaxis()->GetXmin() = %f \n", h->GetXaxis()->GetXmin()));
    result.append(Form("h->GetXaxis()->GetXmax() = %f \n", h->GetXaxis()->GetXmax()));
    result.append(Form("BinLowEdge(h->GetXaxis()->GetFirst()) = %f \n", h->GetXaxis()->GetBinLowEdge(binFirst)));
    result.append(Form("BinLowEdge(h->GetXaxis()->GetLast()) = %f \n", h->GetXaxis()->GetBinLowEdge(binLast)));
    result.append(Form("h->GetBinContent(0) = %f \n", h->GetBinContent(0)));
    result.append(Form("h->GetBinContent(NbinsX+1) = %f \n", h->GetBinContent(h->GetNbinsX()+1)));
    result.append(Form("h->Integral() = %f \n", h->Integral()));
    result.append(Form("h->Integral(\"width\") = %f \n", h->Integral("width")));
    result.append(Form("h->Integral(firstBin, lastBin) = %f \n", h->Integral(binFirst, binLast)));
    result.append(Form("h->GetSumOfWeights() = %f \n", h->GetSumOfWeights()));
    result.append(Form("h->GetMean() = %f , h->GetMeanError() = %f \n", h->GetMean(), h->GetMeanError()));
    result.append(Form("h->GetStdDev() = %f , h->GetStdDevError() = %f \n", h->GetStdDev(), h->GetStdDevError()));
    result.append(Form("h->GetMinimum() = %f , h->GetMaximum() = %f ", h->GetMinimum(), h->GetMaximum()));
    // do not put a new line to the end of the last line

    return result;
}

/*
 * convert a TGraphAsymmErrors to 1D TH1 by setting the data points bin by bin.
 * NOTE : cannot set the asymmetric error bars for TH1. So cannot carry over the asymmetric errors
 * in "graph" to the histogram that is returned.
 * The error in the histogram will be
 * TMath::Sqrt(0.5*(elow*elow + ehigh*ehigh))  which is returned by graph->GetErrorY(i)
 * https://root.cern.ch/root/html/src/TGraphAsymmErrors.cxx.html#rBLO5D
 */
TH1* Graph2Histogram(TGraphAsymmErrors* graph)
{
    int fNpoints = graph->GetN();
    double* fX   = graph->GetX();
    double* fY   = graph->GetY();

    if (fNpoints == 0) {
        return NULL;
    }

    // prepare x-bins for the histograms
    // xbins  : array of low-edges for each bin
    //          This is an array of size nbins+1
    double xbins[fNpoints+1];
    for (int i=0; i<fNpoints; ++i)  {
        xbins[i]=fX[i]-graph->GetErrorXlow(i);
    }
    xbins[fNpoints]=fX[fNpoints-1]+graph->GetErrorXhigh(fNpoints-1);

    TH1* h = new TH1D(graph->GetName(),graph->GetTitle(), fNpoints, xbins);

    for (int i=0; i<fNpoints; ++i)  {
        h->SetBinContent(i+1, fY[i]);
        h->SetBinError(i+1, graph->GetErrorY(i));
    }

    return h;
}

void setTH1_energyScale(TH1* h, float titleOffsetX, float titleOffsetY) {

    h->SetYTitle("< Reco p_{T} / Gen p_{T} >");
    h->SetTitleOffset(titleOffsetX, "X");
    h->SetTitleOffset(titleOffsetY, "Y");
    h->SetAxisRange(0.8,1.5,"Y");
    h->SetStats(false);
    h->SetMarkerStyle(kFullCircle);
}

void setTH1_energyWidth(TH1* h, float titleOffsetX, float titleOffsetY) {

    h->SetYTitle("#sigma( Reco p_{T} / Gen p_{T} )");
    h->SetTitleOffset(titleOffsetX, "X");
    h->SetTitleOffset(titleOffsetY, "Y");
    h->SetAxisRange(0,0.5,"Y");
    h->SetStats(false);
    h->SetMarkerStyle(kFullCircle);
}

void setTH1_efficiency(TH1* h, float titleOffsetX, float titleOffsetY) {

    h->SetTitleOffset(titleOffsetX, "X");
    h->SetTitleOffset(titleOffsetY, "Y");
    h->SetStats(false);
    h->SetMarkerStyle(kFullCircle);
}

/*
 * get minimum of an array of TH1D
 */
double getMinimumTH1s(TH1D* h[], int nHistos) {

    double result = h[0]->GetMinimum();
    for (int i = 1; i < nHistos; ++i) {
        if (h[i]->GetMinimum() < result)  result = h[i]->GetMinimum();
    }

    return result;
}

/*
 * get maximum of an array of TH1D
 */
double getMaximumTH1s(TH1D* h[], int nHistos) {

    double result = h[0]->GetMaximum();
    for (int i = 1; i < nHistos; ++i) {
        if (h[i]->GetMaximum() > result)  result = h[i]->GetMaximum();
    }

    return result;
}

void scaleBinErrors(TH1* h, double scale)
{
    int nBins = h->GetNbinsX();
    for ( int i = 0; i <= nBins+1; i++)
    {
        h->SetBinError(i, h->GetBinError(i)*scale);
    }
}

void scaleBinContentErrors(TH1* h, double scaleContent, double scaleError)
{
    int nBins = h->GetNbinsX();
    for ( int i = 0; i <= nBins+1; i++)
    {
        h->SetBinContent(i, h->GetBinContent(i)*scaleContent);
        h->SetBinError(i,   h->GetBinError(i)*scaleError);
    }
}

/*
 * returns the bins along x-axis of a "TH1" object as a std::vector.
 * size of the vector is nBins+1.
 * ith element is the lower edge of bin i.
 * last element is the upper edge of the last bin.
 */
std::vector<double> getTH1xBins(TH1* h) {

    std::vector<double> bins;
    int nBins = h->GetNbinsX();
    for ( int i = 1; i <= nBins; i++)
    {
        bins.push_back(h->GetXaxis()->GetBinLowEdge(i));
        if (i == nBins) bins.push_back(h->GetXaxis()->GetBinUpEdge(i));
    }

    return bins;
}



void fillTH1fromTF1(TH1* h, TF1* f)
{
    int nBins = h->GetNbinsX();
    for ( int i = 1; i <= nBins; i++)
    {
        double x = h->GetBinCenter(i);
        double y = f->Eval(x);

        h->SetBinContent(i, y);
        h->SetBinError(i, 0.0001);
    }
}

/*
 * it is assumed that "h" contains the content of hVaried, in particular "h" is clone of hVaried
 * h = hRatio = (( (hVaried - hNominal) * scaleFactor ) - hNominal ) / hNominal
 * if scaleFactor = 1, then : h = hRatio = hVaried / hNominal
 */
void calcTH1Ratio4SysUnc(TH1* h, TH1* hNominal, float scaleFactor)
{
    if (scaleFactor != 1) {
        h->Add(hNominal,-1);
        h->Scale(scaleFactor);
        h->Add(hNominal);
    }
    h->Divide(hNominal);
}

/*
 * it is assumed that "h" is a dummy clone of the histogram for which the uncertainty is calculated.
 */
void calcTF1Ratio4SysUnc(TH1* h, TF1* fNominal, TF1* fVaried, float scaleFactor)
{
  int nBins = h->GetNbinsX();
  for ( int i = 1; i <= nBins; i++)
  {
      double x = h->GetBinCenter(i);
      double yNom = fNominal->Eval(x);
      double yVar = fVaried->Eval(x);

      if (scaleFactor != 1)  h->SetBinContent(i, ((yVar - yNom)*scaleFactor + yNom) / yNom);
      else  h->SetBinContent(i, yVar/yNom);
      h->SetBinError(i, 0.0001);
  }
}

/*
 * "h" is a clone of hRatio.
 * absolute value of the entries of "h" will be uncertainty.
 */
void subtractIdentity4SysUnc(TH1* h)
{
    int nBins = h -> GetNbinsX();
    for(int i = 1; i <= nBins; i++)
    {
        float val   = TMath::Abs(h->GetBinContent(i)) - 1;
        float error = h->GetBinError(i) - 1;
        if( val == -1 )
        {
            val = 0;
            error = 0;
        }

        h->SetBinContent(i, val);
        h->SetBinError(i,error);
    }
}

/*
 * "hTot" is the histogram that contains total uncertainty
 * add the individual uncertainty contribution from "h"
 */
void addSysUnc(TH1* hTot, TH1* h)
{
    int nBins = hTot->GetNbinsX();
    for (int i = 1; i <= nBins; ++i)
    {
        double unc1 = hTot->GetBinContent(i);
        double unc2 = h->GetBinContent(i);
        double uncTot = TMath::Sqrt(unc1*unc1 + unc2*unc2);

        double err1 = hTot->GetBinError(i);
        double err2 = h->GetBinError(i);
        double errTot = TMath::Sqrt(err1*err1 + err2*err2);

        hTot->SetBinContent(i, uncTot);
        hTot->SetBinError(i, errTot);
        // hTot->SetBinError(i, 0.0001);
    }
}

void setSysUncBox(TBox* box, TH1* h, TH1* hSys, int bin, double binWidth, double binWidthScale)
{
   double val = h->GetBinContent(bin);
   double x   = h->GetBinCenter(bin);
   int binSys = hSys->FindBin(x);

   // double error = TMath::Abs(val * hSys->GetBinContent(binSys));    // if the uncertainty is calculated using ratios
   double error = TMath::Abs(hSys->GetBinContent(binSys));             // if the uncertainty is calculated using differences
   std::string hSysName = hSys->GetName();
   if (hSysName.find("ratio") != std::string::npos)  error = TMath::Abs(val * hSys->GetBinContent(binSys));

   if (binWidth < 0) {
     binWidth = h->GetBinLowEdge(bin+1) - h->GetBinLowEdge(bin);
   }

   double errorLow = val - error;
   double errorUp = val + error;
   if (errorLow < h->GetMinimum())  errorLow = h->GetMinimum();
   if (errorUp  > h->GetMaximum())  errorUp = h->GetMaximum();

   box->SetX1(x - (binWidth/2)*binWidthScale);
   box->SetX2(x + (binWidth/2)*binWidthScale);
   box->SetY1(errorLow);
   box->SetY2(errorUp);
}

void drawSysUncBoxes(TBox* box, TH1* h, TH1* hSys, double binWidth, double binWidthScale)
{
    int nBins = h->GetNbinsX();
    for (int i = 1; i <= nBins; ++i) {
        if (h->GetBinError(i) == 0) continue;
        if (h->GetBinContent(i) < h->GetMinimum()) continue;
        if (h->GetBinContent(i) > h->GetMaximum()) continue;

        setSysUncBox(box, h, hSys, i, binWidth, binWidthScale);
        box->DrawClone();
    }
}

#endif /* TH1UTIL_H_ */

