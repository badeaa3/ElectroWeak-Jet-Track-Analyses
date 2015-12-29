/*
 * utilities related to TCanvas, TPad objects.
 */

#include <TCanvas.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TList.h>
#include <TH1.h>
#include <TH1D.h>
#include <TAttFill.h>

#include <string>
#include <vector>

#ifndef CANVASUTIL_H_
#define CANVASUTIL_H_

void setCanvasFinal(TCanvas* c);
void setTH1Final   (TH1* c);
void setLegendFinal(TLegend* legend);
void setLegendPosition(TLegend* legend, std::string position, TCanvas* c, double height, double width);

void setCanvasTLatex(TCanvas* c, float px, float py, std::vector<std::string> lines, float pyOffset = 0.05);

void setCanvas_InvMass(TCanvas* c, float px = 0.65, float py = 0.85,
                       const char* collision = "CMS pp", const char* energy    = "#sqrt{s} = 5.02 TeV",
                       const char* ptCut     = "p^{e}_{T} > 20 GeV/c", const char* etaCut = "|#eta^{e}| <1 .44",
                       const char* extra = "");

void setCanvas_InvMass_pp(TCanvas* c, float px = 0.65, float py = 0.85,
                          const char* ptCut = "p^{e}_{T} > 20 GeV/c", const char* etaCut = "|#eta^{e}| < 1.44",
                          const char* extra = "");

void setCanvas_InvMass_PbPb(TCanvas* c, float px = 0.65, float py = 0.85,
                          const char* ptCut = "p^{e}_{T} > 20 GeV/c", const char* etaCut = "|#eta^{e}| < 1.44",
                          const char* extra = "");

void setCanvasFinal(TCanvas* c)
{
    c->SetBorderMode(0);
    c->SetBorderSize(0);
    c->SetFrameBorderMode(0);
    c->SetFrameLineColor(0);

    // put ticks to upper and right part of the axis.
    c->SetTickx(1);
    c->SetTicky(1);
}

void setTH1Final(TH1* h)
{
    h->SetTitleOffset(1.25, "X");
    h->SetTitleOffset(1.5,  "Y");

    h->SetTitle("");
    h->SetStats(false);
}

void setLegendFinal(TLegend* legend)
{
    legend->SetBorderSize(0);
    legend->SetTextFont(43);
    legend->SetTextSize(20*0.8);
    legend->SetLineColor(1);
    legend->SetLineStyle(kSolid); // kSolid = 1
    legend->SetLineWidth(1);
    legend->SetFillColor(kWhite);   // kWhite = 0
    legend->SetFillStyle(0);  // kFEmpty = 0
}

void setLegendPosition(TLegend* legend, std::string position, TCanvas* c, double height, double width)
{
    // histograms for different centralities
    if (position.compare("NW") == 0) { // upper-left corner
        legend->SetX1(c->GetLeftMargin());
        legend->SetY1(1 - c->GetTopMargin() - height);
        legend->SetX2(c->GetLeftMargin() + width);
        legend->SetY2(1 - c->GetTopMargin());
    }
    else if (position.compare("NE") == 0) { // upper-right corner
        legend->SetX1(1 - c->GetRightMargin() - width);
        legend->SetY1(1 - c->GetTopMargin() - height);
        legend->SetX2(1 - c->GetRightMargin());
        legend->SetY2(1 - c->GetTopMargin());
    }
    else if (position.compare("SW") == 0) { // lower-left corner
        legend->SetX1(c->GetLeftMargin());
        legend->SetY1(c->GetBottomMargin());
        legend->SetX2(c->GetLeftMargin() + width);
        legend->SetY2(c->GetBottomMargin() + height);
    }
    else if (position.compare("SE") == 0) { // lower-right corner
        legend->SetX1(1 - c->GetRightMargin() - width);
        legend->SetY1(c->GetBottomMargin());
        legend->SetX2(1 - c->GetRightMargin());
        legend->SetY2(c->GetBottomMargin() + height);
    }
}

void setCanvasTLatex(TCanvas* c, float px, float py, std::vector<std::string> lines, float pyOffset)
{
    c->cd();

    float pyNew = py;
    TLatex* latex;
    for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it){

        latex = new TLatex(px, pyNew, (*it).c_str());
        latex->SetTextFont(43);
        latex->SetTextSize(20*0.8);
        latex->SetNDC();
        latex->Draw();
        pyNew = pyNew - pyOffset;
    }
}

void setCanvas_InvMass(TCanvas* c, float px, float py, const char* collision, const char* energy,
                       const char* ptCut, const char* etaCut, const char* extra)
{
  c->cd();

  float pyNew = py;
  TLatex *latexCMS = new TLatex(px,pyNew, collision);
  latexCMS->SetTextFont(43);
  latexCMS->SetTextSize(20);
  latexCMS->SetNDC();
  latexCMS->Draw();

  pyNew = pyNew-0.05;
  TLatex *latexEnergy = new TLatex(px, pyNew, energy);
  latexEnergy->SetTextFont(43);
  latexEnergy->SetTextSize(20);
  latexEnergy->SetNDC();
  latexEnergy->Draw();

  pyNew = pyNew-0.05;
  TLatex *latexPt = new TLatex(px, pyNew, ptCut);
  latexPt->SetTextFont(43);
  latexPt->SetTextSize(20);
  latexPt->SetNDC();
  latexPt->Draw();

  pyNew = pyNew-0.06;
  TLatex *latexEta = new TLatex(px, pyNew, etaCut);
  latexEta->SetTextFont(43);
  latexEta->SetTextSize(20);
  latexEta->SetNDC();
  latexEta->Draw();

  std::string str = extra;
  if (str.compare("") != 0)
  {
      pyNew = pyNew-0.05;
      TLatex *latexExtra = new TLatex(px, pyNew, extra);
      latexExtra->SetTextFont(43);
      latexExtra->SetTextSize(20);
      latexExtra->SetNDC();
      latexExtra->Draw();
  }
}

void setCanvas_InvMass_pp(TCanvas* c, float px, float py, const char* ptCut, const char* etaCut, const char* extra)
{
    setCanvas_InvMass(c, px, py, "CMS preliminary pp", "#sqrt{s} = 5.02 TeV", ptCut, etaCut, extra);
}

void setCanvas_InvMass_PbPb(TCanvas* c, float px, float py, const char* ptCut, const char* etaCut, const char* extra)
{
    setCanvas_InvMass(c, px, py, "CMS preliminary PbPb", "#sqrt{s_{NN}} = 5.02 TeV", ptCut, etaCut, extra);
}

void easyLeg( TLegend *legend=0 , const char* head="")
{
  legend->SetBorderSize(0);
  legend->SetHeader(head);
  legend->SetTextFont(43);
  legend->SetTextSize(20);
  legend->SetLineColor(1);
  legend->SetLineStyle(kSolid); // kSolid = 1
  legend->SetLineWidth(1);
  legend->SetFillColor(kWhite);   // kWhite = 0
  legend->SetFillStyle(0);  // kFEmpty = 0
}

/*
 * h1 is assumed to be the histogram for opposite charge.
 * h2 is assumed to be the histogram for same charge.
 */
void setCanvas_InvMass_Histo2Legend(TCanvas* c, TH1* h1, TH1* h2)
{
    c->cd();

    TLegend *legend = new TLegend(0.1,0.75,0.4,0.9,"","brNDC");
    easyLeg(legend,"");

    legend->AddEntry(h1, "opposite charge", "p");
    legend->AddEntry(h2, "same charge", "p");

    legend->Draw();
}

#endif /* CANVASUTIL_H_ */