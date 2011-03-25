//  [12/23/2009 SERGEY] - revision
#pragma once

#include <string>
#include <vector>

class  CTexID
{
public:
    std::string Name;
    unsigned int ID;
    int Cnt;
    void * image;
public:
    CTexID () {image = NULL;}
};

typedef std::vector<CTexID> VCTexID;

class  CTex
{
public:
    std::string TX_sErrorMessage;
    VCTexID TX_texNameIdCnt;
public:
    CTex ();
    ~CTex ();
public:
    int LoadTexture (std::string sFilename);
    unsigned int GetNumTextures (void);
    unsigned int GetTexID (std::string name);
    unsigned int Bind (std::string name);
    void FreeTexture (unsigned int ID);
    void FreeTexture (std::string name);
    void FreeAll (void);
    const char * GetErrorMessage (void);
private:
    void PreBind (CTexID & tex);
    int FindTex (std::string sFilename);
};

 CTex & GETTEMANAGER_();

#define TEXMANAGER GETTEMANAGER_()
