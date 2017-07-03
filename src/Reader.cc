#include <Reader.h>

using namespace lhaaso;

// Units definition to set the units system in expression evaluator
// ----------------------------------------------------------------
static const double r_length         = 1.0;                     // meter          
static const double r_mass           = 1.0 / (e_SI * 1.0e-18);  // eV/c**2        
static const double r_time           = 1.0e+9;                  // nanosecond     
static const double r_current        = 1.0 / (e_SI / 1.0e-9);   // e/nanosecond   
static const double r_temperature    = 1.0;                     // Kelvin         
static const double r_amount         = 1.0;                     // mole           
static const double r_luminous_intensity = 1.0;                 // candela        

HepTool::Evaluator eval;
    
/*! Old constructor with older validation options.  Preserved to keep
old code from breaking. */
Reader::Reader(string name, bool b) 
{
  if (b) 
    setUp(name, Reader::DTD);
  else
    setUp(name, Reader::NONE);
}

Reader::Reader(string name, Validation v)
{
  setUp(name,v);
}

void Reader::setUp(string name, Validation v)
{
  validationMode = v;
  XMLFileName = name;  
  initialize();  // do not move Initialize() away from the beginning!! other DOM stuff needs it. 
  // Set up the evaluator and devine additional units
  eval.setStdMath();
  eval.setSystemOfUnits(r_length,r_mass,r_time,r_current,r_temperature,r_amount,r_luminous_intensity);
  eval.setVariable("EeV","1e18*eV");
  
  parse();  
}


Reader::~Reader()
{
  delete parser;    
}


//  Private method(s)
// --------------------

void Reader::initialize()
{  
  XMLPlatformUtils::Initialize();
}

void Reader::parse() 
{
  parser = new XercesDOMParser;
  
  parser->setDoSchema(false);
  parser->setValidationScheme(XercesDOMParser::Val_Never);
  parser->setCreateEntityReferenceNodes(false);    // MAY WANT TO CHANGE THIS AT SOME POINT
  
  if (validationMode == Reader::DTD)
    {
      parser->setValidationScheme(XercesDOMParser::Val_Auto);
    }	
  else if (validationMode == Reader::SCHEMA)
    {
      // 	cerr << "**Error in Reader" << endl;
      // 	cerr << "You requested validation by Schema. " << endl;
      // 	cerr << "This is not supported yet, sorry ! " << endl;       
      parser->setDoSchema(true);
      parser->setValidationScheme(XercesDOMParser::Val_Auto);
      parser->setValidationSchemaFullChecking(true);	
      parser->setDoNamespaces(true);
    } 
  else if (validationMode == Reader::NONE)
    {
      parser->setValidationScheme(XercesDOMParser::Val_Never);
    }
  else
    {
      cerr << "**ERROR in Reader::Reader" << endl;
      cout << "You have selected an invalid validation mode " << endl;
      cout << "Terminating execution." << endl;
      exit(EXIT_SUCCESS);
    }
  
  // if (validationFlag) {
  //TPTP  ErrorHandler *errReporter = new DOMTreeErrorReporter();
  //    ErrorHandler* errReporter = (ErrorHandler*) new HandlerBase();
  
  ErrorHandler* errReporter = (ErrorHandler*) new DOMTreeErrorReporter();
  parser->setErrorHandler(errReporter);
  // }
  
  try {
    parser->parse(XMLFileName.c_str());	
  }
  catch (const XMLException& e) {
    cerr << " An error occured during parsing\n   Message: "
	 << XMLString::transcode(e.getMessage()) << endl;	
  }
  
  documentNode = parser->getDocument();   
  // initialize top branch to the document element.
  topBranch.domNode = documentNode->getDocumentElement();  
}


// Methods for FFREAD-style API
// ----------------------------
void Reader::FillDataCards(string c)
{
  cardname = c;
  
  if (topBranch != 0) 
    {	
      findDataCardBranch( topBranch );
    }
  else
    {
      cout << "**ERROR in Reader::FillDataCards" << endl;
      cout << "This should never happen " << endl;
      cout << "XMLFileName " << XMLFileName << endl;
      exit(EXIT_SUCCESS);
    }
  
  if (dataCardBranch == 0) 
    {
      cerr << "**ERROR in Reader::FillDataCards " << endl;
      cerr << "Did not find the cards named : " << cardname << endl;
      exit(EXIT_SUCCESS);
    }        
}


