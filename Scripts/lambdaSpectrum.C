#include "FileReader.h"
#include "Fitter.h"
#include "Plotter.h"
#include "TCanvas.h"

int main(int argc, char* argv[]) {
  Fitter lambdaFitter;
  Fitter antiLambdaFitter;

  Plotter::SetStyle();

  auto filename = TString(argv[1]);
  auto prefix = TString(argv[2]);
  auto suffix = TString(argv[3]);
  auto lambdaHist = FileReader::GetHist2D(filename, prefix, "v0Cuts",
                                          {{"v0Cuts"}}, "InvMassPt", suffix);
  auto lambdaSpectrum = (TH1F*)lambdaHist->ProjectionY("lambdaSpectrum");
  lambdaSpectrum->GetXaxis()->SetTitle("M_{p#pi^{-}} (GeV/#it{c}^{2})");
  Plotter::SetStyleHisto(lambdaSpectrum);
  lambdaFitter.SetSpectrum(lambdaSpectrum);
  lambdaFitter.SetIntegralWidth(0.004);

  auto antiLambdaHist = FileReader::GetHist2D(
      filename, prefix, "Antiv0Cuts", {{"Antiv0Cuts"}}, "InvMassPt", suffix);
  auto antiLambdaSpectrum =
      (TH1F*)antiLambdaHist->ProjectionY("antiLambdaSpectrum");
  antiLambdaSpectrum->GetXaxis()->SetTitle(
      "M_{#bar{p}#pi^{+}} (GeV/#it{c}^{2})");
  Plotter::SetStyleHisto(antiLambdaSpectrum);
  antiLambdaFitter.SetSpectrum(antiLambdaSpectrum);
  antiLambdaFitter.SetIntegralWidth(0.004);

  auto c = new TCanvas();
  lambdaFitter.FitLambda();
  lambdaSpectrum->Draw("PE");
  lambdaSpectrum->GetXaxis()->SetRangeUser(1.1, 1.13);
  lambdaFitter.PrintStats("#Lambda");
  lambdaFitter.GetBackgroundFit()->Draw("same");
  c->Print("Plot/Lambda.pdf");

  auto d = new TCanvas();
  antiLambdaFitter.FitLambda();
  antiLambdaSpectrum->Draw("PE");
  antiLambdaSpectrum->GetXaxis()->SetRangeUser(1.1, 1.13);
  antiLambdaFitter.PrintStats("#bar{#Lambda}");
  antiLambdaFitter.GetBackgroundFit()->Draw("same");
  d->Print("Plot/AntiLambda.pdf");

  return 1;
}
