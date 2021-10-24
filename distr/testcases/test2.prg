//Global and local variable check
//Function call, argument type check, return type check

class abc{
  string a;
  int c;
  int b;
  string d="new";

  fun fun1 returns string (int a,string b) {
      a=3;
      return d;
  }

  fun fun2 returns int (int a) {
      string temp=fun1(b,d);
      return 123;
  }

}