// *************************************************************************
// * DOMTreeErrorReporter.cc                                               *
// *                                                                       *
// * Date : 6 July 2015                                                    *
// * The Apache Software License, Version 1.1                              *
// * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights   *
// * reserved.                                                             *
// *                                                                       *
// * Description :                                                         *
// * This software consists of voluntary contributions made by many        *
// * individuals on behalf of the Apache Software Foundation, and was      *
// * originally based on software copyright (c) 1999, International        *
// * Business Machines, Inc., http://www.ibm.com .                         *
// *  For more informationon the Apache Software Foundation, please see:   *
// * <http://www.apache.org/>.                                             *
// *************************************************************************


#include <xercesc/sax/SAXParseException.hpp>
#include <DOMTreeErrorReporter.h>
#include <iostream.h>
#include <stdlib.h>
#include <string.h>


void DOMTreeErrorReporter::warning(const SAXParseException&)
{
  // Ignore all warnings.
}

void DOMTreeErrorReporter::error(const SAXParseException& toCatch)
{
  fSawErrors = true;
  cerr << "Error at file \"" << StrX(toCatch.getSystemId())
       << "\", line " << toCatch.getLineNumber()
       << ", column " << toCatch.getColumnNumber()
       << "\n   Message: " << StrX(toCatch.getMessage()) << endl;
}

void DOMTreeErrorReporter::fatalError(const SAXParseException& toCatch)
{
  fSawErrors = true;
  cerr << "Fatal Error at file \"" << StrX(toCatch.getSystemId())
       << "\", line " << toCatch.getLineNumber()
       << ", column " << toCatch.getColumnNumber()
       << "\n   Message: " << StrX(toCatch.getMessage()) << endl;
}

void DOMTreeErrorReporter::resetErrors()
{
  // No-op in this case
}


