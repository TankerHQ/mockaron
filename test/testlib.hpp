#ifndef MYLIB_HPP
#define MYLIB_HPP

class My
{
public:
  struct NoCopy
  {
    NoCopy() = default;
    NoCopy(NoCopy const&) = delete;
    NoCopy& operator=(NoCopy const&) = delete;
    NoCopy(NoCopy&&) = delete;
    NoCopy& operator=(NoCopy&&) = delete;
  };

  My(float);
  ~My();

  int f(int);
  int g(int const&);
  float g(float);
  int h(int) const;
  int i();
  void j();
  NoCopy& k();
  static char const* l(int);
  static bool m();
  static int n(int xx);
  static float n(float xx);

private:
  int _i;
  NoCopy _no;
};

#endif
