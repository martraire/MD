// *************************************************************************
// * DOMTreeErrorReporter.h                                                *
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


#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <iostream.h>

// using namespace xercesc_2_5;
using namespace xercesc;

using namespace std;


class DOMTreeErrorReporter : public xercesc::ErrorHandler
{
 public:
  // Constructors and Destructor
  DOMTreeErrorReporter() :
    fSawErrors(false)
    {
    }
    
    ~DOMTreeErrorReporter()
      {
      }
    
    
    // Implementation of the error handler interface
    void warning(const SAXParseException& toCatch);
    void error(const SAXParseException& toCatch);
    void fatalError(const SAXParseException& toCatch);
    void resetErrors();
    
    // Getter methods
    bool getSawErrors() const;
    
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fSawErrors
    //      This is set if we get any errors, and is queryable via a getter
    //      method. Its used by the main code to suppress output if there are
    //      errors.
    // -----------------------------------------------------------------------
    bool    fSawErrors;
};

inline bool DOMTreeErrorReporter::getSawErrors() const
{
  return fSawErrors;
}

// ---------------------------------------------------------------------------
//  This is a simple class that lets us do easy (though not terribly efficient)
//  trancoding of XMLCh data to local code page for display.
// ---------------------------------------------------------------------------
class StrX
{
 public :
  // Constructors and Destructor
  StrX(const XMLCh* const toTranscode)
    {
      // Call the private transcoding method
      fLocalForm = XMLString::transcode(toTranscode);
    }
  
  ~StrX()
    {
        delete [] fLocalForm;
    }
  
  
    // Getter methods
  const char* localForm() const
    {
      return fLocalForm;
    }
  
 private :
  // -----------------------------------------------------------------------
  //  Private data members
  //
  //  fLocalForm
  //      This is the local code page form of the string.
  // -----------------------------------------------------------------------
  char*   fLocalForm;
};

inline ostream& operator<<(ostream& target, const StrX& toDump)
{
  target << toDump.localForm();
  return target;
}

