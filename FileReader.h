#ifndef FileReader_H
#define FileReader_H

#include <iostream>
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

class FileReader {
 public:
  FileReader() = default;
  ~FileReader() = default;

  /// Read a TH2F from the file
  /// \param filename Path to the file
  /// \param appendix Appendix of the content (kMB, kHighMultV0, ..._
  /// \param path TList inside the file
  /// \param histname name of the histogram
  /// \param prefix Name of the folder
  /// \return TH2F from the file
  static TH2F *GetHist2D(TString filename, TString appendix, TString dirName,
                         std::vector<TString> path, TString histname, TString prefix = "histo");
  /// Read a TH1F from the file
  /// \param filename Path to the file
  /// \param appendix Appendix of the content (kMB, kHighMultV0, ..._
  /// \param path TList inside the file
  /// \param histname name of the histogram
  /// \param prefix Name of the folder
  /// \return TH1F from the file
  static TH1F *GetHist1D(TString filename, TString appendix, TString dirName,
                         std::vector<TString> path, TString histname, TString prefix = "histo");

  /// Read a TProfile from the file
  /// \param filename Path to the file
  /// \param appendix Appendix of the content (kMB, kHighMultV0, ..._
  /// \param path TList inside the file
  /// \param histname name of the histogram
  /// \param prefix Name of the folder
  /// \return TProfile from the file
  static TProfile *GetProfile(TString filename, TString appendix, TString dirName,
                              std::vector<TString> path, TString histname, TString prefix = "histo");
};
#endif  // FileReader_H
