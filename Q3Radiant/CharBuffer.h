#ifndef __CHARBUFFER_HPP
#define __CHARBUFFER_HPP

class CCharBuffer
{
    char* m_pCharBuffer;
    unsigned m_uSize;
  
  public:
    CCharBuffer();

    CCharBuffer(unsigned uSize);
  
    CCharBuffer(const char* pString);
  
    CCharBuffer(const CCharBuffer& rhs);
  
   ~CCharBuffer();

    char* Allocate(uint uSize);

    void DeAllocate();

    size_t StringLength()
      { return strlen(m_pCharBuffer); }
  
    void StripTrailing(char c, int nLen);
  
    char& operator *()
      { return *m_pCharBuffer; }
  
    char& operator *() const
      { return *const_cast<CCharBuffer*>(this)->m_pCharBuffer; }
  
    operator void*()
      { return m_pCharBuffer; }
  
    operator char*()
      { return m_pCharBuffer; }
  
    operator const char*()
      { return reinterpret_cast<const char*>(m_pCharBuffer); }
  
    operator unsigned char*()
      { return reinterpret_cast<unsigned char*>(m_pCharBuffer); }
  
    operator const unsigned char*()
      { return reinterpret_cast<const unsigned char*>(m_pCharBuffer); }
  
    unsigned SizeOf()
      { return m_uSize; }
  
    CCharBuffer& operator =(const CCharBuffer& rhs);
  
    CCharBuffer& operator =(const char* pString);
  
    bool operator ==(const CCharBuffer& rhs) const
      { return strcmp(m_pCharBuffer, rhs.m_pCharBuffer) == 0; }
  
    bool operator ==(char* pString) const
      { return strcmp(m_pCharBuffer, pString) == 0; }
  
    bool operator ==(const char* pString) const
      { return strcmp(m_pCharBuffer, pString) == 0; }
  
    bool operator !=(CCharBuffer& rhs) const
      { return strcmp(m_pCharBuffer, rhs.m_pCharBuffer) != 0; }
  
    bool operator !=(char* pString) const
      { return strcmp(m_pCharBuffer, pString) != 0; }
  
    bool operator !=(const char* pString) const
      { return strcmp(m_pCharBuffer, pString) != 0; }
  
    char& operator [](int nIndex)
      { return m_pCharBuffer[nIndex]; }
  
    char& operator [](int nIndex) const
      { return m_pCharBuffer[nIndex]; }
  
    char* Fill(char FillChar)
      { memset(m_pCharBuffer, FillChar, m_uSize-1); return m_pCharBuffer; }
};
//
//-----------------------------------------------------------------------------
#endif // __CCHARBUFFER_HPP
//-----------------------------------------------------------------------------
//
