// ***************************************************************************
// * Reader.h                                                                *
// *                                                                         *
// * Date : 15 July 2015                                                     * 
// * Authors : T. Paul, P. Cattaneo for Auger                                * 
// * Modified : D. Martraire for LHAASO                                      *
// *                                                                         *
// * Description :                                                           *
// * Reader class for parsing XML files and transferring simple "data card"  *
// * contents into a variety of data types.  Here "data card" means the data *
// * data is formatted in a simple <key> value </key> format.                *
// * This is similar in spirit to the old FFREAD of cernlib days.            *
// *                                                                         *
// * To Do List:                                                             *
// * -----------                                                             *
// *  1/ Namespace this !!                                                   *
// *  2/Templatize the FindDataCard methods.                                 *
// *  3/ Smarter error handling. The XML way to do it is put most of the work*
// * in the DTD.  Exception handling should be used (more).  There is some   *
// * error catching that terminates execution - maybe these should be        *
// * handled in a more graceful way.                                         *
// *  4/ It was necessary to copy over the validation stuff from the DOMPrint*
// * directory and add a transcode() method for the DOMString's fed to       * 
// * cout in DOMTreeErrorReporter.cpp ..  This is a bit of a pain, if for    *
// * example we update to a new version of XML4C (somebody will have to copy *
// * over the new DOMTreeErrorReporter and fix the places where DOMString    *
// * appears in a cout.                                                      * 
// *  5/ Schema support                                                      *
// ***************************************************************************


#ifndef _Reader_
#define _Reader_

using namespace std;

#include <iostream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <strstream>

// STL includes
// ------------
#include <vector>
#include <list>
#include <map>
#include <ext/algorithm>

// XML includes
// ------------
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>

// CLHEP includes
// --------------
#include <CLHEP/Evaluator/Evaluator.h>
#include <LhaasoUnits.h>

// Local DOM includes
// ------------------
#include <DOMTreeErrorReporter.h>

//! Predicate used in STL for searching for whitespace
class NotSpace {
public:
    bool operator()(char alpha){return (alpha != ' ' && alpha != '\015' && alpha !='\012');}
};

//! Predicate used in STL for searching for whitespace
class IsSpace {
public:
    bool operator()(char alpha){return (alpha == ' ' || alpha == '\015' || alpha == '\012');}
};

// Dummy class (no implementation) used in overloaded functions as a way
// to pass 0 or null.
// ---------------------------------------------------------------------
class BranchNull;
 

//! Class representing a document branch
/*! A branch is essentially a DOM element node (a subset of what the DOM 
considers to be a node).
That is, DOM nodes such as comment or text nodes which may not have children
do not qualify to be branches.
The idea of the Branch is to provide a simple tool for navigating trees of 
data that might be more palatable for LHAASO applications (ie. those which 
do not require especially sophisticated document traversal.)

In any case, if you don't want to use the Branch class, you are free to 
retrieve the DOM document element via Reader::GetDocument() and apply 
standard DOM traversal tools.
Thus the reader allows the full power of DOM2 should the user need it.
*/

class Branch 

// to do:
// - make assignment operator set the domNode member of the branch