// Find the requested "data-card" branch (recursive)
// -------------------------------------------------
void Reader::findDataCardBranch(const Branch& b)
{            
  for (Branch cb = b.GetFirstChild() ; cb != 0 ; cb = cb.GetNextSibling())
    {
      if ( !strncmp(cb.GetBranchName(),cardname.c_str(),100)) 
	{
	  dataCardBranch = cb;
	  return;    
	}
      else
	{
	    findDataCardBranch(cb);
	}
    }
}

// Class Branch methods
// --------------------
Branch::Branch()
{
  domNode=0;  // Branch starts out null. Used to verify that requested branch was found.        
}

Branch::Branch(const Branch& b)
{
  domNode = b.domNode;
}


Branch::~Branch()
{}


Branch Branch::GetFirstChild() const
{
  Branch childBranch;
  
  DOMNode* childNode = domNode->getFirstChild();
  
  childNode = domNode->getFirstChild();
  while (childNode->getNodeType() != DOMNode::ELEMENT_NODE) 
    {
      childNode = childNode->getNextSibling();
      if (childNode == 0) 
	{
	  //  	    cerr << "**WARNING reading XML !" << endl;
	  //  	    cerr << "The problem is in the method Branch::GetFirstChild() " << endl;
	  //  	    cerr << "You requested the first child of branch  '" << GetBranchName() << "', " << endl;
	  //  	    cerr << "but this branch has no children ! " << endl;
	  return childBranch;  // return childBranch with no DOM_node associated to it
	}	
    }
  
  childBranch.domNode = childNode;
  return childBranch;	    
}


Branch Branch::GetChild(const string requestedName, map<string,string> requestedAttributeMap) const
{
  Branch childBranch;
  
  for (DOMNode* childNode = domNode->getFirstChild() ; 
       childNode !=0 ; childNode = childNode->getNextSibling())
    {
      if (childNode->getNodeType() == DOMNode::ELEMENT_NODE) 
	{
	  string foundName = XMLString::transcode(childNode->getNodeName());
	  
	  // Get all the attributes into a map.
	  map<string,string> foundAttributeMap;
	  DOMNamedNodeMap* attributes = childNode->getAttributes();
	  
	  for (unsigned int j=0 ; j<attributes->getLength() ; j++)
	    {
	      DOMNode* attribute = attributes->item(j);
	      
	      foundAttributeMap.insert(pair<string,string> (XMLString::transcode(attribute->getNodeName()),XMLString::transcode(attribute->getNodeValue()) ));
	    } // filled up foundAttributeMap
	  
	  
	  // First, discard any unit attributes in either the found or requested maps.
	  // Units are treated as a special case.
	  requestedAttributeMap.erase("unit");requestedAttributeMap.erase("UNIT");
	  foundAttributeMap.erase("unit");    foundAttributeMap.erase("UNIT");
	  
	  // Check for exact match between tag name, remaining attributes and attribute
	  // values in the requested and found maps
	  bool foundIt = 1;
	  
	  if (foundName != requestedName)
	    foundIt = 0; // the tag names are different
	  
	  if (foundAttributeMap.size() != requestedAttributeMap.size())
	    foundIt = 0; // reject immediately if attribute lists are different lengths
	  else
	    {	
	      map<string,string>::iterator foundIter;
	      for (foundIter = foundAttributeMap.begin() ; foundIter != foundAttributeMap.end() ; foundIter++)
		{		   
		  map<string,string>::iterator requestedIter;		
		  requestedIter = requestedAttributeMap.find( foundIter->first);
		  if (requestedIter == requestedAttributeMap.end()) 
		    {
		      foundIt = 0;
		    }		
		  else
		    {
		      if (foundIter->second != requestedIter->second) foundIt = 0;
		    }
		}  // end of check that length of requested and found maps are same 
	    }
	  if (foundIt) 
	    {      
	      childBranch.domNode = childNode;
	      return childBranch;
	    }
	}  // end condition ELEMENT_NODE
    }  // end loop on child nodes   
  
  // requested element not found.. returning an empty Branch.
  //
  cout << "**WARNING from Branch::GetChild()" << endl;
  cout << "  The Branch you requested was not found " << endl;
  cout << "  Requested name : " << requestedName << endl;
  cout << "  Attributes     : ";
  for (map<string,string>::iterator i = requestedAttributeMap.begin() ;
       i != requestedAttributeMap.end() ; i++)
    {
      cout << " " << i->first << " " << i->second << endl;		    
    }
  cout << endl;			
  
  return childBranch;    
}


