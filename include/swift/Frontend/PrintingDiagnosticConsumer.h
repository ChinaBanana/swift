//===--- PrintingDiagnosticConsumer.h - Print Text Diagnostics --*- C++ -*-===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2020 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://swift.org/LICENSE.txt for license information
// See https://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//
//
//  This file defines the PrintingDiagnosticConsumer class, which displays
//  diagnostics as text to a terminal.
//
//===----------------------------------------------------------------------===//

#ifndef SWIFT_PRINTINGDIAGNOSTICCONSUMER_H
#define SWIFT_PRINTINGDIAGNOSTICCONSUMER_H

#include "swift/Basic/LLVM.h"
#include "swift/AST/DiagnosticConsumer.h"

#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Process.h"

namespace swift {
class AnnotatedSourceSnippet;

/// Diagnostic consumer that displays diagnostics to standard error.
class PrintingDiagnosticConsumer : public DiagnosticConsumer {
  llvm::raw_ostream &Stream;
  bool ForceColors = false;
  bool DidErrorOccur = false;
  bool ExperimentalFormattingEnabled = false;
  // The current snippet used to display an error/warning/remark and the notes
  // implicitly associated with it. Uses `std::unique_ptr` so that
  // `AnnotatedSourceSnippet` can be forward declared.
  std::unique_ptr<AnnotatedSourceSnippet> currentSnippet;

public:
  PrintingDiagnosticConsumer(llvm::raw_ostream &stream = llvm::errs());
  ~PrintingDiagnosticConsumer();

  virtual void handleDiagnostic(SourceManager &SM,
                                const DiagnosticInfo &Info) override;

  virtual bool finishProcessing() override;

  void flush(bool includeTrailingBreak);

  virtual void flush() override { flush(false); }

  void forceColors() {
    ForceColors = true;
    llvm::sys::Process::UseANSIEscapeCodes(true);
  }

  void enableExperimentalFormatting() { ExperimentalFormattingEnabled = true; }

  bool didErrorOccur() {
    return DidErrorOccur;
  }

private:
  void printDiagnostic(SourceManager &SM, const DiagnosticInfo &Info);
};
  
}

#endif