{
public:

    // ==============
    //  Constructors
    // ==============
    Branch();
    ~Branch();

    Branch(const Branch&);    

    //    void deleteChildren(Branch* b);

    //! Get first child of this Branch 
    /*! Returns null if no child found */
    // -------------------------------
    Branch GetFirstChild() const;

    //! Get child of this Branch by child name
    /*! Returns null if no child found */
    // ---------------------------------------
    Branch GetChild(const string childName) const;

    //! Get child of this Branch by child name and ID
    /*! Returns null if no child found */
    // -----------------------------------------------
    Branch GetChild(const string childName, const string multiID) const;

    //! Get child of this Branch by child name and any number of attribute-value pairs.
    /*! This method allows the user to specify values for any number of attributes.  A 
      (non-null) Branch is returned only if the requested name is found in the XML file and {\em all} the attributes 
      are found {\em with} their specified value, {\em with the single exception of the unit attribute}.
      Unit is handled as a special case by the reader, and any unit attribute which may be present,
      either in the XML file or the second argument of this method, will be ignored by this method.
      The requested attributes are specified in a map<string, string>
      where the first string is the attribute name and the second string is the attribute value. */
    // -------------------------------------------------------------------------------------------
    Branch GetChild(const string childName, map<string,string> attributeMap) const;    

    //! Get next sibling of this branch
    /*! Returns null if no child found */
    // --------------------------------
    Branch GetNextSibling() const;

     //! Get sibling by name
    /*! returns null if no sibling found */
    // ----------------------------------------------
    Branch GetSibling(const string childName) const;

    //! Get sibling by name and ID
    /*! returns null if no sibling found */
    // ----------------------------------------------
    Branch GetSibling(const string childName, const string multiID) const;

    //! Get sibling of this Branch by child name and any number of attribute-value pairs.
    /*! returns null if no sibling found */
    // ---------------------------------------------------------
    Branch GetSibling(const string childName, map<string,string> attributeMap) const;

    //! Get a map<list,list> containing all the attributes of this Branch
    /*! Unlike the GetData() method, GetAttributes() makes no attempt
      to cast the attributes */
    // ---------------------------------------------------------
    map<string,string> GetAttributes() const;    

    // GetData methods.
    // These methods find data in the current branch and attempt to
    // cast it according to the type of argument in the GetData 
    // argument list.  Note that, if you look look at the actual
    // implementation of GetData methods, most of them simply invoke
    // the (private) castData template function.
    // The reason for the intermediate
    // GetData method is that is some cases, one might want to deal
    // with special cases that are not dealt with by a simple 
    // template.  For example, suppose one has an XML element:
    //  <someData> 13 14 15 16 </someData>
    // If the user requests to retrieve <someData> as an int, probably
    // one should return just 13.  If one asks for a string, probably
    // one should return the whole list of numbers.  These two different
    // interpretations can be dealt with using the intermediate
    // GetData methods to compliment the templated castData method.
    // Note that, currently, scaling by the appropriate unit factor
    // is handled in the GetData methods, not the castData method.
    // (in principle this could change, but keep in mind that for
    // the case of strings you don't want to try to multiply by a 
    // scale factor.)
    // --------------------------------------------------------------        
    
    //! Get data in the current branch and cast as a bool.
    // ---------------------------------------------------
    void GetData(bool& b) const;
    
    //! Get data for int type
    // ----------------------
    void GetData(int& i) const;

    //! Get data for float type
    // ------------------------
    void GetData(float& f) const;

    //! Get data for double type
    // -------------------------
    void GetData(double& d) const;

    //! Get data for string type
    // -------------------------
    void GetData(string& s) const;

    //! Get data for char* type
    // ------------------------
    void GetData(char*& c) const;

    //! Get data for vector<int>
    // -------------------------
    void GetData(vector<int>& i) const;
    
    //! Get data for vector<float>
    // ---------------------------
    void GetData(vector<float>& f) const;

    //! Get data for vector<double>
    // ----------------------------
    void GetData(vector<double>& d) const;
    
    //! Get data for vector<string>
    // ---------------------------
    void GetData(vector<string>& s) const;
    
    //! Get data for vector<bool>
    // --------------------------
    void GetData(vector<bool>& b) const;
    
    //! Get data for list<int>
    // -------------------------
    void GetData(list<int>& i) const;
    
    //! Get data for list<float>
    // ---------------------------
    void GetData(list<float>& f) const;

    //! Get data for list<double>
    // ----------------------------
    void GetData(list<double>& d) const;
    
    //! Get data for list<string>
    // ---------------------------
    void GetData(list<string>& s) const;
    
    //! Get data for list<bool>
    // --------------------------
    void GetData(list<bool>& b) const;

    const char*  GetBranchName() const;    

    //! Assignment operator
    // --------------------
    Branch& operator=(const Branch& b);    

    //! Overloaded == for comparison with null pointer
    // -----------------------------------------------
    bool operator==(const BranchNull* b) const;

    //! Overloaded != for comparison with null pointer
    // -----------------------------------------------
    bool operator!=(const BranchNull* b) const;
    
private:

    DOMNode* domNode; 
    
    // Return data for the Branch, attempt to cast it as
    // the requested type, and multiply by the unit multiplier.
    // These methods use the getDataAndUnit method to find the
    // data string and unit multiplier
    // -----------------------------------------------------------------
    template<class T> void castData(T&) const;
    template<class T> void castData(vector<T>&) const;
    template<class T> void castData(list<T>&) const;

    // helper function to get the data inside an element as one big string
    // -------------------------------------------------------------------
    string getDataString() const;

    // helper function to the (optional) unit attribute and return the
    // appropriate scale factor.
    // ---------------------------------------------------------------
    double getUnit() const;

    friend class Reader;
    friend ostream& operator<<(ostream& os, const Branch b);
};