Branch Branch::GetChild(const string requestedName) const
{
  map<string,string> dummy;
  return GetChild(requestedName, dummy);
}

Branch Branch::GetChild(const string requestedName, const string id) const
{
  map<string,string> idMap;
  idMap["id"]=id;    
  return GetChild(requestedName, idMap);
}

map<string,string> Branch::GetAttributes() const
{
  if (domNode == 0)
    {
      cout << "**Error from Branch::GetAttributes() " << endl;
      cout << "  You requested the attributes of a null branch " << endl;
      exit(EXIT_SUCCESS);	
    }
  
  map<string,string> attMap;
  
  DOMNamedNodeMap* attributes = domNode->getAttributes();
  for (unsigned int j=0 ; j<attributes->getLength() ; j++)
    {
      DOMNode* attribute = attributes->item(j);
      
      attMap[ XMLString::transcode(attribute->getNodeName()) ] 
	=   XMLString::transcode(attribute->getNodeValue());
    }
  return attMap;
}


const char* Branch::GetBranchName() const
{
  if (domNode == 0)
    {
      cout << "**Error from Branch::GetBranchName() " << endl;
      cout << "  You requested the name of a null branch " << endl;
      exit(EXIT_SUCCESS);	
    }
  
  string name = XMLString::transcode(domNode->getNodeName());
  
  
  DOMNamedNodeMap* attributes = domNode->getAttributes();
  for (unsigned int j=0 ; j<attributes->getLength() ; j++)
    {
      DOMNode* attribute = attributes->item(j);
      
      if ( static_cast<string>(XMLString::transcode(attribute->getNodeName())) == "ID" ||
	   static_cast<string>(XMLString::transcode(attribute->getNodeName())) == "id" )
	{
	  name += " (id = ";
	  name += XMLString::transcode(attribute->getNodeValue());
	  name += ")";
	}
    }
  return name.c_str();
}

Branch Branch::GetNextSibling() const
{
  Branch siblingBranch;
  
  DOMNode* siblingNode = domNode->getNextSibling();
  
  while (siblingNode->getNodeType() != DOMNode::ELEMENT_NODE)
    {
      siblingNode = siblingNode->getNextSibling();
      if (siblingNode == 0) {
	return siblingBranch;
      }
    }
  
  siblingBranch.domNode = siblingNode;
  return siblingBranch;
}


Branch Branch::GetSibling(const string requestedName, map<string,string> attributeMap) const
{
  // check that current node is valid.
  if (this == 0)
    {
      cerr << "**ERROR in reader." << endl;
      cerr << "Problem is in Branch::GetSibling(const string childName, map<string,string> attributeMap) " << endl;
      cerr << "You requested a sibling of a branch which is NULL." << endl;
      cerr << "Terminating execution." << endl;
      exit(EXIT_SUCCESS);
    }
  
  // back up to the parent.
  Branch parentBranch;
  parentBranch.domNode = domNode->getParentNode();
  
  return parentBranch.GetChild(requestedName, attributeMap);
}

Branch Branch::GetSibling(const string requestedName) const 
{
  map<string,string> dummy;    
  return GetSibling(requestedName, dummy);   
}

Branch Branch::GetSibling(const string requestedName, const string id) const     
{
  map<string,string> idMap;
  idMap["id"]=id;    
  return GetSibling(requestedName, idMap);
}



// GetData methods
// ---------------

void Branch::GetData(int& i) const
{
  castData(i);
  i = i*static_cast<int>(getUnit());
}

void Branch::GetData(bool& b) const
{
  // note we don't bother with the unit here.
  castData(b);
}	

void Branch::GetData(float& f) const
{
  castData(f);
  f = f*static_cast<float>(getUnit());
}

void Branch::GetData(double& d) const
{
  castData(d);
  d = d*static_cast<double>(getUnit());
}

void Branch::GetData(string& s) const
{
  // do not use castData, as in this case
  // we want to return the ENTIRE string, including
  // possible white space. 
  s = getDataString();
}

