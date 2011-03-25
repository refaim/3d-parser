//  [12/23/2009 SERGEY] - revision

#include <atlimage.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "TextureManager.h"

/* вариант №1 */
static CTex TEXMANAGER_;
 CTex & GETTEMANAGER_()
{
    return TEXMANAGER_;
}
/* КОНЕЦ вариант №1 */

inline void ToUpper (std::string & sStr)
{
    //sStr.MakeUpper ();
    for (std::string::iterator i = sStr.begin (); i != sStr.end (); ++i)
    {
        *i = toupper (*i);
    }
}

CTex::CTex ()
{
    TX_sErrorMessage = "Manager started";
}

CTex::~CTex ()
{
    FreeAll ();
}

int CTex::LoadTexture (std::string sFilename)
{
    ToUpper (sFilename);
    int tex = FindTex (sFilename);
    if (tex != -1)
    {
        TX_sErrorMessage = "Load: already exist." + sFilename;
        TX_texNameIdCnt[tex].Cnt++;
        return TX_texNameIdCnt[tex].ID;
    }

    CTexID IdCnt;
    IdCnt.Name = sFilename;
    IdCnt.Cnt = 1;
    IdCnt.ID = -1;
    IdCnt.image = new CImage;

    CImage & image = *(CImage*)IdCnt.image;
    if (image.Load(sFilename.c_str ()) != 0)
    {
        TX_sErrorMessage = "File failed to load:" + sFilename;
        delete (CImage*)IdCnt.image;
        return -1;
    }

    PreBind (IdCnt);
    TX_texNameIdCnt.push_back (IdCnt);
    TX_sErrorMessage = "Loaded W/O a hitch!:" + sFilename;
    return IdCnt.ID;
}

void CTex::PreBind (CTexID & tex)
{
    if (!tex.image) // если не NULL значит мы еще не загрузили текстуру в видеопамять
    {
        return; // иначе просто можно выходить
    }
    while (GL_NO_ERROR != glGetError ()); // сбросить все накопившиеся ошибки GL

    glGenTextures (1, &tex.ID);
    glBindTexture (GL_TEXTURE_2D, tex.ID);

    if (GL_NO_ERROR != glGetError ())
    {
        tex.ID = -1;
        return;
    }

    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    CImage & image = *(CImage*)tex.image;
    int w = image.GetWidth ();
    int h = image.GetHeight ();
    int suc = 1;

    int pitch = image.GetPitch();
    int nBPP = image.GetBPP() / 8;
    BYTE *pData = (BYTE *)image.GetBits();

    if (nBPP >= 3)
    {
        suc = gluBuild2DMipmaps (GL_TEXTURE_2D,
            nBPP, w, h, (3 == nBPP ? GL_BGR_EXT : GL_BGRA_EXT),
            GL_UNSIGNED_BYTE,
            (pitch < 0 ? (pData + pitch * (h - 1)) : pData));
    }
    else
    {
        BYTE * clrImage = new BYTE[w * h * 3];
        for (int y = 0; y < h; ++y)
        {
            for (int x = 0; x < w; ++x)
            {

                int ind = ((h- 1 - y) * w + x) * 3;
                COLORREF c = image.GetPixel (x, y);
                BYTE * clr = (BYTE*)&c;
                clrImage[ind + 0] = clr[0];
                clrImage[ind + 1] = clr[1];
                clrImage[ind + 2] = clr[2];
            }
        }
        suc = gluBuild2DMipmaps (GL_TEXTURE_2D,
            3, w, h, GL_RGB, GL_UNSIGNED_BYTE, clrImage);
        delete [] clrImage;
    }

    if (suc != 0)
    {
        TX_sErrorMessage = "Prebind failed: gluBuild2DMipmaps.";
        return;
    }
    delete (CImage*)tex.image;
    tex.image = NULL;
}

unsigned int CTex::Bind (std::string name)
{
    int tex = FindTex (name);
    if (tex == -1)
    {
        TX_sErrorMessage = "No such texture:" + name;
        return -1;
    }
    PreBind (TX_texNameIdCnt[tex]);
    unsigned int ID = TX_texNameIdCnt[tex].ID;// GetTexID (name);
    if (ID != -1)
    {
        glBindTexture (GL_TEXTURE_2D, ID);
    }
    return ID;
}

unsigned int CTex::GetNumTextures (void)
{
    return (unsigned int)TX_texNameIdCnt.size ();
}

unsigned int CTex::GetTexID (std::string name)
{
    ToUpper (name);
    int tex = FindTex (name);
    TX_sErrorMessage = "GetTexID success.";
    return TX_texNameIdCnt[tex].ID;
}

void CTex::FreeTexture (unsigned int ID)
{
    int tex;
    for (tex = 0; tex < (int)TX_texNameIdCnt.size (); ++tex)
    {
        if (TX_texNameIdCnt[tex].ID == ID)
        {
            --TX_texNameIdCnt[tex].Cnt;
            TX_sErrorMessage = "Texture " + TX_texNameIdCnt[tex].Name + " down counted.";
            if (TX_texNameIdCnt[tex].Cnt == 0)
            {
                glDeleteTextures (1, &TX_texNameIdCnt[tex].ID);
                TX_sErrorMessage = "Texture " + TX_texNameIdCnt[tex].Name + " deleted successfully.";
                if (TX_texNameIdCnt[tex].image) delete (CImage*)TX_texNameIdCnt[tex].image;
                TX_texNameIdCnt.erase (TX_texNameIdCnt.begin () + tex);
            }
            break;
        }
    }
}

void CTex::FreeTexture (std::string name)
{
    FreeTexture (GetTexID (name));
}

void CTex::FreeAll (void)
{
    int tex;
    for (tex = 0; tex < (int)TX_texNameIdCnt.size (); ++tex)
    {
        glDeleteTextures (1, &TX_texNameIdCnt[tex].ID);
        if (TX_texNameIdCnt[tex].image) delete (CImage*)TX_texNameIdCnt[tex].image;
    }
    TX_texNameIdCnt.clear ();
}

const char * CTex::GetErrorMessage (void)
{
    return TX_sErrorMessage.c_str ();
}

int CTex::FindTex (std::string sFilename)
{
    ToUpper (sFilename);
    for (int i = 0; i < (int)TX_texNameIdCnt.size (); ++i)
    {
        if (sFilename == TX_texNameIdCnt[i].Name)
        {
            return i;
        }
    }
    return -1;
}
