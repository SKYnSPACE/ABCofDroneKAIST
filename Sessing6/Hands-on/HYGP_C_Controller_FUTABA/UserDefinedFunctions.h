#ifndef USERDEFINEDFUNCTIONS_H
#define USERDEFINEDFUNCTIONS_H

#define D2R ((float)0.0174533)
#define R2D ((float)57.29578)

template<typename T>
int UDF_sign(T x)
{
  return ((x>0) - (x<0));
}

#endif