void Branch::GetData(char*& c) const
{
  // do not use castData, as in this case
  // we want to return the ENTIRE string, including
  // possible white space.
  
  string s=getDataString();
  c = new char[s.size()+1]; // check for memory leak
  strcpy(c,s.c_str());
}

void Branch::GetData(vector<int>& i) const
{
  castData(i);
  int u = static_cast<int>( getUnit() );
  for (vector<int>::iterator it = i.begin();
       it != i.end(); *it = *it*u, it++);    
}

void Branch::GetData(vector<float>& f) const
{
  castData(f);
  float u = static_cast<float>( getUnit() );
  for (vector<float>::iterator it = f.begin();
       it != f.end(); *it = *it*u, it++);    
}

void Branch::GetData(vector<double>& d) const
{
  castData(d);
  double u = static_cast<double>( getUnit() );
  for (vector<double>::iterator it = d.begin();
       it != d.end(); *it = *it*u, it++);
} 

void Branch::GetData(vector<string>& s) const
{
  // ignore units (if they exist) in this case
  castData(s);
}	

void Branch::GetData(vector<bool>& b) const
{
  castData(b);
}

void Branch::GetData(list<int>& i) const
{
  castData(i);
  int u = static_cast<int>( getUnit() );
  for (list<int>::iterator it = i.begin();
       it != i.end(); *it = *it*u, it++);    
}

void Branch::GetData(list<float>& f) const
{
  castData(f);
  float u = static_cast<float>( getUnit() );
  for (list<float>::iterator it = f.begin();
       it != f.end(); *it = *it*u, it++);    
}

void Branch::GetData(list<double>& d) const
{
  castData(d);
  double u = static_cast<double>( getUnit() );
  for (list<double>::iterator it = d.begin();
       it != d.end(); *it = *it*u, it++);
} 

void Branch::GetData(list<string>& s) const
{
  // ignore units (if they exist) in this case
  castData(s);
}	

void Branch::GetData(list<bool>& b) const
{
  castData(b);
}

void Reader::DumpTree()
{
  cout << endl;
  cout << "Dumping information in XML file : " << XMLFileName << endl;
  cout << "-----------------------------------------------------------" << endl;
  dumpMe( GetTopBranch() );
}

// Dumps information in the XML tree (recursive)
// --------------------------------------------
void Reader::dumpMe(const Branch& b)
{
  string additionalSpace = "   ";
  
  vector<string> bData;   // put data in a string vector to avoid spaces & CR in printout
  b.GetData(bData);
  
  cout << spaces << b.GetBranchName() << " : ";
  for (vector<string>::iterator it = bData.begin();
       it != bData.end(); it++) cout << *it << " ";
  cout << endl;
  spaces += additionalSpace;
  
  Branch cb;
  
  for (cb = b.GetFirstChild(); cb != 0; cb = cb.GetNextSibling())
    {		
      dumpMe( cb );
    }
  
  int len = spaces.length();
  spaces.erase(len-additionalSpace.length());	     
}

// Template for casting data to int, float, double. Strings are handled as a special case
// --------------------------------------------------------------------------------------
template<class T>
void Branch::castData(T& dataT) const 
{
  strstream s;
  s << getDataString().c_str();
  
  T dataValue;    
  s >> dataValue;
  dataT = dataValue;    
}


// Template for casting data to vector<int>, vector<float>, vector<double> and vector<string>
// ------------------------------------------------------------------------------------------
template<class T>
void Branch::castData(vector<T>& v) const
{
  string dataString = getDataString();
  string::iterator first, last;       
  string temp;
  
  last=find_if(dataString.begin(), dataString.end(), NotSpace()); //get past any beginning spaces
  first = dataString.begin();					   
  while (last != dataString.end())                                //loop until end of string
    {						
      first=find_if(last, dataString.end(), NotSpace());          //sets first to first char not a space
      last=find_if(first, dataString.end(), IsSpace());           //sets last to first char that is a space
      if (first != last)                                          //this is needed because otherwise scanf reads the last number twice
	{
	  T value;
	  strstream s;
	    
	  temp.assign(first,last);
	  s << temp.c_str();
	  s >> value;	    
	  v.push_back(value);	
	}      
    }   
}