class Reader {

public:

    enum Validation 
    {	
	DTD, SCHEMA, NONE
    };
    	

    // ==============
    //  Constructors
    // ==============

    //! Constructor with arguments for XML file name and an optional flag to switch on or off validation
    Reader(string, bool);
    Reader(string, Validation validationType = Reader::NONE);
 
    ~Reader();

    //! Get the top Branch (represents same entity as document node)
    Branch GetTopBranch()
    {
      return topBranch;
    };
    
    
    //! Get the document node
    DOMDocument* GetDocument() 
      {
	return documentNode;
      };
    
    //! Utility to make ASCII dump of the XML tree
    // -------------------------------------------
    void DumpTree();

    // FindDataCard methods (to preserve the old FFREAD-style API)
    // -----------------------------------------------------------
    //! {\em deprecated}: Find the tag named in argument list and extract the information under said tag, assuming it is in <key> value(s) </key> format.
    void FillDataCards(string c);     

    //! {\em deprecated}: Search for key name (first argument) and return associated value(s) (second argument).
    void FindDataCard(const string name, bool& b);
    //! {\em deprecated}: Search for key name (first argument) and return associated value(s) (second argument).
    void FindDataCard(const string name, int& i);
    //! {\em deprecated}: Search for key name (first argument) and return associated value(s) (second argument).
    void FindDataCard(const string name, float& f);
    //! {\em deprecated}: Search for key name (first argument) and return associated value(s) (second argument).
    void FindDataCard(const string name, double& d);
    //! {\em deprecated}: Search for key name (first argument) and return associated value(s) (second argument).
    void FindDataCard(const string name, string& s);
    //! {\em deprecated}: Search for key name (first argument) and return associated value(s) (second argument).
    void FindDataCard(const string name, char*& c);


    //! {\em deprecated}: Search for key name (first argument) and return associated value(s) (second argument).
    void FindDataCard(const string name, vector<bool>&);
    //! {\em deprecated}: Search for key name (first argument) and return associated value(s) (second argument).
    void FindDataCard(const string name, vector<int>&);
    //! {\em deprecated}: Search for key name (first argument) and return associated value(s) (second argument).
    void FindDataCard(const string name, vector<float>&);
    //! {\em deprecated}: Search for key name (first argument) and return associated value(s) (second argument).
    void FindDataCard(const string name, vector<double>&);
    //! {\em deprecated}: Search for key name (first argument) and return associated value(s) (second argument).
    void FindDataCard(const string name, vector<string>&);

