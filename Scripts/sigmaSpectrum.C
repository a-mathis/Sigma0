#include "FileReader.h"
#include "Fitter.h"
#include "Plotter.h"
#include "TCanvas.h"

int main(int argc, char* argv[]) {
  Fitter sigmaFitter;

  Plotter::SetStyle();

  auto filename = TString(argv[1]);
  auto prefix = TString(argv[2]);
  auto suffix = TString(argv[3]);
  auto sigmaHist = FileReader::GetHist2D(filename, prefix, "Sigma0Cuts", {{}}, "fHistInvMassPtRaw", suffix);
  auto sigmaSpectrum = (TH1F*)sigmaHist->ProjectionY("sigmaSpectrum");

  auto antisigmaHist = FileReader::GetHist2D(
      filename, prefix, "AntiSigmaCuts", {{}}, "fHistInvMassPtRaw", suffix);
  auto antisigmaSpectrum =
      (TH1F*)antisigmaHist->ProjectionY("antisigmaSpectrum");

  antisigmaSpectrum->GetXaxis()->SetTitle(
      "M_{#bar{p}#pi^{+}} (GeV/#it{c}^{2})");
  sigmaSpectrum->Add(antisigmaSpectrum);

  Plotter::SetStyleHisto(sigmaSpectrum);
  sigmaSpectrum->GetXaxis()->SetTitle("M_{#Lambda#gamma} (GeV/#it{c}^{2})");
  sigmaFitter.SetSpectrum(sigmaSpectrum);

  // Histogram for Sigma0 Integral width
  auto histSigmaCuts = FileReader::GetHist1D(
      filename, prefix, "Sigma0Cuts", {{}}, "fHistCutBooking", suffix);
  const float intervalWidth = histSigmaCuts->GetBinContent(1);
  sigmaFitter.SetIntegralWidth(intervalWidth);

  auto c = new TCanvas();
  sigmaSpectrum->GetXaxis()->SetRangeUser(1.155, 1.225);
  sigmaFitter.FitSigma();
  sigmaSpectrum->Draw("PE");
  sigmaFitter.PrintStats("#Sigma");
  sigmaFitter.GetBackgroundFit()->Draw("same");
  c->Print("Plot/Sigma.pdf");

  return 1;
}