// Template for casting data to list<int>, list<float>, list<double>
// and list<string>.
//
template<class T>
void Branch::castData(list<T>& v) const
{
  string dataString = getDataString();
  string::iterator first, last;       
  string temp;
  
  last=find_if(dataString.begin(), dataString.end(), NotSpace()); //get past any beginning spaces
  first = dataString.begin();					   
  while (last != dataString.end())                                //loop until end of string
    {						
      first=find_if(last, dataString.end(), NotSpace());          //sets first to first char not a space
      last=find_if(first, dataString.end(), IsSpace());           //sets last to first char that is a space
      if (first != last)                                          //this is needed because otherwise scanf reads the last number twice
	{
	  T value;
	  strstream s;
	  
	  temp.assign(first,last);
	  s << temp.c_str();
	  s >> value;	    
	  v.push_back(value);	
	}      
    }   
}


string Branch::getDataString() const
{
  //    Find the data residing beneath this tag
  DOMNodeList* dataNodes = domNode->getChildNodes();
  string dataString;
  
  for (unsigned int k=0 ; k < dataNodes->getLength() ; k++)
    {
      DOMNode* currentNode = dataNodes->item(k);
      
      // For TEXT nodes, postpend the node onto the dataString.
      // NB if there are multiple TEXT nodes beneath this element,
      // and they are, for example, separated
      // by one or more sub- elements, these text nodes will be 
      // concatenated into a single data string.
      if (currentNode->getNodeType() == DOMNode::TEXT_NODE)
	dataString += XMLString::transcode(currentNode->getNodeValue());
    }
  
    // Remove any leading and/or trailing whitespace (and CR's) from the string
    // -----------------------------------------------------------------------
    string trimmedString;
    string::iterator start,stop,first,last;
    start = find_if(dataString.begin() , dataString.end() , NotSpace());

    first = dataString.begin();
    last  = start;
    while (last != dataString.end())
      {
	first=find_if(last, dataString.end(), NotSpace());          //sets first to first char not a space
	last=find_if(first, dataString.end(), IsSpace());           //sets last to first char that is a space
	if (first != last) trimmedString.assign(start,last);
      }
    return trimmedString;
}


double Branch::getUnit() const
{
  double unit;
  
  // Check for a unit attribute
  // --------------------------
  DOMNamedNodeMap* attributes = domNode->getAttributes();
  
  string unitString;
  for (unsigned int j=0 ; j < attributes->getLength() ; j++ )
    {
      DOMNode* attribute = attributes->item(j);
      if (!strncmp(XMLString::transcode(attribute->getNodeName()), "UNIT",100) ||
	  !strncmp(XMLString::transcode(attribute->getNodeName()), "unit",100))		
	{	    
	  unitString = XMLString::transcode(attribute->getNodeValue());	    
	}
    }
  
  // If found, convert the unit to the appropriate scale factor, otherwise scale factor = 1
  // --------------------------------------------------------------------------------------
  
  if (!unitString.empty())
    {
      // HepTool::Evaluator eval;
      unit = eval.evaluate(unitString.c_str());
      if (eval.status() != HepTool::Evaluator::OK ) {
	eval.print_error();
	cout << "**ERROR from Branch::getUnit" << endl;
	cout << "The unit '" << unitString << "' was not understood by the evaluator" << endl;
	cout << "Terminating execution " << endl;
	exit(EXIT_SUCCESS);	    
      }	
    }
  else       
    {
      unit = 1.0; // no unit found ==> scale factor is 1
    }
  
  return unit;
}

Branch& Branch::operator=(const Branch& b)
{
  domNode = b.domNode;
  return *this;    
}


// Overloaded operators
// --------------------
bool Branch::operator==(const BranchNull* b) const
{
  return domNode == 0;
}

bool Branch::operator!=(const BranchNull* b) const
{
  return domNode != 0;
}


// Overloaded operators (friends of Branch)
// ----------------------------------------
ostream& operator<<(ostream& os, const Branch b)
{
  // For now, just prints the branch name.
  // Could have more detailed information later.
  
  os << b.GetBranchName() << endl;
  return os;
}

// Methods for the old FFREAD-style API (to be templatized)
// --------------------------------------------------------
void Reader::FindDataCard(const string name, bool& b)	
{
  dataCardBranch.GetChild(name).GetData(b);
}	

void Reader::FindDataCard(const string name, int& i)
{
  dataCardBranch.GetChild(name).GetData(i);
}	