    //! {\em deprecated}: Search for key name (first argument) and return associated value(s) (second argument).
    void FindDataCard(const string name, list<bool>&);
    //! {\em deprecated}: Search for key name (first argument) and return associated value(s) (second argument).
    void FindDataCard(const string name, list<int>&);
    //! {\em deprecated}: Search for key name (first argument) and return associated value(s) (second argument).
    void FindDataCard(const string name, list<float>&);
    //! {\em deprecated}: Search for key name (first argument) and return associated value(s) (second argument).
    void FindDataCard(const string name, list<double>&);
    //! {\em deprecated}: Search for key name (first argument) and return associated value(s) (second argument).
    void FindDataCard(const string name, list<string>&);

    //! {\em deprecated}: Search for key name (first argument), attribute name (second argument) and return associated value(s) (third argument)
    void FindDataCard(const string name, const string multiID, int& i); 
    //! {\em deprecated}: Search for key name (first argument), attribute name (second argument) and return associated value(s) (third argument)
    void FindDataCard(const string name, const string multiID, bool& b); 
    //! {\em deprecated}: Search for key name (first argument), attribute name (second argument) and return associated value(s) (third argument)
    void FindDataCard(const string name, const string multiID, float& f);
    //! {\em deprecated}: Search for key name (first argument), attribute name (second argument) and return associated value(s) (third argument)
    void FindDataCard(const string name, const string multiID, double& d);
    //! {\em deprecated}: Search for key name (first argument), attribute name (second argument) and return associated value(s) (third argument)
    void FindDataCard(const string name, const string multiID, string& s);
    //! {\em deprecated}: Search for key name (first argument), attribute name (second argument) and return associated value(s) (third argument)
    void FindDataCard(const string name, const string multiID, char*& c);

    //! {\em deprecated}: Search for key name (first argument), attribute name (second argument) and return associated value(s) (third argument)
    void FindDataCard(const string name, const string multiID, vector<bool>&);
    //! {\em deprecated}: Search for key name (first argument), attribute name (second argument) and return associated value(s) (third argument)
    void FindDataCard(const string name, const string multiID, vector<int>&);
    //! {\em deprecated}: Search for key name (first argument), attribute name (second argument) and return associated value(s) (third argument)
    void FindDataCard(const string name, const string multiID, vector<float>&);
    //! {\em deprecated}: Search for key name (first argument), attribute name (second argument) and return associated value(s) (third argument)
    void FindDataCard(const string name, const string multiID, vector<double>&);
    //! {\em deprecated}: Search for key name (first argument), attribute name (second argument) and return associated value(s) (third argument)
    void FindDataCard(const string name, const string multiID, vector<string>&);
    
    //! {\em deprecated}: Search for key name (first argument), attribute name (second argument) and return associated value(s) (third argument)
    void FindDataCard(const string name, const string multiID, list<bool>&);
    //! {\em deprecated}: Search for key name (first argument), attribute name (second argument) and return associated value(s) (third argument)
    void FindDataCard(const string name, const string multiID, list<int>&);
    //! {\em deprecated}: Search for key name (first argument), attribute name (second argument) and return associated value(s) (third argument)
    void FindDataCard(const string name, const string multiID, list<float>&);
    //! {\em deprecated}: Search for key name (first argument), attribute name (second argument) and return associated value(s) (third argument)
    void FindDataCard(const string name, const string multiID, list<double>&);
    //! {\em deprecated}: Search for key name (first argument), attribute name (second argument) and return associated value(s) (third argument)
    void FindDataCard(const string name, const string multiID, list<string>&);

private:

    void findDataCardBranch(const Branch& b);
    void setUp(string, Validation);

    string XMLFileName;

    XercesDOMParser* parser;    

    // Name of XML "data-cards" (for the FFREAD-style API)
    string cardname;
    // Branch where the XML "data-cards" live (for the FFREAD-style API)
    Branch dataCardBranch;
      
    // root node of the document
    DOMDocument* documentNode;         

    // set type of validation requested (DTD, Schema, or None)
    Validation validationMode;
    
    // Branch at the top of the document tree.
    Branch topBranch;
    
    void initialize();  
    void parse();

    // helper function for dumping out the XML tree (recursive)
    void dumpMe(const Branch& b);
    string spaces; 
};

#endif



