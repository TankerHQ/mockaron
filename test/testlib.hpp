#ifndef MYLIB_HPP
#define MYLIB_HPP

class My
{
public:
  My(float);
  ~My();

  int f(int);
  int g(int const&);
  float g(float);
  int h(int) const;
  int i();
  void j();

private:
  int _i;
};

#endif