void Reader::FindDataCard(const string name, float& f)
{
  dataCardBranch.GetChild(name).GetData(f);
}

void Reader::FindDataCard(const string name, double& d)
{
  dataCardBranch.GetChild(name).GetData(d);
}

void Reader::FindDataCard(const string name, string& s)
{
  dataCardBranch.GetChild(name).GetData(s);
}

void Reader::FindDataCard(const string name, char*& c)
{
  dataCardBranch.GetChild(name).GetData(c);
}

void Reader::FindDataCard(const string name, vector<bool>& vb)
{
  dataCardBranch.GetChild(name).GetData(vb);
}

void Reader::FindDataCard(const string name, vector<int>& vi)
{
  dataCardBranch.GetChild(name).GetData(vi);
}

void Reader::FindDataCard(const string name, vector<float>& vf)
{	
  dataCardBranch.GetChild(name).GetData(vf);
}

void Reader::FindDataCard(const string name, vector<double>& vd)
{
  dataCardBranch.GetChild(name).GetData(vd);
}

void Reader::FindDataCard(const string name, vector<string>& vs)
{
  dataCardBranch.GetChild(name).GetData(vs);
}

void Reader::FindDataCard(const string name, list<bool>& lb)
{
  dataCardBranch.GetChild(name).GetData(lb);
}

void Reader::FindDataCard(const string name, list<int>& li)
{
  dataCardBranch.GetChild(name).GetData(li);
}

void Reader::FindDataCard(const string name, list<float>& lf)
{
  dataCardBranch.GetChild(name).GetData(lf);
}

void Reader::FindDataCard(const string name, list<double>& ld)
{
  dataCardBranch.GetChild(name).GetData(ld);
}

void Reader::FindDataCard(const string name, list<string>& ls)
{
  dataCardBranch.GetChild(name).GetData(ls);
}

void Reader::FindDataCard(const string name, const string multiID, bool& b)	
{
  dataCardBranch.GetChild(name, multiID).GetData(b);
}	

void Reader::FindDataCard(const string name, const string multiID, int& i)
{
  dataCardBranch.GetChild(name, multiID).GetData(i);
}	

void Reader::FindDataCard(const string name, const string multiID, float& f)
{
  dataCardBranch.GetChild(name, multiID).GetData(f);
}

void Reader::FindDataCard(const string name, const string multiID, double& d)
{
  dataCardBranch.GetChild(name, multiID).GetData(d);
}

void Reader::FindDataCard(const string name, const string multiID, string& s)
{
  dataCardBranch.GetChild(name, multiID).GetData(s);
}

void Reader::FindDataCard(const string name, const string multiID, char*& c)
{
  dataCardBranch.GetChild(name, multiID).GetData(c);
}

void Reader::FindDataCard(const string name, const string multiID, vector<bool>& vb)
{
  dataCardBranch.GetChild(name, multiID).GetData(vb);
}

void Reader::FindDataCard(const string name, const string multiID, vector<int>& vi)
{
  dataCardBranch.GetChild(name, multiID).GetData(vi);
}

void Reader::FindDataCard(const string name, const string multiID, vector<float>& vf)
{
  dataCardBranch.GetChild(name, multiID).GetData(vf);
}

void Reader::FindDataCard(const string name, const string multiID, vector<double>& vd)
{
  dataCardBranch.GetChild(name, multiID).GetData(vd);
}

void Reader::FindDataCard(const string name, const string multiID, vector<string>& vs)
{
  dataCardBranch.GetChild(name, multiID).GetData(vs);
}

void Reader::FindDataCard(const string name, const string multiID, list<bool>& lb)
{
  dataCardBranch.GetChild(name, multiID).GetData(lb);
}

void Reader::FindDataCard(const string name, const string multiID, list<int>& li)
{
  dataCardBranch.GetChild(name, multiID).GetData(li);
}

void Reader::FindDataCard(const string name, const string multiID, list<float>& lf)
{
  dataCardBranch.GetChild(name, multiID).GetData(lf);
}

void Reader::FindDataCard(const string name, const string multiID, list<double>& ld)
{
  dataCardBranch.GetChild(name, multiID).GetData(ld);
}

void Reader::FindDataCard(const string name, const string multiID, list<string>& ls)
{
  dataCardBranch.GetChild(name, multiID).GetData(ls);
}


