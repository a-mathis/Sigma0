#include "FileReader.h"
#include "Fitter.h"
#include "Plotter.h"
#include "TCanvas.h"
#include "TLegend.h"

int main(int argc, char* argv[]) {
  Plotter::SetStyle();
  auto filename = TString(argv[1]);
  auto appendix = TString(argv[2]);

  TString foldername = filename;
  foldername.ReplaceAll("AnalysisResults.root", "");

  // ========================================================================
  // Event properties
  auto cutStats = FileReader::GetHist1D(
      filename, appendix, {{"EventCuts", "AliEventCuts"}}, "fCutStats");
  Plotter::SetStyleHisto(cutStats, 24);

  auto vtxZ = FileReader::GetHist1D(
      filename, appendix, {{"EventCuts", "Event Cuts", "after"}}, "VtxZ_after");
  vtxZ->SetTitle(";z_{vertex} (cm); Entries;");
  Plotter::SetStyleHisto(vtxZ, 0);

  auto multRef08 = FileReader::GetHist1D(filename, appendix,
                                         {{"EventCuts", "Event Cuts", "after"}},
                                         "MultiplicityRef08_after");
  multRef08->SetTitle(";Ref08 multiplicity; Entries;");
  Plotter::SetStyleHisto(multRef08, 0);

  auto c1 = new TCanvas();
  cutStats->Draw();
  c1->Print(Form("%s/Plot/CutStats.pdf", foldername.Data()));

  auto c2 = new TCanvas("Cc", "c2", 1400, 500);
  c2->Divide(2, 1);
  c2->cd(1);
  c2->cd(1)->SetLogy();
  multRef08->Draw("hist");
  multRef08->GetXaxis()->SetRangeUser(0, 250);
  c2->cd(2);
  vtxZ->Draw("hist");
  c2->Print(Form("%s/Plot/MixingVariables.pdf", foldername.Data()));

  // ========================================================================
  // Proton properties

  auto protonPt = FileReader::GetHist1D(
      filename, appendix, {{"Proton", "after"}}, "pTDist_after", "femto");
  Plotter::SetStyleHisto(protonPt, 20);
  protonPt->SetTitle("; #it{p}_{T} (GeV/#it{c}); Entries;");

  auto protonPhi = FileReader::GetHist1D(
      filename, appendix, {{"Proton", "after"}}, "phiDist_after", "femto");
  Plotter::SetStyleHisto(protonPhi, 20);
  protonPhi->SetTitle("; #varphi (rad); Entries;");

  auto protonEta = FileReader::GetHist1D(
      filename, appendix, {{"Proton", "after"}}, "EtaDist_after", "femto");
  Plotter::SetStyleHisto(protonEta, 20);
  protonEta->SetTitle("; #eta; Entries;");

  auto protonDCAxy =
      (TH1F*)FileReader::GetHist2D(filename, appendix, {{"Proton", "after"}},
                                   "DCAXY_after", "femto")
          ->ProjectionY();
  Plotter::SetStyleHisto(protonDCAxy, 20);
  protonDCAxy->SetTitle("; DCA_{xy} (cm); Entries;");

  auto protonPIDTPC = FileReader::GetHist2D(
      filename, appendix, {{"Proton", "after"}}, "NSigTPC_after", "femto");
  Plotter::SetStyleHisto(protonPIDTPC, 20);
  protonPIDTPC->SetTitle("PID p TPC; #it{p} (GeV/#it{c}); n_{#sigma} TPC");

  auto protonPIDTOF = FileReader::GetHist2D(
      filename, appendix, {{"Proton", "after"}}, "NSigTOF_after", "femto");
  Plotter::SetStyleHisto(protonPIDTOF, 20);
  protonPIDTOF->SetTitle("PID p TOF; #it{p} (GeV/#it{c}); n_{#sigma} TOF");

  auto b1 = new TCanvas("b1", "b1", 1400, 1000);
  b1->Divide(2, 2);
  b1->cd(1);
  b1->cd(1)->SetLogy();
  protonPt->Draw("hist");
  b1->cd(2);
  protonPhi->SetMinimum(0);
  protonPhi->Draw("hist");
  b1->cd(3);
  b1->cd(3)->SetLogy();
  protonEta->GetXaxis()->SetRangeUser(-1, 1);
  protonEta->Draw("hist");
  b1->cd(4);
  b1->cd(4)->SetLogy();
  protonDCAxy->Draw("hist");
  protonDCAxy->GetXaxis()->SetRangeUser(-1, 1);
  b1->Print(Form("%s/Plot/ProtonKinematics.pdf", foldername.Data()));

  auto b2 = new TCanvas("b2", "b2", 1400, 500);
  b2->Divide(2, 1);
  b2->cd(1);
  b2->cd(1)->SetRightMargin(0.16);
  b2->cd(1)->SetLogz();
  protonPIDTPC->Draw("colz");
  b2->cd(2);
  b2->cd(2)->SetRightMargin(0.16);
  b2->cd(2)->SetLogz();
  protonPIDTOF->Draw("colz");
  b2->Print(Form("%s/Plot/ProtonPID.pdf", foldername.Data()));

  // ========================================================================
  // Photon properties

  // Inv. mass vs. pT
  auto photonInvMassPt = FileReader::GetHist2D(
      filename, appendix, {{"V0_Photon"}}, "fHistV0MassPt");
  Plotter::SetStyleHisto(photonInvMassPt, 20);
  auto photonInvMass = (TH1F*)photonInvMassPt->ProjectionY();
  Plotter::SetStyleHisto(photonInvMass, 20);
  photonInvMass->SetTitle(";M_{e^{+}e^{-}} (GeV/#it{c}^{2}); Entries;");
  auto photonPt = (TH1F*)photonInvMassPt->ProjectionX();
  Plotter::SetStyleHisto(photonPt, 20);
  photonPt->SetTitle("; #it{p}_{T} (GeV/#it{c}); Entries;");

  // Eta & phi
  auto photonEtaPhi =
      FileReader::GetHist2D(filename, appendix, {{"V0_Photon"}}, "fHistEtaPhi");
  Plotter::SetStyleHisto(photonEtaPhi, 20);
  auto photonPhi = (TH1F*)photonEtaPhi->ProjectionY();
  Plotter::SetStyleHisto(photonPhi, 20);
  photonPhi->SetTitle("; #varphi (rad); Entries;");
  auto photonEta = (TH1F*)photonEtaPhi->ProjectionX();
  Plotter::SetStyleHisto(photonEta, 20);
  photonEta->SetTitle("; #eta; Entries;");

  // Inv. mass under K0 hypothesis
  auto photonK0 =
      FileReader::GetHist1D(filename, appendix, {{"V0_Photon"}}, "fHistK0Mass");
  Plotter::SetStyleHisto(photonK0, 20);
  photonK0->SetTitle(";M_{#pi^{+}#pi^{-}} (GeV/#it{c}^{2}); Entries;");

  // Inv. mass under Lambda hypothesis
  auto photonLambda = FileReader::GetHist1D(filename, appendix, {{"V0_Photon"}},
                                            "fHistLambdaMass");
  Plotter::SetStyleHisto(photonLambda, 20);
  photonLambda->SetTitle(";M_{p#pi^{-}} (GeV/#it{c}^{2}); Entries;");

  // Inv. mass under Anti-Lambda hypothesis
  auto photonAntiLambda = FileReader::GetHist1D(
      filename, appendix, {{"V0_Photon"}}, "fHistAntiLambdaMass");
  Plotter::SetStyleHisto(photonAntiLambda, 20);
  photonAntiLambda->SetTitle(";M_{#bar{p}#pi^{+}} (GeV/#it{c}^{2}); Entries;");

  // Psi pair
  auto photonPsiPair = (TH1F*)FileReader::GetHist2D(
                           filename, appendix, {{"V0_Photon"}}, "fHistPsiPair")
                           ->ProjectionY();
  Plotter::SetStyleHisto(photonPsiPair, 20);
  photonPsiPair->SetTitle(";#Psi_{pair}; Entries;");

  // Transverse radius
  auto photonR =
      (TH1F*)FileReader::GetHist2D(filename, appendix, {{"V0_Photon", "After"}},
                                   "fHistTransverseRadiusAfter")
          ->ProjectionY();
  Plotter::SetStyleHisto(photonR, 20);
  photonR->SetTitle(";#it{r}_{xy} (cm); Entries;");

  // Cosine pointing angle
  auto photonCPA =
      (TH1F*)FileReader::GetHist2D(filename, appendix, {{"V0_Photon", "After"}},
                                   "fHistCosPAAfter")
          ->ProjectionY();
  Plotter::SetStyleHisto(photonCPA, 20);
  photonCPA->SetTitle(";cos(#alpha); Entries;");

  // DCA daughters at the decay vertex
  auto photonDCA =
      (TH1F*)FileReader::GetHist2D(filename, appendix, {{"V0_Photon", "After"}},
                                   "fHistDCADaughtersAfter")
          ->ProjectionY();
  Plotter::SetStyleHisto(photonDCA, 20);
  photonDCA->SetTitle("; DCA(e^{+}e^{-}) (cm); Entries;");

  // DCA pos. daughter / PV
  auto photonDCAPosPV =
      (TH1F*)FileReader::GetHist2D(filename, appendix,
                                   {{"V0_Photon", "V0_PosDaughter"}},
                                   "fHistSingleParticleDCAtoPVAfter_pos")
          ->ProjectionY();
  Plotter::SetStyleHisto(photonDCAPosPV, 20);
  photonDCAPosPV->SetTitle("; DCA(e^{+} PV) (cm); Entries;");

  // Armenteros
  auto photonArmenteros = FileReader::GetHist2D(
      filename, appendix, {{"V0_Photon", "After"}}, "fHistArmenterosAfter");
  photonArmenteros->SetTitle("Armenteros-Podolandski");
  Plotter::SetStyleHisto(photonArmenteros, 20);

  // PID pos. daughter
  auto photonPIDPos = FileReader::GetHist2D(filename, appendix,
                                            {{"V0_Photon", "V0_PosDaughter"}},
                                            "fHistSingleParticlePID_pos");
  photonPIDPos->SetTitle("PID e^{+}");
  Plotter::SetStyleHisto(photonPIDPos, 20);

  // DCA en. daughter / PV
  auto photonDCANegPV =
      (TH1F*)FileReader::GetHist2D(filename, appendix,
                                   {{"V0_Photon", "V0_NegDaughter"}},
                                   "fHistSingleParticleDCAtoPVAfter_neg")
          ->ProjectionY();
  Plotter::SetStyleHisto(photonDCANegPV, 20);
  photonDCANegPV->SetTitle("; DCA(e^{-} PV) (cm); Entries;");

  // PID neg. daughter
  auto photonPIDNeg = FileReader::GetHist2D(filename, appendix,
                                            {{"V0_Photon", "V0_NegDaughter"}},
                                            "fHistSingleParticlePID_neg");
  photonPIDNeg->SetTitle("PID e^{-}");
  Plotter::SetStyleHisto(photonPIDNeg, 20);

  auto d1 = new TCanvas();
  d1->SetRightMargin(0.16);
  photonInvMassPt->Draw("colz");
  d1->Print(Form("%s/Plot/PhotonInvMassPt.pdf", foldername.Data()));

  auto d2 = new TCanvas("d2", "d2", 1400, 1000);
  d2->Divide(2, 2);
  d2->cd(1);
  d2->cd(1)->SetLogy();
  photonInvMass->Draw("hist");
  d2->cd(2);
  d2->cd(2)->SetLogy();
  photonK0->Draw("hist");
  d2->cd(3);
  d2->cd(3)->SetLogy();
  photonLambda->Draw("hist");
  d2->cd(4);
  d2->cd(4)->SetLogy();
  photonAntiLambda->Draw("hist");
  d2->Print(Form("%s/Plot/PhotonMass.pdf", foldername.Data()));

  auto d3 = new TCanvas("d3", "d3", 1400, 1000);
  d3->Divide(2, 2);
  d3->cd(1);
  d3->cd(1)->SetLogy();
  photonPt->Draw("hist");
  d3->cd(2);
  photonPhi->SetMinimum(0);
  photonPhi->Draw("hist");
  d3->cd(3);
  d3->cd(3)->SetLogy();
  photonEta->Draw("hist");
  d3->Print(Form("%s/Plot/PhotonKinematics.pdf", foldername.Data()));

  auto d4 = new TCanvas();
  d4->SetRightMargin(0.16);
  photonEtaPhi->Draw("colz");
  d4->Print(Form("%s/Plot/PhotonEtaPhi.pdf", foldername.Data()));

  auto d5 = new TCanvas();
  d5->SetLogy();
  photonPsiPair->Draw("hist");
  d5->Print(Form("%s/Plot/PhotonPsiPair.pdf", foldername.Data()));

  auto d6 = new TCanvas("d6", "d6", 2100, 1000);
  d6->Divide(3, 2);
  d6->cd(1);
  d6->cd(1)->SetLogy();
  photonDCA->GetXaxis()->SetRangeUser(0, 1.5);
  photonDCA->Draw("hist");
  d6->cd(2);
  d6->cd(2)->SetLogy();
  photonDCAPosPV->Draw("hist");
  d6->cd(3);
  d6->cd(3)->SetLogy();
  photonDCANegPV->Draw("hist");
  d6->cd(4);
  photonR->Draw("hist");
  d6->cd(5);
  d6->cd(5)->SetLogy();
  photonCPA->Draw("hist");
  d6->Print(Form("%s/Plot/PhotonTopologic.pdf", foldername.Data()));

  auto d8 = new TCanvas("d8", "d8", 1400, 1000);
  d8->Divide(2, 2);
  d8->cd(1);
  d8->cd(1)->SetRightMargin(0.16);
  d8->cd(1)->SetLogz();
  photonPIDPos->Draw("colz");
  d8->cd(2);
  d8->cd(2)->SetRightMargin(0.16);
  d8->cd(2)->SetLogz();
  photonPIDNeg->Draw("colz");
  d8->cd(3);
  d8->cd(3)->SetRightMargin(0.16);
  d8->cd(3)->SetLogz();
  photonArmenteros->GetYaxis()->SetRangeUser(0, 0.2);
  photonArmenteros->Draw("colz");
  d8->Print(Form("%s/Plot/PhotonPID.pdf", foldername.Data()));

  // ========================================================================
  // Lambda properties

  // Inv. mass vs. pT
  auto lambdaInvMassPt = FileReader::GetHist2D(
      filename, appendix, {{"V0_Lambda"}}, "fHistV0MassPt");
  Plotter::SetStyleHisto(lambdaInvMassPt, 20);
  auto lambdaInvMass = (TH1F*)lambdaInvMassPt->ProjectionY();
  Plotter::SetStyleHisto(lambdaInvMass, 20);
  lambdaInvMass->SetTitle(";M_{p #pi^{-}} (GeV/#it{c}^{2}); Entries;");
  auto lambdaPt = (TH1F*)lambdaInvMassPt->ProjectionX();
  Plotter::SetStyleHisto(lambdaPt, 20);
  lambdaPt->SetTitle("; #it{p}_{T} (GeV/#it{c}); Entries;");

  // Eta & phi
  auto lambdaEtaPhi =
      FileReader::GetHist2D(filename, appendix, {{"V0_Lambda"}}, "fHistEtaPhi");
  Plotter::SetStyleHisto(lambdaEtaPhi, 20);
  auto lambdaPhi = (TH1F*)lambdaEtaPhi->ProjectionY();
  Plotter::SetStyleHisto(lambdaPhi, 20);
  lambdaPhi->SetTitle("; #varphi (rad); Entries;");
  auto lambdaEta = (TH1F*)lambdaEtaPhi->ProjectionX();
  Plotter::SetStyleHisto(lambdaEta, 20);
  lambdaEta->SetTitle("; #eta; Entries;");

  // Inv. mass under K0 hypothesis
  auto lambdaK0 =
      FileReader::GetHist1D(filename, appendix, {{"V0_Lambda"}}, "fHistK0Mass");
  Plotter::SetStyleHisto(lambdaK0, 20);
  lambdaK0->SetTitle(";M_{#pi^{+}#pi^{-}} (GeV/#it{c}^{2}); Entries;");

  // Inv. mass under Anti-Lambda hypothesis
  auto lambdaAntiLambda = FileReader::GetHist1D(
      filename, appendix, {{"V0_Lambda"}}, "fHistAntiLambdaMass");
  Plotter::SetStyleHisto(lambdaAntiLambda, 20);
  lambdaAntiLambda->SetTitle(";M_{#bar{p}#pi^{+}} (GeV/#it{c}^{2}); Entries;");

  // Transverse radius
  auto lambdaR =
      (TH1F*)FileReader::GetHist2D(filename, appendix, {{"V0_Lambda", "After"}},
                                   "fHistTransverseRadiusAfter")
          ->ProjectionY();
  Plotter::SetStyleHisto(lambdaR, 20);
  lambdaR->SetTitle(";#it{r}_{xy} (cm); Entries;");

  // Cosine pointing angle
  auto lambdaCPA =
      (TH1F*)FileReader::GetHist2D(filename, appendix, {{"V0_Lambda", "After"}},
                                   "fHistCosPAAfter")
          ->ProjectionY();
  Plotter::SetStyleHisto(lambdaCPA, 20);
  lambdaCPA->SetTitle(";cos(#alpha); Entries;");

  // DCA daughters at the decay vertex
  auto lambdaDCA =
      (TH1F*)FileReader::GetHist2D(filename, appendix, {{"V0_Lambda", "After"}},
                                   "fHistDCADaughtersAfter")
          ->ProjectionY();
  Plotter::SetStyleHisto(lambdaDCA, 20);
  lambdaDCA->SetTitle("; DCA(p #pi^{-}) (cm); Entries;");

  // DCA pos. daughter / PV
  auto lambdaDCAPosPV =
      (TH1F*)FileReader::GetHist2D(filename, appendix,
                                   {{"V0_Lambda", "V0_PosDaughter"}},
                                   "fHistSingleParticleDCAtoPVAfter_pos")
          ->ProjectionY();
  Plotter::SetStyleHisto(lambdaDCAPosPV, 20);
  lambdaDCAPosPV->SetTitle("; DCA(p PV) (cm); Entries;");

  // Armenteros
  auto lambdaArmenteros = FileReader::GetHist2D(
      filename, appendix, {{"V0_Lambda", "After"}}, "fHistArmenterosAfter");
  lambdaArmenteros->SetTitle("Armenteros-Podolandski");
  Plotter::SetStyleHisto(lambdaArmenteros, 20);

  // PID pos. daughter
  auto lambdaPIDPos = FileReader::GetHist2D(filename, appendix,
                                            {{"V0_Lambda", "V0_PosDaughter"}},
                                            "fHistSingleParticlePID_pos");
  lambdaPIDPos->SetTitle("PID p");
  Plotter::SetStyleHisto(lambdaPIDPos, 20);

  // DCA en. daughter / PV
  auto lambdaDCANegPV =
      (TH1F*)FileReader::GetHist2D(filename, appendix,
                                   {{"V0_Lambda", "V0_NegDaughter"}},
                                   "fHistSingleParticleDCAtoPVAfter_neg")
          ->ProjectionY();
  Plotter::SetStyleHisto(lambdaDCANegPV, 20);
  lambdaDCANegPV->SetTitle("; DCA(#pi^{-} PV) (cm); Entries;");

  // PID neg. daughter
  auto lambdaPIDNeg = FileReader::GetHist2D(filename, appendix,
                                            {{"V0_Lambda", "V0_NegDaughter"}},
                                            "fHistSingleParticlePID_neg");
  lambdaPIDNeg->SetTitle("PID #pi^{-}");
  Plotter::SetStyleHisto(lambdaPIDNeg, 20);

  auto e1 = new TCanvas();
  e1->SetRightMargin(0.16);
  lambdaInvMassPt->Draw("colz");
  e1->Print(Form("%s/Plot/LambdaInvMassPt.pdf", foldername.Data()));

  auto e2 = new TCanvas("e2", "e2", 1400, 1000);
  e2->Divide(2, 2);
  e2->cd(1);
  lambdaInvMass->Draw("hist");
  e2->cd(2);
  lambdaK0->Draw("hist");
  e2->cd(3);
  lambdaAntiLambda->Draw("hist");
  e2->Print(Form("%s/Plot/LambdaMass.pdf", foldername.Data()));

  auto e3 = new TCanvas("e3", "e3", 1400, 1000);
  e3->Divide(2, 2);
  e3->cd(1);
  e3->cd(1)->SetLogy();
  lambdaPt->Draw("hist");
  e3->cd(2);
  lambdaPhi->SetMinimum(0);
  lambdaPhi->Draw("hist");
  e3->cd(3);
  e3->cd(3)->SetLogy();
  lambdaEta->Draw("hist");
  e3->Print(Form("%s/Plot/LambdaKinematics.pdf", foldername.Data()));

  auto e4 = new TCanvas();
  e4->SetRightMargin(0.16);
  lambdaEtaPhi->Draw("colz");
  e4->Print(Form("%s/Plot/LambdaEtaPhi.pdf", foldername.Data()));

  auto e5 = new TCanvas("e11", "e11", 2100, 1000);
  e5->Divide(3, 2);
  e5->cd(1);
  e5->cd(1)->SetLogy();
  lambdaDCA->GetXaxis()->SetRangeUser(0, 1.5);
  lambdaDCA->Draw("hist");
  e5->cd(2);
  e5->cd(2)->SetLogy();
  lambdaDCAPosPV->Draw("hist");
  e5->cd(3);
  e5->cd(3)->SetLogy();
  lambdaDCANegPV->Draw("hist");
  e5->cd(4);
  lambdaR->Draw("hist");
  e5->cd(5);
  e5->cd(5)->SetLogy();
  lambdaCPA->GetXaxis()->SetRangeUser(0.99, 1);
  lambdaCPA->Draw("hist");
  e5->Print(Form("%s/Plot/LambdaTopologic.pdf", foldername.Data()));

  auto e7 = new TCanvas("e7", "e7", 1400, 1000);
  e7->Divide(2, 2);
  e7->cd(1);
  e7->cd(1)->SetRightMargin(0.16);
  e7->cd(1)->SetLogz();
  lambdaPIDPos->Draw("colz");
  e7->cd(2);
  e7->cd(2)->SetRightMargin(0.16);
  e7->cd(2)->SetLogz();
  lambdaPIDNeg->Draw("colz");
  e7->cd(3);
  e7->cd(3)->SetLogz();
  lambdaArmenteros->GetYaxis()->SetRangeUser(0, 0.2);
  lambdaArmenteros->Draw("colz");
  e7->Print(Form("%s/Plot/LambdaPID.pdf", foldername.Data()));

  // ========================================================================
  // Sigma0 properties

  // Inv. mass vs. pT
  auto sigmaInvMassPt =
      FileReader::GetHist2D(filename, appendix, {{"Sigma0"}}, "fHistInvMassPt");
  Plotter::SetStyleHisto(sigmaInvMassPt, 20);
  sigmaInvMassPt->GetYaxis()->SetRangeUser(1.1, 1.25);
  auto sigmaInvMass =
      FileReader::GetHist1D(filename, appendix, {{"Sigma0"}}, "fHistInvMass");
  Plotter::SetStyleHisto(sigmaInvMass, 20);
  sigmaInvMass->SetTitle(";M_{#Lambda#gamma} (GeV/#it{c}^{2}); Entries;");
  sigmaInvMass->GetXaxis()->SetRangeUser(1.1, 1.25);
  auto sigmaPt = (TH1F*)sigmaInvMassPt->ProjectionX();
  Plotter::SetStyleHisto(sigmaPt, 20);
  sigmaPt->SetTitle("; #it{p}_{T} (GeV/#it{c}); Entries;");

  // Eta & phi
  auto sigmaEtaPhi =
      FileReader::GetHist2D(filename, appendix, {{"Sigma0"}}, "fHistEtaPhi");
  Plotter::SetStyleHisto(sigmaEtaPhi, 20);
  auto sigmaPhi = (TH1F*)sigmaEtaPhi->ProjectionY();
  Plotter::SetStyleHisto(sigmaPhi, 20);
  sigmaPhi->SetTitle("; #varphi (rad); Entries;");
  auto sigmaEta = (TH1F*)sigmaEtaPhi->ProjectionX();
  Plotter::SetStyleHisto(sigmaEta, 20);
  sigmaEta->SetTitle("; #eta; Entries;");

  // Number of Sigma
  auto NSigma =
      FileReader::GetHist1D(filename, appendix, {{"Sigma0"}}, "fHistNSigma");
  Plotter::SetStyleHisto(NSigma, 20);
  NSigma->SetTitle("; Number of #Sigma^{0} candidates / event; Entries;");

  // Photon and Lambda Cleaner
  auto NGammaBefore = FileReader::GetHist1D(filename, appendix, {{"Sigma0"}},
                                            "fHistNPhotonBefore");
  Plotter::SetStyleHisto(NGammaBefore, 20);
  auto NGammaAfter = FileReader::GetHist1D(filename, appendix, {{"Sigma0"}},
                                           "fHistNPhotonAfter");
  Plotter::SetStyleHisto(NGammaAfter, 20, kRed + 2);
  NGammaBefore->SetTitle("; Number of #gamma candidates / event; Entries;");

  auto NLambdaBefore = FileReader::GetHist1D(filename, appendix, {{"Sigma0"}},
                                             "fHistNLambdaBefore");
  Plotter::SetStyleHisto(NLambdaBefore, 20);
  auto NLambdaAfter = FileReader::GetHist1D(filename, appendix, {{"Sigma0"}},
                                            "fHistNLambdaAfter");
  Plotter::SetStyleHisto(NLambdaAfter, 20, kRed + 2);
  NLambdaBefore->SetTitle("; Number of #Lambda candidates / event; Entries;");

  auto f1 = new TCanvas();
  f1->SetRightMargin(0.16);
  sigmaInvMassPt->Draw("colz");
  f1->Print(Form("%s/Plot/SigmaMassPt.pdf", foldername.Data()));

  auto f2 = new TCanvas();
  sigmaInvMass->Draw("hist");
  f2->Print(Form("%s/Plot/SigmaInvMass.pdf", foldername.Data()));

  auto f3 = new TCanvas("f3", "f3", 1400, 1000);
  f3->Divide(2, 2);
  f3->cd(1);
  f3->cd(1)->SetLogy();
  sigmaPt->Draw("hist");
  f3->cd(2);
  sigmaPhi->SetMinimum(0);
  sigmaPhi->Draw("hist");
  f3->cd(3);
  f3->cd(3)->SetLogy();
  sigmaEta->Draw("hist");
  f3->Print(Form("%s/Plot/Sigma0Kinematics.pdf", foldername.Data()));

  auto f4 = new TCanvas();
  f4->SetRightMargin(0.16);
  sigmaEtaPhi->Draw("colz");
  f4->Print(Form("%s/Plot/SigmaEtaPhi.pdf", foldername.Data()));

  auto f5 = new TCanvas();
  f5->SetLogy();
  NSigma->Draw("hist");
  f5->Print(Form("%s/Plot/NSigma.pdf", foldername.Data()));

  // ========================================================================
  // Anti-Sigma0 properties

  // Inv. mass vs. pT
  auto antiSigmaInvMassPt = FileReader::GetHist2D(
      filename, appendix, {{"AntiSigma0"}}, "fHistInvMassPt");
  Plotter::SetStyleHisto(antiSigmaInvMassPt, 20);
  antiSigmaInvMassPt->GetYaxis()->SetRangeUser(1.1, 1.25);
  auto antiSigmaInvMass = FileReader::GetHist1D(
      filename, appendix, {{"AntiSigma0"}}, "fHistInvMass");
  Plotter::SetStyleHisto(antiSigmaInvMass, 20);
  antiSigmaInvMass->SetTitle(
      ";M_{#bar{#Lambda}#gamma} (GeV/#it{c}^{2}); Entries;");
  antiSigmaInvMass->GetXaxis()->SetRangeUser(1.1, 1.25);
  auto antiSigmaPt = (TH1F*)antiSigmaInvMassPt->ProjectionX();
  Plotter::SetStyleHisto(antiSigmaPt, 20);
  antiSigmaPt->SetTitle("; #it{p}_{T} (GeV/#it{c}); Entries;");

  // Eta & phi
  auto antiSigmaEtaPhi = FileReader::GetHist2D(filename, appendix,
                                               {{"AntiSigma0"}}, "fHistEtaPhi");
  Plotter::SetStyleHisto(antiSigmaEtaPhi, 20);
  auto antiSigmaPhi = (TH1F*)antiSigmaEtaPhi->ProjectionY();
  Plotter::SetStyleHisto(antiSigmaPhi, 20);
  antiSigmaPhi->SetTitle("; #varphi (rad); Entries;");
  auto antiSigmaEta = (TH1F*)antiSigmaEtaPhi->ProjectionX();
  Plotter::SetStyleHisto(antiSigmaEta, 20);
  antiSigmaEta->SetTitle("; #eta; Entries;");

  // Number of Sigma
  auto NAntiSigma = FileReader::GetHist1D(filename, appendix, {{"AntiSigma0"}},
                                          "fHistNSigma");
  Plotter::SetStyleHisto(NAntiSigma, 20);
  NAntiSigma->SetTitle(
      "; Number of #bar{#Sigma^{0}} candidates / event; Entries;");

  // Photon and Lambda Cleaner
  auto NGammaAntiBefore = FileReader::GetHist1D(
      filename, appendix, {{"AntiSigma0"}}, "fHistNPhotonBefore");
  Plotter::SetStyleHisto(NGammaAntiBefore, 20);
  auto NGammaAntiAfter = FileReader::GetHist1D(
      filename, appendix, {{"AntiSigma0"}}, "fHistNPhotonAfter");
  Plotter::SetStyleHisto(NGammaAntiAfter, 20, kRed + 2);
  NGammaAntiBefore->SetTitle("; Number of #gamma candidates / event; Entries;");

  auto NLambdaAntiBefore = FileReader::GetHist1D(
      filename, appendix, {{"AntiSigma0"}}, "fHistNLambdaBefore");
  Plotter::SetStyleHisto(NLambdaAntiBefore, 20);
  auto NLambdaAntiAfter = FileReader::GetHist1D(
      filename, appendix, {{"AntiSigma0"}}, "fHistNLambdaAfter");
  Plotter::SetStyleHisto(NLambdaAntiAfter, 20, kRed + 2);
  NLambdaAntiBefore->SetTitle(
      "; Number of #bar{#Lambda} candidates / event; Entries;");

  auto g1 = new TCanvas();
  g1->SetRightMargin(0.16);
  antiSigmaInvMassPt->Draw("colz");
  g1->Print(Form("%s/Plot/AntiSigmaMassPt.pdf", foldername.Data()));

  auto g2 = new TCanvas();
  antiSigmaInvMass->Draw("hist");
  g2->Print(Form("%s/Plot/AntiSigmaInvMass.pdf", foldername.Data()));

  auto g3 = new TCanvas("g3", "g3", 1400, 1000);
  g3->Divide(2, 2);
  g3->cd(1);
  g3->cd(1)->SetLogy();
  antiSigmaPt->Draw("hist");
  g3->cd(2);
  antiSigmaPhi->SetMinimum(0);
  antiSigmaPhi->Draw("hist");
  g3->cd(3);
  g3->cd(3)->SetLogy();
  antiSigmaEta->Draw("hist");
  g3->Print(Form("%s/Plot/AntiSigma0Kinematics.pdf", foldername.Data()));

  auto g4 = new TCanvas();
  g4->SetRightMargin(0.16);
  antiSigmaEtaPhi->Draw("colz");
  g4->Print(Form("%s/Plot/AntiSigmaEtaPhi.pdf", foldername.Data()));

  auto g5 = new TCanvas();
  g5->SetLogy();
  NAntiSigma->Draw("hist");
  g5->Print(Form("%s/Plot/NAntiSigma.pdf", foldername.Data()));

  auto trackCleaner = new TCanvas("trackCleaner", "trackCleaner", 1400, 1000);
  trackCleaner->Divide(2, 2);
  trackCleaner->cd(1);
  trackCleaner->cd(1)->SetLogy();
  NLambdaBefore->Draw("hist");
  NLambdaAfter->Draw("hist same");
  auto leg = new TLegend(0.6, 0.7, 0.85, 0.85);
  leg->AddEntry(NGammaBefore, "Before", "l");
  leg->AddEntry(NGammaAfter, "After", "l");
  leg->Draw("same");
  trackCleaner->cd(2);
  trackCleaner->cd(2)->SetLogy();
  NLambdaAntiBefore->Draw("hist");
  NLambdaAntiAfter->Draw("hist same");
  leg->Draw("same");
  trackCleaner->cd(3);
  trackCleaner->cd(3)->SetLogy();
  NGammaBefore->Draw("hist");
  NGammaAfter->Draw("hist same");
  leg->Draw("same");
  trackCleaner->Print(Form("%s/Plot/TrackCleanerSigma.pdf", foldername.Data()));

  // ========================================================================
  // Track cleaner

  auto cleaner_pp = FileReader::GetHist1D(filename, appendix, {{"PairCleaner"}},
                                          "DaugthersSharedTracks_0", "femto");
  auto cleaner_apap =
      FileReader::GetHist1D(filename, appendix, {{"PairCleaner"}},
                            "DaugthersSharedTracks_1", "femto");
  auto cleaner_pSigma0 =
      FileReader::GetHist1D(filename, appendix, {{"PairCleaner"}},
                            "DaugthersSharedTracks_2", "femto");
  auto cleaner_apaSigma0 =
      FileReader::GetHist1D(filename, appendix, {{"PairCleaner"}},
                            "DaugthersSharedTracks_3", "femto");
  Plotter::SetStyleHisto(cleaner_pp, 20);
  cleaner_pp->SetTitle("; p-p pairs with shared tracks; Entries;");
  Plotter::SetStyleHisto(cleaner_apap, 20);
  cleaner_apap->SetTitle(
      "; #bar{p}-#bar{p} pairs with shared tracks; Entries;");
  Plotter::SetStyleHisto(cleaner_pSigma0, 20);
  cleaner_pSigma0->SetTitle(
      "; p-#Sigma^{0} pairs with shared tracks; Entries;");
  Plotter::SetStyleHisto(cleaner_apaSigma0, 20);
  cleaner_apaSigma0->SetTitle(
      "; #bar{p}-#bar{#Sigma^{0}} pairs with shared tracks; Entries;");

  auto h1 = new TCanvas("h1", "h1", 1400, 500);
  h1->Divide(2, 1);
  h1->cd(1);
  h1->cd(1)->SetLogy();
  cleaner_pp->Draw("hist");
  h1->cd(2);
  h1->cd(2)->SetLogy();
  cleaner_apap->Draw("hist");
  h1->Print(Form("%s/Plot/Cleaner_pp.pdf", foldername.Data()));

  auto h2 = new TCanvas("h2", "h2", 1400, 500);
  h2->Divide(2, 1);
  h2->cd(1);
  h2->cd(1)->SetLogy();
  cleaner_pSigma0->Draw("hist");
  h2->cd(2);
  h2->cd(2)->SetLogy();
  cleaner_apaSigma0->Draw("hist");
  h2->Print(Form("%s/Plot/Cleaner_pSigma0.pdf", foldername.Data()));
}
